#ifndef _PSUNPACKET_H_
#define _PSUNPACKET_H_

#include "External.h"

// Transport Stream packets are 188 bytes in length.
#define SRS_TS_PACKET_SIZE          188

// The aggregate pure audio for hls, in ts tbn(ms * 90).
#define SRS_CONSTS_HLS_PURE_AUDIO_AGGREGATE 720 * 90

// The pid of ts packet,
// Table 2-3 - PID table, hls-mpeg-ts-iso13818-1.pdf, page 37
// NOTE - The transport packets with PID values 0x0000, 0x0001, and 0x0010-0x1FFE are allowed to carry a PCR.
enum TSPid
{
	// Program Association Table(see Table 2-25).
	TSPidPAT = 0x00,
	// Conditional Access Table (see Table 2-27).
	TSPidCAT = 0x01,
	// Transport Stream Description Table
	TSPidTSDT = 0x02,
	// Reserved
	TSPidReservedStart = 0x03,
	TSPidReservedEnd = 0x0f,
	// May be assigned as network_PID, Program_map_PID, elementary_PID, or for other purposes
	TSPidAppStart = 0x10,
	TSPidAppEnd = 0x1ffe,
	// For null packets (see Table 2-3)
	TSPidNULL = 0x01FFF,
};

// 2.4.4.4 Table_id assignments, hls-mpeg-ts-iso13818-1.pdf, page 62
// The table_id field identifies the contents of a Transport Stream PSI section as shown in Table 2-26.
enum TSPsiId
{
	// program_association_section
	TSPsiIdPas = 0x00,
	// conditional_access_section (CA_section)
	TSPsiIdCas = 0x01,
	// TS_program_map_section
	TSPsiIdPms = 0x02,
	// TS_description_section
	TSPsiIdDs = 0x03,
	// ISO_IEC_14496_scene_description_section
	TSPsiIdSds = 0x04,
	// ISO_IEC_14496_object_descriptor_section
	TSPsiIdOds = 0x05,
	// ITU-T Rec. H.222.0 | ISO/IEC 13818-1 reserved
	TSPsiIdIso138181Start = 0x06,
	TSPsiIdIso138181End = 0x37,
	// Defined in ISO/IEC 13818-6
	TSPsiIdIso138186Start = 0x38,
	TSPsiIdIso138186End = 0x3F,
	// User private
	TSPsiIdUserStart = 0x40,
	TSPsiIdUserEnd = 0xFE,
	// forbidden
	TSPsiIdForbidden = 0xFF,
};

enum TSAdaptationFieldType
{
	// Reserved for future use by ISO/IEC
	TSAdaptationFieldTypeReserved = 0x00,
	// No adaptation_field, payload only
	TSAdaptationFieldTypePayloadOnly = 0x01,
	// Adaptation_field only, no payload	
	TSAdaptationFieldTypeAdaptionOnly = 0x02,
	// Adaptation_field followed by payload
	TSAdaptationFieldTypeBoth = 0x03,
};

// the actually parsed ts pid,
// @see TSPid, some pid, for example, PMT/Video/Audio is specified by PAT or other tables.
enum TSPidApply
{
	TSPidApplyReserved = 0, // TSPidTypeReserved, nothing parsed, used reserved.

	TSPidApplyPAT, // Program associtate table
	TSPidApplyPMT, // Program map table.

	TSPidApplyVideo, // for video
	TSPidApplyAudio, // vor audio
};

// Table 2-29 - Stream type assignments
enum TSStream
{
	// ITU-T | ISO/IEC Reserved
	TSStreamReserved = 0x00,
	TSStreamForbidden = 0xff,
	// ISO/IEC 11172 Video
	// ITU-T Rec. H.262 | ISO/IEC 13818-2 Video or ISO/IEC 11172-2 constrained parameter video stream
	// ISO/IEC 11172 Audio
	// ISO/IEC 13818-3 Audio
	TSStreamAudioMp3 = 0x04,
	// ITU-T Rec. H.222.0 | ISO/IEC 13818-1 private_sections
	// ITU-T Rec. H.222.0 | ISO/IEC 13818-1 PES packets containing private data
	// ISO/IEC 13522 MHEG
	// ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Annex A DSM-CC
	// ITU-T Rec. H.222.1
	// ISO/IEC 13818-6 type A
	// ISO/IEC 13818-6 type B
	// ISO/IEC 13818-6 type C
	// ISO/IEC 13818-6 type D
	// ITU-T Rec. H.222.0 | ISO/IEC 13818-1 auxiliary
	// ISO/IEC 13818-7 Audio with ADTS transport syntax
	TSStreamAudioAAC = 0x0f,
	// ISO/IEC 14496-2 Visual
	TSStreamVideoMpeg4 = 0x10,
	// ISO/IEC 14496-3 Audio with the LATM transport syntax as defined in ISO/IEC 14496-3 / AMD 1
	TSStreamAudioMpeg4 = 0x11,
	// ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in PES packets
	// ISO/IEC 14496-1 SL-packetized stream or FlexMux stream carried in ISO/IEC14496_sections.
	// ISO/IEC 13818-6 Synchronized Download Protocol
	// ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Reserved
	// 0x15-0x7F
	TSStreamVideoH264 = 0x1b,
	// User Private
	// 0x80-0xFF
	TSStreamAudioAC3 = 0x81,
	TSStreamAudioDTS = 0x8a,
};

// the adaption_field_control of ts packet,
// Table 2-5 - Adaptation field control values, hls-mpeg-ts-iso13818-1.pdf, page 38
enum TSAdaptationFieldType
{
	// Reserved for future use by ISO/IEC
	TSAdaptationFieldTypeReserved = 0x00,
	// No adaptation_field, payload only
	TSAdaptationFieldTypePayloadOnly = 0x01,
	// Adaptation_field only, no payload
	TSAdaptationFieldTypeAdaptionOnly = 0x02,
	// Adaptation_field followed by payload
	TSAdaptationFieldTypeBoth = 0x03,
};

// The stream_id of PES payload of ts packet.
// Table 2-18 - Stream_id assignments, hls-mpeg-ts-iso13818-1.pdf, page 52.
enum TSPESStreamId
{
    // program_stream_map
	TSPESStreamIdProgramStreamMap = 0xbc, // 0b10111100
    // private_stream_1
	TSPESStreamIdPrivateStream1 = 0xbd, // 0b10111101
    // padding_stream
	TSPESStreamIdPaddingStream = 0xbe, // 0b10111110
    // private_stream_2
	TSPESStreamIdPrivateStream2 = 0xbf, // 0b10111111
    
    // 110x xxxx
    // ISO/IEC 13818-3 or ISO/IEC 11172-3 or ISO/IEC 13818-7 or ISO/IEC
    // 14496-3 audio stream number x xxxx
    // ((sid >> 5) & 0x07) == TSPESStreamIdAudio
    // @remark, use TSPESStreamIdAudioCommon as actually audio, TSPESStreamIdAudio to check whether audio.
	TSPESStreamIdAudioChecker = 0x06, // 0b110
	TSPESStreamIdAudioCommon = 0xc0,
    
    // 1110 xxxx
    // ITU-T Rec. H.262 | ISO/IEC 13818-2 or ISO/IEC 11172-2 or ISO/IEC
    // 14496-2 video stream number xxxx
    // ((stream_id >> 4) & 0x0f) == TSPESStreamIdVideo
    // @remark, use TSPESStreamIdVideoCommon as actually video, TSPESStreamIdVideo to check whether video.
	TSPESStreamIdVideoChecker = 0x0e, // 0b1110
	TSPESStreamIdVideoCommon = 0xe0,
    
    // ECM_stream
	TSPESStreamIdEcmStream = 0xf0, // 0b11110000
    // EMM_stream
	TSPESStreamIdEmmStream = 0xf1, // 0b11110001
    // DSMCC_stream
	TSPESStreamIdDsmccStream = 0xf2, // 0b11110010
    // 13522_stream
	TSPESStreamId13522Stream = 0xf3, // 0b11110011
    // H_222_1_type_A
	TSPESStreamIdH2221TypeA = 0xf4, // 0b11110100
    // H_222_1_type_B
	TSPESStreamIdH2221TypeB = 0xf5, // 0b11110101
    // H_222_1_type_C
	TSPESStreamIdH2221TypeC = 0xf6, // 0b11110110
    // H_222_1_type_D
	TSPESStreamIdH2221TypeD = 0xf7, // 0b11110111
    // H_222_1_type_E
	TSPESStreamIdH2221TypeE = 0xf8, // 0b11111000
    // ancillary_stream
	TSPESStreamIdAncillaryStream = 0xf9, // 0b11111001
    // SL_packetized_stream
	TSPESStreamIdSlPacketizedStream = 0xfa, // 0b11111010
    // FlexMux_stream
	TSPESStreamIdFlexMuxStream = 0xfb, // 0b11111011
    // reserved data stream
    // 1111 1100 ... 1111 1110
    // program_stream_directory
	TSPESStreamIdProgramStreamDirectory = 0xff, // 0b11111111
};

#pragma pack(push, 1)
typedef struct tagTSHeader
{
	UInt16 sync_byte;		//同步字节，固定为0x47, 采用固定宏TS_SYNC_0X47替代。每一次PAT和PMT和每一帧数据处理的时候都要添
	UInt16 transport_error_indicator : 1;	//传输错误指示符，表明在ts头的adapt域后由一个无用字节，通常都为0
	UInt16 payload_unit_start_indicator : 1;	//负载开始标志位，用来表示TS包的有效净荷带有PES包或者PSI数据的情况，占位1bit；另若此值为1，且负载为PSI数据时，则在TS头后，负载起始字节会有1个调整字节point_field
	UInt16 transport_priority : 1;				//传输优先级，0为低优先级，1为高优先级，通常取0
	UInt16 pid : 13;							//pid值(Packet ID号码，唯一的号码对应不同的包)
												//指示有效负载中的数据类型，占位13bit；0x0000代表PAT，0x0001代表CAT，0x0002-0x000F保留，0x1FFF表示空包；
	UInt16 transport_scrambling_control : 2;	//传输加扰控制，00表示未加密
	UInt16 adaptation_field_control : 2;		//调整字段标志，表示此TS首部是否跟随调整字段还是负载数据，占位2bit，其中00位保留，01表示无调整字段，只有有效负载数据，10表示只有调整字段，无有效负载，11表示有调整字段，且其后跟有有效负载；空分组此字段应为01
	UInt16 continuity_counter : 4;				//固定为11，递增计数器，从0-f，起始值不一定取0，但必须是连续的
}tagTSHeader;

typedef struct tagTSAdaptationField
{
	UInt8 adaptation_field_length;					//自适应域长度，后面的字节数
	UInt8 flag;
}tagTSAdaptationField;

// The ts channel.
typedef struct tagTSChannel
{
	TSPid Pid;
	TSPidApply ApplyType;
	TSStream StreamType;
}tagTSChannel;
#pragma pack(pop)

typedef function20_handle<int (TSPid, TSPidApply, TSStream)>		HFNSetChannel;

int decode_33bits_dts_pts(const char* c_szData, int64_t* pv)
{
	// decode the 33bits schema.
	// ===========1B
	// 4bits const maybe '0001', '0010' or '0011'.
	// 3bits DTS/PTS [32..30]
	// 1bit const '1'
	int64_t dts_pts_30_32 = (int64_t)c_szData[0];
	if ((dts_pts_30_32 & 0x01) != 0x01) 
	{
		LOG_Print_Error(libThirdWrap_FFmpeg, "decode_33bits_dts_pts() ts: demux PSE dts/pts 30-32");
		return -1;
	}
	// @remark, we donot check the high 4bits, maybe '0001', '0010' or '0011'.
	//      so we just ensure the high 4bits is not 0x00.
	if (((dts_pts_30_32 >> 4) & 0x0f) == 0x00) 
	{
		LOG_Print_Error(libThirdWrap_FFmpeg, "decode_33bits_dts_pts() ts: demux PSE dts/pts 30-32");
		return -1;
	}

	dts_pts_30_32 = (dts_pts_30_32 >> 1) & 0x07;

	// ===========2B
	// 15bits DTS/PTS [29..15]
	// 1bit const '1'
	int64_t dts_pts_15_29 = 0;
	memcpy((void*)&dts_pts_15_29, &c_szData[1], 2);
	if ((dts_pts_15_29 & 0x01) != 0x01) 
	{
		LOG_Print_Error(libThirdWrap_FFmpeg, "decode_33bits_dts_pts() ts: demux PSE dts/pts 15-29");
		return -1;
	}

	dts_pts_15_29 = (dts_pts_15_29 >> 1) & 0x7fff;

	// ===========2B
	// 15bits DTS/PTS [14..0]
	// 1bit const '1'
	int64_t dts_pts_0_14 = 0; 
	memcpy((void*)&dts_pts_0_14, &c_szData[3], 2);
	if ((dts_pts_0_14 & 0x01) != 0x01) 
	{
		LOG_Print_Error(libThirdWrap_FFmpeg, "decode_33bits_dts_pts() ts: demux PSE dts/pts 0-14");
		return -1;
	}

	dts_pts_0_14 = (dts_pts_0_14 >> 1) & 0x7fff;

	int64_t v = 0x00;
	v |= (dts_pts_30_32 << 30) & 0x1c0000000LL;
	v |= (dts_pts_15_29 << 15) & 0x3fff8000LL;
	v |= dts_pts_0_14 & 0x7fff;
	*pv = v;

	return 1;
}

