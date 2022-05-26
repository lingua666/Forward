
#include <libEPoll_Net/TcpSocket.h>

#if defined( PLATFORM_OS_FAMILY_UNIX )

	namespace	_epoll_net_{

		namespace _tcp_{

			typedef _sys_error_					error_code;

			void TcpSocket::CloseSocket(const HSOCKET& Sock)
			{
				APIWSAClose(Sock);
			}

			TcpSocket::TcpSocket( const io_service& service )
				: _Service(service)
				, _Sock(INVALID_SOCKET)
				, _pRecv(NULL)
			{
				memset(&_tagInfo, 0, sizeof(_tagInfo));
			}

			TcpSocket::~TcpSocket( void )
			{
				//必须放在socket关闭之前
				this->close();
			}

			TcpSocket::HSOCKET TcpSocket::Connect( const _string_type& sIP, UInt16 u16Port,
							int iSocketRecv, int iSocketSend )
			{
				_Sock = APIWSACreate(SOCK_STREAM, iSocketRecv, iSocketSend);
				if( _Sock == SOCKET_ERROR )
					return INVALID_SOCKET;

				if( APIWSAConnect(_Sock, sIP.c_str(), u16Port) == SOCKET_ERROR )
				{
					APIWSAClose(_Sock);
					return INVALID_SOCKET;
				}

				return _Sock;
			}

			int TcpSocket::Bind( const HSOCKET& Sock )
			{
				if( Sock == INVALID_SOCKET )
					return -1;

				if( ReadInfo(Sock) == 1 &&
					_Service.bind((HANDLE)Sock) == 1 )
				{
					if( _Sock != INVALID_SOCKET )
					{
						_Service.post_delete(_Sock);
					}

					_Sock = Sock;
					return 1;
				}
				else
				{
					APIWSAClose(Sock);
				}

				return -1;
			}

			int TcpSocket::Attach( const HSOCKET& Sock )
			{
				if( ReadInfo(Sock) != 1 )
				{
					APIWSAClose(Sock);
					return -1;
				}

				_Sock = Sock;
				return 1;
			}

			int TcpSocket::ReadInfo( const HSOCKET& Sock )
			{
				if( APIGetWSASocketLocalIP( Sock, _tagInfo._szLocal, sizeof(_tagInfo._szLocal) ) &&
					APIGetWSASocketLocalPort( Sock, _tagInfo._u16LocalPort ) &&
					APIGetWSASocketRemoteIP( Sock, _tagInfo._szRemote, sizeof(_tagInfo._szRemote) ) &&
					APIGetWSASocketRemotePort( Sock, _tagInfo._u16RemotePort ) )
				{
					return 1;
				}

				return -1;
			}

			int TcpSocket::async_read( const WSABUF& wBuf, const HFNComplete& handle )
			{
				tagOverlapped* p = _Service.Alloc();//_OverlappedManage.Alloc();
				p->_hfnPerform = function20_bind_Ex(APIWSARecv);
				p->_hfnComplete = handle;
				p->_ADR._Sock = _Sock;
				p->_SWSAOverlapp.wsaOverlapped.uOP = IO_OP_READ;
				p->_SWSAOverlapp.wsaBuf.buf = wBuf.buf;
				p->_SWSAOverlapp.wsaBuf.len = wBuf.len;
				p->_Free_Overlapp = function1_handle(&io_service::OverlappedManage_type::Free, _Service.GetOverlappedManage().get());//function1_handle(&TcpSocket::FreeOverlapped, this);
				p->_Offset = 0;

				//增加1,统一close后删除
				tagOverlapped::AddRef(p);

				_Lock.Lock();
				int iRet = APIWSARecv(_Sock, p->_SWSAOverlapp.wsaBuf.buf, p->_SWSAOverlapp.wsaBuf.len);
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

			int TcpSocket::async_read( tagOverlapped* pOverlapped,
				char* pData, UInt32 u32Size )
			{
				pOverlapped->_SWSAOverlapp.wsaBuf.buf = pData;
				pOverlapped->_SWSAOverlapp.wsaBuf.len = u32Size;
				return async_read(pOverlapped);
			}

			int TcpSocket::async_read( tagOverlapped* pOverlapped )
			{
				//tagOverlapped::AddRef(pOverlapped);
				pOverlapped->_RealSize = -1;
				int iRet = APIWSARecv(_Sock, pOverlapped->_SWSAOverlapp.wsaBuf.buf, pOverlapped->_SWSAOverlapp.wsaBuf.len);
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

			int TcpSocket::async_write( const WSABUF& wBuf, const HFNComplete& handle )
			{
				tagOverlapped* p = _OverlappedManage.Alloc();
				p->_hfnPerform = function20_bind_Ex(APIWSASend);
				p->_hfnComplete = handle;
				p->_ADR._Sock = _Sock;
				p->_SWSAOverlapp.wsaOverlapped.uOP = IO_OP_WRITE;
				p->_SWSAOverlapp.wsaBuf.buf = wBuf.buf;
				p->_SWSAOverlapp.wsaBuf.len = wBuf.len;
				p->_Free_Overlapp = function1_handle(&TcpSocket::FreeOverlapped, this);
				p->_RealSize = 0;
				p->_Offset = 0;

				int iRet = APIWSASend(_Sock, p->_SWSAOverlapp.wsaBuf.buf, p->_SWSAOverlapp.wsaBuf.len);
				if( iRet >= 0 )
				{	
					int iTime = 0;
					tagIOData_INFO	IODataINFO = {0};
					IODataINFO._pOverlapped = p;
					p->_RealSize += iRet;

					while( p->_RealSize < wBuf.len )
					{
						iRet = APIWSASend(_Sock, &p->_SWSAOverlapp.wsaBuf.buf[p->_RealSize], p->_SWSAOverlapp.wsaBuf.len - p->_RealSize);
						if( iRet == SOCKET_ERROR )
						{
							// 当socket是非阻塞时,如返回此错误,表示写缓冲队列已满,
							// 在这里做延时后再重试.
							if(errno == EAGAIN)
							{
								p->_Offset = p->_RealSize;
								goto gt_post;
							}
							else
							{
								LOG_Print_Error(libEPoll_Net, "TcpSocket::async_write(%d,%d,%d) APIWSASend Failed, Close", errno, p->_RealSize, p->_SWSAOverlapp.wsaBuf.len);
								_OverlappedManage.Free(p);
								return -1;
							}
						}

						p->_RealSize += iRet;
					}
					
					_Service.process(&IODataINFO, p);
				}
				else if( errno == EAGAIN )
				{
gt_post:
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

			int TcpSocket::async_write( tagOverlapped* pOverlapped )
			{
				tagOverlapped::AddRef(pOverlapped);
				pOverlapped->_RealSize = -1;
				pOverlapped->_Offset = 0;

				int iRet = APIWSASend(_Sock, pOverlapped->_SWSAOverlapp.wsaBuf.buf, pOverlapped->_SWSAOverlapp.wsaBuf.len);
				if( iRet >= 0 )
				{
					int iTime = 0;
					tagIOData_INFO	IODataINFO = {0};
					IODataINFO._pOverlapped = pOverlapped;
					pOverlapped->_RealSize = iRet;

					while( pOverlapped->_RealSize < pOverlapped->_SWSAOverlapp.wsaBuf.len )
					{
						iRet = APIWSASend(_Sock, &pOverlapped->_SWSAOverlapp.wsaBuf.buf[pOverlapped->_RealSize], pOverlapped->_SWSAOverlapp.wsaBuf.len - pOverlapped->_RealSize);
						if( iRet == SOCKET_ERROR )
						{
							// 当socket是非阻塞时,如返回此错误,表示写缓冲队列已满,
							// 在这里做延时后再重试.
							if(errno == EAGAIN)
							{
								pOverlapped->_Offset = pOverlapped->_RealSize;
								goto gt_post;
							}
							else
							{
								LOG_Print_Error(libEPoll_Net, "TcpSocket::async_write11(%d,%d,%d) APIWSASend Failed, Close", errno, pOverlapped->_RealSize, pOverlapped->_SWSAOverlapp.wsaBuf.len);
								_OverlappedManage.Free(pOverlapped);
								return -1;
							}
						}

						pOverlapped->_RealSize += iRet;
					}
					_Service.process(&IODataINFO, pOverlapped);
					//_Service.post(pOverlapped, 0);
				}
				else if( errno == EAGAIN )
				{
gt_post:
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

			void TcpSocket::close( bool isPassivity )
			{
				if( is_open() )
				{	
					HSOCKET s = _Sock;
					_Sock = INVALID_SOCKET;
				
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
					APIWSAShutDown(s);
					_Service.post_delete(s);
					APIWSAClose(s);
				}
			}

		} //namespace _tcp_

	} //_iocp_net_

#endif

