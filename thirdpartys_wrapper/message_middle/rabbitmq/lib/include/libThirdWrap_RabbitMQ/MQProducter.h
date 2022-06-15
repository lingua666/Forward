
#ifndef __RABBITMQ_PRODUCTER_20200928134704_1444199361_H__
#define __RABBITMQ_PRODUCTER_20200928134704_1444199361_H__

#include <libThirdWrap_RabbitMQ/External.h>

namespace _rabbitmq_ {

	class MQProducter
	{
	public:
		MQProducter(void);

		~MQProducter(void);

		int Init(void);

		void Release(void);

		int Login(const _string_t& sIP, UInt16 uPort,
			const _string_t& UserName, const _string_t& sPassWord);

		int Logout(void);

		//��ͨģʽ
		int Open(int ChannelID, const _string_t& QueueName);

		//��ͨģʽ����
		int Publish(const char* c_szData, UInt32 uSize);

		//����ģʽ
		int OpenSubscribe(int ChannelID, const _string_t& ExChangeName);

		//����ģʽ����
		int PublishSubscribe(const char* c_szData, UInt32 uSize, const char* c_szRoutingKey);

		int Close(void);

		bool IsLogin(void) const;

		bool IsOpen(void) const;

	private:
		amqp_connection_state_t _Connection;
		CLock					_Lock;
		_string_t				_QEName;
		int						_iChannelID;
		bool					_isLogin;
	};
}





#endif//__EXTERNAL_20181220100004_1444199361_H__
