

#include <libFrameWork_RtmpCommon/IRtmpSession.h>
#include <rtmp_sys.h>

#ifdef CRYPTO
#include <handshake.h>
#endif

namespace _rtmp_ {

	static const char *RTMPT_cmds[] = {
		"open",
		"send",
		"idle",
		"close"
	};

#define HEX2BIN(a)	(((a)&0x40)?((a)&0xf)+9:((a)&0xf))

#define SAVC(x)	static const AVal av_##x = AVC(#x)

	SAVC(app);
	SAVC(connect);
	SAVC(flashVer);
	SAVC(swfUrl);
	SAVC(pageUrl);
	SAVC(tcUrl);
	SAVC(fpad);
	SAVC(capabilities);
	SAVC(audioCodecs);
	SAVC(videoCodecs);
	SAVC(videoFunction);
	SAVC(objectEncoding);
	SAVC(secureToken);
	SAVC(secureTokenResponse);
	SAVC(type);
	SAVC(nonprivate);
	SAVC(FCUnpublish);
	SAVC(deleteStream);
	SAVC(_result);
	SAVC(releaseStream);
	SAVC(FCPublish);
	SAVC(createStream);
	SAVC(FCSubscribe);
	SAVC(publish);
	SAVC(live);
	SAVC(record);
	SAVC(set_playlist);
	SAVC(0);
	SAVC(play);
	SAVC(_checkbw);
	SAVC(onBWDone);
	SAVC(onFCSubscribe);
	SAVC(onFCUnsubscribe);
	SAVC(_onbwcheck);
	SAVC(_onbwdone);
	SAVC(_error);
	SAVC(close);
	SAVC(code);
	SAVC(level);
	SAVC(onStatus);
	SAVC(playlist_ready);
	SAVC(ping);
	SAVC(pong);
	SAVC(pause);
	SAVC(onMetaData);
	SAVC(duration);
	SAVC(video);
	SAVC(audio);

	static const AVal av_NetStream_Failed = AVC("NetStream.Failed");
	static const AVal av_NetStream_Play_Failed = AVC("NetStream.Play.Failed");
	static const AVal av_NetStream_Play_StreamNotFound =
		AVC("NetStream.Play.StreamNotFound");
	static const AVal av_NetConnection_Connect_InvalidApp =
		AVC("NetConnection.Connect.InvalidApp");
	static const AVal av_NetStream_Play_Start = AVC("NetStream.Play.Start");
	static const AVal av_NetStream_Play_Complete = AVC("NetStream.Play.Complete");
	static const AVal av_NetStream_Play_Stop = AVC("NetStream.Play.Stop");
	static const AVal av_NetStream_Seek_Notify = AVC("NetStream.Seek.Notify");
	static const AVal av_NetStream_Pause_Notify = AVC("NetStream.Pause.Notify");
	static const AVal av_NetStream_Play_UnpublishNotify =
		AVC("NetStream.Play.UnpublishNotify");
	static const AVal av_NetStream_Publish_Start = AVC("NetStream.Publish.Start");

	static const int packetSize[] = { 12, 8, 4, 1 };

	static void AV_queue(RTMP_METHOD **vals, int *num, AVal *av, int txn)
	{
		char *tmp;
		if (!(*num & 0x0f))
			*vals = (RTMP_METHOD *)realloc(*vals, (*num + 16) * sizeof(RTMP_METHOD));
		tmp = (char*)malloc(av->av_len + 1);
		memcpy(tmp, av->av_val, av->av_len);
		tmp[av->av_len] = '\0';
		(*vals)[*num].num = txn;
		(*vals)[*num].name.av_len = av->av_len;
		(*vals)[(*num)++].name.av_val = tmp;
	}

	static void AV_erase(RTMP_METHOD *vals, int *num, int i, int freeit)
	{
		if (freeit)
			free(vals[i].name.av_val);
		(*num)--;
		for (; i < *num; i++)
		{
			vals[i] = vals[i + 1];
		}
		vals[i].name.av_val = NULL;
		vals[i].name.av_len = 0;
		vals[i].num = 0;
	}

	static int EncodeInt32LE(char *output, int nVal)
	{
		output[0] = nVal;
		nVal >>= 8;
		output[1] = nVal;
		nVal >>= 8;
		output[2] = nVal;
		nVal >>= 8;
		output[3] = nVal;
		return 4;
	}

	static int DecodeInt32LE(const char *data)
	{
		unsigned char *c = (unsigned char *)data;
		unsigned int val;

		val = (c[3] << 24) | (c[2] << 16) | (c[1] << 8) | c[0];
		return val;
	}

	static void HandleChangeChunkSize(RTMP *r, const RTMPPacket *packet)
	{
		if (packet->m_nBodySize >= 4)
		{
			r->m_inChunkSize = AMF_DecodeInt32(packet->m_body);
			LOG_Print_Info(libFrameWork_RtmpCommon, "HandleChangeChunkSize() %s, received: chunk size change to %d", __FUNCTION__,
				r->m_inChunkSize);
		}
	}

	static void HandleAudio(RTMP *r, const RTMPPacket *packet)
	{
	}

	static void HandleVideo(RTMP *r, const RTMPPacket *packet)
	{
	}

	static void HandleServerBW(RTMP *r, const RTMPPacket *packet)
	{
		r->m_nServerBW = AMF_DecodeInt32(packet->m_body);
		LOG_Print_Info(libFrameWork_RtmpCommon, "HandleServerBW() %s: server BW = %d", __FUNCTION__, r->m_nServerBW);
	}

	static void HandleClientBW(RTMP *r, const RTMPPacket *packet)
	{
		r->m_nClientBW = AMF_DecodeInt32(packet->m_body);
		if (packet->m_nBodySize > 4)
			r->m_nClientBW2 = packet->m_body[4];
		else
			r->m_nClientBW2 = -1;
		LOG_Print_Info(libFrameWork_RtmpCommon, "HandleClientBW() %s: client BW = %d %d", __FUNCTION__, r->m_nClientBW,
			r->m_nClientBW2);
	}

	static int DumpMetaData(AMFObject *obj)
	{
		AMFObjectProperty *prop;
		int n;
		for (n = 0; n < obj->o_num; n++)
		{
			prop = AMF_GetProp(obj, NULL, n);
			if (prop->p_type != AMF_OBJECT)
			{
				char str[256] = "";
				switch (prop->p_type)
				{
				case AMF_NUMBER:
					snprintf(str, 255, "%.2f", prop->p_vu.p_number);
					break;
				case AMF_BOOLEAN:
					snprintf(str, 255, "%s",
						prop->p_vu.p_number != 0. ? "TRUE" : "FALSE");
					break;
				case AMF_STRING:
					snprintf(str, 255, "%.*s", prop->p_vu.p_aval.av_len,
						prop->p_vu.p_aval.av_val);
					break;
				case AMF_DATE:
					snprintf(str, 255, "timestamp:%.2f", prop->p_vu.p_number);
					break;
				default:
					snprintf(str, 255, "INVALID TYPE 0x%02x",
						(unsigned char)prop->p_type);
				}
				if (prop->p_name.av_len)
				{
					/* chomp */
					if (strlen(str) >= 1 && str[strlen(str) - 1] == '\n')
						str[strlen(str) - 1] = '\0';
					LOG_Print_Info(libFrameWork_RtmpCommon, "DumpMetaData()  %-22.*s%s", prop->p_name.av_len,
						prop->p_name.av_val, str);
				}
			}
			else
			{
				if (prop->p_name.av_len)
					LOG_Print_Info(libFrameWork_RtmpCommon, "DumpMetaData() %.*s:", prop->p_name.av_len, prop->p_name.av_val);
				DumpMetaData(&prop->p_vu.p_object);
			}
		}
		return FALSE;
	}

	/* Like above, but only check if name is a prefix of property */
	static int RTMP_FindPrefixProperty(AMFObject *obj, const AVal *name,
			AMFObjectProperty * p)
	{
		int n;
		for (n = 0; n < obj->o_num; n++)
		{
			AMFObjectProperty *prop = AMF_GetProp(obj, NULL, n);

			if (prop->p_name.av_len > name->av_len &&
				!memcmp(prop->p_name.av_val, name->av_val, name->av_len))
			{
				*p = *prop;
				return TRUE;
			}

			if (prop->p_type == AMF_OBJECT)
			{
				if (RTMP_FindPrefixProperty(&prop->p_vu.p_object, name, p))
					return TRUE;
			}
		}
		return FALSE;
	}

	void IRtmpSession::SetVideoInfo(const tagVideoInfo& Info)
	{
		_VideoInfo.SPS = Info.SPS;
		_VideoInfo.PPS = Info.PPS;
		_VideoInfo.uVideoType = Info.uVideoType;
		_VideoInfo.uWidth = Info.uWidth;
		_VideoInfo.uHeight = Info.uHeight;
		_VideoInfo.uMax_Framerate = Info.uMax_Framerate;
		_VideoInfo.iWaitTime = 1000 / _VideoInfo.uMax_Framerate;
	}

	int IRtmpSession::SendFCUnpublish(RTMP *r)
	{
		RTMPPacket packet;
		char pbuf[1024], *pend = pbuf + sizeof(pbuf);
		char *enc;

		packet.m_nChannel = 0x03;	/* control channel (invoke) */
		packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;
		packet.m_packetType = 0x14;	/* INVOKE */
		packet.m_nTimeStamp = 0;
		packet.m_nInfoField2 = 0;
		packet.m_hasAbsTimestamp = 0;
		packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;

		enc = packet.m_body;
		enc = AMF_EncodeString(enc, pend, &av_FCUnpublish);
		enc = AMF_EncodeNumber(enc, pend, ++r->m_numInvokes);
		*enc++ = AMF_NULL;
		enc = AMF_EncodeString(enc, pend, &r->Link.playpath);
		if (!enc)
			return FALSE;

		packet.m_nBodySize = enc - packet.m_body;

		return SendPacket(r, &packet, FALSE);
	}

