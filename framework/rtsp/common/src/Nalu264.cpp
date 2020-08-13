
#include <libFrameWork_Rtsp/Nalu264.h>

Nalu264::Nalu264( void )
{

}

Nalu264::~Nalu264( void )
{

}

int Nalu264::Init( const char* c_szData, UInt32 uLen )
{
	_Head = *((tagNALU_HEADER*)&c_szData[0]);
	if( _Head.TYPE < 23 )
		_sRbsp = nal_to_rbsp(1, (const uint8_t*)c_szData, (int*)&uLen);
	else if( _Head.TYPE == NAL_UNIT_TYPE_FU_A || _Head.TYPE == NAL_UNIT_TYPE_FU_B )
		_sRbsp = nal_to_rbsp(2, (const uint8_t*)c_szData, (int*)&uLen);

	if( _Head.TYPE > 0 && _Head.TYPE < 24 )
	{//单个NAL单元包

	}

	return 1;
}

int Nalu264::GetSPS( h264_sps_t* pSPS )
{
	if( _Head.TYPE != NAL_UNIT_TYPE_SPS )
		return -1;

	bs_t b ;
	bs_init(&b, (uint8_t*)_sRbsp.c_str(), _sRbsp.size());
	read_seq_parameter_set_rbsp(pSPS, &b);
	return 1;
}

int Nalu264::GetPPS( h264_pps_t* pPPS, h264_sps_t* pSPS )
{
	if( _Head.TYPE != NAL_UNIT_TYPE_PPS )
		return -1;

	bs_t b ;
	bs_init(&b, (uint8_t*)_sRbsp.c_str(), _sRbsp.size());

	read_pic_parameter_set_rbsp(pPPS, pSPS, &b);
	return 1;
}

UInt8 Nalu264::Get_Type( UInt8 uData )
{
	_Head = *((tagNALU_HEADER*)&uData);
	return _Head.TYPE;
}

// 7.4.1.1 Encapsulation of an SODB within an RBSP
_string_type Nalu264::rbsp_to_nal(const uint8_t* rbsp_buf, const int* rbsp_size)
{
	_string_type s;
	int i = 0, count = 0;

	s.append(1, 0x00); // zero out first byte since we start writing from second byte

	for ( i = 0; i < *rbsp_size ; i++ )
	{
		if ( ( count == 2 ) && !(rbsp_buf[i] & 0xFC) ) // HACK 0xFC
		{
			s.append(1, 0x03);
			count = 0;
		}

		s.append(1, rbsp_buf[i]);

		if ( rbsp_buf[i] == 0x00 )
		{
			count++;
		}
		else
		{
			count = 0;
		}
	}

	return s;
}

// 7.4.1.1 Encapsulation of an SODB within an RBSP
_string_type Nalu264::nal_to_rbsp(const int nal_header_size, const uint8_t* nal_buf, int* nal_size)
{
	_string_type s;
	s.reserve(*nal_size);

	int i = 0, count = 0;

	for( i = nal_header_size; i < *nal_size; i++ )
	{ 
		// in NAL unit, 0x000000, 0x000001 or 0x000002 shall not occur at any byte-aligned position
		if( ( count == 2 ) && ( nal_buf[i] < 0x03) ) 
		{
			return "";
		}

		if( ( count == 2 ) && ( nal_buf[i] == 0x03) )
		{
			// check the 4th byte after 0x000003, except when cabac_zero_word is used, in which case the last three bytes of this NAL unit must be 0x000003
			if((i < *nal_size - 1) && (nal_buf[i+1] > 0x03))
			{
				return "";
			}

			// if cabac_zero_word is used, the final byte of this NAL unit(0x03) is discarded, and the last two bytes of RBSP must be 0x0000
			if(i == *nal_size - 1)
			{
				break;
			}

			i++;
			count = 0;
		}

		s.append(1, nal_buf[i]);
		if(nal_buf[i] == 0x00)
		{
			count++;
		}
		else
		{
			count = 0;
		}
	}

	*nal_size = i;
	return s;
}

