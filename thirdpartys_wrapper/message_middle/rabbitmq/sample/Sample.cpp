// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <assert.h>
#include <libThirdWrap_RabbitMQ/modafx.h>

static const int fixed_channel_id = 1;
static const char test_queue_name[] = "device_que";// "user_que";// "device_que";// "test_queue";
static const char test_exchange_name[] = "device";// "user";// "test_exchange";
static const char test_routingkey[] = "user.#";

#if defined(PLATFORM_OS_FAMILY_WINDOWS)
	#pragma comment(lib,"ws2_32.lib")
#endif

void MySyncProducter(void)
{
	amqp_connection_state_t connection_state_ = amqp_new_connection();

	amqp_socket_t *socket = amqp_tcp_socket_new(connection_state_);
	assert(socket);

	int rc = amqp_socket_open(socket, "192.168.1.45", /*15672*/AMQP_PROTOCOL_PORT);
	assert(rc == AMQP_STATUS_OK);

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
		connection_state_, "/", AMQP_DEFAULT_MAX_CHANNELS, AMQP_DEFAULT_FRAME_SIZE,
		AMQP_DEFAULT_HEARTBEAT, AMQP_SASL_METHOD_PLAIN, "guest", "guest");
	assert(rpc_reply.reply_type == AMQP_RESPONSE_NORMAL);

	//�ܵ�
	amqp_channel_open_ok_t *res =
		amqp_channel_open(connection_state_, fixed_channel_id);
	assert(res != NULL);

	//ͨ��������ƻ�channel���ó�confirmģʽ��ʵ����������broker���������֮�����ϢӦ����ƣ�
	//ȷ�������߷���������broker����������Ѿ��յ�
	//����Ч�ʵͣ���ʱ��������ʹ��confirmģʽ

	amqp_confirm_select(connection_state_, fixed_channel_id);  //��confirmģʽ����ͨ���ϴ�Publishȷ��

	amqp_queue_declare(
		connection_state_, fixed_channel_id, amqp_cstring_bytes(test_queue_name),
		/*passive*/ 0,
		/*durable*/ 0,
		/*exclusive*/ 0,
		/*auto_delete*/ 0, amqp_empty_table);

	while (1)
	{
		char message[64] = { '\0' };
		printf("please input the msg: ");
		char * ret_val = fgets(message, sizeof(message), stdin);
		amqp_bytes_t  message_bytes;
		message_bytes.len = strlen(ret_val) - 1;
		message_bytes.bytes = ret_val;

		//int iRet = amqp_basic_publish(connection_state_, fixed_channel_id, amqp_cstring_bytes("amq.fanout"), amqp_cstring_bytes(test_queue_name), 0, 0, NULL, message_bytes);
		int iRet = amqp_basic_publish(connection_state_, fixed_channel_id, amqp_cstring_bytes(""), amqp_cstring_bytes(test_queue_name), 0, 0, NULL, message_bytes);

		printf("send msg over(%d)!\r\n", message_bytes.len);
	}

	getchar();

	rpc_reply = amqp_channel_close(connection_state_, fixed_channel_id, AMQP_REPLY_SUCCESS);
	assert(rpc_reply.reply_type == AMQP_RESPONSE_NORMAL);
	rpc_reply = amqp_connection_close(connection_state_, AMQP_REPLY_SUCCESS);
	assert(rpc_reply.reply_type == AMQP_RESPONSE_NORMAL);

	rc = amqp_destroy_connection(connection_state_);
	assert(rc == AMQP_STATUS_OK);
}