class TSAdaptationField
{
public:
	struct tagAdaptationField_INFO
	{
		// 1B
		// The adaptation_field_length is an 8-bit field specifying the number of bytes in the
		// adaptation_field immediately following the adaptation_field_length. The value 0 is for inserting a single stuffing byte in
		// a Transport Stream packet. When the adaptation_field_control value is '11', the value of the adaptation_field_length shall
		// be in the range 0 to 182. When the adaptation_field_control value is '10', the value of the adaptation_field_length shall
		// be 183. For Transport Stream packets carrying PES packets, stuffing is needed when there is insufficient PES packet data
		// to completely fill the Transport Stream packet payload bytes. Stuffing is accomplished by defining an adaptation field
		// longer than the sum of the lengths of the data elements in it, so that the payload bytes remaining after the adaptation field
		// exactly accommodates the available PES packet data. The extra space in the adaptation field is filled with stuffing bytes.
		//
		// This is the only method of stuffing allowed for Transport Stream packets carrying PES packets. For Transport Stream
		// packets carrying PSI, an alternative stuffing method is described in 2.4.4.
		uint8_t adaption_field_length; //8bits
									   // 1B
									   // This is a 1-bit field which when set to '1' indicates that the discontinuity state is true for the
									   // current Transport Stream packet. When the discontinuity_indicator is set to '0' or is not present, the discontinuity state is
									   // false. The discontinuity indicator is used to indicate two types of discontinuities, system time-base discontinuities and
									   // continuity_counter discontinuities.
									   //
									   // A system time-base discontinuity is indicated by the use of the discontinuity_indicator in Transport Stream packets of a
									   // PID designated as a PCR_PID (refer to 2.4.4.9). When the discontinuity state is true for a Transport Stream packet of a
									   // PID designated as a PCR_PID, the next PCR in a Transport Stream packet with that same PID represents a sample of a
									   // new system time clock for the associated program. The system time-base discontinuity point is defined to be the instant
									   // in time when the first byte of a packet containing a PCR of a new system time-base arrives at the input of the T-STD.
									   // The discontinuity_indicator shall be set to '1' in the packet in which the system time-base discontinuity occurs. The
									   // discontinuity_indicator bit may also be set to '1' in Transport Stream packets of the same PCR_PID prior to the packet
									   // which contains the new system time-base PCR. In this case, once the discontinuity_indicator has been set to '1', it shall
									   // continue to be set to '1' in all Transport Stream packets of the same PCR_PID up to and including the Transport Stream
									   // packet which contains the first PCR of the new system time-base. After the occurrence of a system time-base
									   // discontinuity, no fewer than two PCRs for the new system time-base shall be received before another system time-base
									   // discontinuity can occur. Further, except when trick mode status is true, data from no more than two system time-bases
									   // shall be present in the set of T-STD buffers for one program at any time.
									   //
									   // Prior to the occurrence of a system time-base discontinuity, the first byte of a Transport Stream packet which contains a
									   // PTS or DTS which refers to the new system time-base shall not arrive at the input of the T-STD. After the occurrence of
									   // a system time-base discontinuity, the first byte of a Transport Stream packet which contains a PTS or DTS which refers
									   // to the previous system time-base shall not arrive at the input of the T-STD.
									   //
									   // A continuity_counter discontinuity is indicated by the use of the discontinuity_indicator in any Transport Stream packet.
									   // When the discontinuity state is true in any Transport Stream packet of a PID not designated as a PCR_PID, the
									   // continuity_counter in that packet may be discontinuous with respect to the previous Transport Stream packet of the same
									   // PID. When the discontinuity state is true in a Transport Stream packet of a PID that is designated as a PCR_PID, the
									   // continuity_counter may only be discontinuous in the packet in which a system time-base discontinuity occurs. A
									   // continuity counter discontinuity point occurs when the discontinuity state is true in a Transport Stream packet and the
									   // continuity_counter in the same packet is discontinuous with respect to the previous Transport Stream packet of the same
									   // PID. A continuity counter discontinuity point shall occur at most one time from the initiation of the discontinuity state
									   // until the conclusion of the discontinuity state. Furthermore, for all PIDs that are not designated as PCR_PIDs, when the
									   // discontinuity_indicator is set to '1' in a packet of a specific PID, the discontinuity_indicator may be set to '1' in the next
									   // Transport Stream packet of that same PID, but shall not be set to '1' in three consecutive Transport Stream packet of that
									   // same PID.
									   //
									   // For the purpose of this clause, an elementary stream access point is defined as follows:
									   //       Video - The first byte of a video sequence header.
									   //       Audio - The first byte of an audio frame.
									   //
									   // After a continuity counter discontinuity in a Transport packet which is designated as containing elementary stream data,
									   // the first byte of elementary stream data in a Transport Stream packet of the same PID shall be the first byte of an
									   // elementary stream access point or in the case of video, the first byte of an elementary stream access point or a
									   // sequence_end_code followed by an access point. Each Transport Stream packet which contains elementary stream data
									   // with a PID not designated as a PCR_PID, and in which a continuity counter discontinuity point occurs, and in which a
									   // PTS or DTS occurs, shall arrive at the input of the T-STD after the system time-base discontinuity for the associated
									   // program occurs. In the case where the discontinuity state is true, if two consecutive Transport Stream packets of the same
									   // PID occur which have the same continuity_counter value and have adaptation_field_control values set to '01' or '11', the
									   // second packet may be discarded. A Transport Stream shall not be constructed in such a way that discarding such a packet
									   // will cause the loss of PES packet payload data or PSI data.
									   //
									   // After the occurrence of a discontinuity_indicator set to '1' in a Transport Stream packet which contains PSI information,
									   // a single discontinuity in the version_number of PSI sections may occur. At the occurrence of such a discontinuity, a
									   // version of the TS_program_map_sections of the appropriate program shall be sent with section_length = = 13 and the
									   // current_next_indicator = = 1, such that there are no program_descriptors and no elementary streams described. This shall
									   // then be followed by a version of the TS_program_map_section for each affected program with the version_number
									   // incremented by one and the current_next_indicator = = 1, containing a complete program definition. This indicates a
									   // version change in PSI data.
		int8_t discontinuity_indicator; //1bit
										// The random_access_indicator is a 1-bit field that indicates that the current Transport
										// Stream packet, and possibly subsequent Transport Stream packets with the same PID, contain some information to aid
										// random access at this point. Specifically, when the bit is set to '1', the next PES packet to start in the payload of Transport
										// Stream packets with the current PID shall contain the first byte of a video sequence header if the PES stream type (refer
										// to Table 2-29) is 1 or 2, or shall contain the first byte of an audio frame if the PES stream type is 3 or 4. In addition, in
										// the case of video, a presentation timestamp shall be present in the PES packet containing the first picture following the
										// sequence header. In the case of audio, the presentation timestamp shall be present in the PES packet containing the first
										// byte of the audio frame. In the PCR_PID the random_access_indicator may only be set to '1' in Transport Stream packet
										// containing the PCR fields.
		int8_t random_access_indicator; //1bit
										// The elementary_stream_priority_indicator is a 1-bit field. It indicates, among
										// packets with the same PID, the priority of the elementary stream data carried within the payload of this Transport Stream
										// packet. A '1' indicates that the payload has a higher priority than the payloads of other Transport Stream packets. In the
										// case of video, this field may be set to '1' only if the payload contains one or more bytes from an intra-coded slice. A
										// value of '0' indicates that the payload has the same priority as all other packets which do not have this bit set to '1'.
		int8_t elementary_stream_priority_indicator; //1bit
													 // The PCR_flag is a 1-bit flag. A value of '1' indicates that the adaptation_field contains a PCR field coded in
													 // two parts. A value of '0' indicates that the adaptation field does not contain any PCR field.
		int8_t PCR_flag; //1bit
						 // The OPCR_flag is a 1-bit flag. A value of '1' indicates that the adaptation_field contains an OPCR field
						 // coded in two parts. A value of '0' indicates that the adaptation field does not contain any OPCR field.
		int8_t OPCR_flag; //1bit
						  // The splicing_point_flag is a 1-bit flag. When set to '1', it indicates that a splice_countdown field
						  // shall be present in the associated adaptation field, specifying the occurrence of a splicing point. A value of '0' indicates
						  // that a splice_countdown field is not present in the adaptation field.
		int8_t splicing_point_flag; //1bit
									// The transport_private_data_flag is a 1-bit flag. A value of '1' indicates that the
									// adaptation field contains one or more private_data bytes. A value of '0' indicates the adaptation field does not contain any
									// private_data bytes.
		int8_t transport_private_data_flag; //1bit
											// The adaptation_field_extension_flag is a 1-bit field which when set to '1' indicates
											// the presence of an adaptation field extension. A value of '0' indicates that an adaptation field extension is not present in
											// the adaptation field.
		int8_t adaptation_field_extension_flag; //1bit

												// If PCR_flag, 6B
												// The program_clock_reference (PCR) is a
												// 42-bit field coded in two parts. The first part, program_clock_reference_base, is a 33-bit field whose value is given by
												// PCR_base(i), as given in equation 2-2. The second part, program_clock_reference_extension, is a 9-bit field whose value
												// is given by PCR_ext(i), as given in equation 2-3. The PCR indicates the intended time of arrival of the byte containing
												// the last bit of the program_clock_reference_base at the input of the system target decoder.
		int64_t program_clock_reference_base; //33bits
											  // 6bits reserved, must be '1'
		int8_t const1_value0; // 6bits
		int16_t program_clock_reference_extension; //9bits

												   // If OPCR_flag, 6B
												   // The optional original
												   // program reference (OPCR) is a 42-bit field coded in two parts. These two parts, the base and the extension, are coded
												   // identically to the two corresponding parts of the PCR field. The presence of the OPCR is indicated by the OPCR_flag.
												   // The OPCR field shall be coded only in Transport Stream packets in which the PCR field is present. OPCRs are permitted
												   // in both single program and multiple program Transport Streams.
												   //
												   // OPCR assists in the reconstruction of a single program Transport Stream from another Transport Stream. When
												   // reconstructing the original single program Transport Stream, the OPCR may be copied to the PCR field. The resulting
												   // PCR value is valid only if the original single program Transport Stream is reconstructed exactly in its entirety. This
												   // would include at least any PSI and private data packets which were present in the original Transport Stream and would
												   // possibly require other private arrangements. It also means that the OPCR must be an identical copy of its associated PCR
												   // in the original single program Transport Stream.
		int64_t original_program_clock_reference_base; //33bits
													   // 6bits reserved, must be '1'
		int8_t const1_value2; // 6bits
		int16_t original_program_clock_reference_extension; //9bits

															// If splicing_point_flag, 1B
															// The splice_countdown is an 8-bit field, representing a value which may be positive or negative. A
															// positive value specifies the remaining number of Transport Stream packets, of the same PID, following the associated
															// Transport Stream packet until a splicing point is reached. Duplicate Transport Stream packets and Transport Stream
															// packets which only contain adaptation fields are excluded. The splicing point is located immediately after the last byte of
															// the Transport Stream packet in which the associated splice_countdown field reaches zero. In the Transport Stream packet
															// where the splice_countdown reaches zero, the last data byte of the Transport Stream packet payload shall be the last byte
															// of a coded audio frame or a coded picture. In the case of video, the corresponding access unit may or may not be
															// terminated by a sequence_end_code. Transport Stream packets with the same PID, which follow, may contain data from
															// a different elementary stream of the same type.
															//
															// The payload of the next Transport Stream packet of the same PID (duplicate packets and packets without payload being
															// excluded) shall commence with the first byte of a PES packet.In the case of audio, the PES packet payload shall
															// commence with an access point. In the case of video, the PES packet payload shall commence with an access point, or
															// with a sequence_end_code, followed by an access point. Thus, the previous coded audio frame or coded picture aligns
															// with the packet boundary, or is padded to make this so. Subsequent to the splicing point, the countdown field may also
															// be present. When the splice_countdown is a negative number whose value is minus n(-n), it indicates that the associated
															// Transport Stream packet is the n-th packet following the splicing point (duplicate packets and packets without payload
															// being excluded).
															//
															// For the purposes of this subclause, an access point is defined as follows:
															//       Video - The first byte of a video_sequence_header.
															//       Audio - The first byte of an audio frame.
		int8_t splice_countdown; //8bits

								 // If transport_private_data_flag, 1+p[0] B
		std::vector<char> transport_private_data; //[transport_private_data_length]bytes

												  // If adaptation_field_extension_flag, 2+x B
												  // The adaptation_field_extension_length is an 8-bit field. It indicates the number of
												  // bytes of the extended adaptation field data immediately following this field, including reserved bytes if present.
		uint8_t adaptation_field_extension_length; //8bits
												   // This is a 1-bit field which when set to '1' indicates the presence of the ltw_offset
												   // field.
		int8_t ltw_flag; //1bit
						 // This is a 1-bit field which when set to '1' indicates the presence of the piecewise_rate field.
		int8_t piecewise_rate_flag; //1bit
									// This is a 1-bit flag which when set to '1' indicates that the splice_type and DTS_next_AU fields
									// are present. A value of '0' indicates that neither splice_type nor DTS_next_AU fields are present. This field shall not be
									// set to '1' in Transport Stream packets in which the splicing_point_flag is not set to '1'. Once it is set to '1' in a Transport
									// Stream packet in which the splice_countdown is positive, it shall be set to '1' in all the subsequent Transport Stream
									// packets of the same PID that have the splicing_point_flag set to '1', until the packet in which the splice_countdown
									// reaches zero (including this packet). When this flag is set, if the elementary stream carried in this PID is an audio stream,
									// the splice_type field shall be set to '0000'. If the elementary stream carried in this PID is a video stream, it shall fulfil the
									// constraints indicated by the splice_type value.
		int8_t seamless_splice_flag; //1bit
									 // reserved 5bits, must be '1'
		int8_t const1_value1; //5bits
							  // if ltw_flag, 2B
							  // (legal time window_valid_flag) - This is a 1-bit field which when set to '1' indicates that the value of the
							  // ltw_offset shall be valid. A value of '0' indicates that the value in the ltw_offset field is undefined.
		int8_t ltw_valid_flag; //1bit
							   // (legal time window offset) - This is a 15-bit field, the value of which is defined only if the ltw_valid flag has
							   // a value of '1'. When defined, the legal time window offset is in units of (300/fs) seconds, where fs is the system clock
							   // frequency of the program that this PID belongs to, and fulfils:
							   //       offset = t1(i) - t(i)
							   //       ltw_offset = offset//1
							   // where i is the index of the first byte of this Transport Stream packet, offset is the value encoded in this field, t(i) is the
							   // arrival time of byte i in the T-STD, and t1(i) is the upper bound in time of a time interval called the Legal Time Window
							   // which is associated with this Transport Stream packet.
		int16_t ltw_offset; //15bits
							// if piecewise_rate_flag, 3B
							//2bits reserved
							// The meaning of this 22-bit field is only defined when both the ltw_flag and the ltw_valid_flag are set
							// to '1'. When defined, it is a positive integer specifying a hypothetical bitrate R which is used to define the end times of
							// the Legal Time Windows of Transport Stream packets of the same PID that follow this packet but do not include the
							// legal_time_window_offset field.
		int32_t piecewise_rate; //22bits
								// if seamless_splice_flag, 5B
								// This is a 4-bit field. From the first occurrence of this field onwards, it shall have the same value in all the
								// subsequent Transport Stream packets of the same PID in which it is present, until the packet in which the
								// splice_countdown reaches zero (including this packet). If the elementary stream carried in that PID is an audio stream,
								// this field shall have the value '0000'. If the elementary stream carried in that PID is a video stream, this field indicates the
								// conditions that shall be respected by this elementary stream for splicing purposes. These conditions are defined as a
								// function of profile, level and splice_type in Table 2-7 through Table 2-16.
		int8_t splice_type; //4bits
							// (decoding time stamp next access unit) - This is a 33-bit field, coded in three parts. In the case of
							// continuous and periodic decoding through this splicing point it indicates the decoding time of the first access unit
							// following the splicing point. This decoding time is expressed in the time base which is valid in the Transport Stream
							// packet in which the splice_countdown reaches zero. From the first occurrence of this field onwards, it shall have the
							// same value in all the subsequent Transport Stream packets of the same PID in which it is present, until the packet in
							// which the splice_countdown reaches zero (including this packet).
		int8_t DTS_next_AU0; //3bits
		int8_t marker_bit0; //1bit
		int16_t DTS_next_AU1; //15bits
		int8_t marker_bit1; //1bit
		int16_t DTS_next_AU2; //15bits
		int8_t marker_bit2; //1bit
							// left bytes.
							// This is a fixed 8-bit value equal to '1111 1111' that can be inserted by the encoder. It is discarded by the
							// decoder.
		int nb_af_ext_reserved;

