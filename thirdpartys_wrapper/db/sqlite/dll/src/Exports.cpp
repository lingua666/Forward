
#include <dllThirdWrap_Sqlite/Exports.h>
#include <libThirdWrap_Sqlite/RecordSet.hpp>
#include <libThirdWrap_Sqlite/SQLiteDB.h>
#include <libFoundation/modafx.h>

using namespace _sqlite_;

MemPool_type	g_SQLite_MemPool( sizeof(RecordSet) );

DLL_INIT_DEF(g_pTagTModuleInfoRecord_201602172020,
	"45000001000W2016021700000000WZZZ00000000",
	"SQLite���ݿ�ģ��",
	"V1.0.0.1",
	"20160217",
	"")

DLL_LOG_DEF(SQLite_Module)

/*****************************************************************
/*����˵����	����SQLite������
/*����˵����	szHost:	 SQLite������IP
/*				u16Port: SQLite�������˿ں�
				szUserName: �û���
				szPwd:		����
				szDBName:   ���ݿ���
				u8Characters: �ַ���
							0: GB2312
							1: GBK
							2: UTF8
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLite_Connect( const char* szPath )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Connect():\n\tszPath:%s",szPath)

	return Singleton<SQLiteDB>::instance()->OpenDataBase(szPath);
}

/*****************************************************************
/*����˵����	�ر����ݿ�����
/*����˵����
/*				
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLite_DisConnect( void )
{
	LOG_Print_Info(SQLite_Module, "SQLite_DisConnect():")

	Singleton<SQLiteDB>::instance()->CloseDataBase();
	return true;
}

/*****************************************************************
/*����˵����	ִ��SQL���
/*����˵����	szSQL:	 SQL���
/*				
/*����ֵ��		true: �ɹ�   false: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLite_ExecuteSQL( const char* szSQL )
{
	LOG_Print_Info(SQLite_Module, "SQLite_ExecuteSQL():\n\tszSQL:%s",szSQL)

	return Singleton<SQLiteDB>::instance()->ExcuteSql(szSQL);
}

/*****************************************************************
/*����˵����	���ݿ��ѯ
/*����˵����	szSQL:	 SQL���
/*				
/*����ֵ��		��¼�����:    0: ʧ��
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS HMRecordSet _CALLTYPE MSQLite_Query_Open( const char* szSQL )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_Open():\n\tszSQL:%s",szSQL)

	RecordSet* p = g_SQLite_MemPool.AllocObj<RecordSet>(Singleton<SQLiteDB>::instance()->GetSQLiteHandle());
	if( p->Open(szSQL) != 1 )
	{
		g_SQLite_MemPool.FreeObj(p);
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_Close( HMRecordSet RSet )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_Close():\n\tRSet:%d",RSet)

	if( RSet != NULL )
	{
		g_SQLite_MemPool.FreeObj(reinterpret_cast<RecordSet*>(RSet));
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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_MoveFirst( HMRecordSet RSet )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_MoveFirst():\n\tRSet:%d",RSet)

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
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_MoveNext( HMRecordSet RSet )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_MoveNext():\n\tRSet:%d",RSet)

	return reinterpret_cast<RecordSet*>(RSet)->MoveNext();
}


/*****************************************************************
/*����˵����	���һ����¼��
/*����˵����	RecordSet:	 ��¼�����
/*				
/*����ֵ��		1��	�ɹ� 
				<=0��������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_MoveLast( HMRecordSet RSet )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_MoveLast():\n\tRSet:%d",RSet)

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
EXTERN_C _SYMBOL_DLL_EXPORTS const char* _CALLTYPE MSQLite_Query_GetValue( HMRecordSet RSet,
																		UInt32 u32Col )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_GetValue():\n\tRSet:%d",RSet)

	return reinterpret_cast<RecordSet*>(RSet)->GetValue(u32Col);
}

/*****************************************************************
/*����˵����	��ȡֵ
/*����˵����	RSet:	 ��¼�����
/*			c_szColName:�ֶ���
/*����ֵ��	1��	�ɹ�
<=0��������
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS const char* _CALLTYPE MSQLite_QueryN_GetValue(HMRecordSet RSet,
	const char* c_szColName)
{
	LOG_Print_Info(SQLite_Module, "MSQLite_QueryN_GetValue():\n\tRSet:%d", RSet);

	uint32_t uCount = reinterpret_cast<RecordSet*>(RSet)->GetColumnsCount();
	for (int i = 0; i < uCount; i ++)
	{
		if (strcmp(reinterpret_cast<RecordSet*>(RSet)->GetColumnName(i), c_szColName) == 0)
		{
			return reinterpret_cast<RecordSet*>(RSet)->GetValue(i);
		}
	}

	return NULL;
}

/*****************************************************************
/*����˵����	��ȡ��¼������
/*����˵����	RSet:	 ��¼�����
/*����ֵ��		����
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_RowCount( HMRecordSet RSet )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_RowCount():\n\tRSet:%d",RSet)

	return reinterpret_cast<RecordSet*>(RSet)->GetRow();
}


/*****************************************************************
/*����˵����	��ȡ��¼������
/*����˵����	RSet:	 ��¼�����
/*����ֵ��		����
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_ColumnCount( HMRecordSet RSet )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_ColumnCount():\n\tRSet:%d",RSet)

	return reinterpret_cast<RecordSet*>(RSet)->GetColumnsCount();
}

/*****************************************************************
/*����˵����	�����ת��Ϊint
/*����˵����	pValue:	 ֵָ��
/*����ֵ��		���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS int _CALLTYPE MSQLite_Query_ValueToInt( const char* pValue )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_ValueToInt():\n\tpValue:%s",pValue)

	return atoi(pValue);
}

/*****************************************************************
/*����˵����	�����ת��Ϊlong
/*����˵����	pValue:	 ֵָ��
/*����ֵ��		���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS long _CALLTYPE MSQLite_Query_ValueToLong( const char* pValue )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_ValueToLong():\n\tpValue:%s",pValue)

	return atol(pValue);
}


/*****************************************************************
/*����˵����	�����ת��Ϊfloat
/*����˵����	pValue:	 ֵָ��
/*����ֵ��		���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS float _CALLTYPE MSQLite_Query_ValueToFloat( const char* pValue )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_ValueToFloat():\n\tpValue:%s",pValue)

	return atof(pValue);
}


/*****************************************************************
/*����˵����	�����ת��Ϊlong
/*����˵����	pValue:	 ֵָ��
/*����ֵ��		���
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS double _CALLTYPE MSQLite_Query_ValueToDouble( const char* pValue )
{
	LOG_Print_Info(SQLite_Module, "SQLite_Query_ValueToDouble():\n\tpValue:%s",pValue)

	return atof(pValue);
}


/*****************************************************************
/*����˵����	��ʼ����
/*����˵����	
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLite_BeginTransaction( void )
{
	return Singleton<SQLiteDB>::instance()->Begin_Transaction();
}


/*****************************************************************
/*����˵����	�ύ����
/*����˵����	
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLite_CommitTransaction( void )
{
	return Singleton<SQLiteDB>::instance()->Commit_Transaction();
}


/*****************************************************************
/*����˵����	����ع�
/*����˵����	
/*����ֵ��		
*****************************************************************/
EXTERN_C _SYMBOL_DLL_EXPORTS bool _CALLTYPE MSQLite_RollBack( void )
{
	return Singleton<SQLiteDB>::instance()->RollBack();
}