void MySyncConsumer(void)
{
	amqp_connection_state_t connection_state_ = amqp_new_connection();

	amqp_socket_t *socket = amqp_tcp_socket_new(connection_state_);
	assert(socket);

	int rc = amqp_socket_open(socket, "192.168.1.188", /*15672*/AMQP_PROTOCOL_PORT);
	assert(rc == AMQP_STATUS_OK);

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
		connection_state_, "/", AMQP_DEFAULT_MAX_CHANNELS, AMQP_DEFAULT_FRAME_SIZE,
		AMQP_DEFAULT_HEARTBEAT, AMQP_SASL_METHOD_PLAIN, "guest", "guest");
	assert(rpc_reply.reply_type == AMQP_RESPONSE_NORMAL);

	//�ܵ�
	amqp_channel_open_ok_t *res =
		amqp_channel_open(connection_state_, fixed_channel_id);
	assert(res != NULL);

	/*amqp_queue_declare_ok_t *que_res = amqp_queue_declare(
	connection_state_, fixed_channel_id, amqp_cstring_bytes(test_queue_name),
	0,
	0,
	0,
	1, amqp_empty_table);
	assert(que_res != NULL);*/

	amqp_queue_declare_ok_t *que_res = amqp_queue_declare(
		connection_state_, fixed_channel_id, amqp_cstring_bytes(test_queue_name),
		0,
		0,
		0,
		0, amqp_empty_table);

	while (1) {
		amqp_basic_consume_ok_t *msg = amqp_basic_consume(connection_state_, fixed_channel_id, amqp_cstring_bytes(test_queue_name), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
		amqp_envelope_t envelope;
		amqp_consume_message(connection_state_, &envelope, NULL, 0);
		printf("The result is(%d) : %s\r\n", envelope.message.body.len, envelope.message.body.bytes);
		amqp_destroy_envelope(&envelope);
	}

	getchar();

	rpc_reply = amqp_channel_close(connection_state_, fixed_channel_id, AMQP_REPLY_SUCCESS);
	assert(rpc_reply.reply_type == AMQP_RESPONSE_NORMAL);

	rpc_reply = amqp_connection_close(connection_state_, AMQP_REPLY_SUCCESS);
	assert(rpc_reply.reply_type == AMQP_RESPONSE_NORMAL);

	rc = amqp_destroy_connection(connection_state_);
	assert(rc == AMQP_STATUS_OK);
}

void MyAsyncConsumer(void)
{
	amqp_connection_state_t connection_state_ = amqp_new_connection();

	amqp_socket_t *socket = amqp_tcp_socket_new(connection_state_);
	assert(socket);

	int rc = amqp_socket_open(socket, "192.168.1.188", /*15672*/AMQP_PROTOCOL_PORT);
	assert(rc == AMQP_STATUS_OK);

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
		connection_state_, "/", AMQP_DEFAULT_MAX_CHANNELS, AMQP_DEFAULT_FRAME_SIZE,
		AMQP_DEFAULT_HEARTBEAT, AMQP_SASL_METHOD_PLAIN, "guest", "guest");
	assert(rpc_reply.reply_type == AMQP_RESPONSE_NORMAL);

	//�ܵ�
	amqp_channel_open_ok_t *res =
		amqp_channel_open(connection_state_, fixed_channel_id);
	assert(res != NULL);

	/*amqp_queue_declare_ok_t *que_res = amqp_queue_declare(
	connection_state_, fixed_channel_id, amqp_cstring_bytes(test_queue_name),
	0,
	0,
	0,
	1, amqp_empty_table);
	assert(que_res != NULL);*/

	amqp_queue_declare_ok_t *que_res = amqp_queue_declare(
		connection_state_, fixed_channel_id, amqp_cstring_bytes(test_queue_name),
		0,
		0,
		0,
		0, amqp_empty_table);

	while (1) {

		amqp_basic_consume_ok_t *msg = amqp_basic_consume(connection_state_, fixed_channel_id, amqp_cstring_bytes(test_queue_name), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
		amqp_envelope_t envelope;
		amqp_consume_message(connection_state_, &envelope, NULL, 0);
		printf("The result is(%d) : %s\r\n", envelope.message.body.len, envelope.message.body.bytes);
		amqp_destroy_envelope(&envelope);
	}

	getchar();

	rpc_reply =
		amqp_connection_close(connection_state_, AMQP_REPLY_SUCCESS);
	assert(rpc_reply.reply_type == AMQP_RESPONSE_NORMAL);

	rc = amqp_destroy_connection(connection_state_);
	assert(rc == AMQP_STATUS_OK);
}

