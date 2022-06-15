
#include <libThirdWrap_RabbitMQ/MQProducter.h>

namespace _rabbitmq_ {

	MQProducter::MQProducter(void)
		: _Connection(NULL)
		, _isLogin(false)
		, _iChannelID(-1)
	{

	}

	MQProducter::~MQProducter(void)
	{
		Release();
	}

	int MQProducter::Init(void)
	{
		//test
		LOG_Print_SetLog(libThirdWrap_RabbitMQ, MLog_GetAllLevel());
		//test

		return 1;
	}

	void MQProducter::Release(void)
	{
		Logout();
	}

	int MQProducter::Logout(void)
	{
		_isLogin = false;
		Close();

		if (_Connection != NULL)
		{
			amqp_connection_close(_Connection, AMQP_REPLY_SUCCESS);
			amqp_destroy_connection(_Connection);

			/*amqp_rpc_reply_t rpc_reply = amqp_connection_close(_Connection, AMQP_REPLY_SUCCESS);
			if (rpc_reply.reply_type == AMQP_RESPONSE_NORMAL)
			{
				int rc = amqp_destroy_connection(_Connection);
			}*/

			_iChannelID = -1;
			_Connection = NULL;
		}

		return 1;
	}

	int MQProducter::Login(const _string_t& sIP, UInt16 uPort,
		const _string_t& UserName, const _string_t& sPassWord)
	{
		_Connection = amqp_new_connection();
		if (_Connection == NULL)
		{
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQProducter::Login() amqp_new_connection Failed!!");
			return -1;
		}

		amqp_rpc_reply_t rpc_reply;
		int rc;
		amqp_socket_t *socket = amqp_tcp_socket_new(_Connection);
		if (socket == NULL)
		{
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQProducter::Login() amqp_tcp_socket_new Failed!!");
			goto gt_faild;
		}

		rc = amqp_socket_open(socket, sIP.c_str(), uPort);
		if (rc != AMQP_STATUS_OK)
		{
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQProducter::Login() amqp_socket_open(%s,%d) Failed!!",
				sIP.c_str(), uPort);
			goto gt_faild;
		}

		/**
		*login到broker
		*
		* 使用amqp_open_socket和amqp_set_sockfd后，调用amqp_login完成到broker的连接
		*
		* \param [in] state 连接对象
		* \param [in] vhost 虚拟主机连接到broker，大多数broker默认为“/”
		* \param [in] channel_max 连接通道数量的限制，0代表无限制，较好的默认值是AMQP_DEFAULT_MAX_CHANNELS
		* \param [in] frame_max 线路上的AMQP帧的最大大小以请求代理进行此连接,最小4096，
		* 最大2^31-1，较好的默认值是131072 (128KB)或者AMQP_DEFAULT_FRAME_SIZE
		* \param [in] heartbeat 心跳帧到broker的请求之间的秒数。设0表示禁用心跳
		* \param [in] sasl_method SASL method用来验证broker，以下是SASL methods的实现:
		*             -AMQP_SASL_METHOD_PLAIN：该方法需要按如下顺序跟两个参数：
		*               const char* username, and const char* password.
		*             -AMQP_SASL_METHOD_EXTERNAL：该方法需要跟参数：*/
		//登录
		rpc_reply = amqp_login(
			_Connection, "/", AMQP_DEFAULT_MAX_CHANNELS, AMQP_DEFAULT_FRAME_SIZE,
			AMQP_DEFAULT_HEARTBEAT, AMQP_SASL_METHOD_PLAIN, UserName.c_str(), sPassWord.c_str());
		if (rpc_reply.reply_type != AMQP_RESPONSE_NORMAL)
		{
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQProducter::Login() amqp_login(%s,%s) Failed!!",
				UserName.c_str(), sPassWord.c_str());
			goto gt_faild;
		}

		_isLogin = true;
		return 1;

