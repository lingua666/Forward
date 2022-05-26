
#include <libThirdWrap_Mysql/RecordSet.h>

#if _MSC_VER>=1900
#include "stdio.h" 
_ACRTIMP_ALT FILE* __cdecl __acrt_iob_func(unsigned);
#ifdef __cplusplus 
extern "C"
#endif 
FILE* __cdecl __iob_func(unsigned i) {
	return __acrt_iob_func(i);
}
#endif /* _MSC_VER>=1900 */

namespace _mysql_{

	ColumnsHeader::ColumnsHeader(void)
	{
		_ColumnsHeader.clear();
	}

	ColumnsHeader::~ColumnsHeader(void)
	{
		size_t size = _ColumnsHeader.size();
		_ColumnsHeader.clear();
	}

	void	ColumnsHeader::AddColumns(const char* szName, int iType)
	{
		char szColumn[COLUMNSHEADER_SIZE + TYPE_BUF_SIZE] = { 0 };
		strcpy(szColumn, szName);
		size_t len = strlen(szColumn);
		szColumn[len] = 0;
		len++;
		*((int*)&szColumn[len]) = iType;
		len += TYPE_BUF_SIZE;
		_ColumnsHeader.push_back(_string_type(szColumn, len));
	}

	const char*		ColumnsHeader::GetName(uint32_t uCol)
	{
		if (uCol >= _ColumnsHeader.size())
			return NULL;

		return _ColumnsHeader.at(uCol).c_str();
	}

	const int ColumnsHeader::GetType(uint32_t uCol) const
	{
		if (uCol >= _ColumnsHeader.size())
			return NULL;

		size_t len = _ColumnsHeader.at(uCol).length();
		return *((int*)&(_ColumnsHeader.at(uCol).c_str()[len - TYPE_BUF_SIZE]));
	}

	const size_t ColumnsHeader::GetCount(void) const
	{
		return _ColumnsHeader.size();
	}



	RecordSet::RecordSet(MYSQL* pMySQLConn)
		:_pMySQLConn(pMySQLConn)
		, _pResult(NULL)
		, _uRowCount(0)
		, _uColumnCount(0)
		, _uCurrentRow(0)
	{

	}


	RecordSet::~RecordSet(void)
	{
		Close();
	}

	int	RecordSet::Open(const char* c_szSql)
	{
		if (mysql_query(_pMySQLConn, c_szSql) != 0)
		{
			//EXCEPTION_THROW(CMyException,"error:%s\r\n",mysql_error(_pMySQLConn));
			printf("error:%s\r\n", mysql_error(_pMySQLConn));
			return -1;
		}

		_pResult = mysql_store_result(_pMySQLConn);
		if (_pResult == NULL)
		{
			//EXCEPTION_THROW(CMyException,"error:%s\r\n",mysql_error(_pMySQLConn));
			return -1;
		}

		Parse(_pResult);

		//后续数据全部丢弃
		while (!mysql_next_result(_pMySQLConn))
		{
			MYSQL_RES *pRes = mysql_store_result(_pMySQLConn);
			if (pRes != NULL)
				mysql_free_result(pRes);
		};

		return 1;
	}

	void RecordSet::Close(void)
	{
		if (_pResult != NULL)
		{
			mysql_free_result(_pResult);
			_pResult = NULL;
		}
		_uRowCount = 0;
		_uColumnCount = 0;
	}

	const char* RecordSet::GetValue(uint32_t uCol)
	{
		if (_uColumnCount <= uCol ||
			GetRow() < _uCurrentRow)
			return NULL;
		return _Row[uCol];
	}

	const int RecordSet::GetColumnType(uint32_t uCol)
	{
		return _ColumnsHeader.GetType(uCol);
	}

	const char* RecordSet::GetColumnName(uint32_t uCol)
	{
		return _ColumnsHeader.GetName(uCol);
	}

	const uint32_t RecordSet::GetRow(void)
	{
		return _uRowCount;
	}

	const uint32_t	RecordSet::GetColumnsCount(void)
	{
		return _uColumnCount;
	}

	void	RecordSet::MoveFirst(void)
	{
		mysql_data_seek(_pResult, 0);
		mysql_row_seek(_pResult, mysql_row_tell(_pResult));
		if ((_Row = mysql_fetch_row(_pResult)) != NULL)
		{
			_uCurrentRow = 0;
		}
	}

	int	RecordSet::MoveNext(void)
	{
		_Row = mysql_fetch_row(_pResult);
		if (_Row == NULL)
			return -1;
		_uCurrentRow++;
		return 1;
	}

	void	RecordSet::MoveLast(void)
	{
		mysql_data_seek(_pResult, _uRowCount - 1);
		mysql_row_seek(_pResult, mysql_row_tell(_pResult));
		if ((_Row = mysql_fetch_row(_pResult)) != NULL)
		{
			if (_uRowCount > 0)
				_uCurrentRow = _uRowCount - 1;
			else
				_uCurrentRow = 0;
		}
	}

	void RecordSet::Parse(MYSQL_RES* pResult, bool isParseHeader)
	{
		_uRowCount = (uint32_t)(mysql_num_rows(_pResult));
		_uColumnCount = (uint32_t)(mysql_num_fields(_pResult));

		//获得字段数
		if (isParseHeader)
		{
			MYSQL_FIELD *pField = NULL;
			for (uint32_t i = 0; i < _uColumnCount; i++)
			{
				//获得列名称、列类型
				pField = mysql_fetch_field_direct(pResult, i);
				_ColumnsHeader.AddColumns(pField->name, (uint32_t)pField->type);
			}
		}

		if (_uRowCount > 0)
			MoveNext();
	}

	bool RecordSet::is_open(void) const
	{
		return _pMySQLConn != NULL;
	}

} // namespace _mysql_