void MySubscribeProducter(void)
{
	amqp_connection_state_t connection_state_ = amqp_new_connection();

	amqp_socket_t *socket = amqp_tcp_socket_new(connection_state_);
	assert(socket);

	int rc = amqp_socket_open(socket, "192.168.1.188", /*15672*/AMQP_PROTOCOL_PORT);
	assert(rc == AMQP_STATUS_OK);

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
		connection_state_, "/", AMQP_DEFAULT_MAX_CHANNELS, AMQP_DEFAULT_FRAME_SIZE,
		AMQP_DEFAULT_HEARTBEAT, AMQP_SASL_METHOD_PLAIN, "guest", "guest");
	assert(rpc_reply.reply_type == AMQP_RESPONSE_NORMAL);

	//�ܵ�
	amqp_channel_open_ok_t *res =
		amqp_channel_open(connection_state_, fixed_channel_id);
	assert(res != NULL);

	//ͨ��������ƻ�channel���ó�confirmģʽ��ʵ����������broker���������֮�����ϢӦ����ƣ�
	//ȷ�������߷���������broker����������Ѿ��յ�
	//����Ч�ʵͣ���ʱ��������ʹ��confirmģʽ

	amqp_confirm_select(connection_state_, fixed_channel_id);  //��confirmģʽ����ͨ���ϴ�Publishȷ��

	amqp_rpc_reply_t rpc_res = amqp_get_rpc_reply(connection_state_);
	if (rpc_res.reply.id != AMQP_CONFIRM_SELECT_OK_METHOD)
	{
		printf("expecting AMQP_CONFIRM_SELECT_OK_METHOD, but get reply.id: %d", rpc_res.reply.id);
		return ;
	}

	amqp_exchange_declare_t declare = { 0 };
	declare.exchange = amqp_cstring_bytes(test_exchange_name);
	declare.type = amqp_cstring_bytes("topic");  // direct fanout topic
	declare.passive = 0;
	declare.durable = 0;
	declare.auto_delete = 0;
	declare.internal = 0;
	declare.nowait = 0;

	amqp_exchange_declare_ok_t *r = amqp_exchange_declare(connection_state_, fixed_channel_id, declare.exchange,
		declare.type, declare.passive, declare.durable, declare.auto_delete, declare.internal, amqp_empty_table);

	while (1)
	{
		char message[64] = { '\0' };
		printf("please input the msg: ");
		char * ret_val = fgets(message, sizeof(message), stdin);

		_foundation_::_string_t s = ret_val;
		_foundation_::_string_t s1 = s.gbk_utf8();
		amqp_bytes_t  message_bytes;
		message_bytes.len = s1.size();
		message_bytes.bytes = (void*)s1.c_str();

		/*amqp_bytes_t  message_bytes;
		message_bytes.len = strlen(ret_val) - 1;
		message_bytes.bytes = ret_val;*/

		int iRet = amqp_basic_publish(connection_state_, fixed_channel_id, declare.exchange, amqp_cstring_bytes("device.a"), 0, 0, NULL, message_bytes);

		printf("send msg over(%d)!\r\n", message_bytes.len);
	}

	getchar();

	rpc_reply = amqp_channel_close(connection_state_, fixed_channel_id, AMQP_REPLY_SUCCESS);
	assert(rpc_reply.reply_type == AMQP_RESPONSE_NORMAL);

	rpc_reply = amqp_connection_close(connection_state_, AMQP_REPLY_SUCCESS);
	assert(rpc_reply.reply_type == AMQP_RESPONSE_NORMAL);

	rc = amqp_destroy_connection(connection_state_);
	assert(rc == AMQP_STATUS_OK);
}