	int IRtmpSession::SendDeleteStream(RTMP *r, double dStreamId)
	{
		RTMPPacket packet;
		char pbuf[256], *pend = pbuf + sizeof(pbuf);
		char *enc;

		packet.m_nChannel = 0x03;	/* control channel (invoke) */
		packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;
		packet.m_packetType = 0x14;	/* INVOKE */
		packet.m_nTimeStamp = 0;
		packet.m_nInfoField2 = 0;
		packet.m_hasAbsTimestamp = 0;
		packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;

		enc = packet.m_body;
		enc = AMF_EncodeString(enc, pend, &av_deleteStream);
		enc = AMF_EncodeNumber(enc, pend, ++r->m_numInvokes);
		*enc++ = AMF_NULL;
		enc = AMF_EncodeNumber(enc, pend, dStreamId);

		packet.m_nBodySize = enc - packet.m_body;

		/* no response expected */
		return SendPacket(r, &packet, FALSE);
	}

	int IRtmpSession::CreateNetConnect(RTMP *r, RTMPPacket *cp)
	{
		if (r->Link.protocol & RTMP_FEATURE_SSL)
		{
#if defined(CRYPTO) && !defined(NO_SSL)
			TLS_client(RTMP_TLS_ctx, r->m_sb.sb_ssl);
			TLS_setfd(r->m_sb.sb_ssl, r->m_sb.sb_socket);
			if (TLS_connect(r->m_sb.sb_ssl) < 0)
			{
				LOG_Print_Error(libFrameWork_RtmpCommon, "IRtmpSession::CreateNetConnect() %s, TLS_Connect failed", __FUNCTION__);
				Close();
				return FALSE;
			}
#else
			LOG_Print_Error(libFrameWork_RtmpCommon, "IRtmpSession::CreateNetConnect() %s, no SSL/TLS support", __FUNCTION__);
			Close();
			return -1;
#endif
		}

		if (r->Link.protocol & RTMP_FEATURE_HTTP)
		{
			r->m_msgCounter = 1;
			r->m_clientID.av_val = NULL;
			r->m_clientID.av_len = 0;
			HTTP_Post(r, RTMPT_OPEN, "", 1);
			_Status = RTMP_STATUS_RTMPT_OPEN;
			return 0;
		}

		LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::CreateNetConnect() %s, ... connected", __FUNCTION__);
		return 1;
	}

	int IRtmpSession::HTTP_Post(RTMP *r, RTMPTCmd cmd, const char *buf, int len)
	{
		char hbuf[512];
		int hlen = snprintf(hbuf, sizeof(hbuf), "POST /%s%s/%d HTTP/1.1\r\n"
			"Host: %.*s:%d\r\n"
			"Accept: */*\r\n"
			"User-Agent: Shockwave Flash\n"
			"Connection: Keep-Alive\n"
			"Cache-Control: no-cache\r\n"
			"Content-type: application/x-fcs\r\n"
			"Content-length: %d\r\n\r\n", RTMPT_cmds[cmd],
			r->m_clientID.av_val ? r->m_clientID.av_val : "",
			r->m_msgCounter, r->Link.hostname.av_len, r->Link.hostname.av_val,
			r->Link.port, len);
		Send(&r->m_sb, hbuf, hlen);
		hlen = Send(&r->m_sb, buf, len);
		r->m_msgCounter++;
		r->m_unackd++;
		return hlen;
	}

	int IRtmpSession::HTTP_readV20(RTMP *r, int fill, const char* c_szData, UInt32 uSize)
	{
		char *ptr;
		int hlen;

		if (fill)
			SockBuf_FillV20(&r->m_sb, c_szData, uSize);
		if (r->m_sb.sb_size < 144)
			return -1;
		if (strncmp(r->m_sb.sb_start, "HTTP/1.1 200 ", 13))
			return -1;
		ptr = strstr(r->m_sb.sb_start, "Content-Length:");
		if (!ptr)
			return -1;
		hlen = atoi(ptr + 16);
		ptr = strstr(ptr, "\r\n\r\n");
		if (!ptr)
			return -1;
		ptr += 4;
		r->m_sb.sb_size -= ptr - r->m_sb.sb_start;
		r->m_sb.sb_start = ptr;
		r->m_unackd--;

		if (!r->m_clientID.av_val)
		{
			r->m_clientID.av_len = hlen;
			r->m_clientID.av_val = (char*)malloc(hlen + 1);
			if (!r->m_clientID.av_val)
				return -1;
			r->m_clientID.av_val[0] = '/';
			memcpy(r->m_clientID.av_val + 1, ptr, hlen - 1);
			r->m_clientID.av_val[hlen] = 0;
			r->m_sb.sb_size = 0;
		}
		else
		{
			r->m_polling = *ptr++;
			r->m_resplen = hlen - 1;
			r->m_sb.sb_start++;
			r->m_sb.sb_size--;
		}
		return 0;
	}

	int IRtmpSession::WriteN(RTMP *r, const char *buffer, int n)
	{
		const char *ptr = buffer;
#ifdef CRYPTO
		char *encrypted = 0;
		char buf[RTMP_BUFFER_CACHE_SIZE];

		if (r->Link.rc4keyOut)
		{
			if (n > sizeof(buf))
				encrypted = (char *)malloc(n);
			else
				encrypted = (char *)buf;
			ptr = encrypted;
			RC4_encrypt2(r->Link.rc4keyOut, n, buffer, ptr);
		}
#endif
		int iRet = -1;
		if (r->Link.protocol & RTMP_FEATURE_HTTP)
			iRet = HTTP_Post(r, RTMPT_SEND, ptr, n);
		else
			iRet = Send(&r->m_sb, ptr, n) ;

#ifdef CRYPTO
		if (encrypted && encrypted != buf)
			free(encrypted);
#endif
		return iRet;
	}

	int IRtmpSession::SendBytesReceived(RTMP *r)
	{
		RTMPPacket packet;
		char pbuf[256], *pend = pbuf + sizeof(pbuf);

		packet.m_nChannel = 0x02;	/* control channel (invoke) */
		packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;
		packet.m_packetType = 0x03;	/* bytes in */
		packet.m_nTimeStamp = 0;
		packet.m_nInfoField2 = 0;
		packet.m_hasAbsTimestamp = 0;
		packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;

		packet.m_nBodySize = 4;

		AMF_EncodeInt32(packet.m_body, pend, r->m_nBytesIn);	/* hard coded for now */
		r->m_nBytesInSent = r->m_nBytesIn;
		return SendPacket(r, &packet, FALSE);
	}

	int IRtmpSession::SendPacket(RTMP *r, RTMPPacket *packet, int queue)
	{
		const RTMPPacket *prevPacket = r->m_vecChannelsOut[packet->m_nChannel];
		uint32_t last = 0;
		int nSize;
		int hSize, cSize;
		char *header, *hptr, *hend, hbuf[RTMP_MAX_HEADER_SIZE], c;
		uint32_t t;
		char *buffer, *tbuf = NULL, *toff = NULL;
		int nChunkSize;
		int tlen;

		if (prevPacket && packet->m_headerType != RTMP_PACKET_SIZE_LARGE)
		{
			/* compress a bit by using the prev packet's attributes */
			if (prevPacket->m_nBodySize == packet->m_nBodySize
				&& prevPacket->m_packetType == packet->m_packetType
				&& packet->m_headerType == RTMP_PACKET_SIZE_MEDIUM)
				packet->m_headerType = RTMP_PACKET_SIZE_SMALL;

			if (prevPacket->m_nTimeStamp == packet->m_nTimeStamp
				&& packet->m_headerType == RTMP_PACKET_SIZE_SMALL)
				packet->m_headerType = RTMP_PACKET_SIZE_MINIMUM;
			last = prevPacket->m_nTimeStamp;
		}

		if (packet->m_headerType > 3)	/* sanity */
		{
			LOG_Print_Error(libFrameWork_RtmpCommon, "IRtmpSession::SendPacket() sanity failed!! trying to send header of type: 0x%02x.",
				(unsigned char)packet->m_headerType);
			return FALSE;
		}

		nSize = packetSize[packet->m_headerType];
		hSize = nSize; cSize = 0;
		t = packet->m_nTimeStamp - last;

		if (packet->m_body)
		{
			header = packet->m_body - nSize;
			hend = packet->m_body;
		}
		else
		{
			header = hbuf + 6;
			hend = hbuf + sizeof(hbuf);
		}

		if (packet->m_nChannel > 319)
			cSize = 2;
		else if (packet->m_nChannel > 63)
			cSize = 1;
		if (cSize)
		{
			header -= cSize;
			hSize += cSize;
		}

		if (nSize > 1 && t >= 0xffffff)
		{
			header -= 4;
			hSize += 4;
		}

		hptr = header;
		c = packet->m_headerType << 6;
		switch (cSize)
		{
		case 0:
			c |= packet->m_nChannel;
			break;
		case 1:
			break;
		case 2:
			c |= 1;
			break;
		}
		*hptr++ = c;
		if (cSize)
		{
			int tmp = packet->m_nChannel - 64;
			*hptr++ = tmp & 0xff;
			if (cSize == 2)
				*hptr++ = tmp >> 8;
		}

		if (nSize > 1)
		{
			hptr = AMF_EncodeInt24(hptr, hend, t > 0xffffff ? 0xffffff : t);
		}

		if (nSize > 4)
		{
			hptr = AMF_EncodeInt24(hptr, hend, packet->m_nBodySize);
			*hptr++ = packet->m_packetType;
		}

		if (nSize > 8)
			hptr += EncodeInt32LE(hptr, packet->m_nInfoField2);

		if (nSize > 1 && t >= 0xffffff)
			hptr = AMF_EncodeInt32(hptr, hend, t);

		nSize = packet->m_nBodySize;
		buffer = packet->m_body;
		nChunkSize = r->m_outChunkSize;

		LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::SendPacket() %s: fd=%d, size=%d", __FUNCTION__, r->m_sb.sb_socket,
			nSize);
		/* send all chunks in one HTTP request */
		if (r->Link.protocol & RTMP_FEATURE_HTTP)
		{
			int chunks = (nSize + nChunkSize - 1) / nChunkSize;
			if (chunks > 1)
			{
				tlen = chunks * (cSize + 1) + nSize + hSize;
				tbuf = (char*)malloc(tlen);
				if (!tbuf)
					return FALSE;
				toff = tbuf;
			}
		}
		while (nSize + hSize)
		{
			int wrote;

			if (nSize < nChunkSize)
				nChunkSize = nSize;

			if (tbuf)
			{
				memcpy(toff, header, nChunkSize + hSize);
				toff += nChunkSize + hSize;
			}
			else
			{
				wrote = WriteN(r, header, nChunkSize + hSize);
				if (!wrote)
					return FALSE;
			}
			nSize -= nChunkSize;
			buffer += nChunkSize;
			hSize = 0;

			if (nSize > 0)
			{
				header = buffer - 1;
				hSize = 1;
				if (cSize)
				{
					header -= cSize;
					hSize += cSize;
				}
				*header = (0xc0 | c);
				if (cSize)
				{
					int tmp = packet->m_nChannel - 64;
					header[1] = tmp & 0xff;
					if (cSize == 2)
						header[2] = tmp >> 8;
				}
			}
		}
		if (tbuf)
		{
			int wrote = WriteN(r, tbuf, toff - tbuf);
			free(tbuf);
			tbuf = NULL;
			if (!wrote)
				return FALSE;
		}

