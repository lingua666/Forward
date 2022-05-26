
#ifndef _CUSTOM_RECORDSET_H_DEF__
#define _CUSTOM_RECORDSET_H_DEF__

#include <libThirdWrap_Mysql/External.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <mysql.h> //文件位于 MySQL 提供的 C API 目录中
#include <mysqld_error.h>


namespace _mysql_{

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
	ColumnsHeader(void);

	~ColumnsHeader(void);

	inline void	AddColumns(const char* szName, int iType);

	const char*		GetName(uint32_t uCol);

	const int GetType(uint32_t uCol) const;

	const size_t GetCount(void) const;

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
	explicit RecordSet(MYSQL* pMySQLConn);

	~RecordSet(void);

	int	Open(const char* c_szSql);

	void Close(void);
	
	const char* GetValue(uint32_t uCol);

	const int GetColumnType(uint32_t uCol);

	const char* GetColumnName(uint32_t uCol);

	const uint32_t GetRow(void);

	const uint32_t	GetColumnsCount(void);

	void	MoveFirst(void);

	int	MoveNext(void);

	void	MoveLast(void);

private:
	void Parse(MYSQL_RES* pResult, bool isParseHeader = false);

	bool is_open(void) const;

private:
	ColumnsHeader	_ColumnsHeader;
	MYSQL*			_pMySQLConn;
	MYSQL_RES*		_pResult;
	MYSQL_ROW		_Row;
	uint32_t		_uCurrentRow;
	uint32_t		_uRowCount;
	uint32_t		_uColumnCount;
};

typedef void* HRecordSet;

} // namespace _mysql_

#endif