		// left bytes.
		// This is a fixed 8-bit value equal to '1111 1111' that can be inserted by the encoder. It is discarded by the
		// decoder.
		int nb_af_reserved;
	};

public:
	TSAdaptationField(void)
	{
		_FieldINFO.adaption_field_length = 0;
		_FieldINFO.discontinuity_indicator = 0;
		_FieldINFO.random_access_indicator = 0;
		_FieldINFO.elementary_stream_priority_indicator = 0;
		_FieldINFO.PCR_flag = 0;
		_FieldINFO.OPCR_flag = 0;
		_FieldINFO.splicing_point_flag = 0;
		_FieldINFO.transport_private_data_flag = 0;
		_FieldINFO.adaptation_field_extension_flag = 0;
		_FieldINFO.program_clock_reference_base = 0;
		_FieldINFO.program_clock_reference_extension = 0;
		_FieldINFO.original_program_clock_reference_base = 0;
		_FieldINFO.original_program_clock_reference_extension = 0;
		_FieldINFO.splice_countdown = 0;
		_FieldINFO.adaptation_field_extension_length = 0;
		_FieldINFO.ltw_flag = 0;
		_FieldINFO.piecewise_rate_flag = 0;
		_FieldINFO.seamless_splice_flag = 0;
		_FieldINFO.ltw_valid_flag = 0;
		_FieldINFO.ltw_offset = 0;
		_FieldINFO.piecewise_rate = 0;
		_FieldINFO.splice_type = 0;
		_FieldINFO.DTS_next_AU0 = 0;
		_FieldINFO.marker_bit0 = 0;
		_FieldINFO.DTS_next_AU1 = 0;
		_FieldINFO.marker_bit1 = 0;
		_FieldINFO.DTS_next_AU2 = 0;
		_FieldINFO.marker_bit2 = 0;
		_FieldINFO.nb_af_ext_reserved = 0;
		_FieldINFO.nb_af_reserved = 0;

		_FieldINFO.const1_value0 = 0x3F;
		_FieldINFO.const1_value1 = 0x1F;
		_FieldINFO.const1_value2 = 0x3F;
	}

	~TSAdaptationField(void)
	{

	}

	int Parse(const tagTSHeader& Header, const char* c_szData, UInt32 uSize)
	{
		int iOffset = 0;
		if (uSize < sizeof(tagTSAdaptationField))
			return -1;

		tagTSAdaptationField adaptation_field;
		memcpy((void*)&adaptation_field, c_szData, sizeof(tagTSAdaptationField));
		iOffset += sizeof(adaptation_field);

		// no adaptation field.
		if (adaptation_field.adaptation_field_length == 0)
		{
			LOG_Print_Info(libThirdWrap_FFmpeg, "TSAdaptationField::Parse()  ts: demux af empty.");
			return 1;
		}

		if (uSize < adaptation_field.adaptation_field_length + 1)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "TSAdaptationField::Parse(%d)  ts: demux af adaptation_field_length:%d.",
				uSize - 1, adaptation_field.adaptation_field_length);
			return -1;
		}

		// When the adaptation_field_control value is '11', the value of the adaptation_field_length shall
		// be in the range 0 to 182.
		if (Header.adaptation_field_control == TSAdaptationFieldTypeBoth &&
			adaptation_field.adaptation_field_length > 182)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "TSAdaptationField::Parse() ts: demux af length failed, must in [0, 182], actual=%d",
				adaptation_field.adaptation_field_length);
			return -1;
		}
		// When the adaptation_field_control value is '10', the value of the adaptation_field_length shall
		// be 183.
		if (Header.adaptation_field_control == TSAdaptationFieldTypeAdaptionOnly &&
			adaptation_field.adaptation_field_length != 183)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "TSAdaptationField::Parse() ts: demux af length failed, must be 183, actual=%d",
				adaptation_field.adaptation_field_length);
			return -1;
		}

		_FieldINFO.discontinuity_indicator = (adaptation_field.flag >> 7) & 0x01;
		_FieldINFO.random_access_indicator = (adaptation_field.flag >> 6) & 0x01;
		_FieldINFO.elementary_stream_priority_indicator = (adaptation_field.flag >> 5) & 0x01;
		_FieldINFO.PCR_flag = (adaptation_field.flag >> 4) & 0x01;
		_FieldINFO.OPCR_flag = (adaptation_field.flag >> 3) & 0x01;
		_FieldINFO.splicing_point_flag = (adaptation_field.flag >> 2) & 0x01;
		_FieldINFO.transport_private_data_flag = (adaptation_field.flag >> 1) & 0x01;
		_FieldINFO.adaptation_field_extension_flag = adaptation_field.flag & 0x01;

		if (_FieldINFO.PCR_flag)
		{//包含PCR
			if (uSize - iOffset < 6)
			{
				LOG_Print_Error(libThirdWrap_FFmpeg, "TSAdaptationField::Parse() ts: demux af PCR_flag");
				return -1;
			}

			char* pp = NULL;
			char* p = (char*)&c_szData[iOffset];

			int64_t pcrv = 0;
			pp = (char*)&pcrv;
			pp[5] = *p++;
			pp[4] = *p++;
			pp[3] = *p++;
			pp[2] = *p++;
			pp[1] = *p++;
			pp[0] = *p++;

			// @remark, use pcr base and ignore the extension
			_FieldINFO.program_clock_reference_extension = pcrv & 0x1ff;
			_FieldINFO.const1_value0 = (pcrv >> 9) & 0x3F;
			_FieldINFO.program_clock_reference_base = (pcrv >> 15) & 0x1ffffffffLL;

			iOffset += 6;
		}
		
		if(_FieldINFO.OPCR_flag)
		{//不包含PCR(音频数据不需要pcr)
			if (uSize - iOffset < 6)
			{
				LOG_Print_Error(libThirdWrap_FFmpeg, "TSAdaptationField::Parse() ts: demux af OPCR_flag");
				return -1;
			}

			char* pp = NULL;
			char* p = (char*)&c_szData[iOffset];

			int64_t opcrv = 0;
			pp = (char*)&opcrv;
			pp[5] = *p++;
			pp[4] = *p++;
			pp[3] = *p++;
			pp[2] = *p++;
			pp[1] = *p++;
			pp[0] = *p++;

			// @remark, use pcr base and ignore the extension
			_FieldINFO.original_program_clock_reference_extension = opcrv & 0x1ff;
			_FieldINFO.const1_value2 = (opcrv >> 9) & 0x3F;
			_FieldINFO.original_program_clock_reference_base = (opcrv >> 15) & 0x1ffffffffLL;

			iOffset += 6;
		}

		if (_FieldINFO.splicing_point_flag)
		{
			if (uSize - iOffset < 1)
			{
				LOG_Print_Error(libThirdWrap_FFmpeg, "TSAdaptationField::Parse() ts: demux af splicing_point_flag");
				return -1;
			}

			_FieldINFO.splice_countdown = c_szData[iOffset ++];
		}

		if (_FieldINFO.transport_private_data_flag)
		{
			if (uSize - iOffset < 1)
			{
				LOG_Print_Error(libThirdWrap_FFmpeg, "TSAdaptationField::Parse() ts: demux af transport_private_data_flag");
				return -1;
			}
			/**
			* The transport_private_data_length is an 8-bit field specifying the number of
			* private_data bytes immediately following the transport private_data_length field. The number of private_data bytes shall
			* not be such that private data extends beyond the adaptation field.
			*/
			UInt8 transport_private_data_length = (UInt8)c_szData[iOffset ++];

			if (transport_private_data_length > 0) 
			{
				if (uSize - iOffset < transport_private_data_length)
				{
					LOG_Print_Error(libThirdWrap_FFmpeg, "TSAdaptationField::Parse() ts: demux af transport_private_data");
					return -1;
				}

				_FieldINFO.transport_private_data.resize(transport_private_data_length);
				for (int i = 0; i < transport_private_data_length; i++)
				{
					_FieldINFO.transport_private_data[i] = c_szData[iOffset ++];
				}
			}
		}

		if (_FieldINFO.adaptation_field_extension_flag)
		{
			int pos_af_ext = iOffset;
			if (uSize - iOffset < 2)
			{
				LOG_Print_Error(libThirdWrap_FFmpeg, "TSAdaptationField::Parse() ts: demux af adaptation_field_extension_flag");
				return -1;
			}

			_FieldINFO.adaptation_field_extension_length = (UInt8)c_szData[iOffset ++];
			Int8 ltwfv = c_szData[iOffset ++];

			_FieldINFO.ltw_flag = (ltwfv >> 7) & 0x01;
			_FieldINFO.piecewise_rate_flag = (ltwfv >> 6) & 0x01;
			_FieldINFO.seamless_splice_flag = (ltwfv >> 5) & 0x01;
			_FieldINFO.const1_value1 = ltwfv & 0x1F;

			if (_FieldINFO.ltw_flag)
			{
				if (uSize - iOffset < 2)
				{
					LOG_Print_Error(libThirdWrap_FFmpeg, "TSAdaptationField::Parse() ts: demux af ltw_flag");
					return -1;
				}

				memcpy((void*)&_FieldINFO.ltw_offset, &c_szData[iOffset], sizeof(_FieldINFO.ltw_offset));
				iOffset += 2;

				_FieldINFO.ltw_valid_flag = (_FieldINFO.ltw_offset >> 15) & 0x01;
				_FieldINFO.ltw_offset &= 0x7FFF;
			}

			if (_FieldINFO.piecewise_rate_flag)
			{
				if (uSize - iOffset < 3)
				{
					LOG_Print_Error(libThirdWrap_FFmpeg, "TSAdaptationField::Parse() ts: demux af piecewise_rate_flag");
					return -1;
				}

				memcpy((void*)&_FieldINFO.piecewise_rate, &c_szData[iOffset], 3);
				_FieldINFO.piecewise_rate &= 0x3FFFFF;

				iOffset += 3;
			}

			if (_FieldINFO.seamless_splice_flag)
			{
				if (uSize - iOffset < 5)
				{
					LOG_Print_Error(libThirdWrap_FFmpeg, "TSAdaptationField::Parse() ts: demux af seamless_splice_flag");
					return -1;
				}

				_FieldINFO.marker_bit0 = c_szData[iOffset ++];
				memcpy((void*)&_FieldINFO.DTS_next_AU1, &c_szData[iOffset], sizeof(_FieldINFO.DTS_next_AU1));
				iOffset += sizeof(_FieldINFO.DTS_next_AU1);
				memcpy((void*)&_FieldINFO.DTS_next_AU2, &c_szData[iOffset], sizeof(_FieldINFO.DTS_next_AU2));
				iOffset += sizeof(_FieldINFO.DTS_next_AU2);

				_FieldINFO.splice_type = (_FieldINFO.marker_bit0 >> 4) & 0x0F;
				_FieldINFO.DTS_next_AU0 = (_FieldINFO.marker_bit0 >> 1) & 0x07;
				_FieldINFO.marker_bit0 &= 0x01;

				_FieldINFO.marker_bit1 = _FieldINFO.DTS_next_AU1 & 0x01;
				_FieldINFO.DTS_next_AU1 = (_FieldINFO.DTS_next_AU1 >> 1) & 0x7FFF;

				_FieldINFO.marker_bit2 = _FieldINFO.DTS_next_AU2 & 0x01;
				_FieldINFO.DTS_next_AU2 = (_FieldINFO.DTS_next_AU2 >> 1) & 0x7FFF;
			}

			_FieldINFO.nb_af_ext_reserved = _FieldINFO.adaptation_field_extension_length - (iOffset - pos_af_ext);
			if (uSize - iOffset < _FieldINFO.nb_af_ext_reserved)
			{
				LOG_Print_Error(libThirdWrap_FFmpeg, "TSAdaptationField::Parse() ts: demux af nb_af_ext_reserved");
				return -1;
			}

			iOffset += _FieldINFO.nb_af_ext_reserved;
		}
		// LCOV_EXCL_STOP
		iOffset += _FieldINFO.adaption_field_length + 1 - iOffset;

		LOG_Print_Info(libThirdWrap_FFmpeg, "ts: af parsed, discontinuity=%d random=%d priority=%d PCR=%d OPCR=%d slicing=%d private=%d extension=%d/%d pcr=%" PRId64 "/%d opcr=%" PRId64 "/%d",
			_FieldINFO.discontinuity_indicator, _FieldINFO.random_access_indicator, _FieldINFO.elementary_stream_priority_indicator, _FieldINFO.PCR_flag, _FieldINFO.OPCR_flag, _FieldINFO.splicing_point_flag,
			_FieldINFO.transport_private_data_flag, _FieldINFO.adaptation_field_extension_flag, _FieldINFO.adaptation_field_extension_length, _FieldINFO.program_clock_reference_base,
			_FieldINFO.program_clock_reference_extension, _FieldINFO.original_program_clock_reference_base, _FieldINFO.original_program_clock_reference_extension);

		return iOffset;
	}
private:
	tagAdaptationField_INFO	_FieldINFO;
};

class TSPayloadPAT
{
public:
	struct PayloadPATProgram_INFO
	{
		// 4B
		// Program_number is a 16-bit field. It specifies the program to which the program_map_PID is
		// applicable. When set to 0x0000, then the following PID reference shall be the network PID. For all other cases the value
		// of this field is user defined. This field shall not take any single value more than once within one version of the Program
		// Association Table.
		int16_t number; // 16bits
						// reverved value, must be '1'
		int8_t const1_value; //3bits
							 // program_map_PID/network_PID 13bits
							 // network_PID - The network_PID is a 13-bit field, which is used only in conjunction with the value of the
							 // program_number set to 0x0000, specifies the PID of the Transport Stream packets which shall contain the Network
							 // Information Table. The value of the network_PID field is defined by the user, but shall only take values as specified in
							 // Table 2-3. The presence of the network_PID is optional.
		int16_t pid; //13bits
	};

