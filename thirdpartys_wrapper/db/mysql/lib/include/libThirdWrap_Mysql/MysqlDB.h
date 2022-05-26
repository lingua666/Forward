#ifndef  __CUSTOM_MYSQLDB_H_DEF__
#define __CUSTOM_MYSQLDB_H_DEF__

#include <libThirdWrap_Mysql/AbstractDataBases.h>
#include <libFoundation/modafx.h>

namespace _mysql_{

	enum MySQLActionTypes 
	{
		ACTION_TYPE_CREATE = 0x01,
		ACTION_TYPE_DROP = 0x02,
		ACTION_TYPE_SELECT = 0x04,
		ACTION_TYPE_INSERT = 0x08,
		ACTION_TYPE_UPDATE = 0x10,
		ACTION_TYPE_DELETE = 0x20,
		ACTION_TYPE_ALL = 0x40
	};

	enum MySQLDBTableTypes 
	{ 
		DBTABLE_TYPE_DEFAULT,
		DBTABLE_TYPE_MYISAM,
		DBTABLE_TYPE_MRG_MYISAM,
		DBTABLE_TYPE_INNODB,
		DBTABLE_TYPE_MEMORY,
	};

	enum MySQLDBCharacters
	{
		DBTABLE_CHARACTERS_GB2312,
		DBTABLE_CHARACTERS_GBK,
		DBTABLE_CHARACTERS_UTF8
	};

class MySqlDB
{
public:
	MySqlDB( void );
	~MySqlDB( void );

	//��ȡMySqlDBʵ��
	static MySqlDB* GetInstance( void ) ;

	//��ʼ��
	bool Init( bool isReconnect = true, unsigned int uiConnectTimeOut = 60 );

	//�ͷ�
	void Release( void );

	//�������ݿ�
	bool Connect( const char* szHost, unsigned int uiPort, 
		const char* szName, const char* szPwd,
		const char* szDb, UInt8 u8Characters );

	//��������
	bool ReConnect( void );

	//�Ͽ�����
	bool Disconnect( void );

	//ִ�зǲ�ѯSQL���
	bool ExecuteNoQuery( const char* szSql );

	//�ı��û�
	bool ChangeUser( const char* szName, const char *szPwd, const char *szDb );
	
	//�������ݿ���
	bool SelectDB( const char* szDb );

	//��ʼ����
	bool Begin( void );

	//�����ύ
	bool Commit( void );

	//����ع�
	bool RollBack( void );

	//�������ݱ�
	bool CreateTable( const char* szTableName, SDBTABLEHEADER* lpDBTableHeader, unsigned int uiTableType = 0 );


///////////////////MYSQL�ڲ���Ϣ/////////////////
	bool AddUserGrants( const char* szHost, const char* szUserName, 
						const char* szPassword, const char* dzDBName, 
						unsigned long ulActionType );


	//���ޱ��ز���
	bool DeleteUserGrants( const char* szUserName, const char* szHost );
	

	//Э��汾��Ϣ
	unsigned int GetProtolVersion( void ) const;

	//��ȡ�������汾��Ϣ
	const char* GetServerVersion( void ) const;
	
	//��ȡ�ͻ��˰汾��Ϣ
	const char* GetClientVersion( void ) const;

	//��ȡ��ʹ���������͵��ַ���������������������
	const char* GetHostInfo( void ) const;

	//��ȡ������״̬
	const char* GetState( void ) const;

	//��ȡ������
	const char* GetErrorCode( void ) const;

	inline	MYSQL* GetMysqlHandle( void ) const
	{
		return _pMySQLConn;
	}

private:
	MYSQL* MySQLInit( void );
	MYSQL* MySQLConnect( const char* szHost, const char* szUserName,
		                const char* szPassword, const char* szDBName, unsigned int uiPort );
	int    MySQLQuery( const char* szSql );
	void   MySQLClose( void );

private:
	//һ������
	MYSQL* _pMySQLConn;

	//MySql�Ľ����
	MYSQL_RES* _pQueryResult;

	bool     _isAutoCommit;
	bool     _isConnect;
};

DECLARE_EXCEPTION(DataBaseException,CMyException)

} // namespace _mysql_

#endif

