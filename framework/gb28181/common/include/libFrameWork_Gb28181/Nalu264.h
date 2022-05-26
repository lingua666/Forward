
#ifndef __NALU_20171226154204_3344199361_H__
#define __NALU_20171226154204_3344199361_H__

	#include "bs.h"
	#include "External.h"
	#include <vector>

	//Table 7-1 NAL unit type codes
	#define NAL_UNIT_TYPE_UNSPECIFIED                    0    // Unspecified
	#define NAL_UNIT_TYPE_CODED_SLICE_NON_IDR            1    // Coded slice of a non-IDR picture
	#define NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_A   2    // Coded slice data partition A
	#define NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_B   3    // Coded slice data partition B
	#define NAL_UNIT_TYPE_CODED_SLICE_DATA_PARTITION_C   4    // Coded slice data partition C
	#define NAL_UNIT_TYPE_CODED_SLICE_IDR                5    // Coded slice of an IDR picture
	#define NAL_UNIT_TYPE_SEI                            6    // Supplemental enhancement information (SEI)
	#define NAL_UNIT_TYPE_SPS                            7    // Sequence parameter set
	#define NAL_UNIT_TYPE_PPS                            8    // Picture parameter set
	#define NAL_UNIT_TYPE_AUD                            9    // Access unit delimiter
	#define NAL_UNIT_TYPE_END_OF_SEQUENCE               10    // End of sequence
	#define NAL_UNIT_TYPE_END_OF_STREAM                 11    // End of stream
	#define NAL_UNIT_TYPE_FILLER                        12    // Filler data
	#define NAL_UNIT_TYPE_SPS_EXT                       13    // Sequence parameter set extension
															  // 14..18    // Reserved
	#define NAL_UNIT_TYPE_CODED_SLICE_AUX               19    // Coded slice of an auxiliary coded picture without partitioning
															  // 20..23    // Reserved
	#define NAL_UNIT_TYPE_STAP_A						24	  // Ingle-time aggregation packet
	#define NAL_UNIT_TYPE_STAP_B						25	  // Single-time aggregation packet
	#define NAL_UNIT_TYPE_MTAPl6						26	  // Multi-time aggregation packet
	#define NAL_UNIT_TYPE_MTAP24						27	  // Multi-time aggregation packet
	#define NAL_UNIT_TYPE_FU_A							28	  // Fragmentation unit
	#define NAL_UNIT_TYPE_FU_B							29	  // Fragmentation unit
															  // 30..31    // Unspecified

	//7.4.3 Table 7-6. Name association to slice_type
	#define SH_SLICE_TYPE_P        0        // P (P slice)
	#define SH_SLICE_TYPE_B        1        // B (B slice)
	#define SH_SLICE_TYPE_I        2        // I (I slice)
	#define SH_SLICE_TYPE_SP       3        // SP (SP slice)
	#define SH_SLICE_TYPE_SI       4        // SI (SI slice)
	//as per footnote to Table 7-6, the *_ONLY slice types indicate that all other slices in that picture are of the same type
	#define SH_SLICE_TYPE_P_ONLY    5        // P (P slice)
	#define SH_SLICE_TYPE_B_ONLY    6        // B (B slice)
	#define SH_SLICE_TYPE_I_ONLY    7        // I (I slice)
	#define SH_SLICE_TYPE_SP_ONLY   8        // SP (SP slice)
	#define SH_SLICE_TYPE_SI_ONLY   9        // SI (SI slice)

	//Appendix E. Table E-1  Meaning of sample aspect ratio indicator
	#define SAR_Unspecified  0           // Unspecified
	#define SAR_1_1        1             //  1:1
	#define SAR_12_11      2             // 12:11
	#define SAR_10_11      3             // 10:11
	#define SAR_16_11      4             // 16:11
	#define SAR_40_33      5             // 40:33
	#define SAR_24_11      6             // 24:11
	#define SAR_20_11      7             // 20:11
	#define SAR_32_11      8             // 32:11
	#define SAR_80_33      9             // 80:33
	#define SAR_18_11     10             // 18:11
	#define SAR_15_11     11             // 15:11
	#define SAR_64_33     12             // 64:33
	#define SAR_160_99    13             // 160:99
	// 14..254           Reserved
	#define SAR_Extended      255        // Extended_SAR

