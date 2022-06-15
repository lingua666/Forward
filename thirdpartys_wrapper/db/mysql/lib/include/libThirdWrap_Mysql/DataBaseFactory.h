
#ifndef  _CUSTOM_DATABASEFACTORY_HEADER_DEF__
#define _CUSTOM_DATABASEFACTORY_HEADER_DEF__

#include <libThirdWrap_Mysql/AbstractDataBases.h>
#include <stdint.h>

namespace _mysql_{

class CDataBaseFactory
{
public:
	CDataBaseFactory( void );
	~CDataBaseFactory( void );

	//��ȡCMySqlʵ��
	static CDataBaseFactory* GetInstance( void );

	//��ʼ��
	bool Init( bool bIsReconnect = true, uint32_t uiConnectTimeOut = 60 );

	//�ͷ�
	void Release( void );

	//����
	bool Connect( const char* szdb, const char* szHost,
					const char* szName, const char* szPwd,
					uint32_t uiPort, UInt8 u8Characters );

	//��������
	bool ReConnect( void );

	//�Ͽ�����
	bool Disconnect( void );

	//��ʼ����
	bool Begin( void );

	//�����ύ
	bool Commit( void );

	//�������ݿ�
	bool CreateDB( const char* szDBName );

	//ɾ�����ݿ�
	bool DeleteDB( const char* szDBName );

	//������
	bool CreateTable( const char* szTableName, SDBTABLEHEADER* lpDBTableHeader,
					uint32_t uiTableType = 0);

	//ɾ����
	bool DeleteTable( const char* szTableName );

	//�����¼
	bool InsertRecord( const char* szTableName, DBTABLEFIELDS* lpDBTableFields = NULL, uint32_t uiFieldCount = 0,
					   const char* szwhere = NULL ); 

	//���¼�¼
	bool UpdateRecord( const char* szTableName, DBTABLEFIELDS* lpDBTableFields = NULL, uint32_t uiFieldCount = 0,
						const char* szwhere = NULL );

	//ɾ������
	bool DeleteRecord( const char* szTableName, const char* szwhere = NULL );

	//ִ��SQL���
	bool ExecuteNoQuerySql( const char* szSql );

	inline void BindDataBases(CAbstractDataBases*  lpCAbstractDB)
	{
		_pCAbstractDB = lpCAbstractDB;
	}

private:
	//ʵ��ָ��
	CAbstractDataBases*  _pCAbstractDB;
};

} // namespace _mysql_

#endif

