#ifndef _STREAMDEF_H_
#define _STREAMDEF_H_
#define MAX_PS_LENGTH  (0xFFFF)
#define MAX_PES_LENGTH (0xFFFF)
#define MAX_ES_LENGTH  (0x100000)
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
#pragma pack (1)
typedef struct RTP_HEADER
{
#ifdef ORTP_BIGENDIAN
    uint16_t version:2;
    uint16_t padbit:1;
    uint16_t extbit:1;
    uint16_t cc:4;
    uint16_t markbit:1;
    uint16_t paytype:7;
#else
    uint16_t cc:4;
    uint16_t extbit:1;
    uint16_t padbit:1;
    uint16_t version:2;
    uint16_t paytype:7;  //负载类型
    uint16_t markbit:1;  //1表示前面的包为一个解码单元,0表示当前解码单元未结束
#endif
    uint16_t seq_number;  //序号
    uint32_t timestamp; //时间戳
    uint32_t ssrc;  //循环校验码
    //uint32_t csrc[16];
} RTP_header_t;

typedef struct ps_header{
    unsigned char pack_start_code[4];  //'0x000001BA'

    unsigned char system_clock_reference_base21:2;
    unsigned char marker_bit:1;
    unsigned char system_clock_reference_base1:3;
    unsigned char fix_bit:2;    //'01'

    unsigned char system_clock_reference_base22;

    unsigned char system_clock_reference_base31:2;
    unsigned char marker_bit1:1;
    unsigned char system_clock_reference_base23:5;

    unsigned char system_clock_reference_base32;
    unsigned char system_clock_reference_extension1:2;
    unsigned char marker_bit2:1;
    unsigned char system_clock_reference_base33:5; //system_clock_reference_base 33bit

    unsigned char marker_bit3:1;
    unsigned char system_clock_reference_extension2:7; //system_clock_reference_extension 9bit

    unsigned char program_mux_rate1;

    unsigned char program_mux_rate2;
    unsigned char marker_bit5:1;
    unsigned char marker_bit4:1;
    unsigned char program_mux_rate3:6;

    unsigned char pack_stuffing_length:3;
    unsigned char reserved:5;
}ps_header_t;  //14

typedef struct sh_header
{
    unsigned char system_header_start_code[4]; //32

    unsigned char header_length[2];            //16 uimsbf

    uint32_t marker_bit1:1;   //1  bslbf
    uint32_t rate_bound:22;   //22 uimsbf
    uint32_t marker_bit2:1;   //1 bslbf
    
	uint32_t CSPS_flag : 1;   //1 bslbf
    uint32_t fixed_flag:1;    //1 bslbf
	uint32_t audio_bound : 6;   //6 uimsbf

    uint16_t video_bound:5;             // uimsbf
	uint16_t marker_bit3 : 1;             // bslbf
	uint16_t system_video_lock_flag : 1;  // bslbf
	uint16_t system_audio_lock_flag : 1;  // bslbf

    uint16_t packet_rate_restriction_flag:1; //bslbf
    uint16_t reserved_bits:7;                //bslbf
    unsigned char reserved[6];
}sh_header_t; //18

typedef struct psm_header{
    unsigned char promgram_stream_map_start_code[4];

    unsigned char program_stream_map_length[2];

    unsigned char program_stream_map_version:5;
    unsigned char reserved1:2;
    unsigned char current_next_indicator:1;

    unsigned char marker_bit:1;
    unsigned char reserved2:7;

    unsigned char program_stream_info_length[2];
    unsigned char elementary_stream_map_length[2];
    unsigned char stream_type;
    unsigned char elementary_stream_id;
    unsigned char elementary_stream_info_length[2];
    unsigned char CRC_32[4];
    unsigned char reserved[16];
}psm_header_t; //36

typedef struct pes_header
{
    unsigned char pes_start_code_prefix[3];
    unsigned char stream_id;
    unsigned short PES_packet_length;
}pes_header_t; //6

typedef struct optional_pes_header{
    unsigned char original_or_copy:1;
    unsigned char copyright:1;
    unsigned char data_alignment_indicator:1;
    unsigned char PES_priority:1;
    unsigned char PES_scrambling_control:2;
    unsigned char fix_bit:2;	//'10'

    unsigned char PES_extension_flag:1;
    unsigned char PES_CRC_flag:1;
    unsigned char additional_copy_info_flag:1;
    unsigned char DSM_trick_mode_flag:1;
    unsigned char ES_rate_flag:1;
    unsigned char ESCR_flag:1;
    unsigned char PTS_DTS_flags:2;

    unsigned char PES_header_data_length;
}optional_pes_header_t;

#pragma pack ()


enum PSStatus
{
    ps_padding, //未知状态
    ps_ps,      //ps状态
    ps_sh,
    ps_psm,
    ps_pes,
    ps_pes_video,
    ps_pes_audio,
	ps_pes_private
};

enum PSTrickModeCtrl
{
	ps_null = -1,		 //空
	ps_fast_forward = 0, //快进
	ps_slow_motion,      //慢动作
	ps_freeze_frame,	 //冻结帧
	ps_fast_reverse,	 //快倒
	ps_slow_reverse		 //慢倒
};

#endif