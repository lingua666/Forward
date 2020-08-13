// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//#include <windows.h>

#include <libNetFrame_Packet/Packet.h>

typedef Packet<PHead>	MediaNPacket;

#define NUMBER_DEF			10
MediaNPacket	_Pack(1470,100);

void TestNotify( const PKBuf_ptr& sptr, void* lParam )
{
	char sz[1024] = {0};
	int i = sptr->_uPayload;
	memcpy(sz,sptr->_Data,sptr->_uPayload);
	i ++;
}

int	MorePack( const char* pData, UInt16 u16Size )
{
	char sz[1024] = {0};
	int len = 0;
	PKBList_ptr sptr = _Pack.BigPackage(pData, u16Size);
	PKBList* plist = sptr.get();
	while(plist->size())
	{
		PKBuf_ptr dptr = plist->front();
		memcpy(&sz[len], dptr->_Data, dptr->_uPayload);
		len += dptr->_uPayload;
		plist->pop();
	}

	_Pack.SetFunction( function20_bind_Ex( TestNotify ) );

	char sz12[100] = {0};
	memcpy(sz12,sz, 14);
	memcpy(&sz12[14],sz, 14);

	_Pack.Parse(sz12, len + 4);
	_Pack.Parse(&sz12[len + 4], len - 4);

	_Pack.Parse(sz12, 1);
	_Pack.Parse(&sz12[1], 1);
	_Pack.Parse(&sz12[2], 2);

	_Pack.Parse(&sz12[4], len - 2 );
	_Pack.Parse(&sz12[len + 2], 1);
	_Pack.Parse(&sz12[len + 3], 1);
	_Pack.Parse(&sz12[len + 4], 4);
	_Pack.Parse(&sz12[len + 8], len - 8);



	_Pack.Parse(sz, len - 5);
	_Pack.Parse(&sz[len - 5], 5);


	return -1;
}

int	OnePack( const char* pData, UInt16 u16Size )
{
	PKBuf_ptr sptr = _Pack.Package(pData, u16Size);

	char sz[1024] = {0};
	int i = sptr->_uPayload;
	memcpy(sz,sptr->_Data,sptr->_uPayload);
	return -1;
}

UInt32	Send( const char* pData, UInt16 u16Size )
{
	int MaxPayload = _Pack.GetSendSize();
	u16Size > _Pack.GetSendSize() ? MorePack(pData, u16Size) \
		: OnePack(pData, u16Size);
	return 1;
}

int main(int argc, char* argv[])
{
	char r[NUMBER_DEF + 5] = {0};
	char sk[NUMBER_DEF] = {0};
	for(int i = 0; i < NUMBER_DEF; i ++)
	{
		sk[i] = i;
	}

	Send(sk, sizeof(sk));
	/*PKBuf_ptr sptr = _Pack.Package(sk,sizeof(sk));
	tagDBuf* p = sptr.get();
	memcpy(r, p->_Data, p->_uPayload);*/

	std::cin.get();
	return 0;
}