	typedef SmartPTR<PayloadPATProgram_INFO> PayloadPATProgram_sptr;
	typedef SmartPTR<tagTSChannel>	TSChannel_sptr;
	typedef std::map<TSPid, TSChannel_sptr>	ChannelMap_type;

	struct PayloadPAT_INFO
	{
		// 1B
		// This is an 8-bit field, which shall be set to 0x00 as shown in Table 2-26.
		TSPsiId table_id; //8bits

						  // 2B
						  // The section_syntax_indicator is a 1-bit field which shall be set to '1'.
		int8_t section_syntax_indicator; //1bit
										 // const value, must be '0'
		int8_t const0_value; //1bit
							 // reverved value, must be '1'
		int8_t const1_value; //2bits
							 // This is a 12-bit field, the first two bits of which shall be '00'. The remaining 10 bits specify the number
							 // of bytes of the section, starting immediately following the section_length field, and including the CRC. The value in this
							 // field shall not exceed 1021 (0x3FD).
		uint16_t section_length; //12bits

								 // 2B
								 // This is a 16-bit field which serves as a label to identify this Transport Stream from any other
								 // multiplex within a network. Its value is defined by the user.
		uint16_t transport_stream_id; //16bits

									  // 1B
									  // reverved value, must be '1'
		int8_t const3_value; //2bits
							 // This 5-bit field is the version number of the whole Program Association Table. The version number
							 // shall be incremented by 1 modulo 32 whenever the definition of the Program Association Table changes. When the
							 // current_next_indicator is set to '1', then the version_number shall be that of the currently applicable Program Association
							 // Table. When the current_next_indicator is set to '0', then the version_number shall be that of the next applicable Program
							 // Association Table.
		int8_t version_number; //5bits
							   // A 1-bit indicator, which when set to '1' indicates that the Program Association Table sent is
							   // currently applicable. When the bit is set to '0', it indicates that the table sent is not yet applicable and shall be the next
							   // table to become valid.
		int8_t current_next_indicator; //1bit

									   // 1B
									   // This 8-bit field gives the number of this section. The section_number of the first section in the
									   // Program Association Table shall be 0x00. It shall be incremented by 1 with each additional section in the Program
									   // Association Table.
		uint8_t section_number; //8bits

								// 1B
								// This 8-bit field specifies the number of the last section (that is, the section with the highest
								// section_number) of the complete Program Association Table.
		uint8_t last_section_number; //8bits

									 // 1B
									 // This is an 8-bit field whose value shall be the number of bytes, immediately following the pointer_field
									 // until the first byte of the first section that is present in the payload of the Transport Stream packet (so a value of 0x00 in
									 // The pointer_field indicates that the section starts immediately after the pointer_field). When at least one section begins in
									 // a given Transport Stream packet, then the payload_unit_start_indicator (refer to 2.4.3.2) shall be set to 1 and the first
									 // byte of the payload of that Transport Stream packet shall contain the pointer. When no section begins in a given
									 // Transport Stream packet, then the payload_unit_start_indicator shall be set to 0 and no pointer shall be sent in the
									 // payload of that packet.

									 // 4B
									 // This is a 32-bit field that contains the CRC value that gives a zero output of the registers in the decoder
									 // defined in Annex A after processing the entire section.
									 // @remark crc32(bytes without pointer field, before crc32 field)
		int32_t CRC_32; //32bits

		int8_t pointer_field;
		std::vector<PayloadPATProgram_sptr> programs;
	};

	static MemPool_type* GetPoolInstance(void)
	{
		static MemPool_type s_Pool(__max(sizeof(PayloadPATProgram_INFO), sizeof(tagTSChannel)));
		return  &s_Pool;
	}

	static PayloadPATProgram_sptr Alloc(void)
	{
		return PayloadPATProgram_sptr(GetPoolInstance()->AllocObj<PayloadPATProgram_INFO>(),
			function20_bind_Ex(&MemPool_type::FreeObj<PayloadPATProgram_INFO>, GetPoolInstance()));
	}

	static TSChannel_sptr AllocChannel(void)
	{
		return TSChannel_sptr(GetPoolInstance()->AllocObj<tagTSChannel>(),
			function20_bind_Ex(&MemPool_type::FreeObj<tagTSChannel>, GetPoolInstance()));
	}

public:
	TSPayloadPAT(void)
	{
		_PATInfo.pointer_field = 0;
		_PATInfo.const0_value = 0;
		_PATInfo.const1_value = 3;
		_PATInfo.CRC_32 = 0;
		_PATInfo.section_length = 0;
		_PATInfo.section_syntax_indicator = 0;
		_PATInfo.table_id = TSPsiIdPas;

		_PATInfo.transport_stream_id = 0;
		_PATInfo.const3_value = 3;
		_PATInfo.version_number = 0;
		_PATInfo.current_next_indicator = 0;
		_PATInfo.section_number = 0;
		_PATInfo.last_section_number = 0;
	}

	~TSPayloadPAT(void)
	{

	}

	int Parse(const tagTSHeader& Header, const char* c_szData,
		UInt32 uSize, const HFNSetChannel& hSet = HFNSetChannel())
	{
		int iOffset = 0;
		_PATInfo.programs.clear();
		/**
		* When the payload of the Transport Stream packet contains PSI data, the payload_unit_start_indicator has the following
		* significance: if the Transport Stream packet carries the first byte of a PSI section, the payload_unit_start_indicator value
		* shall be '1', indicating that the first byte of the payload of this Transport Stream packet carries the pointer_field. If the
		* Transport Stream packet does not carry the first byte of a PSI section, the payload_unit_start_indicator value shall be '0',
		* indicating that there is no pointer_field in the payload. Refer to 2.4.4.1 and 2.4.4.2. This also applies to private streams of
		* stream_type 5 (refer to Table 2-29).
		*/
		if (Header.payload_unit_start_indicator)
		{
			if (uSize < 1)
			{
				LOG_Print_Error(libThirdWrap_FFmpeg, "TSPayloadPAT::Parse() ts: demux PSI");
				return -1;
			}

			_PATInfo.pointer_field = c_szData[iOffset ++];
		}

		// to calc the crc32
		char* ppat = (char*)&c_szData[iOffset];
		if (uSize - iOffset < 3)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "TSPayloadPAT::Parse() ts: demux PSI");
			return -1;
		}

		// 1B
		_PATInfo.table_id = (TSPsiId)c_szData[iOffset++];

		// 2B
		Int16 slv = 0;
		memcpy((void*)&slv, &c_szData[iOffset], sizeof(slv));
		iOffset += sizeof(slv);

		_PATInfo.section_syntax_indicator = (slv >> 15) & 0x01;
		_PATInfo.const0_value = (slv >> 14) & 0x01;
		_PATInfo.const1_value = (slv >> 12) & 0x03;
		_PATInfo.section_length = slv & 0x0FFF;

		// no section, ignore.
		if (_PATInfo.section_length == 0) 
		{
			LOG_Print_Info(libThirdWrap_FFmpeg, "TSPayloadPAT::Parse() ts: demux PAT ignore empty section");
			return sizeof(_PATInfo.section_length);
		}

		if (uSize - iOffset < _PATInfo.section_length)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "TSPayloadPAT::Parse() ts: demux PSI section");
			return -1;
		}

		/*if (uSize - iOffset < 5)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "TSPayloadPAT::Parse() ts: demux PAT");
			return -1;
		}*/

		// 2B
		memcpy((void*)&_PATInfo.transport_stream_id, &c_szData[iOffset], sizeof(_PATInfo.transport_stream_id));
		iOffset += sizeof(_PATInfo.transport_stream_id);

		// 1B
		Int8 cniv = c_szData[iOffset ++];
		_PATInfo.const3_value = (cniv >> 6) & 0x03;
		_PATInfo.version_number = (cniv >> 1) & 0x1F;
		_PATInfo.current_next_indicator = cniv & 0x01;

		// TODO: FIXME: check the indicator.

		// 1B
		_PATInfo.section_number = c_szData[iOffset++];
		// 1B
		_PATInfo.last_section_number = c_szData[iOffset++];

		// multiple 4B program data.
		TSChannel_sptr Channel;
		PayloadPATProgram_sptr sptr;
		int program_bytes = _PATInfo.section_length - 4 - (uSize - iOffset);
		for (int i = 0; i < program_bytes; i += 4)
		{
			sptr = TSPayloadPAT::Alloc();
			int tmpv = 0;
			memcpy((void*)&tmpv, &c_szData[iOffset], 4);
			sptr->number = (int16_t)((tmpv >> 16) & 0xFFFF);
			sptr->const1_value = (int16_t)((tmpv >> 13) & 0x07);
			sptr->pid = (int16_t)(tmpv & 0x1FFF);
			_PATInfo.programs.push_back(sptr);
			iOffset += 4;
			
			// update the apply pid table.
			if (hSet)
				hSet((TSPid)sptr->pid, TSPidApplyPMT, TSStreamReserved);
		}

		// update the apply pid table.
		/*packet->context->set(packet->pid, TSPidApplyPAT);
		packet->context->on_pmt_parsed();*/

		if (uSize - iOffset < 4)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "TSPayloadPAT::Parse() ts: demux PSI crc32");
			return -1;
		}

		memcpy((void*)&_PATInfo.CRC_32, &c_szData[iOffset], sizeof(_PATInfo.CRC_32));

		Int32 crc32 = _common_::crc32((UInt8*)ppat, &c_szData[iOffset] - c_szData - 4);
		if (_PATInfo.CRC_32 != crc32)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "TSPayloadPAT::Parse() ts: verify PSI crc32");
			return -2;
		}

		iOffset += sizeof(_PATInfo.CRC_32);

		if (uSize - iOffset > 0)
		{
			int nb_stuffings = uSize - iOffset;
			char* stuffing = (char*)&c_szData[iOffset];
			// all stuffing must be 0xff.
			// TODO: FIXME: maybe need to remove the following.
			for (int i = 0; i < nb_stuffings; i++) 
			{
				if ((uint8_t)stuffing[i] != 0xff)
				{
					LOG_Print_Error(libThirdWrap_FFmpeg, "TSPayloadPAT::Parse() ts: stuff is not 0xff, actual=%#x", stuffing[i]);
					break;
				}
			}

			iOffset += nb_stuffings;
		}

		return iOffset;
	}

private:
	PayloadPAT_INFO	_PATInfo;
};

class TSPayloadPMT
{
public:
	struct PayloadPMTES_INFO
	{
		// 1B
		// This is an 8-bit field specifying the type of program element carried within the packets with the PID
		// whose value is specified by the elementary_PID. The values of stream_type are specified in Table 2-29.
		TSStream stream_type; //8bits

								 // 2B
								 // reverved value, must be '1'
		int8_t const1_value0; //3bits
							  // This is a 13-bit field specifying the PID of the Transport Stream packets which carry the associated
							  // program element.
		int16_t elementary_PID; //13bits

								// (2+x)B
								// reverved value, must be '1'
		int8_t const1_value1; //4bits
		std::vector<char> ES_info; //[ES_info_length] bytes.
	};

	typedef SmartPTR<PayloadPMTES_INFO> PayloadPMTES_sptr;

	struct TSPayloadPMT_INFO
	{
		// 1B
		// This is an 8-bit field, which shall be set to 0x00 as shown in Table 2-26.
		TSPsiId table_id; //8bits
						  // 2B
						  // The section_syntax_indicator is a 1-bit field which shall be set to '1'.
		int8_t section_syntax_indicator; //1bit
										 // const value, must be '0'
		int8_t const0_value; //1bit
							 // reverved value, must be '1'
		int8_t const1_value; //2bits
							 // This is a 12-bit field, the first two bits of which shall be '00'. The remaining 10 bits specify the number
							 // of bytes of the section, starting immediately following the section_length field, and including the CRC. The value in this
							 // field shall not exceed 1021 (0x3FD).
		uint16_t section_length; //12bits

								 // 2B
								 // program_number is a 16-bit field. It specifies the program to which the program_map_PID is
								 // applicable. One program definition shall be carried within only one TS_program_map_section. This implies that a
								 // program definition is never longer than 1016 (0x3F8). See Informative Annex C for ways to deal with the cases when
								 // that length is not sufficient. The program_number may be used as a designation for a broadcast channel, for example. By
								 // describing the different program elements belonging to a program, data from different sources (e.g. sequential events)
								 // can be concatenated together to form a continuous set of streams using a program_number. For examples of applications
								 // refer to Annex C.
		uint16_t program_number; //16bits

								 // 1B
								 // reverved value, must be '1'
		int8_t const1_value0; //2bits
							  // This 5-bit field is the version number of the TS_program_map_section. The version number shall be
							  // incremented by 1 modulo 32 when a change in the information carried within the section occurs. Version number refers
							  // to the definition of a single program, and therefore to a single section. When the current_next_indicator is set to '1', then
							  // The version_number shall be that of the currently applicable TS_program_map_section. When the current_next_indicator
							  // is set to '0', then the version_number shall be that of the next applicable TS_program_map_section.
		int8_t version_number; //5bits
							   // A 1-bit field, which when set to '1' indicates that the TS_program_map_section sent is
							   // currently applicable. When the bit is set to '0', it indicates that the TS_program_map_section sent is not yet applicable
							   // and shall be the next TS_program_map_section to become valid.
		int8_t current_next_indicator; //1bit

									   // 1B
									   // The value of this 8-bit field shall be 0x00.
		uint8_t section_number; //8bits

								// 1B
								// The value of this 8-bit field shall be 0x00.
		uint8_t last_section_number; //8bits

									 // 2B
									 // reverved value, must be '1'
		int8_t const1_value1; //3bits
							  // This is a 13-bit field indicating the PID of the Transport Stream packets which shall contain the PCR fields
							  // valid for the program specified by program_number. If no PCR is associated with a program definition for private
							  // streams, then this field shall take the value of 0x1FFF. Refer to the semantic definition of PCR in 2.4.3.5 and Table 2-3
							  // for restrictions on the choice of PCR_PID value.
		int16_t PCR_PID; //13bits

							// 2B
		int8_t const1_value2; //4bits
							  // This is a 12-bit field, the first two bits of which shall be '00'. The remaining 10 bits specify the
							  // number of bytes of the descriptors immediately following the program_info_length field.
		std::vector<char> program_info_desc; //[program_info_length]bytes

		int32_t CRC_32; //32bits
		int8_t pointer_field;
											 // array of TSPMTESInfo.
		std::vector<PayloadPMTES_sptr> infos;
	};