#pragma pack(push, 1)

/**
   Sequence Parameter Set
   @see 7.3.2.1 Sequence parameter set RBSP syntax
   @see read_seq_parameter_set_rbsp
   @see write_seq_parameter_set_rbsp
   @see debug_sps
*/
typedef struct
{
	int profile_idc;
	int constraint_set0_flag;
	int constraint_set1_flag;
	int constraint_set2_flag;
	int constraint_set3_flag;
	int constraint_set4_flag;
	int constraint_set5_flag;
	int reserved_zero_2bits;
	int level_idc;
	int seq_parameter_set_id;
	int chroma_format_idc;
	int separate_colour_plane_flag;
	int ChromaArrayType;
	int bit_depth_luma_minus8;
	int bit_depth_chroma_minus8;
	int qpprime_y_zero_transform_bypass_flag;
	int seq_scaling_matrix_present_flag;
	int seq_scaling_list_present_flag[8];
	int ScalingList4x4[6];
	int UseDefaultScalingMatrix4x4Flag[6];
	int ScalingList8x8[2];
	int UseDefaultScalingMatrix8x8Flag[2];
	int log2_max_frame_num_minus4;
	int pic_order_cnt_type;
	int log2_max_pic_order_cnt_lsb_minus4;
	int delta_pic_order_always_zero_flag;
	int offset_for_non_ref_pic;
	int offset_for_top_to_bottom_field;
	int num_ref_frames_in_pic_order_cnt_cycle;
	int offset_for_ref_frame[256];
	int max_num_ref_frames;
	int gaps_in_frame_num_value_allowed_flag;
	int pic_width_in_mbs_minus1;
	int pic_height_in_map_units_minus1;
	int frame_mbs_only_flag;
	int mb_adaptive_frame_field_flag;
	int direct_8x8_inference_flag;
	int frame_cropping_flag;
	int frame_crop_left_offset;
	int frame_crop_right_offset;
	int frame_crop_top_offset;
	int frame_crop_bottom_offset;
	int vui_parameters_present_flag;

	struct
	{
		int aspect_ratio_info_present_flag;
		int aspect_ratio_idc;
		int sar_width;
		int sar_height;
		int overscan_info_present_flag;
		int overscan_appropriate_flag;
		int video_signal_type_present_flag;
		int video_format;
		int video_full_range_flag;
		int colour_description_present_flag;
		int colour_primaries;
		int transfer_characteristics;
		int matrix_coefficients;
		int chroma_loc_info_present_flag;
		int chroma_sample_loc_type_top_field;
		int chroma_sample_loc_type_bottom_field;
		int timing_info_present_flag;
		int num_units_in_tick;
		int time_scale;
		int fixed_frame_rate_flag;
		int nal_hrd_parameters_present_flag;
		int vcl_hrd_parameters_present_flag;
		int low_delay_hrd_flag;
		int pic_struct_present_flag;
		int bitstream_restriction_flag;
		int motion_vectors_over_pic_boundaries_flag;
		int max_bytes_per_pic_denom;
		int max_bits_per_mb_denom;
		int log2_max_mv_length_horizontal;
		int log2_max_mv_length_vertical;
		int num_reorder_frames;
		int max_dec_frame_buffering;
	} vui;

	struct
	{
		int cpb_cnt_minus1;
		int bit_rate_scale;
		int cpb_size_scale;
		int bit_rate_value_minus1[32]; // up to cpb_cnt_minus1, which is <= 31
		int cpb_size_value_minus1[32];
		int cbr_flag[32];
		int initial_cpb_removal_delay_length_minus1;
		int cpb_removal_delay_length_minus1;
		int dpb_output_delay_length_minus1;
		int time_offset_length;
	} hrd;

} h264_sps_t;