		/* we invoked a remote method */
		if (packet->m_packetType == 0x14)
		{
			AVal method;
			char *ptr;
			ptr = packet->m_body + 1;
			AMF_DecodeString(ptr, &method);
			LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::SendPacket() Invoking %s", method.av_val);
			/* keep it in call queue till result arrives */
			if (queue) {
				int txn;
				ptr += 3 + method.av_len;
				txn = (int)AMF_DecodeNumber(ptr);
				AV_queue(&r->m_methodCalls, &r->m_numCalls, &method, txn);
			}
		}

		if (!r->m_vecChannelsOut[packet->m_nChannel])
			r->m_vecChannelsOut[packet->m_nChannel] = (RTMPPacket*)malloc(sizeof(RTMPPacket));
		memcpy(r->m_vecChannelsOut[packet->m_nChannel], packet, sizeof(RTMPPacket));
		return TRUE;
	}

	int IRtmpSession::SendConnectPacket(RTMP *r, RTMPPacket *cp)
	{
		RTMPPacket packet;
		char pbuf[4096], *pend = pbuf + sizeof(pbuf);
		char *enc;

		if (cp)
			return SendPacket(r, cp, TRUE);

		packet.m_nChannel = 0x03;	/* control channel (invoke) */
		packet.m_headerType = RTMP_PACKET_SIZE_LARGE;
		packet.m_packetType = 0x14;	/* INVOKE */
		packet.m_nTimeStamp = 0;
		packet.m_nInfoField2 = 0;
		packet.m_hasAbsTimestamp = 0;
		packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;

		enc = packet.m_body;
		enc = AMF_EncodeString(enc, pend, &av_connect);
		enc = AMF_EncodeNumber(enc, pend, ++r->m_numInvokes);
		*enc++ = AMF_OBJECT;

		enc = AMF_EncodeNamedString(enc, pend, &av_app, &r->Link.app);
		if (!enc)
			return FALSE;
		if (r->Link.protocol & RTMP_FEATURE_WRITE)
		{
			enc = AMF_EncodeNamedString(enc, pend, &av_type, &av_nonprivate);
			if (!enc)
				return FALSE;
		}
		if (r->Link.flashVer.av_len)
		{
			enc = AMF_EncodeNamedString(enc, pend, &av_flashVer, &r->Link.flashVer);
			if (!enc)
				return FALSE;
		}
		if (r->Link.swfUrl.av_len)
		{
			enc = AMF_EncodeNamedString(enc, pend, &av_swfUrl, &r->Link.swfUrl);
			if (!enc)
				return FALSE;
		}
		if (r->Link.tcUrl.av_len)
		{
			enc = AMF_EncodeNamedString(enc, pend, &av_tcUrl, &r->Link.tcUrl);
			if (!enc)
				return FALSE;
		}
		if (!(r->Link.protocol & RTMP_FEATURE_WRITE))
		{
			enc = AMF_EncodeNamedBoolean(enc, pend, &av_fpad, FALSE);
			if (!enc)
				return FALSE;
			enc = AMF_EncodeNamedNumber(enc, pend, &av_capabilities, 15.0);
			if (!enc)
				return FALSE;
			enc = AMF_EncodeNamedNumber(enc, pend, &av_audioCodecs, r->m_fAudioCodecs);
			if (!enc)
				return FALSE;
			enc = AMF_EncodeNamedNumber(enc, pend, &av_videoCodecs, r->m_fVideoCodecs);
			if (!enc)
				return FALSE;
			enc = AMF_EncodeNamedNumber(enc, pend, &av_videoFunction, 1.0);
			if (!enc)
				return FALSE;
			if (r->Link.pageUrl.av_len)
			{
				enc = AMF_EncodeNamedString(enc, pend, &av_pageUrl, &r->Link.pageUrl);
				if (!enc)
					return FALSE;
			}
		}
		if (r->m_fEncoding != 0.0 || r->m_bSendEncoding)
		{	/* AMF0, AMF3 not fully supported yet */
			enc = AMF_EncodeNamedNumber(enc, pend, &av_objectEncoding, r->m_fEncoding);
			if (!enc)
				return FALSE;
		}
		if (enc + 3 >= pend)
			return FALSE;
		*enc++ = 0;
		*enc++ = 0;			/* end of object - 0x00 0x00 0x09 */
		*enc++ = AMF_OBJECT_END;

		/* add auth string */
		if (r->Link.auth.av_len)
		{
			enc = AMF_EncodeBoolean(enc, pend, r->Link.lFlags & RTMP_LF_AUTH);
			if (!enc)
				return FALSE;
			enc = AMF_EncodeString(enc, pend, &r->Link.auth);
			if (!enc)
				return FALSE;
		}
		if (r->Link.extras.o_num)
		{
			int i;
			for (i = 0; i < r->Link.extras.o_num; i++)
			{
				enc = AMFProp_Encode(&r->Link.extras.o_props[i], enc, pend);
				if (!enc)
					return FALSE;
			}
		}
		packet.m_nBodySize = enc - packet.m_body;

		return SendPacket(r, &packet, TRUE);
	}

	int IRtmpSession::ClientPacket(RTMP *r, RTMPPacket *packet)
	{
		int bHasMediaPacket = 0;
		switch (packet->m_packetType)
		{
		case 0x01:
			/* chunk size */
			HandleChangeChunkSize(r, packet);
			break;

		case 0x03:
			/* bytes read report */
			LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::ClientPacket() %s, received: bytes read report", __FUNCTION__);
			break;

		case 0x04:
			/* ctrl */
			HandleCtrl(r, packet);
			break;

		case 0x05:
			/* server bw */
			HandleServerBW(r, packet);
			break;

		case 0x06:
			/* client bw */
			HandleClientBW(r, packet);
			break;

		case 0x08:
			/* audio data */
			/*RTMP_Log(RTMP_LOGDEBUG, "%s, received: audio %lu bytes", __FUNCTION__, packet.m_nBodySize); */
			HandleAudio(r, packet);
			bHasMediaPacket = 1;
			if (!r->m_mediaChannel)
				r->m_mediaChannel = packet->m_nChannel;
			if (!r->m_pausing)
				r->m_mediaStamp = packet->m_nTimeStamp;
			break;

		case 0x09:
			/* video data */
			/*RTMP_Log(RTMP_LOGDEBUG, "%s, received: video %lu bytes", __FUNCTION__, packet.m_nBodySize); */
			HandleVideo(r, packet);
			bHasMediaPacket = 1;
			if (!r->m_mediaChannel)
				r->m_mediaChannel = packet->m_nChannel;
			if (!r->m_pausing)
				r->m_mediaStamp = packet->m_nTimeStamp;
			break;

		case 0x0F:			/* flex stream send */
			LOG_Print_Info(libFrameWork_RtmpCommon,
				"IRtmpSession::ClientPacket() %s, flex stream send, size %lu bytes, not supported, ignoring",
				__FUNCTION__, packet->m_nBodySize);
			break;

		case 0x10:			/* flex shared object */
			LOG_Print_Info(libFrameWork_RtmpCommon,
				"IRtmpSession::ClientPacket() %s, flex shared object, size %lu bytes, not supported, ignoring",
				__FUNCTION__, packet->m_nBodySize);
			break;

		case 0x11:			/* flex message */
		{
			LOG_Print_Info(libFrameWork_RtmpCommon,
				"IRtmpSession::ClientPacket() %s, flex message, size %lu bytes, not fully supported",
				__FUNCTION__, packet->m_nBodySize);
			/*RTMP_LogHex(packet.m_body, packet.m_nBodySize); */

			/* some DEBUG code */
#if 0
			RTMP_LIB_AMFObject obj;
			int nRes = obj.Decode(packet.m_body + 1, packet.m_nBodySize - 1);
			if (nRes < 0) {
				LOG_Print_Error(libFrameWork_RtmpCommon,
					"IRtmpSession::ClientPacket()%s, error decoding AMF3 packet", __FUNCTION__);
				/*return; */
			}

			obj.Dump();
#endif

			if (HandleInvoke(r, packet->m_body + 1, packet->m_nBodySize - 1) == 1)
				bHasMediaPacket = 2;
			break;
		}
		case 0x12:
			/* metadata (notify) */
			LOG_Print_Info(libFrameWork_RtmpCommon,
				"IRtmpSession::ClientPacket() %s, received: notify %lu bytes", __FUNCTION__,
				packet->m_nBodySize);
			if (HandleMetadata(r, packet->m_body, packet->m_nBodySize))
				bHasMediaPacket = 1;
			break;

		case 0x13:
			LOG_Print_Info(libFrameWork_RtmpCommon,
				"IRtmpSession::ClientPacket() %s, shared object, not supported, ignoring",
				__FUNCTION__);
			break;

		case 0x14:
			/* invoke */
			LOG_Print_Info(libFrameWork_RtmpCommon,
				"IRtmpSession::ClientPacket() %s, received: invoke %lu bytes", __FUNCTION__,
				packet->m_nBodySize);
			/*RTMP_LogHex(packet.m_body, packet.m_nBodySize); */

			printf("%s, received: invoke %lu bytes\r\n", __FUNCTION__,
				packet->m_nBodySize);

			if (HandleInvoke(r, packet->m_body, packet->m_nBodySize) == 1)
				bHasMediaPacket = 2;
			break;

		case 0x16:
		{
			/* go through FLV packets and handle metadata packets */
			unsigned int pos = 0;
			uint32_t nTimeStamp = packet->m_nTimeStamp;

			while (pos + 11 < packet->m_nBodySize)
			{
				uint32_t dataSize = AMF_DecodeInt24(packet->m_body + pos + 1);	/* size without header (11) and prevTagSize (4) */

				if (pos + 11 + dataSize + 4 > packet->m_nBodySize)
				{
					LOG_Print_Warning(libFrameWork_RtmpCommon,
						"IRtmpSession::ClientPacket() Stream corrupt?!");
					break;
				}
				if (packet->m_body[pos] == 0x12)
				{
					HandleMetadata(r, packet->m_body + pos + 11, dataSize);
				}
				else if (packet->m_body[pos] == 8 || packet->m_body[pos] == 9)
				{
					nTimeStamp = AMF_DecodeInt24(packet->m_body + pos + 4);
					nTimeStamp |= (packet->m_body[pos + 7] << 24);
				}
				pos += (11 + dataSize + 4);
			}
			if (!r->m_pausing)
				r->m_mediaStamp = nTimeStamp;

			/* FLV tag(s) */
			/*RTMP_Log(RTMP_LOGDEBUG, "%s, received: FLV tag(s) %lu bytes", __FUNCTION__, packet.m_nBodySize); */
			bHasMediaPacket = 1;
			break;
		}
		default:
			LOG_Print_Info(libFrameWork_RtmpCommon,
				"IRtmpSession::ClientPacket() %s, unknown packet type received: 0x%02x", __FUNCTION__,
				packet->m_packetType);
#ifdef _DEBUG
			LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::SockBuf_FillV20() %s", _rtmp_string::HexToStr(packet->m_body, packet->m_nBodySize).c_str());
#endif
		}

		return bHasMediaPacket;
	}

	int IRtmpSession::HandleInvoke(RTMP *r, const char *body, unsigned int nBodySize)
	{
		AMFObject obj;
		AVal method;
		int txn;
		int ret = 0, nRes;
		if (body[0] != 0x02)		/* make sure it is a string method name we start with */
		{
			LOG_Print_Error(libFrameWork_RtmpCommon, "IRtmpSession::HandleInvoke() %s, Sanity failed. no string method in invoke packet",
				__FUNCTION__);
			return 0;
		}

		nRes = AMF_Decode(&obj, body, nBodySize, FALSE);
		if (nRes < 0)
		{
			LOG_Print_Error(libFrameWork_RtmpCommon, "IRtmpSession::HandleInvoke() %s, error decoding invoke packet", __FUNCTION__);
			return 0;
		}

		AMF_Dump(&obj);
		AMFProp_GetString(AMF_GetProp(&obj, NULL, 0), &method);
		txn = (int)AMFProp_GetNumber(AMF_GetProp(&obj, NULL, 1));
		LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::HandleInvoke() %s, server invoking <%s>", __FUNCTION__, method.av_val);

		if (AVMATCH(&method, &av__result))
		{
			AVal methodInvoked = { 0 };
			int i;

			for (i = 0; i < r->m_numCalls; i++) {
				if (r->m_methodCalls[i].num == txn) {
					methodInvoked = r->m_methodCalls[i].name;
					AV_erase(r->m_methodCalls, &r->m_numCalls, i, FALSE);
					break;
				}
			}
			if (!methodInvoked.av_val) {
				LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::HandleInvoke() %s, received result id %d without matching request",
					__FUNCTION__, txn);
				goto leave;
			}

			LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::HandleInvoke() %s, received result for method call <%s>", __FUNCTION__,
				methodInvoked.av_val);

			if (AVMATCH(&methodInvoked, &av_connect))
			{
				if (r->Link.token.av_len)
				{
					AMFObjectProperty p;
					if (RTMP_FindFirstMatchingProperty(&obj, &av_secureToken, &p))
					{
						DecodeTEA(&r->Link.token, &p.p_vu.p_aval);
						SendSecureTokenResponse(r, &p.p_vu.p_aval);
					}
				}
				if (r->Link.protocol & RTMP_FEATURE_WRITE)
				{
					SendReleaseStream(r);
					SendFCPublish(r);
				}
				else
				{
					SendServerBW(r);
					SendCtrl(r, 3, 0, 300);
				}
				SendCreateStream(r);

				if (!(r->Link.protocol & RTMP_FEATURE_WRITE))
				{
					/* Send the FCSubscribe if live stream or if subscribepath is set */
					if (r->Link.subscribepath.av_len)
						SendFCSubscribe(r, &r->Link.subscribepath);
					else if (r->Link.lFlags & RTMP_LF_LIVE)
						SendFCSubscribe(r, &r->Link.playpath);
				}
			}
			else if (AVMATCH(&methodInvoked, &av_createStream))
			{
				r->m_stream_id = (int)AMFProp_GetNumber(AMF_GetProp(&obj, NULL, 3));

				if (r->Link.protocol & RTMP_FEATURE_WRITE)
				{
					SendPublish(r);
				}
				else
				{
					if (r->Link.lFlags & RTMP_LF_PLST)
						SendPlaylist(r);
					SendPlay(r);
					SendCtrl(r, 3, r->m_stream_id, r->m_nBufferMS);
				}
			}
			else if (AVMATCH(&methodInvoked, &av_play) ||
				AVMATCH(&methodInvoked, &av_publish))
			{
				r->m_bPlaying = TRUE;
			}
			free(methodInvoked.av_val);
		}
		else if (AVMATCH(&method, &av_onBWDone))
		{
			if (!r->m_nBWCheckCounter)
				SendCheckBW(r);
		}
		else if (AVMATCH(&method, &av_onFCSubscribe))
		{
			/* SendOnFCSubscribe(); */
		}
		else if (AVMATCH(&method, &av_onFCUnsubscribe))
		{
			Close();
			ret = 1;
		}
		else if (AVMATCH(&method, &av_ping))
		{
			SendPong(r, txn);
		}
		else if (AVMATCH(&method, &av__onbwcheck))
		{
			SendCheckBWResult(r, txn);
		}
		else if (AVMATCH(&method, &av__onbwdone))
		{
			int i;
			for (i = 0; i < r->m_numCalls; i++)
				if (AVMATCH(&r->m_methodCalls[i].name, &av__checkbw))
				{
					AV_erase(r->m_methodCalls, &r->m_numCalls, i, TRUE);
					break;
				}
		}
		else if (AVMATCH(&method, &av__error))
		{
			LOG_Print_Error(libFrameWork_RtmpCommon, "IRtmpSession::HandleInvoke() rtmp server sent error");
		}
		else if (AVMATCH(&method, &av_close))
		{
			LOG_Print_Error(libFrameWork_RtmpCommon, "IRtmpSession::HandleInvoke() rtmp server requested close");
			Close();
		}
		else if (AVMATCH(&method, &av_onStatus))
		{
			AMFObject obj2;
			AVal code, level;
			AMFProp_GetObject(AMF_GetProp(&obj, NULL, 3), &obj2);
			AMFProp_GetString(AMF_GetProp(&obj2, &av_code, -1), &code);
			AMFProp_GetString(AMF_GetProp(&obj2, &av_level, -1), &level);

			LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::HandleInvoke() %s, onStatus: %s", __FUNCTION__, code.av_val);
			if (AVMATCH(&code, &av_NetStream_Failed)
				|| AVMATCH(&code, &av_NetStream_Play_Failed)
				|| AVMATCH(&code, &av_NetStream_Play_StreamNotFound)
				|| AVMATCH(&code, &av_NetConnection_Connect_InvalidApp))
			{
				r->m_stream_id = -1;
				Close();
				LOG_Print_Error(libFrameWork_RtmpCommon, "IRtmpSession::HandleInvoke() Closing connection: %s", code.av_val);
			}

			else if (AVMATCH(&code, &av_NetStream_Play_Start))
			{
				int i;
				r->m_bPlaying = TRUE;
				for (i = 0; i < r->m_numCalls; i++)
				{
					if (AVMATCH(&r->m_methodCalls[i].name, &av_play))
					{
						AV_erase(r->m_methodCalls, &r->m_numCalls, i, TRUE);
						break;
					}
				}
			}

			else if (AVMATCH(&code, &av_NetStream_Publish_Start))
			{
				int i;
				r->m_bPlaying = TRUE;
				for (i = 0; i < r->m_numCalls; i++)
				{
					if (AVMATCH(&r->m_methodCalls[i].name, &av_publish))
					{
						AV_erase(r->m_methodCalls, &r->m_numCalls, i, TRUE);
						break;
					}
				}
			}

			/* Return 1 if this is a Play.Complete or Play.Stop */
			else if (AVMATCH(&code, &av_NetStream_Play_Complete)
				|| AVMATCH(&code, &av_NetStream_Play_Stop)
				|| AVMATCH(&code, &av_NetStream_Play_UnpublishNotify))
			{
				Close();
				ret = 1;
			}

			else if (AVMATCH(&code, &av_NetStream_Seek_Notify))
			{
				r->m_read.flags &= ~RTMP_READ_SEEKING;
			}

			else if (AVMATCH(&code, &av_NetStream_Pause_Notify))
			{
				if (r->m_pausing == 1 || r->m_pausing == 2)
				{
					SendPause(r, FALSE, r->m_pauseStamp);
					r->m_pausing = 3;
				}
			}
		}
		else if (AVMATCH(&method, &av_playlist_ready))
		{
			int i;
			for (i = 0; i < r->m_numCalls; i++)
			{
				if (AVMATCH(&r->m_methodCalls[i].name, &av_set_playlist))
				{
					AV_erase(r->m_methodCalls, &r->m_numCalls, i, TRUE);
					break;
				}
			}
		}
		else
		{

		}
	leave:
		AMF_Reset(&obj);
		return ret;
	}

	void IRtmpSession::DecodeTEA(AVal *key, AVal *text)
	{
		uint32_t *v, k[4] = { 0 }, u;
		uint32_t z, y, sum = 0, e, DELTA = 0x9e3779b9;
		int32_t p, q;
		int i, n;
		unsigned char *ptr, *out;

		/* prep key: pack 1st 16 chars into 4 LittleEndian ints */
		ptr = (unsigned char *)key->av_val;
		u = 0;
		n = 0;
		v = k;
		p = key->av_len > 16 ? 16 : key->av_len;
		for (i = 0; i < p; i++)
		{
			u |= ptr[i] << (n * 8);
			if (n == 3)
			{
				*v++ = u;
				u = 0;
				n = 0;
			}
			else
			{
				n++;
			}
		}
		/* any trailing chars */
		if (u)
			*v = u;

		/* prep text: hex2bin, multiples of 4 */
		n = (text->av_len + 7) / 8;
		out = (unsigned char*)malloc(n * 8);
		ptr = (unsigned char *)text->av_val;
		v = (uint32_t *)out;
		for (i = 0; i < n; i++)
		{
			u = (HEX2BIN(ptr[0]) << 4) + HEX2BIN(ptr[1]);
			u |= ((HEX2BIN(ptr[2]) << 4) + HEX2BIN(ptr[3])) << 8;
			u |= ((HEX2BIN(ptr[4]) << 4) + HEX2BIN(ptr[5])) << 16;
			u |= ((HEX2BIN(ptr[6]) << 4) + HEX2BIN(ptr[7])) << 24;
			*v++ = u;
			ptr += 8;
		}
		v = (uint32_t *)out;

		/* http://www.movable-type.co.uk/scripts/tea-block.html */
#define MX (((z>>5)^(y<<2)) + ((y>>3)^(z<<4))) ^ ((sum^y) + (k[(p&3)^e]^z));
		z = v[n - 1];
		y = v[0];
		q = 6 + 52 / n;
		sum = q * DELTA;
		while (sum != 0)
		{
			e = sum >> 2 & 3;
			for (p = n - 1; p > 0; p--)
				z = v[p - 1], y = v[p] -= MX;
			z = v[n - 1];
			y = v[0] -= MX;
			sum -= DELTA;
		}

		text->av_len /= 2;
		memcpy(text->av_val, out, text->av_len);
		free(out);
	}

	int IRtmpSession::SendSecureTokenResponse(RTMP *r, AVal *resp)
	{
		RTMPPacket packet;
		char pbuf[1024], *pend = pbuf + sizeof(pbuf);
		char *enc;

		packet.m_nChannel = 0x03;	/* control channel (invoke) */
		packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;
		packet.m_packetType = 0x14;
		packet.m_nTimeStamp = 0;
		packet.m_nInfoField2 = 0;
		packet.m_hasAbsTimestamp = 0;
		packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;

		enc = packet.m_body;
		enc = AMF_EncodeString(enc, pend, &av_secureTokenResponse);
		enc = AMF_EncodeNumber(enc, pend, 0.0);
		*enc++ = AMF_NULL;
		enc = AMF_EncodeString(enc, pend, resp);
		if (!enc)
			return FALSE;

		packet.m_nBodySize = enc - packet.m_body;

		return SendPacket(r, &packet, FALSE);
	}

	int IRtmpSession::SendReleaseStream(RTMP *r)
	{
		RTMPPacket packet;
		char pbuf[1024], *pend = pbuf + sizeof(pbuf);
		char *enc;

		packet.m_nChannel = 0x03;	/* control channel (invoke) */
		packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;
		packet.m_packetType = 0x14;	/* INVOKE */
		packet.m_nTimeStamp = 0;
		packet.m_nInfoField2 = 0;
		packet.m_hasAbsTimestamp = 0;
		packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;

		enc = packet.m_body;
		enc = AMF_EncodeString(enc, pend, &av_releaseStream);
		enc = AMF_EncodeNumber(enc, pend, ++r->m_numInvokes);
		*enc++ = AMF_NULL;
		enc = AMF_EncodeString(enc, pend, &r->Link.playpath);
		if (!enc)
			return FALSE;

		packet.m_nBodySize = enc - packet.m_body;

		return SendPacket(r, &packet, FALSE);
	}

	int IRtmpSession::ReadPacketV20(RTMP *r, RTMPPacket *packet,
		const char* c_szData, UInt32 uSize, UInt32& out_uSize)
	{
		uint8_t hbuf[RTMP_MAX_HEADER_SIZE] = { 0 };
		UInt16 uOffset = 0;
		char *header = (char *)hbuf;
		int nSize, hSize, nToRead, nChunk;
		int didAlloc = FALSE;

		LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::ReadPacketV20() %s: fd=%d", __FUNCTION__, r->m_sb.sb_socket);

		if (ReadNV20(r, &c_szData[uOffset], 1, (char *)hbuf, 1) == 0)
		{
			LOG_Print_Error(libFrameWork_RtmpCommon, "IRtmpSession::ReadPacketV20() %s, failed to read RTMP packet header", __FUNCTION__);
			return FALSE;
		}

		uOffset += 1;
		packet->m_headerType = (hbuf[0] & 0xc0) >> 6;
		packet->m_nChannel = (hbuf[0] & 0x3f);
		header++;
		if (packet->m_nChannel == 0)
		{
			if (ReadNV20(r, &c_szData[uOffset], 1, (char *)&hbuf[1], 1) != 1)
			{
				LOG_Print_Error(libFrameWork_RtmpCommon, "IRtmpSession::ReadPacketV20() %s, failed to read RTMP packet header 2nd byte",
					__FUNCTION__);
				return FALSE;
			}
			packet->m_nChannel = hbuf[1];
			packet->m_nChannel += 64;
			header++;
			uOffset += 1;
		}
		else if (packet->m_nChannel == 1)
		{
			int tmp;
			if (ReadNV20(r, &c_szData[uOffset], 2, (char *)&hbuf[1], 2) != 2)
			{
				LOG_Print_Error(libFrameWork_RtmpCommon, "IRtmpSession::ReadPacketV20() %s, failed to read RTMP packet header 3nd byte",
					__FUNCTION__);
				return FALSE;
			}
			tmp = (hbuf[2] << 8) + hbuf[1];
			packet->m_nChannel = tmp + 64;
			LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::ReadPacketV20() %s, m_nChannel: %0x", __FUNCTION__, packet->m_nChannel);
			header += 2;
			uOffset += 2;
		}

		nSize = packetSize[packet->m_headerType];

		if (nSize == RTMP_LARGE_HEADER_SIZE)	/* if we get a full header the timestamp is absolute */
			packet->m_hasAbsTimestamp = TRUE;

		else if (nSize < RTMP_LARGE_HEADER_SIZE)
		{				/* using values from the last message of this channel */
			if (r->m_vecChannelsIn[packet->m_nChannel])
				memcpy(packet, r->m_vecChannelsIn[packet->m_nChannel],
					sizeof(RTMPPacket));
		}

		nSize--;
		
		if (nSize > 0 && uSize - uOffset < nSize)
		{
			LOG_Print_Error(libFrameWork_RtmpCommon, "IRtmpSession::ReadPacketV20() %s, failed to read RTMP packet header. type: %x",
				__FUNCTION__, (unsigned int)hbuf[0]);
			return FALSE;
		}

		if (ReadNV20(r, &c_szData[uOffset], nSize, header, nSize) != nSize)
		{
			LOG_Print_Error(libFrameWork_RtmpCommon, "IRtmpSession::ReadPacketV20() %s, failed to read RTMP packet header 3nd byte",
				__FUNCTION__);
			return FALSE;
		}

		uOffset += nSize;

		hSize = nSize + (header - (char *)hbuf);

		if (nSize >= 3)
		{
			packet->m_nTimeStamp = AMF_DecodeInt24(header);

			/*RTMP_Log(RTMP_LOGDEBUG, "%s, reading RTMP packet chunk on channel %x, headersz %i, timestamp %i, abs timestamp %i", __FUNCTION__, packet.m_nChannel, nSize, packet.m_nTimeStamp, packet.m_hasAbsTimestamp); */

			if (nSize >= 6)
			{
				packet->m_nBodySize = AMF_DecodeInt24(header + 3);
				packet->m_nBytesRead = 0;
				RTMPPacket_Free(packet);

				if (nSize > 6)
				{
					packet->m_packetType = header[6];

					if (nSize == 11)
						packet->m_nInfoField2 = DecodeInt32LE(header + 7);
				}
			}
			if (packet->m_nTimeStamp == 0xffffff)
			{
				if (ReadNV20(r, &c_szData[uOffset], 4, header + nSize, 4) != 4)
				{
					LOG_Print_Error(libFrameWork_RtmpCommon, "IRtmpSession::ReadPacketV20() %s, failed to read extended timestamp",
						__FUNCTION__);
					return FALSE;
				}
				packet->m_nTimeStamp = AMF_DecodeInt32(header + nSize);
				hSize += 4;
				uOffset += 4;
			}
		}

		if (packet->m_nBodySize > 0 && packet->m_body == NULL)
		{
			if (!RTMPPacket_Alloc(packet, packet->m_nBodySize))
			{
				LOG_Print_Error(libFrameWork_RtmpCommon, "IRtmpSession::ReadPacketV20() %s, failed to allocate packet", __FUNCTION__);
				return FALSE;
			}
			didAlloc = TRUE;
			packet->m_headerType = (hbuf[0] & 0xc0) >> 6;
		}

		nToRead = packet->m_nBodySize - packet->m_nBytesRead;
		nChunk = r->m_inChunkSize;
		if (nToRead < nChunk)
			nChunk = nToRead;

		/* Does the caller want the raw chunk? */
		if (packet->m_chunk)
		{
			packet->m_chunk->c_headerSize = hSize;
			memcpy(packet->m_chunk->c_header, hbuf, hSize);
			packet->m_chunk->c_chunk = packet->m_body + packet->m_nBytesRead;
			packet->m_chunk->c_chunkSize = nChunk;
		}

		if (uSize - uOffset < nChunk)
		{
			LOG_Print_Error(libFrameWork_RtmpCommon, "IRtmpSession::ReadPacketV20() %s, failed to read RTMP packet body. len: %lu",
				__FUNCTION__, packet->m_nBodySize);
			return FALSE;
		}

		ReadNV20(r, &c_szData[uOffset], nChunk, packet->m_body + packet->m_nBytesRead, nChunk);
		uOffset += nChunk;

		packet->m_nBytesRead += nChunk;

		/* keep the packet as ref for other packets on this channel */
		if (!r->m_vecChannelsIn[packet->m_nChannel])
			r->m_vecChannelsIn[packet->m_nChannel] = (RTMPPacket*)malloc(sizeof(RTMPPacket));
		memcpy(r->m_vecChannelsIn[packet->m_nChannel], packet, sizeof(RTMPPacket));

		if (RTMPPacket_IsReady(packet))
		{
			/* make packet's timestamp absolute */
			if (!packet->m_hasAbsTimestamp)
				packet->m_nTimeStamp += r->m_channelTimestamp[packet->m_nChannel];	/* timestamps seem to be always relative!! */

			r->m_channelTimestamp[packet->m_nChannel] = packet->m_nTimeStamp;

			/* reset the data from the stored packet. we keep the header since we may use it later if a new packet for this channel */
			/* arrives and requests to re-use some info (small packet header) */
			r->m_vecChannelsIn[packet->m_nChannel]->m_body = NULL;
			r->m_vecChannelsIn[packet->m_nChannel]->m_nBytesRead = 0;
			r->m_vecChannelsIn[packet->m_nChannel]->m_hasAbsTimestamp = FALSE;	/* can only be false if we reuse header */
		}
		else
		{
			packet->m_body = NULL;	/* so it won't be erased on free */
		}

		out_uSize = uOffset;
		return TRUE;
	}

	int IRtmpSession::SendFCPublish(RTMP *r)
	{
		RTMPPacket packet;
		char pbuf[1024], *pend = pbuf + sizeof(pbuf);
		char *enc;

		packet.m_nChannel = 0x03;	/* control channel (invoke) */
		packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;
		packet.m_packetType = 0x14;	/* INVOKE */
		packet.m_nTimeStamp = 0;
		packet.m_nInfoField2 = 0;
		packet.m_hasAbsTimestamp = 0;
		packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;

		enc = packet.m_body;
		enc = AMF_EncodeString(enc, pend, &av_FCPublish);
		enc = AMF_EncodeNumber(enc, pend, ++r->m_numInvokes);
		*enc++ = AMF_NULL;
		enc = AMF_EncodeString(enc, pend, &r->Link.playpath);
		if (!enc)
			return FALSE;

		packet.m_nBodySize = enc - packet.m_body;

		return SendPacket(r, &packet, FALSE);
	}

	int IRtmpSession::SendServerBW(RTMP *r)
	{
		RTMPPacket packet;
		char pbuf[256], *pend = pbuf + sizeof(pbuf);

		packet.m_nChannel = 0x02;	/* control channel (invoke) */
		packet.m_headerType = RTMP_PACKET_SIZE_LARGE;
		packet.m_packetType = 0x05;	/* Server BW */
		packet.m_nTimeStamp = 0;
		packet.m_nInfoField2 = 0;
		packet.m_hasAbsTimestamp = 0;
		packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;

		packet.m_nBodySize = 4;

		AMF_EncodeInt32(packet.m_body, pend, r->m_nServerBW);
		return SendPacket(r, &packet, FALSE);
	}

	int IRtmpSession::SendCtrl(RTMP *r, short nType, unsigned int nObject, unsigned int nTime)
	{
		RTMPPacket packet;
		char pbuf[256], *pend = pbuf + sizeof(pbuf);
		int nSize;
		char *buf;

		LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::SendCtrl() sending ctrl. type: 0x%04x", (unsigned short)nType);

		packet.m_nChannel = 0x02;	/* control channel (ping) */
		packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;
		packet.m_packetType = 0x04;	/* ctrl */
		packet.m_nTimeStamp = 0;	/* RTMP_GetTime(); */
		packet.m_nInfoField2 = 0;
		packet.m_hasAbsTimestamp = 0;
		packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;

		switch (nType) {
		case 0x03: nSize = 10; break;	/* buffer time */
		case 0x1A: nSize = 3; break;	/* SWF verify request */
		case 0x1B: nSize = 44; break;	/* SWF verify response */
		default: nSize = 6; break;
		}

		packet.m_nBodySize = nSize;

		buf = packet.m_body;
		buf = AMF_EncodeInt16(buf, pend, nType);

		if (nType == 0x1B)
		{
#ifdef CRYPTO
			memcpy(buf, r->Link.SWFVerificationResponse, 42);
			LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::SendCtrl() Sending SWFVerification response: ");
#endif
		}
		else if (nType == 0x1A)
		{
			*buf = nObject & 0xff;
		}
		else
		{
			if (nSize > 2)
				buf = AMF_EncodeInt32(buf, pend, nObject);

			if (nSize > 6)
				buf = AMF_EncodeInt32(buf, pend, nTime);
		}

		return SendPacket(r, &packet, FALSE);
	}

	int IRtmpSession::SendCreateStream(RTMP *r)
	{
		RTMPPacket packet;
		char pbuf[256], *pend = pbuf + sizeof(pbuf);
		char *enc;

		packet.m_nChannel = 0x03;	/* control channel (invoke) */
		packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;
		packet.m_packetType = 0x14;	/* INVOKE */
		packet.m_nTimeStamp = 0;
		packet.m_nInfoField2 = 0;
		packet.m_hasAbsTimestamp = 0;
		packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;

		enc = packet.m_body;
		enc = AMF_EncodeString(enc, pend, &av_createStream);
		enc = AMF_EncodeNumber(enc, pend, ++r->m_numInvokes);
		*enc++ = AMF_NULL;		/* NULL */

		packet.m_nBodySize = enc - packet.m_body;

		return SendPacket(r, &packet, TRUE);
	}

	int IRtmpSession::SendFCSubscribe(RTMP *r, AVal *subscribepath)
	{
		RTMPPacket packet;
		char pbuf[512], *pend = pbuf + sizeof(pbuf);
		char *enc;
		packet.m_nChannel = 0x03;	/* control channel (invoke) */
		packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;
		packet.m_packetType = 0x14;	/* INVOKE */
		packet.m_nTimeStamp = 0;
		packet.m_nInfoField2 = 0;
		packet.m_hasAbsTimestamp = 0;
		packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;

		LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::SendFCSubscribe() FCSubscribe: %s", subscribepath->av_val);
		enc = packet.m_body;
		enc = AMF_EncodeString(enc, pend, &av_FCSubscribe);
		enc = AMF_EncodeNumber(enc, pend, ++r->m_numInvokes);
		*enc++ = AMF_NULL;
		enc = AMF_EncodeString(enc, pend, subscribepath);

		if (!enc)
			return FALSE;

		packet.m_nBodySize = enc - packet.m_body;

		return SendPacket(r, &packet, TRUE);
	}

	int IRtmpSession::SendPublish(RTMP *r)
	{
		RTMPPacket packet;
		char pbuf[1024], *pend = pbuf + sizeof(pbuf);
		char *enc;

		packet.m_nChannel = 0x04;	/* source channel (invoke) */
		packet.m_headerType = RTMP_PACKET_SIZE_LARGE;
		packet.m_packetType = 0x14;	/* INVOKE */
		packet.m_nTimeStamp = 0;
		packet.m_nInfoField2 = r->m_stream_id;
		packet.m_hasAbsTimestamp = 0;
		packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;

		enc = packet.m_body;
		enc = AMF_EncodeString(enc, pend, &av_publish);
		enc = AMF_EncodeNumber(enc, pend, ++r->m_numInvokes);
		*enc++ = AMF_NULL;
		enc = AMF_EncodeString(enc, pend, &r->Link.playpath);
		if (!enc)
			return FALSE;

		/* FIXME: should we choose live based on Link.lFlags & RTMP_LF_LIVE? */
		enc = AMF_EncodeString(enc, pend, &av_live);
		if (!enc)
			return FALSE;

		packet.m_nBodySize = enc - packet.m_body;

		return SendPacket(r, &packet, TRUE);
	}

	int IRtmpSession::SendPlaylist(RTMP *r)
	{
		RTMPPacket packet;
		char pbuf[1024], *pend = pbuf + sizeof(pbuf);
		char *enc;

		packet.m_nChannel = 0x08;	/* we make 8 our stream channel */
		packet.m_headerType = RTMP_PACKET_SIZE_LARGE;
		packet.m_packetType = 0x14;	/* INVOKE */
		packet.m_nTimeStamp = 0;
		packet.m_nInfoField2 = r->m_stream_id;	/*0x01000000; */
		packet.m_hasAbsTimestamp = 0;
		packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;

		enc = packet.m_body;
		enc = AMF_EncodeString(enc, pend, &av_set_playlist);
		enc = AMF_EncodeNumber(enc, pend, 0);
		*enc++ = AMF_NULL;
		*enc++ = AMF_ECMA_ARRAY;
		*enc++ = 0;
		*enc++ = 0;
		*enc++ = 0;
		*enc++ = AMF_OBJECT;
		enc = AMF_EncodeNamedString(enc, pend, &av_0, &r->Link.playpath);
		if (!enc)
			return FALSE;
		if (enc + 3 >= pend)
			return FALSE;
		*enc++ = 0;
		*enc++ = 0;
		*enc++ = AMF_OBJECT_END;

		packet.m_nBodySize = enc - packet.m_body;

		return SendPacket(r, &packet, TRUE);
	}

	int IRtmpSession::SendPlay(RTMP *r)
	{
		RTMPPacket packet;
		char pbuf[1024], *pend = pbuf + sizeof(pbuf);
		char *enc;

		packet.m_nChannel = 0x08;	/* we make 8 our stream channel */
		packet.m_headerType = RTMP_PACKET_SIZE_LARGE;
		packet.m_packetType = 0x14;	/* INVOKE */
		packet.m_nTimeStamp = 0;
		packet.m_nInfoField2 = r->m_stream_id;	/*0x01000000; */
		packet.m_hasAbsTimestamp = 0;
		packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;

		enc = packet.m_body;
		enc = AMF_EncodeString(enc, pend, &av_play);
		enc = AMF_EncodeNumber(enc, pend, ++r->m_numInvokes);
		*enc++ = AMF_NULL;

		LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::SendPlay() %s, seekTime=%d, stopTime=%d, sending play: %s",
			__FUNCTION__, r->Link.seekTime, r->Link.stopTime,
			r->Link.playpath.av_val);
		enc = AMF_EncodeString(enc, pend, &r->Link.playpath);
		if (!enc)
			return FALSE;

		/* Optional parameters start and len.
		*
		* start: -2, -1, 0, positive number
		*  -2: looks for a live stream, then a recorded stream,
		*      if not found any open a live stream
		*  -1: plays a live stream
		* >=0: plays a recorded streams from 'start' milliseconds
		*/
		if (r->Link.lFlags & RTMP_LF_LIVE)
			enc = AMF_EncodeNumber(enc, pend, -1000.0);
		else
		{
			if (r->Link.seekTime > 0.0)
				enc = AMF_EncodeNumber(enc, pend, r->Link.seekTime);	/* resume from here */
			else
				enc = AMF_EncodeNumber(enc, pend, 0.0);	/*-2000.0);*/ /* recorded as default, -2000.0 is not reliable since that freezes the player if the stream is not found */
		}
		if (!enc)
			return FALSE;

		/* len: -1, 0, positive number
		*  -1: plays live or recorded stream to the end (default)
		*   0: plays a frame 'start' ms away from the beginning
		*  >0: plays a live or recoded stream for 'len' milliseconds
		*/
		/*enc += EncodeNumber(enc, -1.0); */ /* len */
		if (r->Link.stopTime)
		{
			enc = AMF_EncodeNumber(enc, pend, r->Link.stopTime - r->Link.seekTime);
			if (!enc)
				return FALSE;
		}

		packet.m_nBodySize = enc - packet.m_body;

		return SendPacket(r, &packet, TRUE);
	}

	int IRtmpSession::SendCheckBW(RTMP *r)
	{
		RTMPPacket packet;
		char pbuf[256], *pend = pbuf + sizeof(pbuf);
		char *enc;

		packet.m_nChannel = 0x03;	/* control channel (invoke) */
		packet.m_headerType = RTMP_PACKET_SIZE_LARGE;
		packet.m_packetType = 0x14;	/* INVOKE */
		packet.m_nTimeStamp = 0;	/* RTMP_GetTime(); */
		packet.m_nInfoField2 = 0;
		packet.m_hasAbsTimestamp = 0;
		packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;

		enc = packet.m_body;
		enc = AMF_EncodeString(enc, pend, &av__checkbw);
		enc = AMF_EncodeNumber(enc, pend, ++r->m_numInvokes);
		*enc++ = AMF_NULL;

		packet.m_nBodySize = enc - packet.m_body;

		/* triggers _onbwcheck and eventually results in _onbwdone */
		return SendPacket(r, &packet, FALSE);
	}

	int IRtmpSession::SendPong(RTMP *r, double txn)
	{
		RTMPPacket packet;
		char pbuf[256], *pend = pbuf + sizeof(pbuf);
		char *enc;

		packet.m_nChannel = 0x03;	/* control channel (invoke) */
		packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;
		packet.m_packetType = 0x14;	/* INVOKE */
		packet.m_nTimeStamp = 0x16 * r->m_nBWCheckCounter;	/* temp inc value. till we figure it out. */
		packet.m_nInfoField2 = 0;
		packet.m_hasAbsTimestamp = 0;
		packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;

		enc = packet.m_body;
		enc = AMF_EncodeString(enc, pend, &av_pong);
		enc = AMF_EncodeNumber(enc, pend, txn);
		*enc++ = AMF_NULL;

		packet.m_nBodySize = enc - packet.m_body;

		return SendPacket(r, &packet, FALSE);
	}

	int IRtmpSession::SendCheckBWResult(RTMP *r, double txn)
	{
		RTMPPacket packet;
		char pbuf[256], *pend = pbuf + sizeof(pbuf);
		char *enc;

		packet.m_nChannel = 0x03;	/* control channel (invoke) */
		packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;
		packet.m_packetType = 0x14;	/* INVOKE */
		packet.m_nTimeStamp = 0x16 * r->m_nBWCheckCounter;	/* temp inc value. till we figure it out. */
		packet.m_nInfoField2 = 0;
		packet.m_hasAbsTimestamp = 0;
		packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;

		enc = packet.m_body;
		enc = AMF_EncodeString(enc, pend, &av__result);
		enc = AMF_EncodeNumber(enc, pend, txn);
		*enc++ = AMF_NULL;
		enc = AMF_EncodeNumber(enc, pend, (double)r->m_nBWCheckCounter++);

		packet.m_nBodySize = enc - packet.m_body;

		return SendPacket(r, &packet, FALSE);
	}

	int IRtmpSession::SendPause(RTMP *r, int DoPause, int iTime)
	{
		RTMPPacket packet;
		char pbuf[256], *pend = pbuf + sizeof(pbuf);
		char *enc;

		packet.m_nChannel = 0x08;	/* video channel */
		packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM;
		packet.m_packetType = 0x14;	/* invoke */
		packet.m_nTimeStamp = 0;
		packet.m_nInfoField2 = 0;
		packet.m_hasAbsTimestamp = 0;
		packet.m_body = pbuf + RTMP_MAX_HEADER_SIZE;

		enc = packet.m_body;
		enc = AMF_EncodeString(enc, pend, &av_pause);
		enc = AMF_EncodeNumber(enc, pend, ++r->m_numInvokes);
		*enc++ = AMF_NULL;
		enc = AMF_EncodeBoolean(enc, pend, DoPause);
		enc = AMF_EncodeNumber(enc, pend, (double)iTime);

		packet.m_nBodySize = enc - packet.m_body;

		LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::SendPause() %s, %d, pauseTime=%d", __FUNCTION__, DoPause, iTime);
		return SendPacket(r, &packet, TRUE);
	}

	void IRtmpSession::HandleCtrl(RTMP *r, const RTMPPacket *packet)
	{
		short nType = -1;
		unsigned int tmp;
		if (packet->m_body && packet->m_nBodySize >= 2)
			nType = AMF_DecodeInt16(packet->m_body);
		LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::HandleCtrl() %s, received ctrl. type: %d, len: %d", __FUNCTION__, nType,
			packet->m_nBodySize);

		if (packet->m_nBodySize >= 6)
		{
			switch (nType)
			{
			case 0:
				tmp = AMF_DecodeInt32(packet->m_body + 2);
				LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::HandleCtrl() %s, Stream Begin %d", __FUNCTION__, tmp);
				break;

			case 1:
				tmp = AMF_DecodeInt32(packet->m_body + 2);
				LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::HandleCtrl() %s, Stream EOF %d", __FUNCTION__, tmp);
				if (r->m_pausing == 1)
					r->m_pausing = 2;
				break;

			case 2:
				tmp = AMF_DecodeInt32(packet->m_body + 2);
				LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::HandleCtrl() %s, Stream Dry %d", __FUNCTION__, tmp);
				break;

			case 4:
				tmp = AMF_DecodeInt32(packet->m_body + 2);
				LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::HandleCtrl() %s, Stream IsRecorded %d", __FUNCTION__, tmp);
				break;

			case 6:		/* server ping. reply with pong. */
				tmp = AMF_DecodeInt32(packet->m_body + 2);
				LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::HandleCtrl() %s, Ping %d", __FUNCTION__, tmp);
				SendCtrl(r, 0x07, tmp, 0);
				break;

				/* FMS 3.5 servers send the following two controls to let the client
				* know when the server has sent a complete buffer. I.e., when the
				* server has sent an amount of data equal to m_nBufferMS in duration.
				* The server meters its output so that data arrives at the client
				* in realtime and no faster.
				*
				* The rtmpdump program tries to set m_nBufferMS as large as
				* possible, to force the server to send data as fast as possible.
				* In practice, the server appears to cap this at about 1 hour's
				* worth of data. After the server has sent a complete buffer, and
				* sends this BufferEmpty message, it will wait until the play
				* duration of that buffer has passed before sending a new buffer.
				* The BufferReady message will be sent when the new buffer starts.
				* (There is no BufferReady message for the very first buffer;
				* presumably the Stream Begin message is sufficient for that
				* purpose.)
				*
				* If the network speed is much faster than the data bitrate, then
				* there may be long delays between the end of one buffer and the
				* start of the next.
				*
				* Since usually the network allows data to be sent at
				* faster than realtime, and rtmpdump wants to download the data
				* as fast as possible, we use this RTMP_LF_BUFX hack: when we
				* get the BufferEmpty message, we send a Pause followed by an
				* Unpause. This causes the server to send the next buffer immediately
				* instead of waiting for the full duration to elapse. (That's
				* also the purpose of the ToggleStream function, which rtmpdump
				* calls if we get a read timeout.)
				*
				* Media player apps don't need this hack since they are just
				* going to play the data in realtime anyway. It also doesn't work
				* for live streams since they obviously can only be sent in
				* realtime. And it's all moot if the network speed is actually
				* slower than the media bitrate.
				*/
			case 31:
				tmp = AMF_DecodeInt32(packet->m_body + 2);
				LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::HandleCtrl() %s, Stream BufferEmpty %d", __FUNCTION__, tmp);
				if (!(r->Link.lFlags & RTMP_LF_BUFX))
					break;
				if (!r->m_pausing)
				{
					r->m_pauseStamp = r->m_channelTimestamp[r->m_mediaChannel];
					SendPause(r, TRUE, r->m_pauseStamp);
					r->m_pausing = 1;
				}
				else if (r->m_pausing == 2)
				{
					SendPause(r, FALSE, r->m_pauseStamp);
					r->m_pausing = 3;
				}
				break;

			case 32:
				tmp = AMF_DecodeInt32(packet->m_body + 2);
				LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::HandleCtrl() %s, Stream BufferReady %d", __FUNCTION__, tmp);
				break;
			default:
				tmp = AMF_DecodeInt32(packet->m_body + 2);
				LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::HandleCtrl() %s, Stream xx %d", __FUNCTION__, tmp);
				break;
			}
		}

		if (nType == 0x1A)
		{
			LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::HandleCtrl() %s, SWFVerification ping received: ", __FUNCTION__);
#ifdef CRYPTO
			/* respond with HMAC SHA256 of decompressed SWF, key is the 30byte player key, also the last 30 bytes of the server handshake are applied */
			if (r->Link.SWFSize)
			{
				SendCtrl(r, 0x1B, 0, 0);
			}
			else
			{
				LOG_Print_Error(libFrameWork_RtmpCommon, "IRtmpSession::HandleCtrl() %s: Ignoring SWFVerification request, use --swfVfy!",
					__FUNCTION__);
			}
#else
			LOG_Print_Error(libFrameWork_RtmpCommon, "IRtmpSession::HandleCtrl() %s: Ignoring SWFVerification request, no CRYPTO support!",
				__FUNCTION__);
#endif
		}
	}

	int IRtmpSession::HandleMetadata(RTMP *r, char *body, unsigned int len)
	{
		/* allright we get some info here, so parse it and print it */
		/* also keep duration or filesize to make a nice progress bar */

		AMFObject obj;
		AVal metastring;
		int ret = FALSE;

		int nRes = AMF_Decode(&obj, body, len, FALSE);
		if (nRes < 0)
		{
			LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::HandleMetadata() %s, error decoding meta data packet", __FUNCTION__);
			return FALSE;
		}

		AMF_Dump(&obj);
		AMFProp_GetString(AMF_GetProp(&obj, NULL, 0), &metastring);

		if (AVMATCH(&metastring, &av_onMetaData))
		{
			AMFObjectProperty prop;
			/* Show metadata */
			LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::HandleMetadata() Metadata:");
			DumpMetaData(&obj);
			if (RTMP_FindFirstMatchingProperty(&obj, &av_duration, &prop))
			{
				r->m_fDuration = prop.p_vu.p_number;
				/*RTMP_Log(RTMP_LOGDEBUG, "Set duration: %.2f", m_fDuration); */
			}
			/* Search for audio or video tags */
			if (RTMP_FindPrefixProperty(&obj, &av_video, &prop))
				r->m_read.dataType |= 1;

			if (RTMP_FindPrefixProperty(&obj, &av_audio, &prop))
				r->m_read.dataType |= 4;
			ret = TRUE;
		}
		AMF_Reset(&obj);
		return ret;
	}

	int IRtmpSession::ReadNV20(RTMP *r, const char *in_buffer, int in_n, char *out_buffer, int out_n)
	{
		int nOriginalSize = out_n;
		int avail;
		char *ptr;

		r->m_sb.sb_timedout = FALSE;

#ifdef _DEBUG
		memset(out_buffer, 0, out_n);
#endif

		ptr = out_buffer;
		while (out_n > 0)
		{
			int nBytes = 0, nRead;
			if (r->Link.protocol & RTMP_FEATURE_HTTP)
			{
				while (!r->m_resplen)
				{
					if (r->m_sb.sb_size < 144)
					{
						if (!r->m_unackd)
							HTTP_Post(r, RTMPT_IDLE, "", 1);
						if (SockBuf_FillV20(&r->m_sb, in_buffer, in_n) < 1)
						{
							if (!r->m_sb.sb_timedout)
								Close();
							return 0;
						}
					}
					HTTP_readV20(r, 0, in_buffer, in_n);
				}
				if (r->m_resplen && !r->m_sb.sb_size)
					SockBuf_FillV20(&r->m_sb, in_buffer, in_n);
				avail = r->m_sb.sb_size;
				if (avail > r->m_resplen)
					avail = r->m_resplen;
			}
			else
			{
				avail = r->m_sb.sb_size;
				if (avail == 0)
				{
					if (SockBuf_FillV20(&r->m_sb, in_buffer, in_n) < 1)
					{
						if (!r->m_sb.sb_timedout)
							Close();
						return 0;
					}
					avail = r->m_sb.sb_size;
				}
			}
			nRead = ((out_n < avail) ? out_n : avail);
			if (nRead > 0)
			{
				memcpy(ptr, r->m_sb.sb_start, nRead);
				r->m_sb.sb_start += nRead;
				r->m_sb.sb_size -= nRead;
				nBytes = nRead;
				r->m_nBytesIn += nRead;
				if (r->m_bSendCounter
					&& r->m_nBytesIn > r->m_nBytesInSent + r->m_nClientBW / 2)
					SendBytesReceived(r);
			}
			/*RTMP_Log(RTMP_LOGDEBUG, "%s: %d bytes\n", __FUNCTION__, nBytes); */
#ifdef _DEBUG
			//fwrite(ptr, 1, nBytes, netstackdump_read);
#endif

			if (nBytes == 0)
			{
				LOG_Print_Error(libFrameWork_RtmpCommon, "IRtmpSession::ReadNV20() %s, RTMP socket closed by peer", __FUNCTION__);
				/*goto again; */
				Close();
				break;
			}

			if (r->Link.protocol & RTMP_FEATURE_HTTP)
				r->m_resplen -= nBytes;

#ifdef CRYPTO
			if (r->Link.rc4keyIn)
			{
				RC4_encrypt(r->Link.rc4keyIn, nBytes, ptr);
			}
#endif

			out_n -= nBytes;
			ptr += nBytes;
		}

		return nOriginalSize - out_n;
	}

	int IRtmpSession::SockBuf_FillV20(RTMPSockBuf *sb, const char* c_szData, UInt32 uSize)
	{
		int nBytes;

		if (!sb->sb_size)
			sb->sb_start = sb->sb_buf;

		while (1)
		{
			nBytes = sizeof(sb->sb_buf) - sb->sb_size - (sb->sb_start - sb->sb_buf);
#if defined(CRYPTO) && !defined(NO_SSL)
			if (sb->sb_ssl)
			{
				nBytes = TLS_read(sb->sb_ssl, sb->sb_start + sb->sb_size, nBytes);
			}
			else
#endif
			{
				nBytes = __min(nBytes, uSize);
				memcpy(sb->sb_start + sb->sb_size, c_szData, nBytes);
			}
			if (nBytes != -1)
			{
				sb->sb_size += nBytes;
			}
			else
			{
				int sockerr = GetSockError();
				LOG_Print_Info(libFrameWork_RtmpCommon, "IRtmpSession::SockBuf_FillV20() %s, recv returned %d. GetSockError(): %d (%s)",
					__FUNCTION__, nBytes, sockerr, strerror(sockerr));
				if (sockerr == EINTR && !RTMP_ctrlC)
					continue;

				if (sockerr == EWOULDBLOCK || sockerr == EAGAIN)
				{
					sb->sb_timedout = TRUE;
					nBytes = 0;
				}
			}
			break;
		}

		return nBytes;
	}

	void IRtmpSession::AV_clear(RTMP_METHOD *vals, int num)
	{
		int i;
		for (i = 0; i < num; i++)
			free(vals[i].name.av_val);
		free(vals);
	}

	int IRtmpSession::SendHandleHandShake(RTMP *r, RTMPStatus Status, const char* c_szData, UInt32 uSize)
	{
		int i;
		uint32_t uptime, suptime;
		int bMatch;
		char type;
		char clientbuf[RTMP_SIG_SIZE + 1], *clientsig = clientbuf + 1;
		char serversig[RTMP_SIG_SIZE];

		clientbuf[0] = 0x03;		/* not encrypted */

		uptime = htonl(RTMP_GetTime());
		memcpy(clientsig, &uptime, 4);

		memset(&clientsig[4], 0, 4);

		switch (Status)
		{
		case RTMP_STATUS_HandShake_C0C1:
		{
			if (c_szData == NULL)
			{
				for (i = 8; i < RTMP_SIG_SIZE; i++)
					clientsig[i] = (char)(rand() % 256);
			}
			else
			{
				memcpy(clientsig, c_szData, __min(RTMP_SIG_SIZE, uSize));
			}

			if (!WriteN(r, clientbuf, RTMP_SIG_SIZE + 1))
				return FALSE;
			
			//等待服务器握手S0++S1+S2
			_Status = RTMP_STATUS_HandShake_S0S1S2;
		}
		break;
		case RTMP_STATUS_HandShake_S0S1S2:
		{

		}
		break;
		case RTMP_STATUS_HandShake_C2:
		{
			if (!WriteN(r, c_szData, uSize))
				return FALSE;
		}
		break;
		}

		return 1;
	}

}