//7.3.2.1 Sequence parameter set RBSP syntax
void Nalu264::read_seq_parameter_set_rbsp(h264_sps_t* sps, bs_t* b)
{
	int i;

	// NOTE can't read directly into sps because seq_parameter_set_id not yet known and so sps is not selected

	int profile_idc = bs_read_u8(b);
	int constraint_set0_flag = bs_read_u1(b);
	int constraint_set1_flag = bs_read_u1(b);
	int constraint_set2_flag = bs_read_u1(b);
	int constraint_set3_flag = bs_read_u1(b);
	int constraint_set4_flag = bs_read_u1(b);
	int constraint_set5_flag = bs_read_u1(b);
	int reserved_zero_2bits  = bs_read_u(b,2);  /* all 0's */
	int level_idc = bs_read_u8(b);
	int seq_parameter_set_id = bs_read_ue(b);

	// select the correct sps
	sps->chroma_format_idc = 1;

	sps->profile_idc = profile_idc; // bs_read_u8(b);
	sps->constraint_set0_flag = constraint_set0_flag;//bs_read_u1(b);
	sps->constraint_set1_flag = constraint_set1_flag;//bs_read_u1(b);
	sps->constraint_set2_flag = constraint_set2_flag;//bs_read_u1(b);
	sps->constraint_set3_flag = constraint_set3_flag;//bs_read_u1(b);
	sps->constraint_set4_flag = constraint_set4_flag;//bs_read_u1(b);
	sps->constraint_set5_flag = constraint_set5_flag;//bs_read_u1(b);
	sps->reserved_zero_2bits = reserved_zero_2bits;//bs_read_u(b,2);
	sps->level_idc = level_idc; //bs_read_u8(b);
	sps->seq_parameter_set_id = seq_parameter_set_id; // bs_read_ue(b);
	if( sps->profile_idc == 100 || sps->profile_idc == 110 ||
		sps->profile_idc == 122 || sps->profile_idc == 144 )
	{
		sps->chroma_format_idc = bs_read_ue(b);
		sps->ChromaArrayType = sps->chroma_format_idc;
		if( sps->chroma_format_idc == 3 )
		{
			sps->separate_colour_plane_flag = bs_read_u1(b);
			if (sps->separate_colour_plane_flag) sps->ChromaArrayType = 0;
		}
		sps->bit_depth_luma_minus8 = bs_read_ue(b);
		sps->bit_depth_chroma_minus8 = bs_read_ue(b);
		sps->qpprime_y_zero_transform_bypass_flag = bs_read_u1(b);
		sps->seq_scaling_matrix_present_flag = bs_read_u1(b);
		if( sps->seq_scaling_matrix_present_flag )
		{
			for( i = 0; i < ((sps->chroma_format_idc!=3) ? 8 : 12); i++ )
			{
				sps->seq_scaling_list_present_flag[ i ] = bs_read_u1(b);
				if( sps->seq_scaling_list_present_flag[ i ] )
				{
					if( i < 6 )
					{
						read_scaling_list( b, &sps->ScalingList4x4[ i ], 16,
							sps->UseDefaultScalingMatrix4x4Flag[ i ]);
					}
					else
					{
						read_scaling_list( b, &sps->ScalingList8x8[ i - 6 ], 64,
							sps->UseDefaultScalingMatrix8x8Flag[ i - 6 ] );
					}
				}
			}
		}
	}
	sps->log2_max_frame_num_minus4 = bs_read_ue(b);
	sps->pic_order_cnt_type = bs_read_ue(b);
	if( sps->pic_order_cnt_type == 0 )
	{
		sps->log2_max_pic_order_cnt_lsb_minus4 = bs_read_ue(b);
	}
	else if( sps->pic_order_cnt_type == 1 )
	{
		sps->delta_pic_order_always_zero_flag = bs_read_u1(b);
		sps->offset_for_non_ref_pic = bs_read_se(b);
		sps->offset_for_top_to_bottom_field = bs_read_se(b);
		sps->num_ref_frames_in_pic_order_cnt_cycle = bs_read_ue(b);
		for( i = 0; i < sps->num_ref_frames_in_pic_order_cnt_cycle; i++ )
		{
			sps->offset_for_ref_frame[ i ] = bs_read_se(b);
		}
	}
	sps->max_num_ref_frames = bs_read_ue(b);
	sps->gaps_in_frame_num_value_allowed_flag = bs_read_u1(b);
	sps->pic_width_in_mbs_minus1 = bs_read_ue(b);
	sps->pic_height_in_map_units_minus1 = bs_read_ue(b);
	sps->frame_mbs_only_flag = bs_read_u1(b);
	if( !sps->frame_mbs_only_flag )
	{
		sps->mb_adaptive_frame_field_flag = bs_read_u1(b);
	}
	sps->direct_8x8_inference_flag = bs_read_u1(b);
	sps->frame_cropping_flag = bs_read_u1(b);
	if( sps->frame_cropping_flag )
	{
		sps->frame_crop_left_offset = bs_read_ue(b);
		sps->frame_crop_right_offset = bs_read_ue(b);
		sps->frame_crop_top_offset = bs_read_ue(b);
		sps->frame_crop_bottom_offset = bs_read_ue(b);
	}
	sps->vui_parameters_present_flag = bs_read_u1(b);
	if( sps->vui_parameters_present_flag )
	{
		read_vui_parameters(sps, b);
		/* 注：这里的帧率计算还有问题，x264编码25fps，time_scale为50，num_units_in_tick为1，计算得50fps
		网上说法，当nuit_field_based_flag为1时，再除以2，又说x264将该值设置为0.
		地址：http://forum.doom9.org/showthread.php?t=153019
		*/
		/*if (sps->vui.num_units_in_tick != 0)
		h->info->max_framerate = (float)(sps->vui.time_scale) / (float)(sps->vui.num_units_in_tick);*/
	}
	//read_rbsp_trailing_bits(h, b);
}