/**
   Picture Parameter Set
   @see 7.3.2.2 Picture parameter set RBSP syntax
   @see read_pic_parameter_set_rbsp
   @see write_pic_parameter_set_rbsp
   @see debug_pps
*/
typedef struct 
{
	int pic_parameter_set_id;
	int seq_parameter_set_id;
	int entropy_coding_mode_flag;
	int pic_order_present_flag; // 2005版本为此字段名 保留，不影响库本身write的编译，但实际不使用
	int bottom_field_pic_order_in_frame_present_flag; // 2013版本为此字段名
	int num_slice_groups_minus1;
	int slice_group_map_type;
	int run_length_minus1[8]; // up to num_slice_groups_minus1, which is <= 7 in Baseline and Extended, 0 otheriwse
	int top_left[8];
	int bottom_right[8];
	int slice_group_change_direction_flag;
	int slice_group_change_rate_minus1;
	int pic_size_in_map_units_minus1;
	int slice_group_id_bytes;
	std::vector<int> slice_group_id; // FIXME what size?
	int num_ref_idx_l0_active_minus1;
	int num_ref_idx_l1_active_minus1;
	int weighted_pred_flag;
	int weighted_bipred_idc;
	int pic_init_qp_minus26;
	int pic_init_qs_minus26;
	int chroma_qp_index_offset;
	int deblocking_filter_control_present_flag;
	int constrained_intra_pred_flag;
	int redundant_pic_cnt_present_flag;

    // see if we carry any of the optional headers
	int _more_rbsp_data_present;
	int transform_8x8_mode_flag;
	int pic_scaling_matrix_present_flag;
	int pic_scaling_list_present_flag[8];
	int* ScalingList4x4[6];
	int UseDefaultScalingMatrix4x4Flag[6];
	int* ScalingList8x8[2];
	int UseDefaultScalingMatrix8x8Flag[2];
	int second_chroma_qp_index_offset;
} h264_pps_t;

// predictive weight table
typedef struct
{
	int luma_log2_weight_denom;
	int chroma_log2_weight_denom;
	int luma_weight_l0_flag[64];
	int luma_weight_l0[64];
	int luma_offset_l0[64];
	int chroma_weight_l0_flag[64];
	int chroma_weight_l0[64][2];
	int chroma_offset_l0[64][2];
	int luma_weight_l1_flag[64];
	int luma_weight_l1[64];
	int luma_offset_l1[64];
	int chroma_weight_l1_flag[64];
	int chroma_weight_l1[64][2];
	int chroma_offset_l1[64][2];
} pwt_t;

// ref pic list modification
typedef struct
{
	int modification_of_pic_nums_idc;
	int abs_diff_pic_num_minus1;
	int long_term_pic_num;
} rplm_tt;

typedef struct
{
	int ref_pic_list_modification_flag_l0;
	int ref_pic_list_modification_flag_l1;

	std::vector<rplm_tt> rplm;
} rplm_t;

// decoded ref pic marking
typedef struct
{
	int memory_management_control_operation;
	int difference_of_pic_nums_minus1;
	int long_term_pic_num;
	int long_term_frame_idx;
	int max_long_term_frame_idx_plus1;
} drpm_tt;

typedef struct
{
	int no_output_of_prior_pics_flag;
	int long_term_reference_flag;
	int adaptive_ref_pic_marking_mode_flag;

	std::vector<drpm_tt> drpm;
} drpm_t;

/**
  Slice Header
  @see 7.3.3 Slice header syntax
  @see read_slice_header_rbsp
  @see write_slice_header_rbsp
  @see debug_slice_header_rbsp
*/
typedef struct
{
	int read_slice_type; // see if we only read slice type and return

	int first_mb_in_slice;
	int slice_type;
	int pic_parameter_set_id;
	int colour_plane_id;
	int frame_num_bytes;
	int frame_num;
	int field_pic_flag;
	int bottom_field_flag;
	int idr_pic_id;
	int pic_order_cnt_lsb_bytes;
	int pic_order_cnt_lsb;
	int delta_pic_order_cnt_bottom;
	int delta_pic_order_cnt[ 2 ];
	int redundant_pic_cnt;
	int direct_spatial_mv_pred_flag;
	int num_ref_idx_active_override_flag;
	int num_ref_idx_l0_active_minus1;
	int num_ref_idx_l1_active_minus1;
	int cabac_init_idc;
	int slice_qp_delta;
	int sp_for_switch_flag;
	int slice_qs_delta;
	int disable_deblocking_filter_idc;
	int slice_alpha_c0_offset_div2;
	int slice_beta_offset_div2;
	int slice_group_change_cycle_bytes;
	int slice_group_change_cycle;
	
	pwt_t pwt;
	rplm_t rplm;
	drpm_t drpm;
} slice_header_t;

