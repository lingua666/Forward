#ifndef  _CUSTOM_ABSTRACTDATABASES_H_DEF__
#define _CUSTOM_ABSTRACTDATABASES_H_DEF__

#include <libThirdWrap_Mysql/RecordSet.h>

namespace _mysql_{

#define DEFAULT_FIELD        32  //����Ҫ�����Ŀ��У����ݱ�������ֶ���������student����5���ֶΣ�
								//�ֱ���name,old,sex,profession,class����teacher����10���ֶΣ���
								//��ֵӦ��10���������Ϊ����ֶ���������̫���ռ�÷ǳ�����ڴ档

#define DEFAULT_FIELD_SIZE   32  //ÿ���ֶ����Ƶ�����ֽ������磺nameΪ4���ֽڣ�professionΪ10���ֽ�
								//�˴�Ӧ����10��ע�⣺һ�����ֵ���2���ֽڣ�������ʹ�ú�����Ϊ�ֶ�����
								//ͬ������ֵҲ��Ӧ�����ù��󣬷���Ҳ��ռ�÷ǳ�����ڴ档


#pragma pack(push,1)

//һ���ֶνṹ
typedef struct _FIELDHEADER
{
	char     cField_Name[DEFAULT_FIELD_SIZE];	//�ֶ���
	uint32_t uiFieldType;						//�ֶ�����
	uint32_t uiIndex;							//����
}SFIELDHEADER,*LP_SFIELDHEADER;

typedef struct _FIELDS
{
	void*        vFiled_Value;				//�ֶ�ֵ��ָ�� 
	uint32_t uiValue_Len;					//�ֶεĳ��ȣ�ֻ�������ַ�������
}SFIELDS,*LP_SFIELDS;

typedef struct DBTABLEHEADER
{
	//bool			bIsNull[DEFAULT_FIELD];				//�Ƿ�Ϊ��
	bool			bIsPrimaryKey[DEFAULT_FIELD];			//�Ƿ�����
	uint32_t		uiFieldLength[DEFAULT_FIELD];			//�ֶγ���  ֻ���ֶ�����ΪVARCHAR����Ч
	uint32_t		SFieldCount;
	SFIELDHEADER	SFieldHeader[DEFAULT_FIELD];
}SDBTABLEHEADER,*LP_DBTABLEHEADER;

typedef struct DBTABLEFIELDS
{
	char     cField_Name[DEFAULT_FIELD_SIZE];		    //�ֶ���
	char*    clpFieldContent;							//����
	uint32_t uiFieldType;							//�ֶ�����
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

//���ݿ������
class CAbstractDataBases
{
public:
	virtual ~CAbstractDataBases( void ) { };

	//��ʼ��
	virtual bool Init( bool bIsReconnect = true, uint32_t uiConnectTimeOut = 60 ) = 0;

	//�ͷ�
	virtual void Release( void ) = 0;

	//����
	virtual bool Connect( const char* szDb, const char* szHost, const char* szName,
							const char* szPwd, uint32_t uiPort, UInt8 u8Characters ) = 0;

	//��������
	virtual bool ReConnect( void ) = 0;

	//�Ͽ�����
	virtual bool Disconnect( void ) = 0;

	//ִ�зǲ�ѯSQL���
	virtual bool ExecuteNoQuery( const char* szSql ) = 0;

	//�ı��û�
	virtual bool ChangeUser( const char* szName,const char *szPwd,const char *szDb ) = 0;
	
	//�������ݿ���
	virtual bool SelectDB( const char* szDb ) = 0;

	//��ʼ����
	virtual bool Begin( void ) = 0;

	//�����ύ
	virtual bool Commit( void ) = 0;

	//����ع�
	virtual bool RollBack( void ) = 0;

	//�������ݱ�
	virtual bool CreateTable( const char* szTableName, SDBTABLEHEADER* lpDBTableHeader,
		                      uint32_t uiTableType ) = 0;
};

} // namespace _mysql_

#endif