//7.3.2.1.1 Scaling list syntax
void Nalu264::read_scaling_list(bs_t* b, int* scalingList, int sizeOfScalingList, int useDefaultScalingMatrixFlag )
{
	int j;
	if(scalingList == NULL)
	{
		return;
	}

	int lastScale = 8;
	int nextScale = 8;
	for( j = 0; j < sizeOfScalingList; j++ )
	{
		if( nextScale != 0 )
		{
			int delta_scale = bs_read_se(b);
			nextScale = ( lastScale + delta_scale + 256 ) % 256;
			useDefaultScalingMatrixFlag = ( j == 0 && nextScale == 0 );
		}
		scalingList[ j ] = ( nextScale == 0 ) ? lastScale : nextScale;
		lastScale = scalingList[ j ];
	}
}

//Appendix E.1.1 VUI parameters syntax
void Nalu264::read_vui_parameters(h264_sps_t* sps, bs_t* b)
{
	sps->vui.aspect_ratio_info_present_flag = bs_read_u1(b);
	if( sps->vui.aspect_ratio_info_present_flag )
	{
		sps->vui.aspect_ratio_idc = bs_read_u8(b);
		if( sps->vui.aspect_ratio_idc == SAR_Extended )
		{
			sps->vui.sar_width = bs_read_u(b,16);
			sps->vui.sar_height = bs_read_u(b,16);
		}
	}
	sps->vui.overscan_info_present_flag = bs_read_u1(b);
	if( sps->vui.overscan_info_present_flag )
	{
		sps->vui.overscan_appropriate_flag = bs_read_u1(b);
	}
	sps->vui.video_signal_type_present_flag = bs_read_u1(b);
	if( sps->vui.video_signal_type_present_flag )
	{
		sps->vui.video_format = bs_read_u(b,3);
		sps->vui.video_full_range_flag = bs_read_u1(b);
		sps->vui.colour_description_present_flag = bs_read_u1(b);
		if( sps->vui.colour_description_present_flag )
		{
			sps->vui.colour_primaries = bs_read_u8(b);
			sps->vui.transfer_characteristics = bs_read_u8(b);
			sps->vui.matrix_coefficients = bs_read_u8(b);
		}
	}
	sps->vui.chroma_loc_info_present_flag = bs_read_u1(b);
	if( sps->vui.chroma_loc_info_present_flag )
	{
		sps->vui.chroma_sample_loc_type_top_field = bs_read_ue(b);
		sps->vui.chroma_sample_loc_type_bottom_field = bs_read_ue(b);
	}
	sps->vui.timing_info_present_flag = bs_read_u1(b);
	if( sps->vui.timing_info_present_flag )
	{
		sps->vui.num_units_in_tick = bs_read_u(b,32);
		sps->vui.time_scale = bs_read_u(b,32);
		sps->vui.fixed_frame_rate_flag = bs_read_u1(b);
	}
	sps->vui.nal_hrd_parameters_present_flag = bs_read_u1(b);
	if( sps->vui.nal_hrd_parameters_present_flag )
	{
		read_hrd_parameters(sps, b);
	}
	sps->vui.vcl_hrd_parameters_present_flag = bs_read_u1(b);
	if( sps->vui.vcl_hrd_parameters_present_flag )
	{
		read_hrd_parameters(sps, b);
	}
	if( sps->vui.nal_hrd_parameters_present_flag || sps->vui.vcl_hrd_parameters_present_flag )
	{
		sps->vui.low_delay_hrd_flag = bs_read_u1(b);
	}
	sps->vui.pic_struct_present_flag = bs_read_u1(b);
	sps->vui.bitstream_restriction_flag = bs_read_u1(b);
	if( sps->vui.bitstream_restriction_flag )
	{
		sps->vui.motion_vectors_over_pic_boundaries_flag = bs_read_u1(b);
		sps->vui.max_bytes_per_pic_denom = bs_read_ue(b);
		sps->vui.max_bits_per_mb_denom = bs_read_ue(b);
		sps->vui.log2_max_mv_length_horizontal = bs_read_ue(b);
		sps->vui.log2_max_mv_length_vertical = bs_read_ue(b);
		sps->vui.num_reorder_frames = bs_read_ue(b);
		sps->vui.max_dec_frame_buffering = bs_read_ue(b);
	}
}