typedef struct
{
	int rbsp_size;
	uint8_t* rbsp_buf;
} slice_data_rbsp_t;

#if defined( LITTLE_ENDIAN_DEF )

	struct tagNALU_HEADER
	{
		unsigned char TYPE	: 5;	//H.264 中, 只取 1~23 是有效的值,单个NAL单元包
									//0		未规定
									//1		非IDR图像中不采用数据划分的片段
									//2		非IDR图像中A类数据划分片段
									//3		非IDR图像中B类数据划分片段
									//4		非IDR图像中C类数据划分片段
									//5		IDR图像的片段
									//6		补充增强信息（SEI）
									//7		序列参数集（SPS）
									//8		图像参数集（PPS）
									//9		分割符
									//10	序列结束符
									//11	流结束符
									//12	填充数据
									//13	序列参数集扩展
									//14	带前缀的NAL单元
									//15	子序列参数集
									//16–18：保留
									//19	不采用数据划分的辅助编码图像片段
									//20	编码片段扩展
									//24    STAP-A  单一时间的组合包（Single-time aggregation packet）
									//25    STAP-B  单一时间的组合包
									//26    MTAP16  多个时间的组合包（Multi-time aggregation packet）
									//27    MTAP24  多个时间的组合包
									//28    FU-A    分片的单元 (Fragmentation unit)
									//29    FU-B    分片的单元
									//30-31			没有定义

		unsigned char NRI	: 2;	//表示NAL的优先级。
									//0～3，取值越大，
									//表示当前NAL越重要，需要优先受到保护。
									//如果当前NAL是属于参考帧的片，或是序列参数集，
									//或是图像参数集这些重要的单位时，本句法元素必需大于0。
									//byte 0

		unsigned char F		: 1;	//在 H.264 规范中规定了这一位必须为 0.
	}; /* 1 BYTES */

	struct tagFU_A_HEADER
	{
		tagNALU_HEADER	Indicator;//NaluH;
		unsigned char	TYPE: 5;	//内容同NaluH成员TYPE
		unsigned char	R: 1;		//保留位必须设置为0，接收者必须忽略该位。
		unsigned char	E: 1;		//当设置成1, 结束位指示分片NAL单元的结束，即, 荷载的最后字节也是分片NAL单元的最后一个字节
		unsigned char	S: 1;		//内当设置成1,开始位指示分片NAL单元的开始。当跟随的FU荷载不是分片NAL单元荷载的开始，开始位设为0。
	};

#endif