void MySubscribeConsumer(void)
{
	amqp_connection_state_t connection_state_ = amqp_new_connection();

	amqp_socket_t *socket = amqp_tcp_socket_new(connection_state_);
	assert(socket);

	int rc = amqp_socket_open(socket, "192.168.1.188", /*15672*/AMQP_PROTOCOL_PORT);
	assert(rc == AMQP_STATUS_OK);

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
		connection_state_, "/", AMQP_DEFAULT_MAX_CHANNELS, AMQP_DEFAULT_FRAME_SIZE,
		AMQP_DEFAULT_HEARTBEAT, AMQP_SASL_METHOD_PLAIN, "guest", "guest");
	if(rpc_reply.reply_type == AMQP_RESPONSE_NORMAL)
	assert(rpc_reply.reply_type == AMQP_RESPONSE_NORMAL);

	//�ܵ�
	amqp_channel_open_ok_t *res =
		amqp_channel_open(connection_state_, fixed_channel_id);
	assert(res != NULL);

	//ͨ��������ƻ�channel���ó�confirmģʽ��ʵ����������broker���������֮�����ϢӦ����ƣ�
	//ȷ�������߷���������broker����������Ѿ��յ�
	//����Ч�ʵͣ���ʱ��������ʹ��confirmģʽ

	amqp_confirm_select(connection_state_, fixed_channel_id);  //��confirmģʽ����ͨ���ϴ�Publishȷ��

	amqp_rpc_reply_t rpc_res = amqp_get_rpc_reply(connection_state_);
	if (rpc_res.reply.id != AMQP_CONFIRM_SELECT_OK_METHOD)
	{
		printf("expecting AMQP_CONFIRM_SELECT_OK_METHOD, but get reply.id: %d", rpc_res.reply.id);
		return ;
	}

	amqp_exchange_declare_t declare = { 0 };
	declare.exchange = amqp_cstring_bytes(test_exchange_name);
	declare.type = amqp_cstring_bytes("topic");  // direct fanout topic
	declare.passive = 0;
	declare.durable = 0;
	declare.auto_delete = 0;
	declare.internal = 0;
	declare.nowait = 0;

	amqp_exchange_declare_ok_t *r = amqp_exchange_declare(connection_state_, fixed_channel_id, declare.exchange,
		declare.type, declare.passive, declare.durable, declare.auto_delete, declare.internal, amqp_empty_table);

	amqp_queue_declare(
		connection_state_, fixed_channel_id, amqp_cstring_bytes(test_queue_name),
		/*passive*/ 0,
		/*durable*/ 0,
		/*exclusive*/ 0,
		/*auto_delete*/ 0, amqp_empty_table);

	amqp_queue_bind_t bind = { 0 };
	bind.queue = amqp_cstring_bytes(test_queue_name);
	bind.exchange = declare.exchange;
	bind.routing_key = amqp_cstring_bytes(test_routingkey);// amqp_cstring_bytes("routing_key");
	bind.nowait = 0;

	amqp_queue_bind_ok_t *bind_ret = amqp_queue_bind(connection_state_, fixed_channel_id, bind.queue, bind.exchange, bind.routing_key, amqp_empty_table);

	rpc_res = amqp_get_rpc_reply(connection_state_);

	amqp_maybe_release_buffers_on_channel(connection_state_, fixed_channel_id);

	/*amqp_basic_qos_t qos = { 0 };
	qos.prefetch_size = 0;  // not implemented for RabbitMQ
	qos.prefetch_count = 1;
	qos.global = 1;

	if (qos.prefetch_count != 1) {
		printf("Attention: qos.prefetch_count = %d", qos.prefetch_count);
	}

	amqp_basic_qos_ok_t *r_qos = amqp_basic_qos(connection_state_, fixed_channel_id, qos.prefetch_size, qos.prefetch_count, qos.global);

	rpc_res = amqp_get_rpc_reply(connection_state_);
	amqp_maybe_release_buffers_on_channel(connection_state_, fixed_channel_id);*/

	while (1) {

		amqp_basic_consume_ok_t *msg = amqp_basic_consume(connection_state_, fixed_channel_id, amqp_cstring_bytes(test_queue_name), amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
		amqp_envelope_t envelope;
		amqp_consume_message(connection_state_, &envelope, NULL, 0);
		printf("The result is(%d) : %s\r\n", envelope.message.body.len, envelope.message.body.bytes);
		amqp_destroy_envelope(&envelope);
	}

	amqp_queue_unbind_ok_t * unbind_ret = amqp_queue_unbind(connection_state_, fixed_channel_id, bind.queue, bind.exchange, bind.routing_key, amqp_empty_table);

	rpc_reply = amqp_channel_close(connection_state_, fixed_channel_id, AMQP_REPLY_SUCCESS);
	assert(rpc_reply.reply_type == AMQP_RESPONSE_NORMAL);

	rpc_reply = amqp_connection_close(connection_state_, AMQP_REPLY_SUCCESS);
	assert(rpc_reply.reply_type == AMQP_RESPONSE_NORMAL);

	rc = amqp_destroy_connection(connection_state_);
	assert(rc == AMQP_STATUS_OK);
}

