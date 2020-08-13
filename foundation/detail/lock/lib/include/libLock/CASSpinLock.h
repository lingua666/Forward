
#ifndef __CASSPINLOCK_20150930171651_1443604611_H__
#define __CASSPINLOCK_20150930171651_1443604611_H__

//CAS×ÔÐýËø

namespace	_lock_{

	class CASSpinLock
	{
	public:
		/*!
		* @function   CASLock
		* @brief    
		* @return  
		*/
		CASSpinLock( void );

		void Lock( void );

		void UnLock( void );

		bool TryLock( void );

	private:
		long	_mutex;
	};

	
	class CASSpinLockNone
	{
	public:
		/*!
		* @function   CASSpinLockNone
		* @brief    
		* @return  
		*/
		CASSpinLockNone( void )
		{}

		void Lock( void )
		{}

		void UnLock( void )
		{}

		bool TryLock( void )
		{ return false; }
	};
}

#endif//__CASSPINLOCK_20150930171651_1443604611_H__
