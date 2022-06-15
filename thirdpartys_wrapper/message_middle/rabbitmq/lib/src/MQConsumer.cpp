
#include <libThirdWrap_RabbitMQ/MQConsumer.h>

namespace _rabbitmq_ {

	MQConsumer::MQConsumer(void)
		: _Connection(NULL)
		, _isLogin(false)
		, _iChannelID(-1)
	{

	}

	MQConsumer::~MQConsumer(void)
	{
		Release();
	}

	int MQConsumer::Init(void)
	{
		//test
		LOG_Print_SetLog(libThirdWrap_RabbitMQ, MLog_GetAllLevel());
		//test

		return 1;
	}

	void MQConsumer::Release(void)
	{
		Logout();
	}

	int MQConsumer::Logout(void)
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

	int MQConsumer::Login(const _string_t& sIP, UInt16 uPort,
		const _string_t& UserName, const _string_t& sPassWord)
	{
		_Connection = amqp_new_connection();
		if (_Connection == NULL)
		{
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQConsumer::Login() amqp_new_connection Failed!!");
			return -1;
		}

		amqp_socket_t *socket = amqp_tcp_socket_new(_Connection);
		if (socket == NULL)
		{
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQConsumer::Login() amqp_tcp_socket_new Failed!!");
			return -1;
		}

		int rc = amqp_socket_open(socket, sIP.c_str(), uPort);
		if (rc != AMQP_STATUS_OK)
		{
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQConsumer::Login() amqp_socket_open(%s,%d) Failed!!",
				sIP.c_str(), uPort);
			return -1;
		}

