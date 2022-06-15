
#ifndef __RABBITMQ_CONSUMER_20200928134704_1444199361_H__
#define __RABBITMQ_CONSUMER_20200928134704_1444199361_H__

#include <libThirdWrap_RabbitMQ/External.h>

namespace _rabbitmq_ {

	class MQConsumer
	{
	public:
		MQConsumer(void);

		~MQConsumer(void);

		int Init(void);

		void Release(void);

		int Login(const _string_t& sIP, UInt16 uPort,
			const _string_t& UserName, const _string_t& sPassWord);

		int Logout(void);

		//普通模式
		int Open(int ChannelID, const _string_t& QueueName);

		//普通消费模式
		_string_t ConsumeSync(void);

		int OpenSubscribe(int ChannelID, const _string_t& QueueName,
			const _string_t& ExChangeName, const _string_t& RoutingKey);

		//订阅消费模式
		_string_t ConsumeSubscribeSync(void);

		int Close(void);

		bool IsLogin(void) const;

		bool IsOpen(void) const;

	private:
		amqp_connection_state_t _Connection;
		_string_t				_QueueName;
		_string_t				_ExChangeName;
		_string_t				_RoutingKey;
		int						_iChannelID;
		bool					_isLogin;
	};
}





#endif//__EXTERNAL_20181220100004_1444199361_H__