//Appendix E.1.2 HRD parameters syntax
void Nalu264::read_hrd_parameters(h264_sps_t* sps, bs_t* b)
{
	int SchedSelIdx;

	sps->hrd.cpb_cnt_minus1 = bs_read_ue(b);
	sps->hrd.bit_rate_scale = bs_read_u(b,4);
	sps->hrd.cpb_size_scale = bs_read_u(b,4);
	for( SchedSelIdx = 0; SchedSelIdx <= sps->hrd.cpb_cnt_minus1; SchedSelIdx++ )
	{
		sps->hrd.bit_rate_value_minus1[ SchedSelIdx ] = bs_read_ue(b);
		sps->hrd.cpb_size_value_minus1[ SchedSelIdx ] = bs_read_ue(b);
		sps->hrd.cbr_flag[ SchedSelIdx ] = bs_read_u1(b);
	}
	sps->hrd.initial_cpb_removal_delay_length_minus1 = bs_read_u(b,5);
	sps->hrd.cpb_removal_delay_length_minus1 = bs_read_u(b,5);
	sps->hrd.dpb_output_delay_length_minus1 = bs_read_u(b,5);
	sps->hrd.time_offset_length = bs_read_u(b,5);
}

//7.3.2.2 Picture parameter set RBSP syntax
void Nalu264::read_pic_parameter_set_rbsp(h264_pps_t* pps, h264_sps_t* sps, bs_t* b)
{
	int i = 0, i_group = 0;

	pps->pic_parameter_set_id = bs_read_ue(b);
	pps->seq_parameter_set_id = bs_read_ue(b);
	pps->entropy_coding_mode_flag = bs_read_u1(b);

	//2005年版本此处为pps->pic_order_present_flag，2013年版本为bottom_field_pic_order_in_frame_present_flag
	pps->bottom_field_pic_order_in_frame_present_flag = bs_read_u1(b);
	pps->num_slice_groups_minus1 = bs_read_ue(b);

	if( pps->num_slice_groups_minus1 > 0 )
	{
		pps->slice_group_map_type = bs_read_ue(b);
		if( pps->slice_group_map_type == 0 )
		{
			for( i_group = 0; i_group <= pps->num_slice_groups_minus1; i_group++ )
			{
				pps->run_length_minus1[ i_group ] = bs_read_ue(b);
			}
		}
		else if( pps->slice_group_map_type == 2 )
		{
			for( i_group = 0; i_group < pps->num_slice_groups_minus1; i_group++ )
			{
				pps->top_left[ i_group ] = bs_read_ue(b);
				pps->bottom_right[ i_group ] = bs_read_ue(b);
			}
		}
		else if( pps->slice_group_map_type == 3 ||
			pps->slice_group_map_type == 4 ||
			pps->slice_group_map_type == 5 )
		{
			pps->slice_group_change_direction_flag = bs_read_u1(b);
			pps->slice_group_change_rate_minus1 = bs_read_ue(b);
		}
		else if( pps->slice_group_map_type == 6 )
		{
			pps->pic_size_in_map_units_minus1 = bs_read_ue(b);
			pps->slice_group_id_bytes = intlog2( pps->num_slice_groups_minus1 + 1 );
			pps->slice_group_id.resize(pps->pic_size_in_map_units_minus1 + 1);
			for( i = 0; i <= pps->pic_size_in_map_units_minus1; i++ )
			{
				pps->slice_group_id[ i ] = bs_read_u(b,  pps->slice_group_id_bytes); // was u(v)
			}
		}
	}

	pps->num_ref_idx_l0_active_minus1 = bs_read_ue(b);
	pps->num_ref_idx_l1_active_minus1 = bs_read_ue(b);
	pps->weighted_pred_flag = bs_read_u1(b);
	pps->weighted_bipred_idc = bs_read_u(b,2);
	pps->pic_init_qp_minus26 = bs_read_se(b);
	pps->pic_init_qs_minus26 = bs_read_se(b);
	pps->chroma_qp_index_offset = bs_read_se(b);
	pps->deblocking_filter_control_present_flag = bs_read_u1(b);
	pps->constrained_intra_pred_flag = bs_read_u1(b);
	pps->redundant_pic_cnt_present_flag = bs_read_u1(b);

	pps->_more_rbsp_data_present = more_rbsp_data_in_pps(pps, sps, b, sps->profile_idc);
	if( pps->_more_rbsp_data_present )
	{
		pps->transform_8x8_mode_flag = bs_read_u1(b);
		pps->pic_scaling_matrix_present_flag = bs_read_u1(b);
		if( pps->pic_scaling_matrix_present_flag )
		{
			for( i = 0; i < 6 + 2* pps->transform_8x8_mode_flag; i++ )
			{
				pps->pic_scaling_list_present_flag[ i ] = bs_read_u1(b);
				if( pps->pic_scaling_list_present_flag[ i ] )
				{
					if( i < 6 )
					{
						read_scaling_list( b, pps->ScalingList4x4[ i ], 16,
							pps->UseDefaultScalingMatrix4x4Flag[ i ] );
					}
					else
					{
						read_scaling_list( b, pps->ScalingList8x8[ i - 6 ], 64,
							pps->UseDefaultScalingMatrix8x8Flag[ i - 6 ] );
					}
				}
			}
		}
		pps->second_chroma_qp_index_offset = bs_read_se(b);
	}
	//read_rbsp_trailing_bits(h, b);
}

