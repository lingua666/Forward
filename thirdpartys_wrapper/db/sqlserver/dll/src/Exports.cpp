
#include <dllThirdWrap_SqlServer/Exports.h>
#include <libThirdWrap_SqlServer/RecordSet.hpp>
#include <libThirdWrap_SqlServer/SQLServerDB.h>
#include <libFoundation/modafx.h>

using namespace _sqlserver_;

MemPool_type	g_SQLServer_MemPool( sizeof(RecordSet) );

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172020,
	"45000001000W2016021700000000WZZZ00000000",
	"SQLite���ݿ�ģ��",
	"V1.0.0.1",
	"20160217",
	"")

DLL_LOG_DEF(SQLServer_Module)

/*****************************************************************
/*����˵����	�������ݿ������
/*����˵����	szHost:	������IP
/*				uPort:  �������˿ں�
				szUserName: �û���
				szPwd:		����
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLServer_Connect( const char* szHost, UInt16 uPort, 
															const char* szUserName, const char* szPwd,
															const char* szDataBase )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Connect():\n\t"
		"szHost:%s\n\tuPort:%d\n\tszUserName:%s\n\tszPwd:%s\n\t",
		szHost, uPort, szUserName, szPwd)

	return Singleton<SQLServer>::instance()->Connect(szHost, uPort, szUserName, szPwd, szDataBase);
}

/*****************************************************************
/*����˵����	�ر����ݿ�����
/*����˵����
/*				
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLServer_DisConnect( void )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_DisConnect():")

	Singleton<SQLServer>::instance()->Close();
	return true;
}

/*****************************************************************
/*����˵����	ִ��SQL���
/*����˵����	szSQL:	 SQL���
/*				
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLServer_ExecuteSQL( const char* szSQL )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_ExecuteSQL():\n\tszSQL:%s",szSQL)

	return Singleton<SQLServer>::instance()->ExecuteNoQuery(szSQL);
}

/*****************************************************************
/*����˵����	���ݿ��ѯ
/*����˵����	szSQL:	 SQL���
/*				
/*����ֵ��		��¼�����:    0: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HMRecordSet _CALLTYPE MSQLServer_Query_Open( const char* szSQL )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_Open():\n\tszSQL:%s",szSQL)

	RecordSet* p = g_SQLServer_MemPool.AllocObj<RecordSet>(Singleton<SQLServer>::instance()->GetHandle());
	if( p->Open(szSQL) != 1 )
	{
		g_SQLServer_MemPool.FreeObj(p);
		return NULL;
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_Close( HMRecordSet RSet )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_Close():\n\tRSet:%p",RSet)

	if( RSet != NULL )
	{
		g_SQLServer_MemPool.FreeObj(reinterpret_cast<RecordSet*>(RSet));
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_MoveFirst( HMRecordSet RSet )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_MoveFirst():\n\tRSet:%p",RSet)

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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_MoveNext( HMRecordSet RSet )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_MoveNext():\n\tRSet:%p",RSet)

	return reinterpret_cast<RecordSet*>(RSet)->MoveNext();
}


/*****************************************************************
/*����˵����	���һ����¼��
/*����˵����	RecordSet:	 ��¼�����
/*				
/*����ֵ��		1��	�ɹ� 
				<=0��������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_MoveLast( HMRecordSet RSet )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_MoveLast():\n\tRSet:%p",RSet)

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
EXTERN_C _SYMBOL_DLL_EXPORTS const char* _CALLTYPE MSQLServer_Query_GetValue( HMRecordSet RSet,
																		UInt32 u32Col )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_GetValue():\n\tRSet:%p",RSet)

	return reinterpret_cast<RecordSet*>(RSet)->GetValue(u32Col);
}

/*****************************************************************
/*����˵����	��ȡ��¼������
/*����˵����	RSet:	 ��¼�����
/*����ֵ��		����
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_RowCount( HMRecordSet RSet )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_RowCount():\n\tRSet:%p",RSet)

	return reinterpret_cast<RecordSet*>(RSet)->GetRow();
}


/*****************************************************************
/*����˵����	��ȡ��¼������
/*����˵����	RSet:	 ��¼�����
/*����ֵ��		����
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_ColumnCount( HMRecordSet RSet )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_ColumnCount():\n\tRSet:%p", RSet)

	return reinterpret_cast<RecordSet*>(RSet)->GetColumnsCount();
}

/*****************************************************************
/*����˵����	�����ת��Ϊint
/*����˵����	pValue:	 ֵָ��
/*����ֵ��		���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLServer_Query_ValueToInt( const char* pValue )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_ValueToInt():\n\tpValue:%s",pValue)

	return atoi(pValue);
}

/*****************************************************************
/*����˵����	�����ת��Ϊlong
/*����˵����	pValue:	 ֵָ��
/*����ֵ��		���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS long _CALLTYPE MSQLServer_Query_ValueToLong( const char* pValue )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_ValueToLong():\n\tpValue:%s",pValue)

	return atol(pValue);
}


/*****************************************************************
/*����˵����	�����ת��Ϊfloat
/*����˵����	pValue:	 ֵָ��
/*����ֵ��		���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS float _CALLTYPE MSQLServer_Query_ValueToFloat( const char* pValue )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_ValueToFloat():\n\tpValue:%s",pValue)

	return atof(pValue);
}


/*****************************************************************
/*����˵����	�����ת��Ϊlong
/*����˵����	pValue:	 ֵָ��
/*����ֵ��		���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS double _CALLTYPE MSQLServer_Query_ValueToDouble( const char* pValue )
{
	LOG_Print_Info(SQLServer_Module, "MSQLServer_Query_ValueToDouble():\n\tpValue:%s",pValue)

	return atof(pValue);
}
