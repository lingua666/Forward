// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <dllLock/Exports.h>
#include <libLock/Lock.hpp>

struct monitor_info {
	HMONITOR	handle;
	int			cur_id;
	int			desired_id;
	int			id;
	RECT		rect;
};

static void draw_cursor(CURSORINFO Info, HDC hdc, HWND window)
{
	HICON      icon;
	ICONINFO   ii;
	CURSORINFO *ci = &Info;
	POINT      win_pos = {0, 0};

	if (!(ci->flags & CURSOR_SHOWING))
		return;

	icon = CopyIcon(ci->hCursor);
	if (!icon)
		return;

	if (GetIconInfo(icon, &ii)) 
	{
		POINT pos;

		if (window)
			ClientToScreen(window, &win_pos);

		pos.x = ci->ptScreenPos.x - (int)ii.xHotspot - win_pos.x;
		pos.y = ci->ptScreenPos.y - (int)ii.yHotspot - win_pos.y;

		DrawIcon(hdc, pos.x, pos.y, icon);

		DeleteObject(ii.hbmColor);
		DeleteObject(ii.hbmMask);
	}

	DestroyIcon(icon);
}

void CaptureScreen( long width, long height, bool capture_cursor = true, bool compatibility = true )
{
	HDC hdc = NULL, hdc_target = NULL, compatibility_hdc = NULL;
	HBITMAP bmp = NULL, old_bmp = NULL;
	CURSORINFO	CurInfo = {0};
	HWND window = NULL;  //ÆÁÄ»¾ä±ú

	if (compatibility) 
	{
		BITMAPINFO bi = {0};
		BITMAPINFOHEADER *bih = &bi.bmiHeader;
		bih->biSize     = sizeof(BITMAPINFOHEADER);
		bih->biBitCount = 32;
		bih->biWidth    = width;
		bih->biHeight   = height;
		bih->biPlanes   = 1;

		compatibility_hdc = CreateCompatibleDC(NULL);
		bmp = CreateDIBSection(compatibility_hdc, &bi,
			DIB_RGB_COLORS, (void**)&bih->biBitCount,
			NULL, 0);
		old_bmp = (HBITMAP)SelectObject(compatibility_hdc, bmp);
	}

	hdc = compatibility_hdc;//dc_capture_get_dc(capture);
	if (!hdc)
	{
		return;
	}

	hdc_target = GetDC(window);

	BOOL bl = BitBlt(hdc, 0, 0, width, height, hdc_target, 0, 0, SRCCOPY);

	ReleaseDC(NULL, hdc_target);

	if (capture_cursor) 
	{
		memset(&CurInfo, 0, sizeof(CURSORINFO));
		CurInfo.cbSize = sizeof(CURSORINFO);
		bool cursor_captured = GetCursorInfo(&CurInfo);

		if(cursor_captured)
			draw_cursor(CurInfo, hdc, window);
	}


	//ÊÍ·Å×ÊÔ´
	if (compatibility)
	{
		SelectObject(compatibility_hdc, old_bmp);
		DeleteDC(compatibility_hdc);
		DeleteObject(bmp);
	}
};

static BOOL CALLBACK enum_monitor(HMONITOR handle, HDC hdc, LPRECT rect,
	LPARAM param)
{
	struct monitor_info *monitor = (struct monitor_info *)param;

	if (monitor->cur_id == 0 || monitor->desired_id == monitor->cur_id) {
		monitor->handle = handle;
		monitor->rect = *rect;
		monitor->id   = monitor->cur_id;
	}

	return (monitor->desired_id > monitor->cur_id++);
}

#include <libTimestamp/Timestamp.h>

void EnumDisplay( void )
{
	struct monitor_info monitor = {0};
	uint32_t width, height;

	monitor.desired_id = 1;

	EnumDisplayMonitors(NULL, NULL, enum_monitor, (LPARAM)&monitor);
	
	while(true)
	{
		int iCount = 0;

		Timestamp_type	time;
		while( !time.isElapsed( 1000 * 1000 ) )
		{
			CaptureScreen(monitor.rect.right, monitor.rect.bottom);
			iCount ++;
		}

		printf("CaptureScreen Count:%d\r\n", iCount);
	}

	std::cin.get();
};

int main(int argc, char* argv[])
{
	EnumDisplay();

	for( int i = 0; i < 100000000; i ++)
	{
		while(true)
		{
			HLOCK hLock = MLock_Alloc();
			MLock_Lock(hLock);
			MLock_UnLock(hLock);
			MLock_Free(hLock);
			Sleep(100);
		}
	}

	CLock	Lock;
	int time = GetTickCount();
	for( int i = 0; i < 100000000; i ++)
	{
		Lock.Lock();
		Lock.UnLock();
	}
	printf("time:%d\r\n", GetTickCount() - time);

	std::cin.get();
	return 0;
}