int Nalu264::intlog2(int x)
{
	int log = 0;
	if (x < 0) { x = 0; }
	while ((x >> log) > 0)
	{
		log++;
	}
	if (log > 0 && x == 1<<(log-1)) { log--; }
	return log;
}

int Nalu264::more_rbsp_data_in_pps(h264_pps_t* pps, h264_sps_t* sps, bs_t* b, int profile_idc) 
{
	//int profile_idc = profile_idc;//h->sps->profile_idc;
	int constraint_set_flags = 0;
	constraint_set_flags |= sps->constraint_set0_flag << 0;   // constraint_set0_flag
	constraint_set_flags |= sps->constraint_set1_flag << 1;   // constraint_set1_flag
	constraint_set_flags |= sps->constraint_set2_flag << 2;   // constraint_set2_flag
	constraint_set_flags |= sps->constraint_set3_flag << 3;   // constraint_set3_flag
	constraint_set_flags |= sps->constraint_set4_flag << 4;   // constraint_set4_flag
	constraint_set_flags |= sps->constraint_set5_flag << 5;   // constraint_set5_flag

	if ((profile_idc == 66 || profile_idc == 77 ||
		profile_idc == 88) && (constraint_set_flags & 7)) 
	{
		return 0;
	}
	return 1;
}

