#ifndef _PSUNPACKET_H_
#define _PSUNPACKET_H_

#include "streamdef.h"
#include "External.h"

class PSPacket
{
public:
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
	PSPacket(void);

	~PSPacket(void);

	int Init(const HFNData& hData);

	void Release(void);

	int Parse(const char* c_szData, UInt32 uSize);

	//推荐使用
	int ParseEx(const char* c_szData, UInt32 uSize);

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