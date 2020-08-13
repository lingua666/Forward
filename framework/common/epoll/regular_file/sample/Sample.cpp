// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <libIOCP_File/FileIO.h>
#include <libFoundation/modafx.h>

#include <sys/eventfd.h>
#include <sys/syscall.h>      /* for __NR_* definitions */
#include <linux/aio_abi.h>    /* for AIO types and constants */
#include <fcntl.h>            /* O_RDWR */
#define TEST_FILE   "aio_test_file"
#define TEST_FILE_SIZE  (128 * 1024)
#define NUM_EVENTS  128
#define ALIGN_SIZE  512
#define RD_WR_SIZE  1024




//#include "TestXml.h"

//using namespace _proto_;


inline int io_setup(unsigned nr, aio_context_t *ctxp)
{
	return syscall(__NR_io_setup, nr, ctxp);
}
inline int io_submit(aio_context_t ctx, long nr,  struct iocb **iocbpp)
{
	return syscall(__NR_io_submit, ctx, nr, iocbpp);
}
inline int io_getevents(aio_context_t ctx, long min_nr, long max_nr,
struct io_event *events, struct timespec *timeout)
{
	return syscall(__NR_io_getevents, ctx, min_nr, max_nr, events, timeout);
}
inline int io_destroy(aio_context_t ctx)
{
	return syscall(__NR_io_destroy, ctx);
}
inline int eventfd2(unsigned int initval, int flags)
{
	return syscall(__NR_eventfd2, initval, flags);
}

struct custom_iocb
{
	struct iocb iocb;
	int nth_request;
};

typedef void io_callback_t(aio_context_t ctx, struct iocb *iocb, long res, long res2);

void aio_callback(aio_context_t ctx, struct iocb *iocb, long res, long res2)
{
	struct custom_iocb *iocbp = (struct custom_iocb *)iocb;
	printf("nth_request: %d, request_type: %s, offset: %lld, length: %lu, res: %ld, res2: %ldn",
		iocbp->nth_request, (iocb->aio_lio_opcode == IOCB_CMD_PREAD) ? "READ" : "WRITE",
		iocb->aio_offset, iocb->aio_nbytes, res, res2);
}

void aio_Test( void )
{
	int efd, fd, epfd;
	aio_context_t ctx;
	struct timespec tms;
	struct io_event events[NUM_EVENTS];
	struct custom_iocb iocbs[NUM_EVENTS];
	struct iocb *iocbps[NUM_EVENTS];
	struct custom_iocb *iocbp;
	int i, j, r;
	void *buf;
	void *aio_buf;
	efd = eventfd2(0, O_NONBLOCK | O_CLOEXEC);
	if (efd == -1) 
	{
		perror("eventfd2");
		return ;
	}
	fd = open(TEST_FILE, O_RDWR | O_CREAT /*| O_DIRECT*/, 0644);
	if (fd == -1) 
	{
		perror("open");
		return ;
	}
	ftruncate(fd, TEST_FILE_SIZE);

	ctx = 0;
	if (io_setup(NUM_EVENTS, &ctx)) 
	{
		perror("io_setup");
		return ;
	}
	if (posix_memalign(&buf, ALIGN_SIZE, RD_WR_SIZE * NUM_EVENTS)) {
		perror("posix_memalign");
		return ;
	}
	printf("buf: %pn", buf);
	for (i = 0, iocbp = iocbs; i < NUM_EVENTS; ++i, ++iocbp) 
	{
		aio_buf = (void *)((char *)buf + (i*RD_WR_SIZE));
		memset(aio_buf, 0, RD_WR_SIZE);
		//io_prep_pread(&iocbp->iocb, fd, buf, RD_WR_SIZE, i * RD_WR_SIZE);
		iocbp->iocb.aio_fildes = fd;
		iocbp->iocb.aio_lio_opcode = IOCB_CMD_PREAD;
		iocbp->iocb.aio_buf = (uint64_t)aio_buf;
		iocbp->iocb.aio_offset = i * RD_WR_SIZE;
		iocbp->iocb.aio_nbytes = RD_WR_SIZE;
		//io_set_eventfd(&iocbp->iocb, efd);
		iocbp->iocb.aio_flags = IOCB_FLAG_RESFD;
		iocbp->iocb.aio_resfd = efd;

		//io_set_callback(&iocbp->iocb, aio_callback);
		iocbp->iocb.aio_data = (__u64)aio_callback;
		iocbp->nth_request = i + 1;
		iocbps[i] = &iocbp->iocb;
	}

	int in = io_submit(ctx, NUM_EVENTS, iocbps);
	if ( in != NUM_EVENTS) 
	{
		printf("io_submit errno:%d, in:%d\r\n", errno, in);
		return ;
	}
}

_iocp_file_::io_service	g_io_service;
char g_szBuf[4000] = {0};

void ReadData(_iocp_file_::tagIOData_INFO* Data)
{

}

void WriteData(_iocp_file_::tagIOData_INFO* Data)
{

}


int main(int argc, char* argv[])
{
	/*HANDLE hSrcFile = CreateFile("D:\\111_mo - 副本.h264",//"D:\\111_mo.h264",
				GENERIC_READ, FILE_SHARE_READ, 
				NULL, OPEN_ALWAYS, FILE_FLAG_OVERLAPPED, 
				NULL);  

	if(hSrcFile == INVALID_HANDLE_VALUE)
	{

	}*/

	HANDLE hDstFile = CreateFile("D:\\111.txt",
				GENERIC_WRITE, 0, 
				NULL, CREATE_ALWAYS,
				FILE_FLAG_OVERLAPPED, NULL);
	if( hDstFile == INVALID_HANDLE_VALUE )
	{
		printf("创建文件失败!!!\r\n");
	}

	g_io_service.open();
	_thread_::ThreadWrap thd = (function20_bind(&_iocp_file_::io_service::run, &g_io_service));
	thd.wait_run();

	_iocp_file_::FileIO	File(g_io_service);
	//File.Bind(hSrcFile);
	File.Bind(hDstFile);


	File.async_write("1234567890123456789012345678901234567890",
		strlen("1234567890123456789012345678901234567890"), WriteData);

//	File.async_read(g_szBuf, 1024, ReadData);

	std::cin.get();
	CloseHandle(hDstFile);
	std::cin.get();
	return 0;
}