#if defined( BIG_ENDIAN_DEF )

	struct tagNALU_HEADER
	{
		//byte 0
		unsigned char F		: 1;	//在 H.264 规范中规定了这一位必须为 0.
		unsigned char NRI	: 2;	//表示NAL的优先级。
		//0～3，取值越大，
		//表示当前NAL越重要，需要优先受到保护。
		//如果当前NAL是属于参考帧的片，或是序列参数集，
		//或是图像参数集这些重要的单位时，本句法元素必需大于0。

		unsigned char TYPE	: 5;	//H.264 中, 只取 1~23 是有效的值,单个NAL单元包
		//0		未规定
		//1		非IDR图像中不采用数据划分的片段
		//2		非IDR图像中A类数据划分片段
		//3		非IDR图像中B类数据划分片段
		//4		非IDR图像中C类数据划分片段
		//5		IDR图像的片段
		//6		补充增强信息（SEI）
		//7		序列参数集（SPS）
		//8		图像参数集（PPS）
		//9		分割符
		//10	序列结束符
		//11	流结束符
		//12	填充数据
		//13	序列参数集扩展
		//14	带前缀的NAL单元
		//15	子序列参数集
		//16–18：保留
		//19	不采用数据划分的辅助编码图像片段
		//20	编码片段扩展
		//24    STAP-A  单一时间的组合包（Single-time aggregation packet）
		//25    STAP-B  单一时间的组合包
		//26    MTAP16  多个时间的组合包（Multi-time aggregation packet）
		//27    MTAP24  多个时间的组合包
		//28    FU-A    分片的单元 (Fragmentation unit)
		//29    FU-B    分片的单元
		//30-31			没有定义
	}; /* 1 BYTES */

	struct tagFU_A_HEADER
	{
		tagNALU_HEADER	Indicator;//NaluH;
		unsigned char	S: 1;		//内当设置成1,开始位指示分片NAL单元的开始。当跟随的FU荷载不是分片NAL单元荷载的开始，开始位设为0。
		unsigned char	E: 1;		//当设置成1, 结束位指示分片NAL单元的结束，即, 荷载的最后字节也是分片NAL单元的最后一个字节
		unsigned char	R: 1;		//保留位必须设置为0，接收者必须忽略该位。
		unsigned char	TYPE: 5;	//内容同NaluH成员TYPE
	};

#endif

#pragma pack(pop)

class Nalu264
{
public:
	Nalu264( void );

	~Nalu264( void );

	int Init( const char* c_szData, UInt32 uLen );

	int GetSPS( h264_sps_t* pSPS );

	int GetPPS( h264_pps_t* pPPS, h264_sps_t* pSPS );

	UInt8 Get_Type( UInt8 uData );

protected:
	// 7.4.1.1 Encapsulation of an SODB within an RBSP
	_string_type rbsp_to_nal(const uint8_t* rbsp_buf, const int* rbsp_size);

	// 7.4.1.1 Encapsulation of an SODB within an RBSP
	_string_type nal_to_rbsp(const int nal_header_size, const uint8_t* nal_buf, int* nal_size);

	//7.3.2.1 Sequence parameter set RBSP syntax
	void read_seq_parameter_set_rbsp(h264_sps_t* sps, bs_t* b);

	//7.3.2.1.1 Scaling list syntax
	void read_scaling_list(bs_t* b, int* scalingList, int sizeOfScalingList, int useDefaultScalingMatrixFlag );

	//Appendix E.1.1 VUI parameters syntax
	void read_vui_parameters(h264_sps_t* sps, bs_t* b);

	//Appendix E.1.2 HRD parameters syntax
	void read_hrd_parameters(h264_sps_t* sps, bs_t* b);

	//7.3.2.2 Picture parameter set RBSP syntax
	void read_pic_parameter_set_rbsp(h264_pps_t* pps, h264_sps_t* sps, bs_t* b);

	int intlog2(int x);

	int more_rbsp_data_in_pps(h264_pps_t* pps, h264_sps_t* sps, bs_t* b, int profile_idc);

	//7.3.2.8 Slice layer without partitioning RBSP syntax
	void read_slice_layer_rbsp(slice_header_t* sh, slice_data_rbsp_t* slice_data,
		h264_sps_t* sps, h264_pps_t* pps, bs_t* b);

	//7.3.3 Slice header syntax
	void read_slice_header(slice_header_t* sh, h264_sps_t* sps, h264_pps_t* pps, bs_t* b);

	int is_slice_type(int slice_type, int cmp_type);

	//7.3.3.1 Reference picture list modification syntax
	void read_ref_pic_list_modification(slice_header_t* sh, bs_t* b);

	//7.3.3.2 Prediction weight table syntax
	void read_pred_weight_table(slice_header_t* sh, h264_sps_t* sps, h264_pps_t* pps, bs_t* b);

	//7.3.3.3 Decoded reference picture marking syntax
	void read_dec_ref_pic_marking(slice_header_t* sh, bs_t* b);

	UInt8 get_type( UInt8 uData );
private:
	tagNALU_HEADER	_Head;
	_string_type	_sRbsp;
};

#endif
