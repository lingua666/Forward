
#ifndef _CUSTOM_SQLITE_RECORDSET_H_DEF__
#define _CUSTOM_SQLITE_RECORDSET_H_DEF__

#include <libFoundation/modafx.h>
#include <sqlite3.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <vector>

namespace _sqlite_{

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


class ColumnsHeader
{
public:
	typedef	std::vector<_string_type>	ColumnsVec;

public:
	ColumnsHeader( void )
	{
		_ColumnsHeader.clear();
	}

	~ColumnsHeader( void )
	{
		size_t size = _ColumnsHeader.size();
		_ColumnsHeader.clear();
	}

	void Release( void )
	{
		_ColumnsHeader.clear();
	}

	inline void	AddColumns( const char* szName, int iType )
	{
		char szColumn[COLUMNSHEADER_SIZE + TYPE_BUF_SIZE] = {0};
		strcpy(szColumn,szName);
		size_t len = strlen(szColumn);
		szColumn[len] = 0;
		len ++;
		memcpy((char*)&szColumn[len], (char*)&iType, sizeof(iType));
		len += TYPE_BUF_SIZE;
		_ColumnsHeader.push_back(_string_type(szColumn,len));
	}

	inline const char*		GetName( uint32_t uCol )
	{
		if(uCol >= _ColumnsHeader.size())
			return NULL;
		
		return _ColumnsHeader.at(uCol).c_str();
	}

	inline const int GetType( uint32_t uCol ) const
	{
		if(uCol >= _ColumnsHeader.size())
			return NULL;
		
		int iType = 0;
		size_t len = _ColumnsHeader.at(uCol).length();
		memcpy((char*)&iType, (char*)&(_ColumnsHeader.at(uCol).c_str()[len - TYPE_BUF_SIZE]), sizeof(iType));
		return iType;
	}

	inline const size_t GetCount( void ) const 
	{
		return _ColumnsHeader.size();
	}

public:
	ColumnsVec	_ColumnsHeader;
};

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
	explicit RecordSet( sqlite3* pConn )
						:_pConn(pConn)
						,_pResult(NULL)
						,_uRowCount(0)
						,_uColumnCount(0)
						,_uCurrentRow(0)
	{
	
	}


	~RecordSet( void )
	{
		Close();
	}

	inline int	Open( const char* c_szSql )
	{
		char *errr_msg = NULL;
		if( sqlite3_get_table(_pConn, c_szSql, &_pResult, &_uRowCount,
			&_uColumnCount, &errr_msg) != SQLITE_OK )
		{
			//EXCEPTION_THROW(CMyException,"error:%s\r\n",errr_msg);
			sqlite3_free(errr_msg);
			return -1;
		}

		Parse(c_szSql, true);
		return 1;
	}

	inline void Close( void )
	{
		if(_pResult != NULL)
		{
			sqlite3_free_table(_pResult);
			_pResult = NULL;
		}
		_uRowCount = 0;
		_uColumnCount = 0;
	}
	
	inline const char* GetValue( uint32_t uCol )
	{
		if( _uColumnCount <= uCol ||
			GetRow() < _uCurrentRow )
			return NULL;

		return _pResult[_uCurrentRow * _uColumnCount + uCol];
	}

	inline const int GetColumnType( uint32_t uCol )
	{
		return _ColumnsHeader.GetType(uCol);
	}

	inline const char* GetColumnName( uint32_t uCol )
	{
		return _ColumnsHeader.GetName(uCol);
	}

	inline const uint32_t GetRow( void )
	{
		return _uRowCount;
	}

	inline const uint32_t	GetColumnsCount( void )
	{
		return _uColumnCount;
	}

	inline void	MoveFirst( void )
	{
		_uCurrentRow = 1;  // 0为字段名
	}

	inline int	MoveNext( void )
	{
		if(GetRow() > _uCurrentRow)
		{
			_uCurrentRow ++;
			return 1;
		}
		return -1;
	}

	inline void	MoveLast( void )
	{
		_uCurrentRow = _uRowCount;
	}
private:
	inline void Parse( const char* c_szSql, bool isParseHeader = false )
	{	
		//获得字段数
		if( isParseHeader )
		{
			char *errr_msg = NULL;
			sqlite3_stmt*	lpstmt = NULL;
			if (SQLITE_OK != sqlite3_prepare_v2(_pConn, c_szSql, 
				strlen(c_szSql), &lpstmt, (const char **)&errr_msg))
			{
				fprintf(stderr, "RecordSet::Parse() sqlite prepare error: %s\n", errr_msg);
				Close();
				sqlite3_free(errr_msg);
				return ;
			}

			if(sqlite3_step(lpstmt) == SQLITE_ROW)
			{
				for(int i = 0; i < _uColumnCount; i ++)
				{
					_ColumnsHeader.AddColumns((char*)sqlite3_column_name(lpstmt, i),
						sqlite3_column_type(lpstmt, i));
				}

				//[5]释放资源  
				sqlite3_finalize(lpstmt);
				lpstmt = NULL;
			}
		}

		MoveNext();
	}

	inline bool is_open( void ) const
	{
		return _pConn != NULL;
	}
private:
	ColumnsHeader	_ColumnsHeader;
	sqlite3*		_pConn;
	char**			_pResult;
	int				_uCurrentRow;
	int				_uRowCount;
	int				_uColumnCount;
};

typedef void* HRecordSet;

} // namespace _sqlite_

#endif

