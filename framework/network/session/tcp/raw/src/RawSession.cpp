
#include <libNetFrame_RawSession/RawSession.h>

namespace	_session_{

	namespace	_tcp_{

		RawSession::RawSession( const UInt32 uHeadSpace )
			: _iUseRef(0)
			, _StreamCtrl( REVBUF_SIZE, uHeadSpace )
		{

		}

		RawSession::~RawSession( void )
		{
			Close();
			if( _DestroyHandle )
			{
				_DestroyHandle();
			}
		}

		void RawSession::Close( void )
		{
			if( _Stream_ptr )
				_Stream_ptr->Close();
		}

		int	RawSession::Init( io_service& ioService, const _SOCKET_::HSOCKET& Socket )
		{
			_Stream_ptr = SockStream::Create(&ioService, 
				_StreamCtrl.GetBufSize(),
				REVBUF_SIZE);

			if( !_Stream_ptr || _Stream_ptr->Attach(Socket) != 1 )
				return -1;

			_Stream_ptr->Init();

			EVENT_REGISTER(_Stream_ptr.get(),&_StreamCtrl);
			EVENT_REGISTER(_Stream_ptr.get(),this);
			EVENT_REGISTER(&_StreamCtrl,this);
			EVENT_EXEC_ATTACH(_Stream_ptr.get(),&_StreamCtrl);
			return 1;
		}

		int RawSession::Send( const char* c_pData, UInt32 u32Size )
		{
			return _StreamCtrl.Send(c_pData, u32Size);
		}

		void	RawSession::HandleRecv( const STREAM_HANDLE& Stream, const StreamBuf_ptr& Buf_ptr )
		{
			EVENT_EXEC_READ(this, Buf_ptr);
		}

		void	RawSession::HandleClose( const STREAM_HANDLE& Stream )
		{
			EVENT_EXEC_CLOSE(this);
		}

		void	RawSession::HandleExcept( const STREAM_HANDLE& Stream, const char* strError )
		{
			EVENT_EXEC_EXCEPT(this, strError);
		}

	} //namespace	_tcp_

} //namespace	_server_