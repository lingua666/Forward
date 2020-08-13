
#include <libNetFrame_Packet/PHead.h>

PHead::PHead( void )
{
	_sHead.clear();
	_sHead.append(1, 0xAA);
	_sHead.append(1, 0xBB);
}

PHead::~PHead( void )
{
	
}
