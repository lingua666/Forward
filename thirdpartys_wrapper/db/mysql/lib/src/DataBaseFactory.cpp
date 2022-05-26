
#include <libThirdWrap_Mysql/DataBaseFactory.h>
#include <libThirdWrap_Mysql/MysqlDB.h>

namespace _mysql_{

#define TYPE_DBDESIGNVALID(handle) {	if(!handle)\
										{\
											return false;\
										}\
									}

#define TYPE_DBDESIGNVALID_NULL(handle) {	if(!handle)\
											{\
												return NULL;\
											}\
										}


CDataBaseFactory::CDataBaseFactory( void )
{
	_pCAbstractDB = NULL;
}

CDataBaseFactory::~CDataBaseFactory( void )
{
	Release();
}

//��ȡCMySqlʵ��
CDataBaseFactory* CDataBaseFactory::GetInstance( void )
{
	static CDataBaseFactory sDataBaseDesign;
	return &sDataBaseDesign;
}

//��ʼ��
bool CDataBaseFactory::Init( bool bIsReconnect, uint32_t uiConnectTimeOut )
{
	TYPE_DBDESIGNVALID(_pCAbstractDB);
	return _pCAbstractDB->Init(bIsReconnect,uiConnectTimeOut);
}

//�ͷ�
void CDataBaseFactory::Release( void )
{
	if(_pCAbstractDB != NULL)
	{
		_pCAbstractDB->Release();
		_pCAbstractDB = NULL;
	}
}

//����
bool CDataBaseFactory::Connect( const char* szdb,const char* szHost,
							  const char* szName, const char* szPwd,
							  uint32_t uiPort, UInt8 u8Characters )
{
	TYPE_DBDESIGNVALID(_pCAbstractDB);
	return _pCAbstractDB->Connect(szdb,szHost,szName,szPwd,uiPort,u8Characters);
}

//��������
bool CDataBaseFactory::ReConnect( void )
{
	TYPE_DBDESIGNVALID(_pCAbstractDB);
	return _pCAbstractDB->ReConnect();
}

//�Ͽ�����
bool CDataBaseFactory::Disconnect( void )
{
	TYPE_DBDESIGNVALID(_pCAbstractDB);
	return _pCAbstractDB->Disconnect();
}

//��ʼ����
bool CDataBaseFactory::Begin( void )
{
	TYPE_DBDESIGNVALID(_pCAbstractDB);
	return _pCAbstractDB->Begin();
}

//�����ύ
bool CDataBaseFactory::Commit( void )
{
	TYPE_DBDESIGNVALID(_pCAbstractDB);
	return _pCAbstractDB->Commit();
}

//�������ݿ�
bool CDataBaseFactory::CreateDB( const char* szDBName )
{
	TYPE_DBDESIGNVALID(_pCAbstractDB);
	TYPE_DBDESIGNVALID(szDBName);
	
	DeleteDB(szDBName);
	char szSql[100] = {0};
	strcpy(szSql,"CREATE DATABASE ");
	strcat(szSql,szDBName);
	return _pCAbstractDB->ExecuteNoQuery(szSql);
}

//ɾ�����ݿ�
bool CDataBaseFactory::DeleteDB( const char* szDBName )
{
	char szSql[100] = {0};
	strcpy(szSql,"DROP DATABASE ");
	strcat(szSql,szDBName);
	return _pCAbstractDB->ExecuteNoQuery(szSql);
}

//������
bool CDataBaseFactory::CreateTable( const char* szTableName, SDBTABLEHEADER* lpDBTableHeader,
								  uint32_t uiTableType )
{
	TYPE_DBDESIGNVALID(_pCAbstractDB);
	TYPE_DBDESIGNVALID(szTableName);
	TYPE_DBDESIGNVALID(lpDBTableHeader);
	if(lpDBTableHeader->SFieldCount <= 0)
		return false;

	return _pCAbstractDB->CreateTable(szTableName,lpDBTableHeader,uiTableType);
}

//ɾ����
bool CDataBaseFactory::DeleteTable( const char* szTableName )
{
	TYPE_DBDESIGNVALID(_pCAbstractDB);
	TYPE_DBDESIGNVALID(szTableName);

	char szSql[100] = {0};
	sprintf(szSql,"drop table %s",szTableName);
	return _pCAbstractDB->ExecuteNoQuery(szSql);
}

