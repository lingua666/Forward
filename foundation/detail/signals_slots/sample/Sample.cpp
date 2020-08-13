// MediaServerSample.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <libSmart_ptr/Shared_ptr.h>
#include <libSignalsSlots/Signals.h>
#include <boost/pool/pool.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/signals2/signal.hpp>

void Print1( void )
{
	printf("Print1\r\n");
}

void Print2( void )
{
	printf("Print2\r\n");
}

void FreeTest(void* p)
{
	int i = 0;
	i ++;
}

int main(int argc, char* argv[])
{
	/*boost::signals2::signal<void ()> sig;
	sig.connect(&Print1, boost::signals2::connect_position::at_front);
	sig.connect(&Print1, boost::signals2::connect_position::at_front);
	sig.disconnect(&Print1);
	boost::signals2::connection c = sig.connect(&Print2, boost::signals2::connect_position::at_front);
	c.disconnect();
	sig();*/
	while(true)
	{
		printf("begin\r\n");
		_signals_slots_::signal<void ( void )> my_sig1;
		{
			_signals_slots_::connection c1;
			//while(true)
			{
				_signals_slots_::signal<void ( void )>	my_sig;
				/*my_sig.connect(&Print1, _signals_slots_::connect_position::at_front);
				my_sig.connect(&Print1, _signals_slots_::connect_position::at_front);
				my_sig.disconnect(&Print1);*/
				c1 = my_sig.connect(function20_bind_Ex(Print2), _signals_slots_::connect_position::at_front);
				//c1.disconnect();
				my_sig();
				my_sig1 = my_sig;
			}
			//c1.disconnect();
			printf("end\r\n");
		}
		my_sig1();
	}

	std::cin.get();
	return 0;
}