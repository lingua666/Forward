
#include <libEPoll_Net/UdpSocket.h>

#if defined( PLATFORM_OS_FAMILY_UNIX )

	namespace	_epoll_net_{

		namespace _udp_{

			typedef _sys_error_					error_code;

			UdpSocket::UdpSocket( const io_service& service )
				: _Service(service)
				, _Sock(INVALID_SOCKET)
				, _pRecv(NULL)
			{
				memset(&_tagInfo, 0, sizeof(_tagInfo));
			}

			UdpSocket::~UdpSocket( void )
			{
				//必须放在socket关闭之前
				//this->close();
			}

			UdpSocket::HSOCKET UdpSocket::Listen( UInt16 u16Port )
			{
				_Sock = APIWSACreate(SOCK_DGRAM, -1, -1);
				if( _Sock == SOCKET_ERROR )
					return INVALID_SOCKET;

				struct sockaddr_in addr = {0};
				addr.sin_family			= AF_INET;
				addr.sin_addr.s_addr	= htonl(INADDR_ANY);
				addr.sin_port			= htons( u16Port );

				if( ::bind( _Sock, (sockaddr *) &addr, sizeof(struct sockaddr_in) ) == -1 )
				{
					APIWSAClose(_Sock);
					return INVALID_SOCKET;
				}

				if( Bind(_Sock) == -1 )
				{
					APIWSAClose(_Sock);
					return INVALID_SOCKET;
				}

				return _Sock;
			}

			int UdpSocket::Bind( const HSOCKET& Sock )
			{
				if( Sock == INVALID_SOCKET )
					return -1;

				if( ReadInfo(Sock) == 1 &&
					_Service.bind((HANDLE)Sock) == 1 )
				{	
					_Sock = Sock;
					return 1;
				}
				else
				{
					APIWSAClose(Sock);
				}

				return -1;
			}

			int UdpSocket::Attach( const HSOCKET& Sock )
			{
				if( ReadInfo(Sock) != 1 )
				{
					APIWSAClose(Sock);
					return -1;
				}

				_Sock = Sock;
				return 1;
			}

			int UdpSocket::ReadInfo( const HSOCKET& Sock )
			{
				if( APIGetWSASocketLocalIP( Sock, _tagInfo._szLocal, sizeof(_tagInfo._szLocal) ) &&
					APIGetWSASocketLocalPort( Sock, _tagInfo._u16LocalPort ) )
				{
					return 1;
				}
				
				return -1;
			}

			int UdpSocket::async_read( const WSABUF& wBuf, const HFNComplete& handle )
			{
				tagOverlapped* p = _Service.Alloc();//_OverlappedManage.Alloc();
				p->_hfnPerform = function20_bind_Ex(APIWSARecvFrom);
				p->_hfnComplete = handle;
				p->_ADR._Sock = _Sock;
				p->_SWSAOverlapp.wsaOverlapped.uOP = IO_OP_READ;
				p->_SWSAOverlapp.wsaBuf.buf = wBuf.buf;
				p->_SWSAOverlapp.wsaBuf.len = wBuf.len;
				p->_Free_Overlapp = function1_handle(&io_service::OverlappedManage_type::Free, _Service.GetOverlappedManage().get());//function1_handle(&UdpSocket::FreeOverlapped, this);

				//增加1,统一close后删除
				tagOverlapped::AddRef(p);

				_Lock.Lock();
				int iRet = APIWSARecvFrom(_Sock, p->_SWSAOverlapp.wsaBuf.buf,
					p->_SWSAOverlapp.wsaBuf.len, (struct sockaddr_in*)p->_pAddr);
				if( iRet >= 0 )
				{	
					_pRecv = p;
					_Lock.UnLock();

					tagOverlapped::AddRef(p);
					CASAddAndFetch(&p->_ProcRef);
					p->_RealSize = iRet;
					_Service.post(p, 0);
				}
				else if( errno == EAGAIN )
				{
					if( _Service.post_read(_Sock, p) == -1 )
					{
						_Lock.UnLock();
						_Service.Free(p);
						return -1;
					}

					_pRecv = p;
					_Lock.UnLock();
				}
				else
				{
					_Lock.UnLock();
					_Service.Free(p);
					return -1;
				}

				return 1;
			}

			int UdpSocket::async_read( tagOverlapped* pOverlapped,
				char* pData, UInt32 u32Size )
			{
				pOverlapped->_SWSAOverlapp.wsaBuf.buf = pData;
				pOverlapped->_SWSAOverlapp.wsaBuf.len = u32Size;
				return async_read(pOverlapped);
			}

			int UdpSocket::async_read( tagOverlapped* pOverlapped )
			{
				//tagOverlapped::AddRef(pOverlapped);
				pOverlapped->_RealSize = -1;
				int iRet = APIWSARecvFrom(_Sock, pOverlapped->_SWSAOverlapp.wsaBuf.buf,
					pOverlapped->_SWSAOverlapp.wsaBuf.len, (struct sockaddr_in*)pOverlapped->_pAddr);
				if( iRet >= 0 )
				{
					tagOverlapped::AddRef(pOverlapped);
					CASAddAndFetch(&pOverlapped->_ProcRef);
					pOverlapped->_RealSize = iRet;							
					_Service.post(pOverlapped, 0);
				}
				else if( errno == EAGAIN )
				{
					if( _Service.post_read(_Sock, pOverlapped) == -1 )
					{
						//tagOverlapped::SubRef(pOverlapped);
						return -1;
					}
				}
				else
				{
					//tagOverlapped::SubRef(pOverlapped);
					return -1;
				}

				return 1;
			}

			int UdpSocket::async_write( struct sockaddr_in* pAddr, const WSABUF& wBuf, const HFNComplete& handle )
			{
				tagOverlapped* p = _OverlappedManage.Alloc();
				p->_hfnPerform = function20_bind_Ex(APIWSASendto);
				p->_hfnComplete = handle;
				p->_ADR._Sock = _Sock;
				p->_SWSAOverlapp.wsaOverlapped.uOP = IO_OP_WRITE;
				p->_SWSAOverlapp.wsaBuf.buf = wBuf.buf;
				p->_SWSAOverlapp.wsaBuf.len = wBuf.len;
				p->_Free_Overlapp = function1_handle(&UdpSocket::FreeOverlapped, this);
				memcpy((void*)p->_pAddr, (void*)pAddr, sizeof(struct sockaddr_in));

				int iRet = APIWSASendto(_Sock, p->_SWSAOverlapp.wsaBuf.buf,
					p->_SWSAOverlapp.wsaBuf.len, (struct sockaddr_in*)p->_pAddr);
				if( iRet >= 0 )
				{		
					tagIOData_INFO	IODataINFO = {0};
					IODataINFO._pOverlapped = p;
					p->_RealSize = iRet;

					while( p->_RealSize < wBuf.len )
					{
						iRet = APIWSASendto(_Sock, &p->_SWSAOverlapp.wsaBuf.buf[p->_RealSize],
							p->_SWSAOverlapp.wsaBuf.len - p->_RealSize, (struct sockaddr_in*)p->_pAddr);
						if( iRet == SOCKET_ERROR )
						{
							if(errno == EINTR)
							{
								_OverlappedManage.Free(p);
								return -1;
							}

							// 当socket是非阻塞时,如返回此错误,表示写缓冲队列已满,
							// 在这里做延时后再重试.
							if(errno == EAGAIN)
							{
								Sleep(0);
								continue;
							}
						}

						p->_RealSize += iRet;
					}

					_Service.process(&IODataINFO, p);
					//_Service.post(p, 0);
				}
				else if( errno == EAGAIN )
				{
					p->_RealSize = -1;
					if( _Service.post_write(_Sock, p) == -1 )
					{
						_OverlappedManage.Free(p);
						return -1;
					}
				}
				else
				{
					_OverlappedManage.Free(p);
					return -1;
				}

				return 1;
			}

			int UdpSocket::async_write( struct sockaddr_in* pAddr, tagOverlapped* pOverlapped )
			{
				tagOverlapped::AddRef(pOverlapped);
				pOverlapped->_RealSize = -1;
				memcpy(&pOverlapped->_pAddr, pAddr, sizeof(struct sockaddr_in));
				int iRet = APIWSASendto(_Sock, pOverlapped->_SWSAOverlapp.wsaBuf.buf,
					pOverlapped->_SWSAOverlapp.wsaBuf.len, (struct sockaddr_in*)pOverlapped->_pAddr);
				if( iRet >= 0 )
				{
					tagIOData_INFO	IODataINFO = {0};
					IODataINFO._pOverlapped = pOverlapped;
					pOverlapped->_RealSize = iRet;

					while( pOverlapped->_RealSize < pOverlapped->_SWSAOverlapp.wsaBuf.len )
					{
						iRet = APIWSASendto(_Sock, &pOverlapped->_SWSAOverlapp.wsaBuf.buf[pOverlapped->_RealSize],
							pOverlapped->_SWSAOverlapp.wsaBuf.len - pOverlapped->_RealSize, (struct sockaddr_in*)pOverlapped->_pAddr);
						if( iRet == SOCKET_ERROR )
						{
							if(errno == EINTR)
							{
								_OverlappedManage.Free(pOverlapped);
								return -1;
							}

							// 当socket是非阻塞时,如返回此错误,表示写缓冲队列已满,
							// 在这里做延时后再重试.
							if(errno == EAGAIN)
							{
								Sleep(0);
								continue;
							}
						}

						pOverlapped->_RealSize += iRet;
					}
					_Service.process(&IODataINFO, pOverlapped);
					//_Service.post(pOverlapped, 0);
				}
				else if( errno == EAGAIN )
				{
					pOverlapped->_RealSize = -1;
					if( _Service.post_write(_Sock, pOverlapped) == -1 )
					{
						tagOverlapped::SubRef(pOverlapped);
						return -1;
					}
				}
				else
				{
					tagOverlapped::SubRef(pOverlapped);
					return -1;
				}

				return 1;
			}

			void UdpSocket::close( bool isPassivity )
			{
				if( is_open() )
				{	
					HSOCKET s = _Sock;
					_Sock = INVALID_SOCKET;
					_Service.post_delete(s);
					APIWSAClose(s);

					_Lock.Lock();
					if( _pRecv != NULL )
					{
						tagOverlapped* p = _pRecv;
						_pRecv->_ADR._Sock = INVALID_SOCKET;
						_pRecv = NULL;
						_Lock.UnLock();

						_Service.CloseOverlapped(p);		
					}
					else
					{
						_Lock.UnLock();
					}
				}
			}

		} //namespace _tcp_

	} //_iocp_net_

#endif