//�����¼
bool CDataBaseFactory::InsertRecord( const char* szTableName, DBTABLEFIELDS* lpDBTableFields, uint32_t uiFieldCount,
				  const char* szwhere )
{
	TYPE_DBDESIGNVALID_NULL(_pCAbstractDB);
	TYPE_DBDESIGNVALID_NULL(szTableName);
	char szSql[1024] = {0};
	if(lpDBTableFields == NULL || uiFieldCount <= 0)
		return false;

	strcpy(szSql,"INSERT INTO ");
	strcat(szSql,szTableName);
	strcat(szSql,"(");
	strcat(szSql,lpDBTableFields[0].cField_Name);
	for(int i = 1; i < uiFieldCount; i ++)
	{
		strcat(szSql,",");
		strcat(szSql,lpDBTableFields[i].cField_Name);
	}
	strcat(szSql,") VALUES( ");
	if(lpDBTableFields[0].clpFieldContent == NULL)
	{
		if(lpDBTableFields[0].uiFieldType == DBFIELD_TYPE_VARCHAR || \
			lpDBTableFields[0].uiFieldType == DBFIELD_TYPE_TEXT)
			strcat(szSql,"''");
		else
			strcat(szSql,"0");
	}
	else
	{
		if(lpDBTableFields[0].uiFieldType == DBFIELD_TYPE_VARCHAR || \
			lpDBTableFields[0].uiFieldType == DBFIELD_TYPE_TEXT)
		{
			strcat(szSql,"'");
			strcat(szSql,lpDBTableFields[0].clpFieldContent);
			strcat(szSql,"'");
		}
		else
			strcat(szSql,lpDBTableFields[0].clpFieldContent);
	}

	for(int i = 1; i < uiFieldCount; i ++)
	{
		strcat(szSql,",");
		if(lpDBTableFields[i].clpFieldContent == NULL)
		{
			if(lpDBTableFields[i].uiFieldType == DBFIELD_TYPE_VARCHAR || \
				lpDBTableFields[i].uiFieldType == DBFIELD_TYPE_TEXT)
			{
				strcat(szSql,"''");
			}
			else
				strcat(szSql,"0");
		}
		else
		{
			if(lpDBTableFields[i].uiFieldType == DBFIELD_TYPE_VARCHAR || \
				lpDBTableFields[i].uiFieldType == DBFIELD_TYPE_TEXT)
			{
				strcat(szSql,"'");
				strcat(szSql,lpDBTableFields[i].clpFieldContent);
				strcat(szSql,"'");
			}
			else
				strcat(szSql,lpDBTableFields[i].clpFieldContent);
		}
	}

	strcat(szSql,")");
	return _pCAbstractDB->ExecuteNoQuery(szSql);
}

//���¼�¼
bool CDataBaseFactory::UpdateRecord( const char* szTableName, DBTABLEFIELDS* lpDBTableFields, uint32_t uiFieldCount,
				  const char* szwhere )
{
	TYPE_DBDESIGNVALID_NULL(_pCAbstractDB);
	TYPE_DBDESIGNVALID_NULL(szTableName);

	char szSql[1024] = {0};
	if(lpDBTableFields == NULL || uiFieldCount <= 0)
		return false;

	strcpy(szSql,"UPDATE ");
	strcat(szSql,szTableName);
	strcat(szSql," SET ");

	strcat(szSql,lpDBTableFields[0].cField_Name);
	strcat(szSql," = ");
	if(lpDBTableFields[0].clpFieldContent == NULL)
	{
		if(lpDBTableFields[0].uiFieldType == DBFIELD_TYPE_VARCHAR || \
			lpDBTableFields[0].uiFieldType == DBFIELD_TYPE_TEXT)
				strcat(szSql,"''");
		else
			strcat(szSql,"0");
	}
	else
	{
		if(lpDBTableFields[0].uiFieldType == DBFIELD_TYPE_VARCHAR || \
			lpDBTableFields[0].uiFieldType == DBFIELD_TYPE_TEXT)
		{
			strcat(szSql,"'");
			strcat(szSql,lpDBTableFields[0].clpFieldContent);
			strcat(szSql,"'");
		}
		else
			strcat(szSql,lpDBTableFields[0].clpFieldContent);
	}

	for(int i = 1; i < uiFieldCount; i ++)
	{
		strcat(szSql,",");
		strcat(szSql,lpDBTableFields[i].cField_Name);
		strcat(szSql,"");
		strcat(szSql," = ");
		if(lpDBTableFields[i].clpFieldContent == NULL)
		{
			if(lpDBTableFields[i].uiFieldType == DBFIELD_TYPE_VARCHAR || \
				lpDBTableFields[i].uiFieldType == DBFIELD_TYPE_TEXT)
				strcat(szSql,"''");
			else
				strcat(szSql,"0");
		}
		else
		{
			if(lpDBTableFields[i].uiFieldType == DBFIELD_TYPE_VARCHAR || \
				lpDBTableFields[i].uiFieldType == DBFIELD_TYPE_TEXT)
			{
				strcat(szSql,"'");
				strcat(szSql,lpDBTableFields[i].clpFieldContent);
				strcat(szSql,"'");
			}
			else
				strcat(szSql,lpDBTableFields[i].clpFieldContent);
		}
	}

	if(szwhere != NULL)
	{
		strcat(szSql," where ");
		strcat(szSql,szwhere);
	}

	return _pCAbstractDB->ExecuteNoQuery(szSql);
}

//ɾ������
bool CDataBaseFactory::DeleteRecord( const char* szTableName, const char* szwhere )
{
	TYPE_DBDESIGNVALID_NULL(_pCAbstractDB);
	TYPE_DBDESIGNVALID_NULL(szTableName);

	char szSql[1024] = {0};
	strcpy(szSql,"DELETE FROM ");
	strcat(szSql,szTableName);
	if(szwhere != NULL)
	{
		strcat(szSql," where ");
		strcat(szSql,szwhere);
	}

	return _pCAbstractDB->ExecuteNoQuery(szSql);
}

//ִ��SQL���
bool CDataBaseFactory::ExecuteNoQuerySql( const char* szSql )
{
	TYPE_DBDESIGNVALID(_pCAbstractDB);
	TYPE_DBDESIGNVALID(szSql);

	return _pCAbstractDB->ExecuteNoQuery(szSql);
}

} // namespace _mysql_
