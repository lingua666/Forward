
#ifndef __IOCPFILE_OVERLAPPEDMANAGE_20151202110154_1449025314_H__
#define __IOCPFILE_OVERLAPPEDMANAGE_20151202110154_1449025314_H__

#include "CAsynIO.h"
#include "External.h"

namespace _iocp_file_{

	struct tagIOData_INFO;
	struct tagOverlapped;

	typedef	function_v20<void (tagIOData_INFO*)> function_handle;
	typedef	function_v10<void (tagOverlapped*)> function1_handle;

#pragma pack(push,1)
	struct tagIOWSABuf
	{
		UInt32	len;
		char*	buf;
	};

	typedef struct tagFileOverlapped
	{
		WSAOVERLAPPED		wsaOverlapped;
		tagIOWSABuf			wsaBuf;
	}FileOverlapped,LP_FileOverlapped;

	struct tagIOData_INFO
	{
		Int32	_ibytes_transferred;
		tagOverlapped*	_pOverlapped;
	};

#pragma pack(pop)
	
	//ÖØµþÀ©Õ¹
	struct tagOverlapped
	{
		tagFileOverlapped	_SWSAOverlapp;
		union {
			HANDLE			_Handle;
		}_ADR;
		long				_Ref;
		function_handle		_Func;
		function1_handle	_Free_Overlapp;

		static int AddRef( tagOverlapped* p )
		{
			return CASAddAndFetch(&p->_Ref);
		}

		static int SubRef( tagOverlapped* p )
		{
			return CASSubAndFetch(&p->_Ref);
		}
	} ;

	/*!
	* @class  OverlappedManage
	* @brief   OverlappedManage
	* @{
	*/
	template<int N = 0>
	class OverlappedManage
	{
	public:
		/*!
		* @function   OverlappedManage
		* @brief    
		* @return  
		*/
		OverlappedManage( void )
			: _Pool(sizeof(tagOverlapped) + N)
		{
			
		}

		/*!
		* @function   ~OverlappedManage
		* @brief    
		* @return  
		*/
		~OverlappedManage( void )
		{

		}

		tagOverlapped* Alloc( void )
		{
			tagOverlapped* p = reinterpret_cast<tagOverlapped*>(_Pool.AllocObj<tagOverlapped>());
			p->_ADR._Handle = 0;
			p->_Ref = 1;
			p->_SWSAOverlapp.wsaBuf.len = N;
			if( N != 0 )
			{
				p->_SWSAOverlapp.wsaBuf.buf = (char*)p + sizeof(tagOverlapped);
			}
			else
			{
				p->_SWSAOverlapp.wsaBuf.buf = NULL;
				p->_SWSAOverlapp.wsaBuf.len = 0;
			}
			
			memset(&p->_SWSAOverlapp.wsaOverlapped, 0, sizeof(p->_SWSAOverlapp.wsaOverlapped));
			return p;
		}

		void Free( tagOverlapped* pOverlapped )
		{
			if( pOverlapped != NULL )
			{
				if( pOverlapped->_Func )
				{
					pOverlapped->_Func.reset();
				}
				_Pool.FreeObj(pOverlapped);
			}
		}

		tagFileOverlapped* GetOverlapped( tagOverlapped* pOverlapped ) const
		{
			return &pOverlapped->_SWSAOverlapp;
		}

		void SetBuf( tagOverlapped* pOverlapped, char* c_szData, UInt32 u32Size )
		{
			pOverlapped->_SWSAOverlapp.wsaBuf.buf = c_szData;
			pOverlapped->_SWSAOverlapp.wsaBuf.len = u32Size;
		}

		void GetBuf( tagOverlapped* pOverlapped, tagIOWSABuf& out_wBuf ) const
		{
			out_wBuf.buf = pOverlapped->_SWSAOverlapp.wsaBuf.buf;
			out_wBuf.len = pOverlapped->_SWSAOverlapp.wsaBuf.len;
		}

	private:
		MemPool_type	_Pool;
	};

} //_iocp_net_

#endif//__OVERLAPPEDMANAGE_20151202110154_1449025314_H__