	static PayloadPMTES_sptr Alloc(void)
	{
		static MemPool_type s_Pool(sizeof(PayloadPMTES_INFO));
		return PayloadPMTES_sptr(s_Pool.AllocObj<PayloadPMTES_INFO>(),
			function20_bind_Ex(&MemPool_type::FreeObj<PayloadPMTES_INFO>, &s_Pool));
	}

public:
	TSPayloadPMT(void)
	{
		_PMTInfo.pointer_field = 0;
		_PMTInfo.const0_value = 0;
		_PMTInfo.const1_value = 3;
		_PMTInfo.CRC_32 = 0;
		_PMTInfo.section_length = 0;
		_PMTInfo.section_syntax_indicator = 0;
		_PMTInfo.table_id = TSPsiIdPas;

		_PMTInfo.const1_value0 = 3;
		_PMTInfo.const1_value1 = 7;
		_PMTInfo.const1_value2 = 0x0f;
		_PMTInfo.PCR_PID = 0;
		_PMTInfo.last_section_number = 0;
		_PMTInfo.program_number = 0;
		_PMTInfo.version_number = 0;
		_PMTInfo.current_next_indicator = 0;
		_PMTInfo.section_number = 0;
	}

	~TSPayloadPMT(void)
	{

	}

	int Parse(const tagTSHeader& Header, const char* c_szData,
		UInt32 uSize, const HFNSetChannel& hSet = HFNSetChannel())
	{
		int iOffset = 0;
		_PMTInfo.infos.clear();

		/**
		* When the payload of the Transport Stream packet contains PSI data, the payload_unit_start_indicator has the following
		* significance: if the Transport Stream packet carries the first byte of a PSI section, the payload_unit_start_indicator value
		* shall be '1', indicating that the first byte of the payload of this Transport Stream packet carries the pointer_field. If the
		* Transport Stream packet does not carry the first byte of a PSI section, the payload_unit_start_indicator value shall be '0',
		* indicating that there is no pointer_field in the payload. Refer to 2.4.4.1 and 2.4.4.2. This also applies to private streams of
		* stream_type 5 (refer to Table 2-29).
		*/
		if (Header.payload_unit_start_indicator)
		{
			if (uSize < 1)
			{
				LOG_Print_Error(libThirdWrap_FFmpeg, "TSPayloadPMT::Parse() ts: demux PSI");
				return -1;
			}

			_PMTInfo.pointer_field = c_szData[iOffset++];
		}

		// to calc the crc32
		char* ppat = (char*)&c_szData[iOffset];
		if (uSize - iOffset < 3)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "TSPayloadPMT::Parse() ts: demux PSI");
			return -1;
		}

		// 1B
		_PMTInfo.table_id = (TSPsiId)c_szData[iOffset++];

		// 2B
		Int16 slv = 0;
		memcpy((void*)&slv, &c_szData[iOffset], sizeof(slv));
		iOffset += sizeof(slv);

		_PMTInfo.section_syntax_indicator = (slv >> 15) & 0x01;
		_PMTInfo.const0_value = (slv >> 14) & 0x01;
		_PMTInfo.const1_value = (slv >> 12) & 0x03;
		_PMTInfo.section_length = slv & 0x0FFF;

		// no section, ignore.
		if (_PMTInfo.section_length == 0)
		{
			LOG_Print_Info(libThirdWrap_FFmpeg, "TSPayloadPMT::Parse() ts: demux PMT ignore empty section");
			return sizeof(_PMTInfo.section_length);
		}

		if (uSize - iOffset < _PMTInfo.section_length)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "TSPayloadPMT::Parse() ts: demux PSI section");
			return -1;
		}

		/*if (uSize - iOffset < 5)
		{
		LOG_Print_Error(libThirdWrap_FFmpeg, "TSPayloadPMT::Parse() ts: demux PAT");
		return -1;
		}*/

		// 2B
		memcpy((void*)&_PMTInfo.program_number, &c_szData[iOffset], sizeof(_PMTInfo.program_number));
		iOffset += sizeof(_PMTInfo.program_number);

		// 1B
		Int8 cniv = c_szData[iOffset++];
		_PMTInfo.const1_value0 = (cniv >> 6) & 0x03;
		_PMTInfo.version_number = (cniv >> 1) & 0x1F;
		_PMTInfo.current_next_indicator = cniv & 0x01;

		// TODO: FIXME: check the indicator.

		// 1B
		_PMTInfo.section_number = c_szData[iOffset++];
		// 1B
		_PMTInfo.last_section_number = c_szData[iOffset++];

		// 2B
		Int16 ppv = 0;
		memcpy((void*)&ppv, &c_szData[iOffset], sizeof(ppv));
		iOffset += sizeof(ppv);

		_PMTInfo.const1_value1 = (ppv >> 13) & 0x07;
		_PMTInfo.PCR_PID = ppv & 0x1FFF;

		// 2B
		Int16 pilv = 0;
		memcpy((void*)&pilv, &c_szData[iOffset], sizeof(pilv));
		iOffset += sizeof(pilv);

		_PMTInfo.const1_value2 = (pilv >> 12) & 0x0F;

		/**
		* This is a 12-bit field, the first two bits of which shall be '00'. The remaining 10 bits specify the
		* number of bytes of the descriptors immediately following the program_info_length field.
		*/
		uint16_t program_info_length = pilv & 0xFFF;
		if (program_info_length > 0)
		{
			if (uSize - iOffset < program_info_length)
			{
				LOG_Print_Error(libThirdWrap_FFmpeg, "TSPayloadPMT::Parse() ts: demux PMT program info");
				return -1;
			}

			_PMTInfo.program_info_desc.resize(program_info_length);
			memcpy(&_PMTInfo.program_info_desc[0], &c_szData[iOffset], program_info_length);
			iOffset += program_info_length;
		}

		// [section_length] - 4(CRC) - 9B - [program_info_length]
		int ES_EOF_pos = iOffset + _PMTInfo.section_length - 4 - 9 - program_info_length;
		while (iOffset < ES_EOF_pos)
		{
			if (uSize - iOffset < 5)
			{
				LOG_Print_Error(libThirdWrap_FFmpeg, "TSPayloadPMT::Parse() ts: demux PMT");
				return -1;
			}

			PayloadPMTES_sptr sptr = TSPayloadPMT::Alloc();
			sptr->stream_type = (TSStream)c_szData[iOffset ++];

			Int16 epv = 0;
			memcpy((void*)&epv, &c_szData[iOffset], sizeof(epv));
			iOffset += epv;

			sptr->const1_value0 = (epv >> 13) & 0x07;
			sptr->elementary_PID = epv & 0x1FFF;

			Int16 eilv = 0;
			memcpy((void*)&eilv, &c_szData[iOffset], sizeof(eilv));
			iOffset += sizeof(eilv);

			sptr->const1_value1 = (eilv >> 12) & 0x0f;
			/**
			* This is a 12-bit field, the first two bits of which shall be '00'. The remaining 10 bits specify the number
			* of bytes of the descriptors of the associated program element immediately following the ES_info_length field.
			*/
			Int16 ES_info_length = eilv & 0x0FFF;

			if (ES_info_length > 0) 
			{
				if (uSize - iOffset < ES_info_length)
				{
					LOG_Print_Error(libThirdWrap_FFmpeg, "TSPayloadPMT::Parse() ts: demux PMT ES_info");
					return -1;
				}

				sptr->ES_info.resize(ES_info_length);
				memcpy(&sptr->ES_info[0], &c_szData[iOffset], ES_info_length);
				iOffset += ES_info_length;
			}

			// update the apply pid table
			switch (sptr->stream_type)
			{
			case TSStreamVideoH264:
			case TSStreamVideoMpeg4:
				if (hSet)
					hSet((TSPid)sptr->elementary_PID, TSPidApplyVideo, sptr->stream_type);
				//packet->context->set(info->elementary_PID, TSPidApplyVideo, info->stream_type);
				break;
			case TSStreamAudioAAC:
			case TSStreamAudioAC3:
			case TSStreamAudioDTS:
			case TSStreamAudioMp3:
				if (hSet)
					hSet((TSPid)sptr->elementary_PID, TSPidApplyAudio, sptr->stream_type);
				//packet->context->set(info->elementary_PID, TSPidApplyAudio, info->stream_type);
				break;
			default:
				LOG_Print_Info(libThirdWrap_FFmpeg, "TSPayloadPMT::Parse() ts: drop pid=%#x, stream=%#x",
					sptr->elementary_PID, sptr->stream_type);
				break;
			}

			_PMTInfo.infos.push_back(sptr);
		}

		// update the apply pid table.
		//packet->context->set(packet->pid, TSPidApplyPMT);

		return iOffset;
	}

private:
	TSPayloadPMT_INFO	_PMTInfo;
};

// The PES payload of ts packet.
// 2.4.3.6 PES packet, hls-mpeg-ts-iso13818-1.pdf, page 49
class TSPayloadPES
{
public:
	typedef _foundation_::_string_big_t		_string_t;
	typedef TSPayloadPAT::TSChannel_sptr	TSChannel_sptr;

	struct tagPayloadPES_INFO
	{
		// 3B
		// The packet_start_code_prefix is a 24-bit code. Together with the stream_id that follows it
		// constitutes a packet start code that identifies the beginning of a packet. The packet_start_code_prefix is the bit string
		// '0000 0000 0000 0000 0000 0001' (0x000001).
		int32_t packet_start_code_prefix; //24bits
										  // 1B
										  // In Program Streams, the stream_id specifies the type and number of the elementary stream as defined by the
										  // stream_id Table 2-18. In Transport Streams, the stream_id may be set to any valid value which correctly describes the
										  // elementary stream type as defined in Table 2-18. In Transport Streams, the elementary stream type is specified in the
										  // Program Specific Information as specified in 2.4.4.
										  // @see TSPESStreamId, value can be TSPESStreamIdAudioCommon or TSPESStreamIdVideoCommon.
		uint8_t stream_id; //8bits
						   // 2B
						   // A 16-bit field specifying the number of bytes in the PES packet following the last byte of the
						   // field. A value of 0 indicates that the PES packet length is neither specified nor bounded and is allowed only in
						   // PES packets whose payload consists of bytes from a video elementary stream contained in Transport Stream packets.
		uint16_t PES_packet_length; //16bits

									// 1B
									// 2bits const '10'
		int8_t const2bits; //2bits
						   // The 2-bit PES_scrambling_control field indicates the scrambling mode of the PES packet
						   // payload. When scrambling is performed at the PES level, the PES packet header, including the optional fields when
						   // present, shall not be scrambled (see Table 2-19).
		int8_t PES_scrambling_control; //2bits
									   // This is a 1-bit field indicating the priority of the payload in this PES packet. A '1' indicates a higher
									   // priority of the payload of the PES packet payload than a PES packet payload with this field set to '0'. A multiplexor can
									   // use the PES_priority bit to prioritize its data within an elementary stream. This field shall not be changed by the transport
									   // mechanism.
		int8_t PES_priority; //1bit
							 // This is a 1-bit flag. When set to a value of '1' it indicates that the PES packet header is
							 // immediately followed by the video start code or audio syncword indicated in the data_stream_alignment_descriptor
							 // in 2.6.10 if this descriptor is present. If set to a value of '1' and the descriptor is not present, alignment as indicated in
							 // alignment_type '01' in Table 2-47 and Table 2-48 is required. When set to a value of '0' it is not defined whether any such
							 // alignment occurs or not.
		int8_t data_alignment_indicator; //1bit
										 // This is a 1-bit field. When set to '1' it indicates that the material of the associated PES packet payload is
										 // protected by copyright. When set to '0' it is not defined whether the material is protected by copyright. A copyright
										 // descriptor described in 2.6.24 is associated with the elementary stream which contains this PES packet and the copyright
										 // flag is set to '1' if the descriptor applies to the material contained in this PES packet
		int8_t copyright; //1bit
						  // This is a 1-bit field. When set to '1' the contents of the associated PES packet payload is an original.
						  // When set to '0' it indicates that the contents of the associated PES packet payload is a copy.
		int8_t original_or_copy; //1bit

								 // 1B
								 // This is a 2-bit field. When the PTS_DTS_flags field is set to '10', the PTS fields shall be present in
								 // the PES packet header. When the PTS_DTS_flags field is set to '11', both the PTS fields and DTS fields shall be present
								 // in the PES packet header. When the PTS_DTS_flags field is set to '00' no PTS or DTS fields shall be present in the PES
								 // packet header. The value '01' is forbidden.
		int8_t PTS_DTS_flags; //2bits
							  // A 1-bit flag, which when set to '1' indicates that ESCR base and extension fields are present in the PES
							  // packet header. When set to '0' it indicates that no ESCR fields are present.
		int8_t ESCR_flag; //1bit
						  // A 1-bit flag, which when set to '1' indicates that the ES_rate field is present in the PES packet header.
						  // When set to '0' it indicates that no ES_rate field is present.
		int8_t ES_rate_flag; //1bit
							 // A 1-bit flag, which when set to '1' it indicates the presence of an 8-bit trick mode field. When
							 // set to '0' it indicates that this field is not present.
		int8_t DSM_trick_mode_flag; //1bit
									// A 1-bit flag, which when set to '1' indicates the presence of the additional_copy_info field.
									// When set to '0' it indicates that this field is not present.
		int8_t additional_copy_info_flag; //1bit
										  // A 1-bit flag, which when set to '1' indicates that a CRC field is present in the PES packet. When set to
										  // '0' it indicates that this field is not present.
		int8_t PES_CRC_flag; //1bit
							 // A 1-bit flag, which when set to '1' indicates that an extension field exists in this PES packet
							 // header. When set to '0' it indicates that this field is not present.
		int8_t PES_extension_flag; //1bit

								   // 1B
								   // An 8-bit field specifying the total number of bytes occupied by the optional fields and any
								   // stuffing bytes contained in this PES packet header. The presence of optional fields is indicated in the byte that precedes
								   // the PES_header_data_length field.
		uint8_t PES_header_data_length; //8bits

										// 5B
										// Presentation times shall be related to decoding times as follows: The PTS is a 33-bit
										// number coded in three separate fields. It indicates the time of presentation, tp n (k), in the system target decoder of a
										// presentation unit k of elementary stream n. The value of PTS is specified in units of the period of the system clock
										// frequency divided by 300 (yielding 90 kHz). The presentation time is derived from the PTS according to equation 2-11
										// below. Refer to 2.7.4 for constraints on the frequency of coding presentation timestamps.
										// ===========1B
										// 4bits const
										// 3bits PTS [32..30]
										// 1bit const '1'
										// ===========2B
										// 15bits PTS [29..15]
										// 1bit const '1'
										// ===========2B
										// 15bits PTS [14..0]
										// 1bit const '1'
		int64_t pts; // 33bits

