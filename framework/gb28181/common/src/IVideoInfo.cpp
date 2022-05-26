

#include <libFrameWork_Rtsp/IVideoInfo.h>
#include <libFrameWork_Rtsp/H264Info.h>
#include <libFrameWork_Rtsp/H265Info.h>

MemPool_type g_VideoInfoPool20190214117( __max(sizeof(H264Info), sizeof(H265Info)) );

MemPool_type*	IVideoInfo::GetMediaInfoPoolInstance( void )
{
	return &g_VideoInfoPool20190214117;
}