gt_faild:
		Close();
		return -1;
	}

	//普通模式
	int MQProducter::Open(int ChannelID, const _string_t& QueueName)
	{
		if (!IsLogin() || IsOpen())
			return false;

		amqp_channel_open_ok_t *res =
			amqp_channel_open(_Connection, ChannelID);
		if (res == NULL)
		{
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQProducter::Open() amqp_channel_open(%d) Failed!!", ChannelID);
			return -1;
		}

		amqp_confirm_select(_Connection, ChannelID);  //打开confirm模式，在通道上打开Publish确认
		amqp_rpc_reply_t rpc_res = amqp_get_rpc_reply(_Connection);
		if (rpc_res.reply.id != AMQP_CONFIRM_SELECT_OK_METHOD)
		{
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQProducter::Open() amqp_confirm_select() Failed!!");
			return -1;
		}

		amqp_queue_declare_ok_t *r = amqp_queue_declare(
			_Connection, ChannelID, amqp_cstring_bytes(QueueName.c_str()),
			/*passive*/ 0,
			/*durable*/ 0,
			/*exclusive*/ 0,
			/*auto_delete*/ 0, amqp_empty_table);
		if (r == NULL)
		{
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQProducter::Open() amqp_queue_declare(%d,%s) Failed!!",
				ChannelID, QueueName.c_str());
			return -1;
		}

		_iChannelID = ChannelID;
		_QEName = QueueName;
		return 1;
	}

	//普通模式发布
	int MQProducter::Publish(const char* c_szData, UInt32 uSize)
	{
		if (IsLogin() && IsOpen())
		{
			amqp_bytes_t  message_bytes;
			message_bytes.len = uSize;
			message_bytes.bytes = (void*)c_szData;

			_Lock.Lock();
			int iRet = amqp_basic_publish(_Connection, _iChannelID, amqp_cstring_bytes(""),
				amqp_cstring_bytes(_QEName.c_str()), 0, 0, NULL, message_bytes);
			_Lock.UnLock();

			if (iRet == AMQP_STATUS_OK)
				return 1;
			else
			{
				LOG_Print_Error(libThirdWrap_RabbitMQ, "MQProducter::Publish() amqp_basic_publish(%d,%s) Failed!!",
					_iChannelID, _QEName.c_str());
			}
		}

		return -1;
	}

	//订阅模式
	int MQProducter::OpenSubscribe(int ChannelID, const _string_t& ExChangeName)
	{
		if (!IsLogin())
			return false;

		amqp_channel_open_ok_t *res =
			amqp_channel_open(_Connection, ChannelID);
		if (res == NULL)
		{
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQProducter::OpenSubscribe() amqp_channel_open(%d) Failed!!",
				ChannelID);
			return -1;
		}

		amqp_confirm_select(_Connection, ChannelID);  //打开confirm模式，在通道上打开Publish确认
		amqp_rpc_reply_t rpc_res = amqp_get_rpc_reply(_Connection);
		if (rpc_res.reply.id != AMQP_CONFIRM_SELECT_OK_METHOD)
		{
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQProducter::OpenSubscribe() amqp_confirm_select() Failed!!");
			return -1;
		}

		amqp_exchange_declare_t declare = { 0 };
		declare.exchange = amqp_cstring_bytes(ExChangeName.c_str());
		declare.type = amqp_cstring_bytes("topic");  // direct fanout topic
		declare.passive = 0;
		declare.durable = 0;
		declare.auto_delete = 0;
		declare.internal = 0;
		declare.nowait = 0;

		amqp_exchange_declare_ok_t *r = amqp_exchange_declare(_Connection, ChannelID, declare.exchange,
			declare.type, declare.passive, declare.durable, declare.auto_delete, declare.internal, amqp_empty_table);
		if (r == NULL)
		{
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQProducter::OpenSubscribe() amqp_exchange_declare(%d,%s) Failed!!",
				ChannelID, ExChangeName.c_str());
			return -1;
		}

		_iChannelID = ChannelID;
		_QEName = ExChangeName;
		return 1;
	}

	//订阅模式发布
	int MQProducter::PublishSubscribe(const char* c_szData, UInt32 uSize, const char* c_szRoutingKey)
	{
		int iRet = -1;
		if (IsLogin() && IsOpen())
		{
			amqp_bytes_t  message_bytes;
			message_bytes.len = uSize;
			message_bytes.bytes = (void*)c_szData;

			_Lock.Lock();
			iRet = amqp_basic_publish(_Connection, _iChannelID, amqp_cstring_bytes(_QEName.c_str()),
				amqp_cstring_bytes(c_szRoutingKey), 0, 0, NULL, message_bytes);
			_Lock.UnLock();

			if (iRet == AMQP_STATUS_OK)
				return 1;
			else
			{
				LOG_Print_Error(libThirdWrap_RabbitMQ, "MQProducter::PublishSubscribe(%d) amqp_basic_publish(%d,%s) Failed!!",
					iRet, _iChannelID, _QEName.c_str());
			}
		}

		return iRet;
	}

	int MQProducter::Close(void)
	{
		if (IsOpen())
		{
			amqp_rpc_reply_t rpc_reply = amqp_channel_close(_Connection, _iChannelID, AMQP_REPLY_SUCCESS);
			if (rpc_reply.reply_type == AMQP_RESPONSE_NORMAL)
			{
				_iChannelID = -1;
				return 1;
			}
		}

		return -1;
	}

	bool MQProducter::IsLogin(void) const
	{
		return _isLogin;
	}

	bool MQProducter::IsOpen(void) const
	{
		return _iChannelID != -1 ? true : false;
	}

}