					 // 5B
					 // The DTS is a 33-bit number coded in three separate fields. It indicates the decoding time,
					 // td n (j), in the system target decoder of an access unit j of elementary stream n. The value of DTS is specified in units of
					 // the period of the system clock frequency divided by 300 (yielding 90 kHz).
					 // ===========1B
					 // 4bits const
					 // 3bits DTS [32..30]
					 // 1bit const '1'
					 // ===========2B
					 // 15bits DTS [29..15]
					 // 1bit const '1'
					 // ===========2B
					 // 15bits DTS [14..0]
					 // 1bit const '1'
		int64_t dts; // 33bits

					 // 6B
					 // The elementary stream clock reference is a 42-bit field coded in two parts. The first
					 // part, ESCR_base, is a 33-bit field whose value is given by ESCR_base(i), as given in equation 2-14. The second part,
					 // ESCR_ext, is a 9-bit field whose value is given by ESCR_ext(i), as given in equation 2-15. The ESCR field indicates the
					 // intended time of arrival of the byte containing the last bit of the ESCR_base at the input of the PES-STD for PES streams
					 // (refer to 2.5.2.4).
					 // 2bits reserved
					 // 3bits ESCR_base[32..30]
					 // 1bit const '1'
					 // 15bits ESCR_base[29..15]
					 // 1bit const '1'
					 // 15bits ESCR_base[14..0]
					 // 1bit const '1'
					 // 9bits ESCR_extension
					 // 1bit const '1'
		int64_t ESCR_base; //33bits
		int16_t ESCR_extension; //9bits

								// 3B
								// The ES_rate field is a 22-bit unsigned integer specifying the rate at which the
								// system target decoder receives bytes of the PES packet in the case of a PES stream. The ES_rate is valid in the PES
								// packet in which it is included and in subsequent PES packets of the same PES stream until a new ES_rate field is
								// encountered. The value of the ES_rate is measured in units of 50 bytes/second. The value 0 is forbidden. The value of the
								// ES_rate is used to define the time of arrival of bytes at the input of a P-STD for PES streams defined in 2.5.2.4. The
								// value encoded in the ES_rate field may vary from PES_packet to PES_packet.
								// 1bit const '1'
								// 22bits ES_rate
								// 1bit const '1'
		int32_t ES_rate; //22bits

						 // 1B
						 // A 3-bit field that indicates which trick mode is applied to the associated video stream. In cases of
						 // other types of elementary streams, the meanings of this field and those defined by the following five bits are undefined.
						 // For the definition of trick_mode status, refer to the trick mode section of 2.4.2.3.
		int8_t trick_mode_control; //3bits
		int8_t trick_mode_value; //5bits

								 // 1B
								 // 1bit const '1'
								 // This 7-bit field contains private data relating to copyright information.
		int8_t additional_copy_info; //7bits

									 // 2B
									 // The previous_PES_packet_CRC is a 16-bit field that contains the CRC value that yields
									 // a zero output of the 16 registers in the decoder similar to the one defined in Annex A,
		int16_t previous_PES_packet_CRC; //16bits

										 // 1B
										 // A 1-bit flag which when set to '1' indicates that the PES packet header contains private data.
										 // When set to a value of '0' it indicates that private data is not present in the PES header.
		int8_t PES_private_data_flag; //1bit
									  // A 1-bit flag which when set to '1' indicates that an ISO/IEC 11172-1 pack header or a
									  // Program Stream pack header is stored in this PES packet header. If this field is in a PES packet that is contained in a
									  // Program Stream, then this field shall be set to '0'. In a Transport Stream, when set to the value '0' it indicates that no pack
									  // header is present in the PES header.
		int8_t pack_header_field_flag; //1bit
									   // A 1-bit flag which when set to '1' indicates that the
									   // program_packet_sequence_counter, MPEG1_MPEG2_identifier, and original_stuff_length fields are present in this
									   // PES packet. When set to a value of '0' it indicates that these fields are not present in the PES header.
		int8_t program_packet_sequence_counter_flag; //1bit
													 // A 1-bit flag which when set to '1' indicates that the P-STD_buffer_scale and P-STD_buffer_size
													 // are present in the PES packet header. When set to a value of '0' it indicates that these fields are not present in the
													 // PES header.
		int8_t P_STD_buffer_flag; //1bit
								  // reverved value, must be '1'
		int8_t const1_value0; //3bits
							  // A 1-bit field which when set to '1' indicates the presence of the PES_extension_field_length
							  // field and associated fields. When set to a value of '0' this indicates that the PES_extension_field_length field and any
							  // associated fields are not present.
		int8_t PES_extension_flag_2; //1bit

									 // 16B
									 // This is a 16-byte field which contains private data. This data, combined with the fields before and
									 // after, shall not emulate the packet_start_code_prefix (0x000001).
		std::vector<char> PES_private_data; //128bits

											// (1+x)B
		std::vector<char> pack_field; //[pack_field_length] bytes

									  // 2B
									  // 1bit const '1'
									  // The program_packet_sequence_counter field is a 7-bit field. It is an optional
									  // counter that increments with each successive PES packet from a Program Stream or from an ISO/IEC 11172-1 Stream or
									  // the PES packets associated with a single program definition in a Transport Stream, providing functionality similar to a
									  // continuity counter (refer to 2.4.3.2). This allows an application to retrieve the original PES packet sequence of a Program
									  // Stream or the original packet sequence of the original ISO/IEC 11172-1 stream. The counter will wrap around to 0 after
									  // its maximum value. Repetition of PES packets shall not occur. Consequently, no two consecutive PES packets in the
									  // program multiplex shall have identical program_packet_sequence_counter values.
		int8_t program_packet_sequence_counter; //7bits
												// 1bit const '1'
												// A 1-bit flag which when set to '1' indicates that this PES packet carries information from
												// an ISO/IEC 11172-1 stream. When set to '0' it indicates that this PES packet carries information from a Program Stream.
		int8_t MPEG1_MPEG2_identifier; //1bit
									   // This 6-bit field specifies the number of stuffing bytes used in the original ITU-T
									   // Rec. H.222.0 | ISO/IEC 13818-1 PES packet header or in the original ISO/IEC 11172-1 packet header.
		int8_t original_stuff_length; //6bits

									  // 2B
									  // 2bits const '01'
									  // The P-STD_buffer_scale is a 1-bit field, the meaning of which is only defined if this PES packet
									  // is contained in a Program Stream. It indicates the scaling factor used to interpret the subsequent P-STD_buffer_size field.
									  // If the preceding stream_id indicates an audio stream, P-STD_buffer_scale shall have the value '0'. If the preceding
									  // stream_id indicates a video stream, P-STD_buffer_scale shall have the value '1'. For all other stream types, the value
									  // may be either '1' or '0'.
		int8_t P_STD_buffer_scale; //1bit
								   // The P-STD_buffer_size is a 13-bit unsigned integer, the meaning of which is only defined if this
								   // PES packet is contained in a Program Stream. It defines the size of the input buffer, BS n , in the P-STD. If
								   // P-STD_buffer_scale has the value '0', then the P-STD_buffer_size measures the buffer size in units of 128 bytes. If
								   // P-STD_buffer_scale has the value '1', then the P-STD_buffer_size measures the buffer size in units of 1024 bytes.
		int16_t P_STD_buffer_size; //13bits

								   // (1+x)B
								   // 1bit const '1'
		std::vector<char> PES_extension_field; //[PES_extension_field_length] bytes

											   // NB
											   // This is a fixed 8-bit value equal to '1111 1111' that can be inserted by the encoder, for example to meet
											   // the requirements of the channel. It is discarded by the decoder. No more than 32 stuffing bytes shall be present in one
											   // PES packet header.
		int nb_stuffings;

		// NB
		// PES_packet_data_bytes shall be contiguous bytes of data from the elementary stream
		// indicated by the packet's stream_id or PID. When the elementary stream data conforms to ITU-T
		// Rec. H.262 | ISO/IEC 13818-2 or ISO/IEC 13818-3, the PES_packet_data_bytes shall be byte aligned to the bytes of this
		// Recommendation | International Standard. The byte-order of the elementary stream shall be preserved. The number of
		// PES_packet_data_bytes, N, is specified by the PES_packet_length field. N shall be equal to the value indicated in the
		// PES_packet_length minus the number of bytes between the last byte of the PES_packet_length field and the first
		// PES_packet_data_byte.
		//
		// In the case of a private_stream_1, private_stream_2, ECM_stream, or EMM_stream, the contents of the
		// PES_packet_data_byte field are user definable and will not be specified by ITU-T | ISO/IEC in the future.
		int nb_bytes;

		// NB
		// This is a fixed 8-bit value equal to '1111 1111'. It is discarded by the decoder.
		int nb_paddings;
	};

public:
	TSPayloadPES(void)
	{
		_PESInfo.nb_stuffings = 0;
		_PESInfo.nb_bytes = 0;
		_PESInfo.nb_paddings = 0;
		_PESInfo.const2bits = 0x02;
		_PESInfo.const1_value0 = 0x07;

		_PESInfo.packet_start_code_prefix = 0;
		_PESInfo.stream_id = 0;
		_PESInfo.PES_packet_length = 0;
		_PESInfo.PES_scrambling_control = 0;
		_PESInfo.PES_priority = 0;
		_PESInfo.data_alignment_indicator = 0;
		_PESInfo.copyright = 0;
		_PESInfo.original_or_copy = 0;
		_PESInfo.PTS_DTS_flags = 0;
		_PESInfo.ESCR_flag = 0;
		_PESInfo.ES_rate_flag = 0;
		_PESInfo.DSM_trick_mode_flag = 0;
		_PESInfo.additional_copy_info_flag = 0;
		_PESInfo.PES_CRC_flag = 0;
		_PESInfo.PES_extension_flag = 0;
		_PESInfo.PES_header_data_length = 0;
		_PESInfo.pts = _PESInfo.dts = 0;
		_PESInfo.ESCR_base = 0;
		_PESInfo.ESCR_extension = 0;
		_PESInfo.ES_rate = 0;
		_PESInfo.trick_mode_control = 0;
		_PESInfo.trick_mode_value = 0;
		_PESInfo.additional_copy_info = 0;
		_PESInfo.previous_PES_packet_CRC = 0;
		_PESInfo.PES_private_data_flag = 0;
		_PESInfo.pack_header_field_flag = 0;
		_PESInfo.program_packet_sequence_counter_flag = 0;
		_PESInfo.P_STD_buffer_flag = 0;
		_PESInfo.PES_extension_flag_2 = 0;
		_PESInfo.program_packet_sequence_counter = 0;
		_PESInfo.MPEG1_MPEG2_identifier = 0;
		_PESInfo.original_stuff_length = 0;
		_PESInfo.P_STD_buffer_scale = 0;
		_PESInfo.P_STD_buffer_size = 0;
	}

	~TSPayloadPES(void)
	{

	}