//7.3.2.8 Slice layer without partitioning RBSP syntax
void Nalu264::read_slice_layer_rbsp(slice_header_t* sh, slice_data_rbsp_t* slice_data,
	h264_sps_t* sps, h264_pps_t* pps, bs_t* b)
{
	read_slice_header(sh, sps, pps, b);
	if ( slice_data != NULL )
	{
		if ( slice_data->rbsp_buf != NULL ) MemoryAllocator::Free( slice_data->rbsp_buf ); 
		uint8_t *sptr = b->p + (!!b->bits_left); // CABAC-specific: skip alignment bits, if there are any
		slice_data->rbsp_size = b->end - sptr;

		slice_data->rbsp_buf = (uint8_t*)MemoryAllocator::Alloc(slice_data->rbsp_size);
		memcpy( slice_data->rbsp_buf, sptr, slice_data->rbsp_size );
		// ugly hack: since next NALU starts at byte border, we are going to be padded by trailing_bits;
		return;
	}

	// FIXME should read or skip data
	//slice_data( ); /* all categories of slice_data( ) syntax */
	//read_rbsp_slice_trailing_bits(h, b);
}

//7.3.3 Slice header syntax
void Nalu264::read_slice_header(slice_header_t* sh, h264_sps_t* sps, h264_pps_t* pps, bs_t* b)
{
	//int read_slice_type = sh->read_slice_type;

	memset(sh, 0, sizeof(slice_header_t));

	sh->first_mb_in_slice = bs_read_ue(b);
	sh->slice_type = bs_read_ue(b);

	//if (read_slice_type) return;

	sh->pic_parameter_set_id = bs_read_ue(b);

	if (sps->separate_colour_plane_flag == 1)
	{
		sh->colour_plane_id = bs_read_u(b, 2);
	}
	sh->frame_num_bytes = sps->log2_max_frame_num_minus4 + 4;
	sh->frame_num = bs_read_u(b,  sh->frame_num_bytes); // was u(v)
	if( !sps->frame_mbs_only_flag )
	{
		sh->field_pic_flag = bs_read_u1(b);
		if( sh->field_pic_flag )
		{
			sh->bottom_field_flag = bs_read_u1(b);
		}
	}
	if( _Head.TYPE == 5 )
	{
		sh->idr_pic_id = bs_read_ue(b);
	}
	if( sps->pic_order_cnt_type == 0 )
	{
		sh->pic_order_cnt_lsb_bytes = sps->log2_max_pic_order_cnt_lsb_minus4 + 4;
		sh->pic_order_cnt_lsb = bs_read_u(b,  sh->pic_order_cnt_lsb_bytes); // was u(v)
		if( pps->bottom_field_pic_order_in_frame_present_flag && !sh->field_pic_flag )
		{
			sh->delta_pic_order_cnt_bottom = bs_read_se(b);
		}
	}
	if( sps->pic_order_cnt_type == 1 && !sps->delta_pic_order_always_zero_flag )
	{
		sh->delta_pic_order_cnt[ 0 ] = bs_read_se(b);
		if( pps->bottom_field_pic_order_in_frame_present_flag && !sh->field_pic_flag )
		{
			sh->delta_pic_order_cnt[ 1 ] = bs_read_se(b);
		}
	}
	if( pps->redundant_pic_cnt_present_flag )
	{
		sh->redundant_pic_cnt = bs_read_ue(b);
	}
	if( is_slice_type( sh->slice_type, SH_SLICE_TYPE_B ) )
	{
		sh->direct_spatial_mv_pred_flag = bs_read_u1(b);
	}
	if( is_slice_type( sh->slice_type, SH_SLICE_TYPE_P ) || is_slice_type( sh->slice_type, SH_SLICE_TYPE_SP ) || is_slice_type( sh->slice_type, SH_SLICE_TYPE_B ) )
	{
		sh->num_ref_idx_active_override_flag = bs_read_u1(b);
		if( sh->num_ref_idx_active_override_flag )
		{
			sh->num_ref_idx_l0_active_minus1 = bs_read_ue(b); // FIXME does this modify the pps?
			if( is_slice_type( sh->slice_type, SH_SLICE_TYPE_B ) )
			{
				sh->num_ref_idx_l1_active_minus1 = bs_read_ue(b);
			}
		}
	}
	if (_Head.TYPE == 20 || _Head.TYPE == 21)
	{
		// todo
		// read_ref_pic_list_mvc_modification(h, b);
	}
	else
	{
		read_ref_pic_list_modification(sh, b);
	}

	if( ( pps->weighted_pred_flag && ( is_slice_type( sh->slice_type, SH_SLICE_TYPE_P ) || is_slice_type( sh->slice_type, SH_SLICE_TYPE_SP ) ) ) ||
		( pps->weighted_bipred_idc == 1 && is_slice_type( sh->slice_type, SH_SLICE_TYPE_B ) ) )
	{
		read_pred_weight_table(sh, sps, pps, b);
	}
	if( _Head.NRI != 0 )
	{
		read_dec_ref_pic_marking(sh, b);
	}
	if( pps->entropy_coding_mode_flag && ! is_slice_type( sh->slice_type, SH_SLICE_TYPE_I ) && ! is_slice_type( sh->slice_type, SH_SLICE_TYPE_SI ) )
	{
		sh->cabac_init_idc = bs_read_ue(b);
	}
	sh->slice_qp_delta = bs_read_se(b);
	if( is_slice_type( sh->slice_type, SH_SLICE_TYPE_SP ) || is_slice_type( sh->slice_type, SH_SLICE_TYPE_SI ) )
	{
		if( is_slice_type( sh->slice_type, SH_SLICE_TYPE_SP ) )
		{
			sh->sp_for_switch_flag = bs_read_u1(b);
		}
		sh->slice_qs_delta = bs_read_se(b);
	}
	if( pps->deblocking_filter_control_present_flag )
	{
		sh->disable_deblocking_filter_idc = bs_read_ue(b);
		if( sh->disable_deblocking_filter_idc != 1 )
		{
			sh->slice_alpha_c0_offset_div2 = bs_read_se(b);
			sh->slice_beta_offset_div2 = bs_read_se(b);
		}
	}
	if( pps->num_slice_groups_minus1 > 0 &&
		pps->slice_group_map_type >= 3 && pps->slice_group_map_type <= 5)
	{
		sh->slice_group_change_cycle_bytes = intlog2( pps->pic_size_in_map_units_minus1 +  
			pps->slice_group_change_rate_minus1 + 1 );
		sh->slice_group_change_cycle = bs_read_u(b,  sh->slice_group_change_cycle_bytes); // was u(v) // FIXME add 2?
	}
	// bs_print_state(b);
}