		/**
		*login��broker
		*
		* ʹ��amqp_open_socket��amqp_set_sockfd�󣬵���amqp_login��ɵ�broker������
		*
		* \param [in] state ���Ӷ���
		* \param [in] vhost �����������ӵ�broker�������brokerĬ��Ϊ��/��
		* \param [in] channel_max ����ͨ�����������ƣ�0���������ƣ��Ϻõ�Ĭ��ֵ��AMQP_DEFAULT_MAX_CHANNELS
		* \param [in] frame_max ��·�ϵ�AMQP֡������С�����������д�����,��С4096��
		* ���2^31-1���Ϻõ�Ĭ��ֵ��131072 (128KB)����AMQP_DEFAULT_FRAME_SIZE
		* \param [in] heartbeat ����֡��broker������֮�����������0��ʾ��������
		* \param [in] sasl_method SASL method������֤broker��������SASL methods��ʵ��:
		*             -AMQP_SASL_METHOD_PLAIN���÷�����Ҫ������˳�������������
		*               const char* username, and const char* password.
		*             -AMQP_SASL_METHOD_EXTERNAL���÷�����Ҫ��������*/
		//��¼
		amqp_rpc_reply_t rpc_reply = amqp_login(
			_Connection, "/", AMQP_DEFAULT_MAX_CHANNELS, AMQP_DEFAULT_FRAME_SIZE,
			60/*AMQP_DEFAULT_HEARTBEAT*/, AMQP_SASL_METHOD_PLAIN, UserName.c_str(), sPassWord.c_str());
		if (rpc_reply.reply_type != AMQP_RESPONSE_NORMAL)
		{
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQConsumer::Login() amqp_login(%s,%s) Failed!!",
				UserName.c_str(), sPassWord.c_str());
			return -1;
		}

		_isLogin = true;
		return 1;
	}

	//��ͨģʽ
	int MQConsumer::Open(int ChannelID, const _string_t& QueueName)
	{
		if (!IsLogin() || IsOpen())
			return false;

		amqp_channel_open_ok_t *res =
			amqp_channel_open(_Connection, ChannelID);
		if (res == NULL)
		{
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQConsumer::Open() amqp_channel_open(%d) Failed!!", ChannelID);
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
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQConsumer::Open() amqp_queue_declare(%d,%s) Failed!!",
				ChannelID, QueueName.c_str());
			return -1;
		}

		_iChannelID = ChannelID;
		_QueueName = QueueName;
		return 1;
	}

	//��ͨ����ģʽ
	_string_t MQConsumer::ConsumeSync(void)
	{
		if (IsLogin() && IsOpen())
		{
			amqp_basic_consume_ok_t *msg = amqp_basic_consume(_Connection, _iChannelID,
				amqp_cstring_bytes(_QueueName.c_str()), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
			if (msg != NULL)
			{
				amqp_envelope_t envelope;
				amqp_consume_message(_Connection, &envelope, NULL, 0);
				_string_t sData = _string_t((char*)envelope.message.body.bytes, envelope.message.body.len);
				amqp_destroy_envelope(&envelope);
				return sData;
			}
			else
			{
				_ExChangeName = "";
				LOG_Print_Error(libThirdWrap_RabbitMQ, "MQConsumer::ConsumeSync() amqp_basic_consume(%d,%s) Failed!!",
					_iChannelID, _QueueName.c_str());
			}
		}

		return "";
	}

	int MQConsumer::OpenSubscribe(int ChannelID, const _string_t& QueueName,
		const _string_t& ExChangeName, const _string_t& RoutingKey)
	{
		if (!IsLogin() || IsOpen())
			return false;

		amqp_channel_open_ok_t *res =
			amqp_channel_open(_Connection, ChannelID);
		if (res == NULL)
		{
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQConsumer::OpenSubscribe() amqp_channel_open(%d) Failed!!",
				ChannelID);
			return -1;
		}

		//ͨ��������ƻ�channel���ó�confirmģʽ��ʵ����������broker���������֮�����ϢӦ����ƣ�
		//ȷ�������߷���������broker����������Ѿ��յ�
		//����Ч�ʵͣ���ʱ��������ʹ��confirmģʽ

		amqp_confirm_select(_Connection, ChannelID);  //��confirmģʽ����ͨ���ϴ�Publishȷ��

		amqp_rpc_reply_t rpc_res = amqp_get_rpc_reply(_Connection);
		if (rpc_res.reply.id != AMQP_CONFIRM_SELECT_OK_METHOD)
		{
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQConsumer::OpenSubscribe() amqp_confirm_select() Failed!!");
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
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQConsumer::OpenSubscribe() amqp_exchange_declare(%d,%s) Failed!!",
				ChannelID, ExChangeName.c_str());
			return -1;
		}

		amqp_queue_declare_ok_t *q = amqp_queue_declare(
			_Connection, ChannelID, amqp_cstring_bytes(QueueName.c_str()),
			/*passive*/ 0,
			/*durable*/ 0,
			/*exclusive*/ 0,
			/*auto_delete*/ 0, amqp_empty_table);
		if (q == NULL)
		{
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQConsumer::OpenSubscribe() amqp_queue_declare(%d,%s) Failed!!",
				ChannelID, QueueName.c_str());
			return -1;
		}

		amqp_queue_bind_ok_t *bind_ret = amqp_queue_bind(_Connection, ChannelID,
			amqp_cstring_bytes(QueueName.c_str()),
			amqp_cstring_bytes(ExChangeName.c_str()),
			amqp_cstring_bytes(RoutingKey.c_str()), amqp_empty_table);
		if (bind_ret == NULL)
		{
			LOG_Print_Error(libThirdWrap_RabbitMQ, "MQConsumer::OpenSubscribe() amqp_queue_bind(%d,%s,%s,%s) Failed!!",
				ChannelID, QueueName.c_str(), ExChangeName.c_str(), RoutingKey.c_str());
			return -1;
		}

		rpc_res = amqp_get_rpc_reply(_Connection);

		amqp_maybe_release_buffers_on_channel(_Connection, ChannelID);

		/*amqp_basic_qos_t qos = { 0 };
		qos.prefetch_size = 0;  // not implemented for RabbitMQ
		qos.prefetch_count = 1;
		qos.global = 1;

		if (qos.prefetch_count != 1) {
		printf("Attention: qos.prefetch_count = %d", qos.prefetch_count);
		}

		amqp_basic_qos_ok_t *r_qos = amqp_basic_qos(_Connection, ChannelID, qos.prefetch_size, qos.prefetch_count, qos.global);

		rpc_res = amqp_get_rpc_reply(connection_state_);
		amqp_maybe_release_buffers_on_channel(_Connection, ChannelID);*/

		_iChannelID = ChannelID;
		_QueueName = QueueName;
		_ExChangeName = ExChangeName;
		_RoutingKey = RoutingKey;
		return 1;
	}

	//��������ģʽ
	_string_t MQConsumer::ConsumeSubscribeSync(void)
	{
		return ConsumeSync();
	}

	int MQConsumer::Close(void)
	{
		if (IsOpen())
		{
			if (_ExChangeName != "")
			{
				amqp_queue_unbind_ok_t * unbind_ret = amqp_queue_unbind(_Connection,
					_iChannelID, amqp_cstring_bytes(_QueueName.c_str()),
					amqp_cstring_bytes(_ExChangeName.c_str()),
					amqp_cstring_bytes(_RoutingKey.c_str()), amqp_empty_table);
			}

			amqp_rpc_reply_t rpc_reply = amqp_channel_close(_Connection, _iChannelID, AMQP_REPLY_SUCCESS);
			if (rpc_reply.reply_type == AMQP_RESPONSE_NORMAL)
			{
				_iChannelID = -1;
				return 1;
			}
		}

		return -1;
	}


	bool MQConsumer::IsLogin(void) const
	{
		return _isLogin;
	}

	bool MQConsumer::IsOpen(void) const
	{
		return _iChannelID != -1 ? true : false;
	}

}