	int Parse(const tagTSHeader& Header, const TSChannel_sptr& Channel,
		const char* c_szData, UInt32 uSize)
	{
		if (!Channel)
		{
			LOG_Print_Error(libThirdWrap_FFmpeg, "TSPayloadPES::Parse() ts: demux PES no channel for pid=%#x", Header.pid);
			return -1;
		}

		if (_PESInfo.stream_id == 0 && Header.payload_unit_start_indicator != 1)
		{//未收到开始帧
			LOG_Print_Error(libThirdWrap_FFmpeg, "TSPayloadPES::Parse() ts: demux PES no start flag");
			return -1;
		}
		
		if (uSize < sizeof(tagPayloadPES_INFO))
			return -1;

		////////////////////////需理清思路，再设计逻辑////////////////////////

		if (Header.payload_unit_start_indicator == 1 && _PESInfo.stream_id != 0)
		{//收到另外一帧的开始标志
			if (_sData.size() < _PESInfo.PES_packet_length)
			{//丢弃上一帧
				_sData.update_size(0);
			}

			//回调上报PES负载数据
		}

		////////////////////////需理清思路，再设计逻辑////////////////////////

		if (Header.payload_unit_start_indicator == 1)
		{//头信息
			const UInt8 packet_start_code_prefix[3] = { 0x00, 0x00, 0x01 };
			if (memcmp(packet_start_code_prefix, c_szData, 3) != 0)
			{
				LOG_Print_Error(libThirdWrap_FFmpeg,
					"TSPayloadPES::Parse() ts: demux PES start code failed, expect=0x000001, actual=0x%2d%2d%2d",
					c_szData[0], c_szData[1], c_szData[2]);
				return -1;
			}

			int iOffset = 3;
			_PESInfo.packet_start_code_prefix = 0x000001;
			_PESInfo.stream_id = c_szData[iOffset++];

			UInt16 uPacketLength = 0;
			memcpy(&uPacketLength, &c_szData[iOffset], sizeof(uPacketLength));
			iOffset += sizeof(uPacketLength);
			_PESInfo.PES_packet_length = uPacketLength;

			int pos_packet = iOffset;

			if (_PESInfo.stream_id != TSPESStreamIdProgramStreamMap
				&& _PESInfo.stream_id != TSPESStreamIdPaddingStream
				&& _PESInfo.stream_id != TSPESStreamIdPrivateStream2
				&& _PESInfo.stream_id != TSPESStreamIdEcmStream
				&& _PESInfo.stream_id != TSPESStreamIdEmmStream
				&& _PESInfo.stream_id != TSPESStreamIdProgramStreamDirectory
				&& _PESInfo.stream_id != TSPESStreamIdDsmccStream
				&& _PESInfo.stream_id != TSPESStreamIdH2221TypeE
				)
			{
				// 1B
				int8_t oocv = c_szData[iOffset++];

				// 1B
				int8_t pefv = c_szData[iOffset++];
				// 1B

				_PESInfo.const2bits = (oocv >> 6) & 0x03;
				_PESInfo.PES_scrambling_control = (oocv >> 4) & 0x03;
				_PESInfo.PES_priority = (oocv >> 3) & 0x01;
				_PESInfo.data_alignment_indicator = (oocv >> 2) & 0x01;
				_PESInfo.copyright = (oocv >> 1) & 0x01;
				_PESInfo.original_or_copy = oocv & 0x01;

				_PESInfo.PTS_DTS_flags = (pefv >> 6) & 0x03;
				_PESInfo.ESCR_flag = (pefv >> 5) & 0x01;
				_PESInfo.ES_rate_flag = (pefv >> 4) & 0x01;
				_PESInfo.DSM_trick_mode_flag = (pefv >> 3) & 0x01;
				_PESInfo.additional_copy_info_flag = (pefv >> 2) & 0x01;
				_PESInfo.PES_CRC_flag = (pefv >> 1) & 0x01;
				_PESInfo.PES_extension_flag = pefv & 0x01;

				_PESInfo.PES_header_data_length = c_szData[iOffset++];

				int pos_header = iOffset;

				// check required together.
				int nb_required = 0;
				nb_required += (_PESInfo.PTS_DTS_flags == 0x2) ? 5 : 0;
				nb_required += (_PESInfo.PTS_DTS_flags == 0x3) ? 10 : 0;
				nb_required += _PESInfo.ESCR_flag ? 6 : 0;
				nb_required += _PESInfo.ES_rate_flag ? 3 : 0;
				nb_required += _PESInfo.DSM_trick_mode_flag ? 1 : 0;
				nb_required += _PESInfo.additional_copy_info_flag ? 1 : 0;
				nb_required += _PESInfo.PES_CRC_flag ? 2 : 0;
				nb_required += _PESInfo.PES_extension_flag ? 1 : 0;

				if (iOffset + nb_required > uSize)
					return -1;

				// 5B
				if (_PESInfo.PTS_DTS_flags == 0x2) 
				{
					int64_t pts = 0;
					if ((decode_33bits_dts_pts(&c_szData[iOffset], &_PESInfo.pts)) == -1)
					{
						return -1;
					}

					_PESInfo.dts = _PESInfo.pts;
					iOffset += 5;
				}

				// 10B
				if (_PESInfo.PTS_DTS_flags == 0x3) 
				{
					if (decode_33bits_dts_pts(&c_szData[iOffset], &_PESInfo.pts) == -1) 
					{
						return -1;
					}

					if (decode_33bits_dts_pts(&c_szData[iOffset], &_PESInfo.dts) == -1) 
					{
						return -1;
					}

					// check sync, the diff of dts and pts should never greater than 1s.
					if (_PESInfo.dts - _PESInfo.pts > 90000 || _PESInfo.pts - _PESInfo.dts > 90000) 
					{
						/*LOG_Print_Warning(libThirdWrap_FFmpeg,
							"TSPayloadPES::Parse() ts: sync dts=%" PRId64 ", pts=%" PRId64,
							_PESInfo.dts, _PESInfo.pts);*/

						LOG_Print_Warning(libThirdWrap_FFmpeg,
							"TSPayloadPES::Parse() ts: sync dts=%lld, pts=%lld",
							_PESInfo.dts, _PESInfo.pts);
					}

					iOffset += 10;
				}

				// Ignore coverage bellow, for we don't use them in HLS.
				// LCOV_EXCL_START

				// 6B
				if (_PESInfo.ESCR_flag)
				{
					_PESInfo.ESCR_extension = 0;
					_PESInfo.ESCR_base = 0;

					iOffset += 6;
					LOG_Print_Warning(libThirdWrap_FFmpeg,
						"TSPayloadPES::Parse() ts: demux PES, ignore the escr.");
				}

				// 3B
				if (_PESInfo.ES_rate_flag)
				{
					int32_t ES_rate = 0;
					memcpy((void*)&ES_rate, &c_szData[iOffset], 3);
					iOffset += 3;
					_PESInfo.ES_rate = ES_rate;

					_PESInfo.ES_rate = _PESInfo.ES_rate >> 1;
					_PESInfo.ES_rate &= 0x3FFFFF;
				}

				// 1B
				if (_PESInfo.DSM_trick_mode_flag) 
				{
					_PESInfo.trick_mode_control = c_szData[iOffset ++];

					_PESInfo.trick_mode_value = _PESInfo.trick_mode_control & 0x1f;
					_PESInfo.trick_mode_control = (_PESInfo.trick_mode_control >> 5) & 0x03;
				}

				// 1B
				if (_PESInfo.additional_copy_info_flag)
				{
					_PESInfo.additional_copy_info = c_szData[iOffset++];
					_PESInfo.additional_copy_info &= 0x7f;
				}

				// 2B
				if (_PESInfo.PES_CRC_flag)
				{
					int16_t previous_PES_packet_CRC = 0;
					memcpy((void*)&previous_PES_packet_CRC, &c_szData[iOffset], 2);
					iOffset += 2;
					_PESInfo.previous_PES_packet_CRC = previous_PES_packet_CRC;
				}

				// 1B
				if (_PESInfo.PES_extension_flag)
				{
					int8_t efv = c_szData[iOffset ++];

					_PESInfo.PES_private_data_flag = (efv >> 7) & 0x01;
					_PESInfo.pack_header_field_flag = (efv >> 6) & 0x01;
					_PESInfo.program_packet_sequence_counter_flag = (efv >> 5) & 0x01;
					_PESInfo.P_STD_buffer_flag = (efv >> 4) & 0x01;
					_PESInfo.const1_value0 = (efv >> 1) & 0x07;
					_PESInfo.PES_extension_flag_2 = efv & 0x01;

					nb_required = 0;
					nb_required += _PESInfo.PES_private_data_flag ? 16 : 0;
					nb_required += _PESInfo.pack_header_field_flag ? 1 : 0; // 1+x bytes.
					nb_required += _PESInfo.program_packet_sequence_counter_flag ? 2 : 0;
					nb_required += _PESInfo.P_STD_buffer_flag ? 2 : 0;
					nb_required += _PESInfo.PES_extension_flag_2 ? 1 : 0; // 1+x bytes.

					if (iOffset + nb_required > uSize)
						return -1;

					// 16B
					if (_PESInfo.PES_private_data_flag)
					{
						_PESInfo.PES_private_data.resize(16);
						memcpy(&_PESInfo.PES_private_data, &c_szData[iOffset], 16);
						iOffset += 16;
					}

					// (1+x)B
					if (_PESInfo.pack_header_field_flag) 
					{
						// This is an 8-bit field which indicates the length, in bytes, of the pack_header_field()
						uint8_t pack_field_length = c_szData[iOffset ++];
						if (pack_field_length > 0) 
						{
							// the adjust required bytes.
							nb_required = nb_required - 16 - 1 + pack_field_length;

							if (iOffset + nb_required > uSize)
								return -1;

							_PESInfo.pack_field.resize(pack_field_length);
							memcpy(&_PESInfo.pack_field[0], &c_szData[iOffset], pack_field_length);
							iOffset += pack_field_length;
						}
					}

					// 2B
					if (_PESInfo.program_packet_sequence_counter_flag)
					{
						_PESInfo.program_packet_sequence_counter = c_szData[iOffset ++];
						_PESInfo.program_packet_sequence_counter &= 0x7f;

						_PESInfo.original_stuff_length = c_szData[iOffset++];
						_PESInfo.MPEG1_MPEG2_identifier = (_PESInfo.original_stuff_length >> 6) & 0x01;
						_PESInfo.original_stuff_length &= 0x3f;
					}

					// 2B
					if (_PESInfo.P_STD_buffer_flag)
					{
						int16_t P_STD_buffer_size = 0;
						memcpy((void*)&P_STD_buffer_size, &c_szData[iOffset], 2);
						iOffset += 2;
						_PESInfo.P_STD_buffer_size = P_STD_buffer_size;

						// '01'
						//int8_t const2bits = (P_STD_buffer_scale >>14) & 0x03;

						_PESInfo.P_STD_buffer_scale = (_PESInfo.P_STD_buffer_scale >> 13) & 0x01;
						_PESInfo.P_STD_buffer_size &= 0x1FFF;
					}

					// (1+x)B
					if (_PESInfo.PES_extension_flag_2)
					{
						/**
						* This is a 7-bit field which specifies the length, in bytes, of the data following this field in
						* the PES extension field up to and including any reserved bytes.
						*/
						uint8_t PES_extension_field_length = c_szData[iOffset ++];
						PES_extension_field_length &= 0x7F;

						if (PES_extension_field_length > 0) 
						{
							if (iOffset + PES_extension_field_length > uSize)
								return -1;

							_PESInfo.PES_extension_field.resize(PES_extension_field_length);
							memcpy(&_PESInfo.PES_extension_field[0], &c_szData[iOffset], PES_extension_field_length);
							iOffset += PES_extension_field_length;
						}
					}
				}

				// stuffing_byte
				_PESInfo.nb_stuffings = _PESInfo.PES_header_data_length - (iOffset - pos_header);
				if (_PESInfo.nb_stuffings > 0)
				{
					if (iOffset + _PESInfo.nb_stuffings > uSize)
						return -1;

					iOffset += _PESInfo.nb_stuffings;
				}

				// LCOV_EXCL_STOP

				// PES_packet_data_byte, page58.
				// the packet size contains the header size.
				// The number of PES_packet_data_bytes, N, is specified by the
				// PES_packet_length field. N shall be equal to the value
				// indicated in the PES_packet_length minus the number of bytes
				// between the last byte of the PES_packet_length field and the
				// first PES_packet_data_byte.
				/**
				* when actual packet length > 0xffff(65535),
				* which exceed the max uint16_t packet length,
				* use 0 packet length, the next unit start indicates the end of packet.
				*/
				if (_PESInfo.PES_packet_length > 0)
				{
					int nb_packet = _PESInfo.PES_packet_length - (iOffset - pos_packet);
					if (nb_packet + iOffset > uSize)
						return -1;

					_sData.append(&c_szData[iOffset], nb_packet);
					iOffset += nb_packet;

					//回调上报PES负载数据

					return iOffset;
				}
				else
				{
					_sData.append(&c_szData[iOffset], uSize - iOffset);
				}
			}
			else if (_PESInfo.stream_id == TSPESStreamIdProgramStreamMap
				|| _PESInfo.stream_id == TSPESStreamIdPrivateStream2
				|| _PESInfo.stream_id == TSPESStreamIdEcmStream
				|| _PESInfo.stream_id == TSPESStreamIdEmmStream
				|| _PESInfo.stream_id == TSPESStreamIdProgramStreamDirectory
				|| _PESInfo.stream_id == TSPESStreamIdDsmccStream
				|| _PESInfo.stream_id == TSPESStreamIdH2221TypeE
				)
			{

			}
			else if (_PESInfo.stream_id == TSPESStreamIdPaddingStream)
			{

			}
			else 
			{

			}
		}

	}

private:
	tagPayloadPES_INFO	_PESInfo;
	_string_t			_sData;
};

class TSDecode
{
public:
	enum TSStatus
	{
		ts_header, //未知状态
		ts_padding, //未知状态
		ts_ps,      //ps状态
		ts_sh,
		ts_psm,
		ts_pes,
		ts_pes_video,
		ts_pes_audio,
		ts_pes_private,
		ts_adaptation_field,
		ts_pat,
		ts_pmt,
		ts_unknow,
	};

	typedef TSPayloadPES::_string_t _string_t;
	typedef SmartPTR<TSAdaptationField>	TSAdaptationField_sptr;
	typedef SmartPTR<TSPayloadPAT>	TSPayloadPAT_sptr;
	typedef SmartPTR<TSPayloadPMT>	TSPayloadPMT_sptr;
	typedef SmartPTR<TSPayloadPES>	TSPayloadPES_sptr;
	typedef TSPayloadPAT::TSChannel_sptr	TSChannel_sptr;
	typedef TSPayloadPAT::ChannelMap_type	ChannelMap_type;

	static MemPool_type* GetPoolInstance(void)
	{
		static MemPool_type s_Pool(__max(sizeof(TSAdaptationField),
			__max(sizeof(TSPayloadPAT),
				__max(sizeof(TSPayloadPMT),
					__max(sizeof(TSPayloadPES), sizeof(tagTSChannel))))));
		return  &s_Pool;
	}

	static TSAdaptationField_sptr AllocAdaptationField(void)
	{
		return TSAdaptationField_sptr(GetPoolInstance()->AllocObj<TSAdaptationField>(),
			function20_bind_Ex(&MemPool_type::FreeObj<TSAdaptationField>, GetPoolInstance()));
	}

	static TSPayloadPAT_sptr AllocPayloadPAT(void)
	{
		return TSPayloadPAT_sptr(GetPoolInstance()->AllocObj<TSPayloadPAT>(),
			function20_bind_Ex(&MemPool_type::FreeObj<TSPayloadPAT>, GetPoolInstance()));
	}

	static TSPayloadPMT_sptr AllocPayloadPMT(void)
	{
		return TSPayloadPMT_sptr(GetPoolInstance()->AllocObj<TSPayloadPMT>(),
			function20_bind_Ex(&MemPool_type::FreeObj<TSPayloadPMT>, GetPoolInstance()));
	}

	static TSPayloadPES_sptr AllocPayloadPES(void)
	{
		return TSPayloadPES_sptr(GetPoolInstance()->AllocObj<TSPayloadPES>(),
			function20_bind_Ex(&MemPool_type::FreeObj<TSPayloadPES>, GetPoolInstance()));
	}

	static TSChannel_sptr AllocChannel(void)
	{
		return TSPayloadPAT::AllocChannel();
	}

public:
	TSDecode(void)
		: _Status(ts_header)
	{

	}

	~TSDecode(void)
	{
		Release();
	}

	int Init(void)
	{
		return 1;
	}

	void Release(void)
	{

	}

	bool IsTsPacket(const char* c_szData, UInt32 uSize, tagTSHeader* pHead)
	{
		if (uSize < sizeof(tagTSHeader))
			return false;

		pHead = (tagTSHeader*)c_szData;
		return pHead->sync_byte == 0x47 ? true : false;
	}

