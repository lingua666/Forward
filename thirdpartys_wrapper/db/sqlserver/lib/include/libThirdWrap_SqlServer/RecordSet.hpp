
#ifndef _CUSTOM_SQLSERVERDB_RECORDSET_H_DEF__
#define _CUSTOM_SQLSERVERDB_RECORDSET_H_DEF__

#include "External.h"

namespace _sqlserver_{

#define COLUMNSHEADER_SIZE	100
#define TYPE_BUF_SIZE		sizeof(int)

#define STRING_TO_ANY(type,convert)\
	namespace _##type##_{\
		inline type conv(const char* sz)\
		{\
			return convert(sz);\
		}\
	}

#define CONVERT(type,string)\
	_##type##_::conv(string)

STRING_TO_ANY(int,atoi)
STRING_TO_ANY(long,atol)
STRING_TO_ANY(double,atof)

class	RecordSet
{
public:
	enum ColumnType
	{
		FDT_BOOL,
		FDT_INT8,
		FDT_UINT8,
		FDT_INT16,
		FDT_UINT16,
		FDT_INT32,
		FDT_UINT32,
		FDT_INT64,
		FDT_UINT64,
		FDT_FLOAT,
		FDT_DOUBLE,
		FDT_STRING,
		FDT_BLOB,
		FDT_CLOB,
		FDT_DATE,
		FDT_TIME,
		FDT_TIMESTAMP,
		FDT_UNKNOWN
	};
public:
	explicit RecordSet( _ConnectionPtr pConn )
						:_pConn(pConn)
	{
		if( FAILED(_pRecordset.CreateInstance( _uuidof( Recordset ) )) )
		{
			printf("记录集对象指针实例化失败！\r\n");
		}
	}


	~RecordSet( void )
	{
		Close();
	}

	inline int	Open( const char* c_szSql )
	{
		try 
		{
			_pRecordset->Open(c_szSql, (IDispatch*)_pConn, adOpenStatic/*adOpenDynamic*/, adLockOptimistic, adCmdText);//打开数据库，执行SQL语句
		}
		catch (_com_error &e)
		{
			printf("sql error:%s\r\n", c_szSql);
			cout <<"SqlServer_Parse error1:" << e.Description()<<endl;
			return -1;
		}
		
		return 1;
	}

	inline void Close( void )
	{
		try 
		{
			_pRecordset->Close();
			_pRecordset = NULL;
		}
		catch (_com_error &e)
		{
			cout << "RecordSet::Close() error2:" << e.Description()<<endl;
		}
	}
	
	inline const char* GetValue( uint32_t uCol )
	{
		return (LPCTSTR)(_bstr_t)(_pRecordset->Fields->GetItem(uCol)->Value);
	}

	inline const char* GetColumnType( uint32_t uCol )
	{
		return (LPCTSTR)(_bstr_t)(_pRecordset->Fields->GetItem(uCol)->Type);
	}

	inline const char* GetColumnName( uint32_t uCol )
	{
		return (LPCTSTR)(_bstr_t)(_pRecordset->Fields->GetItem(uCol)->Name);
	}

	inline const uint32_t GetRow( void )
	{
		return _pRecordset->GetRecordCount();
	}

	inline const uint32_t	GetColumnsCount( void )
	{
		return _pRecordset->Fields->Count;
	}

	inline void	MoveFirst( void )
	{
		try 
		{
			_pRecordset->MoveFirst();
		}
		catch (_com_error &e)
		{
			cout << "RecordSet::MoveFirst() error2:" << e.Description()<<endl;
		}
	}

	inline int	MoveNext( void )
	{
		try 
		{
			_pRecordset->MoveNext();
			if( _pRecordset->adoEOF )
				return -1;
		}
		catch (_com_error &e)
		{
			cout << "RecordSet::MoveNext() error2:" << e.Description()<<endl;
			return -1;
		}

		return 1;
	}

	inline void	MoveLast( void )
	{
		try 
		{
			_pRecordset->MoveLast();
		}
		catch (_com_error &e)
		{
			cout << "RecordSet::MoveLast() error2:" << e.Description()<<endl;
		}
	}

private:
	
	inline bool is_open( void ) const
	{
		return _pConn != NULL;
	}
private:
	_ConnectionPtr	_pConn;
	_RecordsetPtr	_pRecordset;	//记录集对象指针，用来执行SQL语句并记录查询结果
};

} // namespace _sqlite_

#endif