void TestProducter(void)
{
	/*const char* IP = "192.168.1.45";
	const char* UserName = "nkyt";
	const char* PassWord = "Nkyt@2021";*/
	const char* IP = "192.168.1.188";
	const char* UserName = "guest";
	const char* PassWord = "guest";

	_rabbitmq_::MQProducter	 Producter;
	Producter.Init();

	//������
	printf("MQProducter Model\r\n");
	while (true)
	{
		if (Producter.Login(IP, AMQP_PROTOCOL_PORT, UserName, PassWord) == 1)
		{
			printf("Producter Login(%s:%d,%s,%s) Success!!\r\n", IP, AMQP_PROTOCOL_PORT, UserName, PassWord);
		}
		else
		{
			printf("Producter Login(%s:%d,%s,%s) Failed!!\r\n", IP, AMQP_PROTOCOL_PORT, UserName, PassWord);
		}

		char queue_name[] = "device_que";// "user_que";// "device_que";// "test_queue";
		const char exchange_name[] = "device";// "user";// "test_exchange";
		const char routingkey[] = "device.a";

		if (Producter.OpenSubscribe(fixed_channel_id, exchange_name) == 1)
		{
			printf("Open(%d,%s) Success!!\r\n", fixed_channel_id, exchange_name);

			int i = 1000000;
			//while (i--)
			while (true)
			{
				char message[64] = "1234567989";
				_foundation_::_string_t s = message;
				_foundation_::_string_t s1 = s;// s.gbk_utf8();

				if (Producter.PublishSubscribe(s1.c_str(), s1.size(), routingkey) == 1)
				{
					printf("Data(%d):%s\r\n", s1.size(), s1.c_str());
				}
				Sleep(1000);
			}
		}
		else
		{
			printf("Open(%d,%s) Failed!!\r\n", fixed_channel_id, exchange_name);
		}

		Producter.Logout();
	}

	Producter.Release();
}

void TestConsumer(void)
{
	const char* IP = "192.168.1.188";
	const char* UserName = "guest";
	const char* PassWord = "guest";

	printf("MQConsumer Model\r\n");

	_rabbitmq_::MQConsumer	 Consumer;
	Consumer.Init();

	while (true)
	{
		if (Consumer.Login(IP, AMQP_PROTOCOL_PORT, UserName, PassWord) == 1)
		{
			printf("MQConsumer Login(%s:%d,%s,%s) Success!!\r\n", IP, AMQP_PROTOCOL_PORT, UserName, PassWord);
		}
		else
		{
			printf("MQConsumer Login(%s:%d,%s,%s) Failed!!\r\n", IP, AMQP_PROTOCOL_PORT, UserName, PassWord);
		}

		char queue_name[] = "user_que";//"device_que";//  "device_que";// "test_queue";
		const char exchange_name[] = "user";//"device";//  "test_exchange";
		const char routingkey[] = "user.a";// "device.a";

		if (Consumer.OpenSubscribe(fixed_channel_id, queue_name, exchange_name, routingkey) == 1)
		{
			printf("Open(%d,%s) Success!!\r\n", fixed_channel_id, exchange_name);

			int iCount = 0;

			/*while (1)
			{
				_rabbitmq_::_string_t sw = Consumer.ConsumeSubscribeSync();
				//printf("Recvice Data(%d):%s\r\n", sw.size(), sw.c_str());
				iCount ++;
			}*/
		}
		else
		{
			printf("Open(%d,%s) Failed!!\r\n", fixed_channel_id, exchange_name);
		}

		Consumer.Logout();
	}

	Consumer.Release();
	
}

void Test(void)
{
	TestProducter();

	//TestConsumer();
}


int main(void) 
{
	Test();

	//MySyncProducter();
	//MySyncConsumer();

	//MySubscribeProducter();
	//MySubscribeConsumer();



	std::cin.get();
	return 0;
}