	int Parse(const char* c_szData, UInt32 uSize)
	{
		int iProcLen = 0;
		_sData.append(c_szData, uSize);
	gt_data:
		switch (_Status)
		{
		case ts_header:
		{
			if (_sData.size() < sizeof(tagTSHeader))
				return -1;

			if (!IsTsPacket(_sData.c_str(), _sData.size(), &_Head))
				return -1;

			if (_Head.adaptation_field_control == TSAdaptationFieldTypeAdaptionOnly ||
				_Head.adaptation_field_control == TSAdaptationFieldTypeBoth)
			{
				_Status = ts_adaptation_field;
			}
			else if (_Head.adaptation_field_control == TSAdaptationFieldTypePayloadOnly)
			{
				_Status = GetStatusToPid((TSPid)_Head.pid);
			}

			iProcLen = sizeof(tagTSHeader);
		}
		break;
		case ts_adaptation_field:
		{
			_AptationField_sptr = AllocAdaptationField();
			int iProcLen = _AptationField_sptr->Parse(_Head, _sData.c_str(), _sData.size());
			if (iProcLen > 0)
			{
				if (_Head.adaptation_field_control == TSAdaptationFieldTypeBoth)
					_Status = GetStatusToPid((TSPid)_Head.pid);
			}
		}
		break;
		case ts_pat:
		{
			if (!_PayloadPAT_sptr)
			{
				_PayloadPAT_sptr = AllocPayloadPAT();
			}

			iProcLen = _PayloadPAT_sptr->Parse(_Head, _sData.c_str(), _sData.size(),
				function20_bind_Ex(&TSDecode::SetChannel, this));
			if (iProcLen > 0)
			{
				// update the apply pid table.
				SetChannel((TSPid)_Head.pid, TSPidApplyPAT);

				_Status = ts_header;
			}
		}
		break;
		case ts_pmt:
		{
			if (!_PayloadPMT_sptr)
			{
				_PayloadPMT_sptr = AllocPayloadPMT();
			}

			iProcLen = _PayloadPMT_sptr->Parse(_Head, _sData.c_str(),
				_sData.size(), function20_bind_Ex(&TSDecode::SetChannel, this));
			if (iProcLen > 0)
			{
				// update the apply pid table.
				SetChannel((TSPid)_Head.pid, TSPidApplyPMT);

				_Status = ts_header;
			}
		}
		break;
		case ts_pes:
		{
			if (!_PayloadPES_sptr)
			{
				_PayloadPES_sptr = AllocPayloadPES();
			}

			iProcLen = _PayloadPES_sptr->Parse(_Head, FindChannel((TSPid)_Head.pid),
				_sData.c_str(), _sData.size());
			if (iProcLen > 0)
			{
				// update the apply pid table.
				//SetChannel((TSPid)_Head.pid, TSPidApplyPES);

				_Status = ts_header;
			}
		}
		break;
		}

		if (iProcLen > 0)
		{
			if (iProcLen < _sData.size())
			{
				int iRemain = _sData.size() - iProcLen;
				std::memmove(&_sData[0], &_sData[iProcLen], iRemain);
				_sData.update_size(iRemain);
				goto gt_data;
			}

			_sData.update_size(0);
		}
	}

	TSStatus GetStatusToPid(TSPid Pid)
	{
		if (Pid == TSPidPAT)
		{
			return ts_pat;
		}

		TSChannel_sptr Channel = FindChannel(Pid);
		if (!Channel)
			return ts_unknow;

		if (Channel->ApplyType == TSPidApplyPMT)
			return ts_pmt;

		if (Channel->ApplyType == TSPidApplyVideo || Channel->ApplyType == TSPidApplyAudio)
			return ts_pes;

		return ts_unknow;
	}

	int SetChannel(TSPid Pid, TSPidApply ApplyType,
		TSStream StreamType = TSStreamReserved)
	{
		TSChannel_sptr sptr = FindChannel(Pid);
		ChannelMap_type::iterator iter = _ChannelMap.find(Pid);
		if (!sptr)
		{
			sptr = AllocChannel();
			_ChannelMap.insert(std::make_pair(Pid, sptr));
		}

		sptr->Pid = Pid;
		sptr->ApplyType = ApplyType;
		sptr->StreamType = StreamType;
		return 1;
	}

	TSChannel_sptr	FindChannel(TSPid Pid)
	{
		TSChannel_sptr sptr;
		ChannelMap_type::iterator iter = _ChannelMap.find(Pid);
		if (iter != _ChannelMap.end())
			sptr = iter->second;
		return sptr;
	}

	void ClearChannel(void)
	{
		_ChannelMap.clear();
	}

private:
	ChannelMap_type			_ChannelMap;
	tagTSHeader				_Head;
	TSStatus				_Status;
	TSAdaptationField_sptr	_AptationField_sptr;
	TSPayloadPAT_sptr		_PayloadPAT_sptr;
	TSPayloadPMT_sptr		_PayloadPMT_sptr;
	TSPayloadPES_sptr		_PayloadPES_sptr;
	_string_t				_sData;
};


class TSPacket
{
public:
	enum TSStatus
	{
		ts_header, //未知状态
		ts_padding, //未知状态
		ts_ps,      //ps状态
		ts_sh,
		ts_psm,
		ts_pes,
		ts_pes_video,
		ts_pes_audio,
		ts_pes_private
	};

#pragma pack(push, 1)
	typedef struct TSPayloadPAT
	{
		UInt16 table_id;							//PAT表固定为0x00
		UInt16 section_syntax_indicator : 1;		//固定为1
		UInt16 zero : 1;							//固定为0
		UInt16 reserved : 2;						//固定为11
		UInt16 section_length : 12;					//后面数据的长度
		UInt16 transport_stream_id;					//传输流ID，固定为0x0001
		UInt16 reserved : 2;						//固定为11
		UInt16 version_number : 5;					//版本号，固定为00000，如果PAT有变化则版本号加1
		UInt16 current_next_indicator : 1;			//固定为1，表示这个PAT表可以用，如果为0则要等待下一个PAT表
		UInt16 section_number : 8;					//固定为0x00
		UInt16 last_section_number : 8;				//固定为0x00
	};

	typedef struct TSPayloadPATProgram
	{
		UInt16 program_number;				//节目号为0x0000时表示这是NIT，节目号为0x0001时,表示这是PMT
		UInt16 reserved : 3;				//固定为111
		UInt16 PID : 13;					//节目号对应内容的PID值
	}TSPayloadPATProgram;

	typedef struct TSPayloadPMT
	{
		UInt16 table_id;							//PMT表取值随意，0x02
		UInt16 section_syntax_indicator : 1;		//固定为1
		UInt16 zero : 1;							//固定为0
		UInt16 reserved : 2;						//固定为11
		UInt16 section_length : 12;					//后面数据的长度
	}TSPayloadPMT;
#pragma pack(pop)

	struct tagElementaryStreamInfo
	{
		unsigned char stream_type;
		unsigned char elementary_stream_id;
		_string_type  desc;

		tagElementaryStreamInfo(void)
		{}

		tagElementaryStreamInfo(const tagElementaryStreamInfo& t1)
		{
			this->stream_type = t1.stream_type;
			this->elementary_stream_id = t1.elementary_stream_id;
			this->desc = t1.desc;
		}

		tagElementaryStreamInfo& operator=(const tagElementaryStreamInfo& t1)
		{
			this->stream_type = t1.stream_type;
			this->elementary_stream_id = t1.elementary_stream_id;
			this->desc = t1.desc;
			return *this;
		}
	};

	typedef function20_handle<void(PSStatus, const char*, Int32)>	HFNData;
	typedef LinkList_type<tagElementaryStreamInfo>					StreamInfoList;

public:
	TSPacket(void);

	~TSPacket(void);

	int Init(const HFNData& hData);

	void Release(void);

	int Parse(const char* c_szData, UInt32 uSize);

protected:

	Int32	ParsePES(const char* c_szData, Int32 iSize);

	int	PESPayload(pes_header_t* pPesHead, const char* c_szData, Int32 iSize);

	_string_type GetProgramStreamInfoDesc(const psm_header_t* pHead);

	void GetElementaryStreamInfo(const psm_header_t* pHead);

private:
	UInt8	_uStatus;
	HFNData _hData;
	StreamInfoList	_StreamInfos;
	_string_type	_StreamInfoDesc;
	_string_type	_sData;
	UInt32			_uWaitBytes;
	char			_PESHeader[10];
};


/*
_1 是否包含数据
_2 下一个PS状态
_3 数据指针
_4 数据长度
*/
//typedef std::tr1::tuple<bool, PSStatus, pes_header_t*> pes_tuple;
/*
_1 是否包含数据
_2 数据类型
_3 PTS时间戳
_4 DTS时间戳
_5 数据指针
_6 数据长度
*/
//typedef std::tr1::tuple<bool, unsigned char, unsigned __int64, unsigned __int64, char*, unsigned int> naked_tuple;

/*class PSPacket
{
public:
    PSPacket()
    {
        m_status = ps_padding;
        m_nESLength = m_nPESIndicator = m_nPSWrtiePos = m_nPESLength = 0;
    }
    void PSWrite(char* pBuffer, unsigned int sz)
    {
        if(m_nPSWrtiePos + sz < MAX_PS_LENGTH)
        {
            memcpy((m_pPSBuffer + m_nPSWrtiePos), pBuffer, sz);
            m_nPSWrtiePos += sz;
        }
        else
        {
            m_status = ps_padding;
            m_nESLength = m_nPESIndicator = m_nPSWrtiePos = m_nPESLength = 0;
        }

    }
    void RTPWrite(char* pBuffer, unsigned int sz)
    {
        char* data = (pBuffer + sizeof(RTP_header_t));
        unsigned int length =  sz - sizeof(RTP_header_t);
        if(m_nPSWrtiePos + length < MAX_PS_LENGTH)
        {
            memcpy((m_pPSBuffer + m_nPSWrtiePos), data, length);
            m_nPSWrtiePos += length;
        }
        else
        {
            m_status = ps_padding;
            m_nESLength = m_nPESIndicator = m_nPSWrtiePos = m_nPESLength = 0;
        }
    }
	pes_tuple pes_payload()
    {
        if(m_status == ps_padding)
        {
            for(; m_nPESIndicator<m_nPSWrtiePos; m_nPESIndicator++)
            {
                m_ps = (ps_header_t*)(m_pPSBuffer + m_nPESIndicator);
                if(is_ps_header(m_ps))
                {
                    m_status = ps_ps;
                    break;
                }
            }
        }
        if(m_status == ps_ps)
        {
            for(; m_nPESIndicator<m_nPSWrtiePos; m_nPESIndicator++)
            {
                m_sh = (sh_header_t*)(m_pPSBuffer + m_nPESIndicator);
                m_pes = (pes_header_t*)(m_pPSBuffer + m_nPESIndicator);
                if(is_sh_header(m_sh))
                {
                    m_status = ps_sh;
                    break;
                }
                else if (is_pes_header(m_pes))
                {
                    m_status = ps_pes;
                    break;
                }
            }
        }
        if(m_status == ps_sh)
        {
            for(; m_nPESIndicator<m_nPSWrtiePos; m_nPESIndicator++)
            {
                m_psm = (psm_header_t*)(m_pPSBuffer + m_nPESIndicator);
                m_pes = (pes_header_t*)(m_pPSBuffer + m_nPESIndicator);
                if(is_psm_header(m_psm))
                {
                    m_status = ps_psm;//冲掉s_sh状态
                    break;
                }
                if(is_pes_header(m_pes))
                {
                    m_status = ps_pes;
                    break;
                }
            }
        }
        if(m_status == ps_psm)
        {
            for(; m_nPESIndicator<m_nPSWrtiePos; m_nPESIndicator++)
            {
                m_pes = (pes_header_t*)(m_pPSBuffer + m_nPESIndicator);
                if(is_pes_header(m_pes))
                {
                    m_status = ps_pes;
                    break;
                }
            }
        }
        if(m_status == ps_pes)
        {
            //寻找下一个pes 或者 ps
            unsigned short PES_packet_length = ntohs(m_pes->PES_packet_length);
            if((m_nPESIndicator + PES_packet_length + sizeof(pes_header_t)) < m_nPSWrtiePos)
            {
                char* next = (m_pPSBuffer + m_nPESIndicator + sizeof(pes_header_t) + PES_packet_length);
                pes_header_t* pes = (pes_header_t*)next;
                ps_header_t* ps = (ps_header_t*)next;
                m_nPESLength = PES_packet_length + sizeof(pes_header_t);
                memcpy(m_pPESBuffer, m_pes, m_nPESLength);
                if(is_pes_header(pes) || is_ps_header(ps))
                {
                    PSStatus status = ps_padding;
                    if(is_ps_header(ps))
                    {
                        status = m_status = ps_ps;
                    }
                    else
                    {
                        status = pes_type(pes);
                    }
                    int remain = m_nPSWrtiePos - (next - m_pPSBuffer);
                    memcpy(m_pPSBuffer, next, remain);
                    m_nPSWrtiePos = remain; m_nPESIndicator = 0;
                    m_ps = (ps_header_t*)m_pPSBuffer;
                    m_pes = (pes_header_t*)m_pPSBuffer;
                    return pes_tuple(true, status, (pes_header_t*)m_pPESBuffer);
                }
                else
                {
                    m_status = ps_padding;
                    m_nPESIndicator = m_nPSWrtiePos = 0;
                }
            }
        }
        return pes_tuple(false, ps_padding, NULL);
    }
    naked_tuple naked_payload()
    {
        naked_tuple tuple = naked_tuple(false, 0, 0, 0, NULL, 0);
        do 
        {
            pes_tuple t = pes_payload();
            if(! std::tr1::get<0>(t))
            {
                break;
            }
            PSStatus status = std::tr1::get<1>(t);
            pes_header_t* pes = std::tr1::get<2>(t);
            optional_pes_header* option = (optional_pes_header*)((char*)pes + sizeof(pes_header_t));
            if(option->PTS_DTS_flags != 2 && option->PTS_DTS_flags != 3 && option->PTS_DTS_flags != 0)
            {
                break;
            }
            unsigned __int64 pts = get_pts(option);
            unsigned __int64 dts = get_dts(option);
            unsigned char stream_id = pes->stream_id;
            unsigned short PES_packet_length = ntohs(pes->PES_packet_length);
            char* pESBuffer = ((char*)option + sizeof(optional_pes_header) + option->PES_header_data_length);
            int nESLength = PES_packet_length - (sizeof(optional_pes_header) + option->PES_header_data_length);
            memcpy(m_pESBuffer + m_nESLength, pESBuffer, nESLength);
            m_nESLength += nESLength;
            if(stream_id == 0xE0 && (status == ps_ps || status == ps_pes_audio))
            {
                tuple = naked_tuple(true, 0xE0, pts, dts, m_pESBuffer, m_nESLength);
                m_nESLength = 0;
            }
            else if(stream_id == 0xC0)
            {
                tuple = naked_tuple(true, 0xC0, pts, dts, m_pESBuffer, m_nESLength);
                m_nESLength = 0;
            }
        } while (false);
        return tuple;
    }
private:
    PSStatus      m_status;                     //当前状态
    char          m_pPSBuffer[MAX_PS_LENGTH];   //PS缓冲区
    unsigned int  m_nPSWrtiePos;                //PS写入位置
    unsigned int  m_nPESIndicator;              //PES指针
private:
    char          m_pPESBuffer[MAX_PES_LENGTH]; //PES缓冲区
    unsigned int  m_nPESLength;                 //PES数据长度
private:
 	ps_header_t*  m_ps;                         //PS头
 	sh_header_t*  m_sh;                         //系统头
 	psm_header_t* m_psm;                        //节目流头
 	pes_header_t* m_pes;                        //PES头
private:
    char         m_pESBuffer[MAX_ES_LENGTH];    //裸码流
    unsigned int m_nESLength;                   //裸码流长度
};*/

#endif