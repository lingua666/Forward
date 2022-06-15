#ifndef  _CUSTOM_ABSTRACTDATABASES_H_DEF__
#define _CUSTOM_ABSTRACTDATABASES_H_DEF__

#include <libThirdWrap_Mysql/RecordSet.h>

namespace _mysql_{

#define DEFAULT_FIELD        32  //你所要操作的库中，数据表的最大的字段数。假如student表有5个字段，
								//分别是name,old,sex,profession,class，而teacher表有10个字段，则
								//此值应添10。最好设置为最大字段数，设置太大会占用非常大的内存。

#define DEFAULT_FIELD_SIZE   32  //每个字段名称的最大字节数。如：name为4个字节，profession为10个字节
								//此处应该添10。注意：一个汉字等于2个字节，不建议使用汉字作为字段名。
								//同样，此值也不应该设置过大，否则也会占用非常大的内存。


#pragma pack(push,1)

//一个字段结构
typedef struct _FIELDHEADER
{
	char     cField_Name[DEFAULT_FIELD_SIZE];	//字段名
	uint32_t uiFieldType;						//字段类型
	uint32_t uiIndex;							//索引
}SFIELDHEADER,*LP_SFIELDHEADER;

typedef struct _FIELDS
{
	void*        vFiled_Value;				//字段值的指针 
	uint32_t uiValue_Len;					//字段的长度，只适用于字符串类型
}SFIELDS,*LP_SFIELDS;

typedef struct DBTABLEHEADER
{
	//bool			bIsNull[DEFAULT_FIELD];				//是否为空
	bool			bIsPrimaryKey[DEFAULT_FIELD];			//是否主键
	uint32_t		uiFieldLength[DEFAULT_FIELD];			//字段长度  只有字段类型为VARCHAR才有效
	uint32_t		SFieldCount;
	SFIELDHEADER	SFieldHeader[DEFAULT_FIELD];
}SDBTABLEHEADER,*LP_DBTABLEHEADER;

typedef struct DBTABLEFIELDS
{
	char     cField_Name[DEFAULT_FIELD_SIZE];		    //字段名
	char*    clpFieldContent;							//内容
	uint32_t uiFieldType;							//字段类型
	DBTABLEFIELDS()
	{
		memset(cField_Name,0,DEFAULT_FIELD_SIZE);
		clpFieldContent = NULL;
	}
}SDBTABLEFIELDS,*LP_DBTABLEFIELDS;

#pragma pack(pop)

enum enum_MySQLDBField_Types { 
	DBFIELD_TYPE_INT,DBFIELD_TYPE_DOUBLE,
	DBFIELD_TYPE_DATETIME,DBFIELD_TYPE_BOOLEAN,
	DBFIELD_TYPE_VARCHAR,DBFIELD_TYPE_TEXT,
};

//数据库抽象类
class CAbstractDataBases
{
public:
	virtual ~CAbstractDataBases( void ) { };

	//初始化
	virtual bool Init( bool bIsReconnect = true, uint32_t uiConnectTimeOut = 60 ) = 0;

	//释放
	virtual void Release( void ) = 0;

	//连接
	virtual bool Connect( const char* szDb, const char* szHost, const char* szName,
							const char* szPwd, uint32_t uiPort, UInt8 u8Characters ) = 0;

	//重新连接
	virtual bool ReConnect( void ) = 0;

	//断开连接
	virtual bool Disconnect( void ) = 0;

	//执行非查询SQL语句
	virtual bool ExecuteNoQuery( const char* szSql ) = 0;

	//改变用户
	virtual bool ChangeUser( const char* szName,const char *szPwd,const char *szDb ) = 0;
	
	//更改数据库名
	virtual bool SelectDB( const char* szDb ) = 0;

	//开始事务
	virtual bool Begin( void ) = 0;

	//事务提交
	virtual bool Commit( void ) = 0;

	//事务回滚
	virtual bool RollBack( void ) = 0;

	//创建数据表
	virtual bool CreateTable( const char* szTableName, SDBTABLEHEADER* lpDBTableHeader,
		                      uint32_t uiTableType ) = 0;
};

} // namespace _mysql_

#endif

