
#ifndef __CASLOCK_20150731180430_1438337070_H__
#define __CASLOCK_20150731180430_1438337070_H__

namespace	_lock_{

	/*!
	* @class  CASLock
	* @brief   CASLock
	* @{
	*/

	class CASLock
	{
	public:
		/*!
		* @function   CASLock
		* @brief    
		* @return  
		*/
		CASLock( void );

		void Lock(  void );

		void UnLock( void );

		bool TryLock( void );

	private:
		long	_mutex;
	};
	/** @} end CASLock */

	class CASLockNone
	{
	public:
		/*!
		* @function   CASLockNone
		* @brief    
		* @return  
		*/
		CASLockNone( void )
		{}

		inline void Lock(  void ) const
		{}

		inline void UnLock( void ) const
		{}

		inline bool TryLock( void )
		{ return false; }
	};
	/** @} end CASLock */
}

#endif//__CASLOCK_20150731180430_1438337070_H__