int Nalu264::is_slice_type(int slice_type, int cmp_type)
{
	if (slice_type >= 5) { slice_type -= 5; }
	if (cmp_type >= 5) { cmp_type -= 5; }
	if (slice_type == cmp_type) { return 1; }
	else { return 0; }
}

//7.3.3.1 Reference picture list modification syntax
void Nalu264::read_ref_pic_list_modification(slice_header_t* sh, bs_t* b)
{
	rplm_tt rplmtt;

	if( ! is_slice_type( sh->slice_type, SH_SLICE_TYPE_I ) && ! is_slice_type( sh->slice_type, SH_SLICE_TYPE_SI ) )
	{
		sh->rplm.ref_pic_list_modification_flag_l0 = bs_read_u1(b);
		if( sh->rplm.ref_pic_list_modification_flag_l0 )
		{
			do
			{
				rplmtt.modification_of_pic_nums_idc = bs_read_ue(b);
				if( rplmtt.modification_of_pic_nums_idc == 0 ||
					rplmtt.modification_of_pic_nums_idc == 1 )
				{
					rplmtt.abs_diff_pic_num_minus1 = bs_read_ue(b);
				}
				else if( rplmtt.modification_of_pic_nums_idc == 2 )
				{
					rplmtt.long_term_pic_num = bs_read_ue(b);
				}
				sh->rplm.rplm.push_back(rplmtt);
			} while( rplmtt.modification_of_pic_nums_idc != 3 && ! bs_eof(b) );
		}
	}
	if( is_slice_type( sh->slice_type, SH_SLICE_TYPE_B ) )
	{
		sh->rplm.ref_pic_list_modification_flag_l1 = bs_read_u1(b);
		if( sh->rplm.ref_pic_list_modification_flag_l1 )
		{
			do
			{
				rplmtt.modification_of_pic_nums_idc = bs_read_ue(b);
				if( rplmtt.modification_of_pic_nums_idc == 0 ||
					rplmtt.modification_of_pic_nums_idc == 1 )
				{
					rplmtt.abs_diff_pic_num_minus1 = bs_read_ue(b);
				}
				else if( rplmtt.modification_of_pic_nums_idc == 2 )
				{
					rplmtt.long_term_pic_num = bs_read_ue(b);
				}
				sh->rplm.rplm.push_back(rplmtt);
			} while( rplmtt.modification_of_pic_nums_idc != 3 && ! bs_eof(b) );
		}
	}
}

