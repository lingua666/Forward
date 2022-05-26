
#include <dllThirdWrap_Mysql/Exports.h>
#include <libThirdWrap_Mysql/RecordSet.h>
#include <libThirdWrap_Mysql/MysqlDB.h>
#include <libFoundation/modafx.h>

using namespace _mysql_;

MemPool_type	g_MYSQL_MemPool( sizeof(RecordSet) );

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172024,
	"45000001000X2016021700000000WZZZ00000000",
	"MySQL���ݿ�ģ��",
	"V1.0.0.1",
	"20160217",
	"")

DLL_LOG_DEF(Mysql_Module)

/*****************************************************************
/*����˵����	��ʼ��MYSQL ���ýӿڱ�����Mysql_Connectǰ����)
/*����˵����
/*				
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_Init( bool isReconnect,
														UInt32 u32ConnectTimeOut )
{
	LOG_Print_Info(Mysql_Module,"Mysql_Init():\n\tisReconnect:%d\n\tu32ConnectTimeOut:%d",
				isReconnect, u32ConnectTimeOut)

	return Singleton<MySqlDB>::instance()->Init(isReconnect, u32ConnectTimeOut);
}

/*****************************************************************
/*����˵�����ͷ�MYSQL
/*����˵����
/*
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_Release(void)
{
	Singleton<MySqlDB>::instance()->Release();
	return true;
}

/*****************************************************************
/*����˵����	����MYSQL������
/*����˵����	szHost:	 MYSQL������IP
/*				u16Port: MYSQL�������˿ں�
				szUserName: �û���
				szPwd:		����
				szDBName:   ���ݿ���
				u8Characters: �ַ���
							0: GB2312
							1: GBK
							2: UTF8
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_Connect(
							const char* szHost, UInt16 u16Port,
							const char* szUserName, const char* szPwd,
							const char* szDBName, UInt8 u8Characters )
{
	LOG_Print_Info(Mysql_Module,
		"Mysql_Connect():\n\tszHost:%s\n\tu16Port:%d\n\tszUserName:%s\n\tszPwd:%s\n\tszDBName:%s\n\tu8Characters:%d",
		szHost, u16Port, szUserName, szPwd, szDBName, u8Characters)

	return Singleton<MySqlDB>::instance()->Connect(szHost, u16Port, szUserName,
											szPwd, szDBName, u8Characters );
}

/*****************************************************************
/*����˵����	�ر����ݿ�����
/*����˵����
/*				
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_DisConnect( void )
{
	LOG_Print_Info(Mysql_Module, "Mysql_DisConnect():")

	return Singleton<MySqlDB>::instance()->Disconnect();
}

/*****************************************************************
/*����˵����	�������ݿ�
/*����˵����	szDBName:	 ���ݿ���
/*				
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_CreateDB( const char* szDBName )
{
	LOG_Print_Info(Mysql_Module, "Mysql_CreateDB():\n\tszDBName:%s",szDBName)

	char szSql[100] = {0};
	strcpy(szSql,"CREATE DATABASE ");
	strcat(szSql,szDBName);
	return Singleton<MySqlDB>::instance()->ExecuteNoQuery(szSql);
}

/*****************************************************************
/*����˵����	ɾ�����ݿ�
/*����˵����	szDBName:	 ���ݿ���
/*				
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_DeleteDB( const char* szDBName )
{
	LOG_Print_Info(Mysql_Module, "Mysql_DeleteDB():\n\tszDBName:%s",szDBName)

	char szSql[100] = {0};
	strcpy(szSql,"DROP DATABASE ");
	strcat(szSql,szDBName);
	return Singleton<MySqlDB>::instance()->ExecuteNoQuery(szSql);
}

/*****************************************************************
/*����˵����	ִ��SQL���
/*����˵����	szSQL:	 SQL���
/*				
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE Mysql_ExecuteSQL( const char* szSQL )
{
	LOG_Print_Info(Mysql_Module, "Mysql_ExecuteSQL():\n\tszSQL:%s",szSQL)

	return Singleton<MySqlDB>::instance()->ExecuteNoQuery(szSQL);
}

/*****************************************************************
/*����˵����	���ݿ��ѯ
/*����˵����	szSQL:	 SQL���
/*				
/*����ֵ��		��¼�����:    0: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HMRecordSet _CALLTYPE Mysql_Query_Open( const char* szSQL )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_Open():\n\tszSQL:%s", szSQL)

	RecordSet* p = NULL;
	try
	{
		p = g_MYSQL_MemPool.AllocObj<RecordSet>(Singleton<MySqlDB>::instance()->GetMysqlHandle());
		if (p->Open(szSQL) != 1)
		{
			g_MYSQL_MemPool.FreeObj(p);
			p = NULL;
		}
	}
	catch (...)
	{
		printf("RecordSet Open Failed!!\r\n");
		g_MYSQL_MemPool.FreeObj(p);
		p = NULL;
	}

	return p;
}

/*****************************************************************
/*����˵����	�ͷż�¼����Դ  ���м��ڲ�ʹ�ü�¼����ʱ����øýӿڣ��ͷ���Դ��
/*����˵����	RecordSet:	 ��¼�����
/*				
/*����ֵ��		1��	�ɹ� 
				<=0��������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_Close( HMRecordSet RSet )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_Close():\n\tRSet:%d",RSet)

	if( RSet != NULL )
	{
		g_MYSQL_MemPool.FreeObj(reinterpret_cast<RecordSet*>(RSet));
	}
	return 1;
}

/*****************************************************************
/*����˵����	��һ����¼��
/*����˵����	RecordSet:	 ��¼�����
/*				
/*����ֵ��		1��	�ɹ� 
				<=0��������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_MoveFirst( HMRecordSet RSet )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_MoveFirst():\n\tRSet:%d",RSet)

	reinterpret_cast<RecordSet*>(RSet)->MoveFirst();
	return 1;
}


/*****************************************************************
/*����˵����	��һ����¼��
/*����˵����	RecordSet:	 ��¼�����
/*				
/*����ֵ��		1��	�ɹ� 
				<=0��������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_MoveNext( HMRecordSet RSet )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_MoveNext():\n\tRSet:%d",RSet)

	reinterpret_cast<RecordSet*>(RSet)->MoveNext();
	return 1;
}


/*****************************************************************
/*����˵����	���һ����¼��
/*����˵����	RecordSet:	 ��¼�����
/*				
/*����ֵ��		1��	�ɹ� 
				<=0��������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_MoveLast( HMRecordSet RSet )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_MoveLast():\n\tRSet:%d",RSet)

	reinterpret_cast<RecordSet*>(RSet)->MoveLast();
	return 1;
}


/*****************************************************************
/*����˵����	��ȡֵ
/*����˵����	RSet:	 ��¼�����
/*				u32Col:	 ����       ��0��ʼ
/*����ֵ��		1��	�ɹ� 
				<=0��������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS const char* _CALLTYPE Mysql_Query_GetValue( HMRecordSet RSet,
																		UInt32 u32Col )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_GetValue():\n\tRSet:%d\n\tu32Col:%d",RSet,u32Col)

	return reinterpret_cast<RecordSet*>(RSet)->GetValue(u32Col);
}

/*****************************************************************
/*����˵����	��ȡ��¼������
/*����˵����	RSet:	 ��¼�����
/*����ֵ��		����
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_RowCount( HMRecordSet RSet )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_RowCount():\n\tRSet:%d",RSet)

	return reinterpret_cast<RecordSet*>(RSet)->GetRow();
}


/*****************************************************************
/*����˵����	��ȡ��¼������
/*����˵����	RSet:	 ��¼�����
/*����ֵ��		����
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_ColumnCount( HMRecordSet RSet )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_ColumnCount():\n\tRSet:%d",RSet)

	return reinterpret_cast<RecordSet*>(RSet)->GetColumnsCount();
}

/*****************************************************************
/*����˵����	�����ת��Ϊint
/*����˵����	pValue:	 ֵָ��
/*����ֵ��		���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE Mysql_Query_ValueToInt( const char* pValue )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_ValueToInt():\n\tpValue:%d",pValue)

	return atoi(pValue);
}

/*****************************************************************
/*����˵����	�����ת��Ϊlong
/*����˵����	pValue:	 ֵָ��
/*����ֵ��		���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS long _CALLTYPE Mysql_Query_ValueToLong( const char* pValue )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_ValueToInt():\n\tpValue:%d",pValue)

	return atol(pValue);
}


/*****************************************************************
/*����˵����	�����ת��Ϊfloat
/*����˵����	pValue:	 ֵָ��
/*����ֵ��		���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS float _CALLTYPE Mysql_Query_ValueToFloat( const char* pValue )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_ValueToInt():\n\tpValue:%d",pValue)

	return atof(pValue);
}


/*****************************************************************
/*����˵����	�����ת��Ϊlong
/*����˵����	pValue:	 ֵָ��
/*����ֵ��		���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS double _CALLTYPE Mysql_Query_ValueToDouble( const char* pValue )
{
	LOG_Print_Info(Mysql_Module, "Mysql_Query_ValueToInt():\n\tpValue:%d",pValue)

	return atof(pValue);
}
