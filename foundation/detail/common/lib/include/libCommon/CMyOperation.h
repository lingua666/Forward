
#ifndef __MYOPERATION_20160419111505_1438860785_H__
#define __MYOPERATION_20160419111505_1438860785_H__

#define	BIT(n)	(log((double)(n))/log((double)2))

class	CMyOperation
{
public:
	CMyOperation( const unsigned long ulLevel );
	~CMyOperation( void );

	inline const unsigned long	Multiplication( const unsigned long	value ) const;

	inline	const unsigned long	Division( const unsigned long value ) const;

	inline	const unsigned long	Mod( const unsigned long value ) const;

private:
	const unsigned long _ulLevel;
};

#endif