//7.3.3.2 Prediction weight table syntax
void Nalu264::read_pred_weight_table(slice_header_t* sh, h264_sps_t* sps, h264_pps_t* pps, bs_t* b)
{
	int i, j;

	sh->pwt.luma_log2_weight_denom = bs_read_ue(b);
	if( sps->ChromaArrayType != 0 )
	{
		sh->pwt.chroma_log2_weight_denom = bs_read_ue(b);
	}
	for( i = 0; i <= sh->num_ref_idx_l0_active_minus1; i++ )
	{
		sh->pwt.luma_weight_l0_flag[i] = bs_read_u1(b);
		if( sh->pwt.luma_weight_l0_flag[i] )
		{
			sh->pwt.luma_weight_l0[ i ] = bs_read_se(b);
			sh->pwt.luma_offset_l0[ i ] = bs_read_se(b);
		}
		if ( sps->ChromaArrayType != 0 )
		{
			sh->pwt.chroma_weight_l0_flag[i] = bs_read_u1(b);
			if( sh->pwt.chroma_weight_l0_flag[i] )
			{
				for( j =0; j < 2; j++ )
				{
					sh->pwt.chroma_weight_l0[ i ][ j ] = bs_read_se(b);
					sh->pwt.chroma_offset_l0[ i ][ j ] = bs_read_se(b);
				}
			}
		}
	}
	if( is_slice_type( sh->slice_type, SH_SLICE_TYPE_B ) )
	{
		for( i = 0; i <= sh->num_ref_idx_l1_active_minus1; i++ )
		{
			sh->pwt.luma_weight_l1_flag[i] = bs_read_u1(b);
			if( sh->pwt.luma_weight_l1_flag[i] )
			{
				sh->pwt.luma_weight_l1[ i ] = bs_read_se(b);
				sh->pwt.luma_offset_l1[ i ] = bs_read_se(b);
			}
			if( sps->ChromaArrayType != 0 )
			{
				sh->pwt.chroma_weight_l1_flag[i] = bs_read_u1(b);
				if( sh->pwt.chroma_weight_l1_flag[i] )
				{
					for( j = 0; j < 2; j++ )
					{
						sh->pwt.chroma_weight_l1[ i ][ j ] = bs_read_se(b);
						sh->pwt.chroma_offset_l1[ i ][ j ] = bs_read_se(b);
					}
				}
			}
		}
	}
}

//7.3.3.3 Decoded reference picture marking syntax
void Nalu264::read_dec_ref_pic_marking(slice_header_t* sh, bs_t* b)
{
	drpm_tt drpmtt;

	if( _Head.TYPE == 5 )
	{
		sh->drpm.no_output_of_prior_pics_flag = bs_read_u1(b);
		sh->drpm.long_term_reference_flag = bs_read_u1(b);
	}
	else
	{
		sh->drpm.adaptive_ref_pic_marking_mode_flag = bs_read_u1(b);
		if( sh->drpm.adaptive_ref_pic_marking_mode_flag )
		{
			do
			{
				drpmtt.memory_management_control_operation = bs_read_ue(b);
				if( drpmtt.memory_management_control_operation == 1 ||
					drpmtt.memory_management_control_operation == 3 )
				{
					drpmtt.difference_of_pic_nums_minus1 = bs_read_ue(b);
				}
				if(drpmtt.memory_management_control_operation == 2 )
				{
					drpmtt.long_term_pic_num = bs_read_ue(b);
				}
				if( drpmtt.memory_management_control_operation == 3 ||
					drpmtt.memory_management_control_operation == 6 )
				{
					drpmtt.long_term_frame_idx = bs_read_ue(b);
				}
				if( drpmtt.memory_management_control_operation == 4 )
				{
					drpmtt.max_long_term_frame_idx_plus1 = bs_read_ue(b);
				}
				sh->drpm.drpm.push_back(drpmtt);
			} while( drpmtt.memory_management_control_operation != 0 && ! bs_eof(b) );
		}
	}
}