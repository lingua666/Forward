
#ifndef __BASE_MYMEMORY_20160419111505_1438860785_H__
#define __BASE_MYMEMORY_20160419111505_1438860785_H__

#include <stdlib.h>

// new[], delete[], std::nothrow
#include <new>

// std::malloc, std::free
#include <cstdlib>

#define MEMORY_ALIGNMENT_DEF    4

namespace _base_{

	struct allocator_malloc_free
	{
		static inline void*	Alloc( const unsigned int ulSize )
		{
			return std::malloc( ulSize );
		}

		static inline void*	Realloc( void *Mem_Address, const unsigned long ulSize )
		{
			return std::realloc(Mem_Address, ulSize);
		}

		static	inline void	Free( void* pBuf )
		{
			std::free( reinterpret_cast<void*>(pBuf) );
		}
	};

	struct allocator_new_delete
	{
		static inline void*	Alloc( const unsigned long ulSize )
		{
			return reinterpret_cast<void*>(new (std::nothrow) char[ulSize]);
		}

		static inline void*	Calloc( const unsigned long ulSize )
		{
			return reinterpret_cast<void*>(new (std::nothrow) char[ulSize]);
		}

		static	inline void	Free( void* pBuf )
		{
			delete pBuf;
		}

		static inline void*	Realloc( void *Mem_Address, const unsigned long ulSize )
		{
			return allocator_malloc_free::Realloc(Mem_Address, ulSize);
		}

		static	inline void	RFree( void* pBuf )
		{
			return allocator_malloc_free::Free(pBuf);
		}
	};

	/*struct	allocator_virtual
	{
		static inline void*	Alloc( const unsigned long ulSize )
		{
#if defined(PLATFORM_OS_FAMILY_WINDOWS)
			return VirtualAlloc( NULL, ulSize, MEM_COMMIT, PAGE_READWRITE );
#else
			return allocator_malloc_free::Alloc( ulSize );
#endif
		}

		static	inline void	Free( void* pBuf )
		{
			IS_INVALID_NONE(pBuf);
#if defined(PLATFORM_OS_FAMILY_WINDOWS)
			VirtualFree( reinterpret_cast<void*>(pBuf) , 0 , MEM_RELEASE );
#else
			return allocator_malloc_free::Free( pBuf );
#endif
		}
	};*/

	struct allocator_align
	{
		bool is2ndPower(unsigned int Alignment)
		{
			//一个整数是2的整数次幂，把这个数减1,这两个数做按位与，结果一定是0.反之，结果不是零。
			return Alignment & (Alignment - 1) ? false : true;
		}

		unsigned long Convert2ndPower(unsigned int Alignment)
		{
			if (is2ndPower(Alignment))
				return Alignment;

			unsigned int Mask = 0;
			unsigned char Bit = 0, Len = 0;

			const unsigned int AlignArray[4] = { 0xF0 , 0xF000, 0xF00000, 0xF0000000 };
			const unsigned int MaskArray[4] = { 0x80 , 0x8000, 0x800000, 0x80000000 };
			const unsigned char SizeArray[4] = { 7 , 15, 23, 31 };

			int i = 0;
			for (i = 0; i < 4; i++)
			{
				if (Alignment < AlignArray[i])
				{
					Mask = MaskArray[i];
					Len = SizeArray[i];
					break;
				}
			}

			if (Len == 0)
				return 0;

			for (i = Len; i > 0; i--)
			{
				Mask = Mask >> 1;
				if ((Mask & Alignment) > 0)
				{
					Bit = i;
					break;
				}
			}

			return ((unsigned int)0x01) << Bit;
		}

		void* aligned_malloc(size_t size)
		{
			void *praw = malloc(sizeof(void*) + size + MEMORY_ALIGNMENT_DEF);
			if (praw)
			{
				void *pbuf = reinterpret_cast<void*>(reinterpret_cast<size_t>(praw) + sizeof(void*));
				void *palignedbuf = reinterpret_cast<void*>((reinterpret_cast<size_t>(pbuf) | (MEMORY_ALIGNMENT_DEF - 1)) + 1);
				(static_cast<void**>(palignedbuf))[-1] = praw;
				return palignedbuf;
			}

			return NULL;
		}

		void aligned_free(void *palignedmem)
		{
			free(reinterpret_cast<void*>((static_cast<void**>(palignedmem))[-1]));
		}
	};

}

typedef	_base_::allocator_new_delete	MemoryAllocator;

#endif

