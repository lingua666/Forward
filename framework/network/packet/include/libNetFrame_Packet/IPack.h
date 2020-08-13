
#ifndef __IPACK_20151212174229_1449913349_H__
#define __IPACK_20151212174229_1449913349_H__

#include "External.h"

#pragma pack(push, 1)
struct tagPKBuf
{
	UInt16	_uPayload;
	char	_Data[1];
};

struct tagPKBuf_V2
{
	void*	_pHead;
	UInt16	_uPayload;
	char	_Data[1];
};
#pragma pack(pop)

typedef	PKSmartPtr<tagPKBuf>			PKBuf_ptr;
typedef	PLList< PKBuf_ptr, CNoneLock >	PKBList;
typedef	PKSmartPtr<PKBList>			PKBList_ptr;

/*!
* @class  IPack
* @brief   IPack
* @{
*/

class IPack
{
public:
	/*!
	* @function   ~IPack
	* @brief    
	* @return  
	*/
	virtual ~IPack( void ) {}
	virtual void	Release( void ) = 0;
	/*virtual UInt16	GetHeadSize( void ) const = 0;
	virtual UInt16	GetPayload( const char* c_szHead ) const = 0;
	virtual UInt16	GetMaxPayload( void ) const = 0;
	virtual void	SetFunction( const PKFunction& Function ) = 0;
	virtual const PKBuf_ptr	Package( const char* c_InData, UInt16 u16InSize ) = 0;
	virtual const PKBuf_ptr	Package( void* pAddtion, UInt8 u8AddSize, 
						const char* c_InData, UInt16 u16InSize ) = 0;
	virtual const PKBList_ptr	BigPackage( const char* c_InData, UInt16 u16InSize ) = 0;
	virtual const PKBList_ptr	BigPackage( void* pAddtion, UInt8 u8AddSize,
						const char* c_InData, UInt16 u16InSize ) = 0;
	virtual int	Parse( const char* c_szInData, UInt16 u16InSize,
					void* lParam = NULL ) = 0;*/

};
/** @} end IPack */

#endif//__IPACK_20151212174229_1449913349_H__
