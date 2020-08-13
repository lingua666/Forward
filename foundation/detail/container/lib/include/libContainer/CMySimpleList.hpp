
#ifndef __CMYSIMPLELIST_20150806193305_1438860785_H__
#define __CMYSIMPLELIST_20150806193305_1438860785_H__

#include <libLock/Lock.hpp>
#include <libMempool/MyMemory.hpp>

namespace	_container_{

	template <typename T>
	class CMySimpleList //: public noncopyable
	{
	public:

		struct stNode
		{
			stNode* _Next;	// successor node, or first element if head
			T _Myval;	// the stored value, unused if head
		};

		typedef	unsigned long	size_type;
		typedef	stNode			node_type;
		typedef	T				iterator;

	public:
		CMySimpleList( void ) : m_pUseNode(NULL)
			,m_pFreeNode(NULL)
		{

		}

		~CMySimpleList( void )
		{
			clear();
		}

		T begin( void )
		{
			if(m_pUseNode == NULL)
				return NULL;
			return m_pUseNode->_Myval;
		}

		T end( void )
		{
			return NULL;
		}

		T erase( T _First_arg )
		{
			return NULL;
		}

		bool empty( void ) const
		{
			return (m_pUseNode == NULL);
		}

		const size_type size( void ) const 
		{
			return 0;
		}

		void clear( void )
		{
			while(!empty())
			{
				pop_front1();
			}

			while(m_pFreeNode != NULL)
			{
				m_pFreeNode = Free(m_pUseNode);
			}
		}

		T front( void )
		{
			return begin();
		}

		T back( void )
		{
			return NULL;
		}

		void	 push_front( const T& _Val )
		{
			node_type* const pNode = Alloc();
			pNode->_Myval = _Val;
			pNode->_Next = m_pUseNode;
			m_pUseNode = pNode;
		}

		void pop_front1( void )
		{
			if(m_pUseNode != NULL)
			{
				m_pUseNode = RetBack(m_pUseNode);
			}
		}

		T	pop_front2( void )
		{
			T t1 = front();
			pop_front1();
			return t1;
		}

		void	 push_back(const T& _Val )
		{

		}

		void pop_back1( void )
		{

		}

		T&	pop_back2( void )
		{
			return NULL;
		}

	protected:
		node_type*	const Alloc( void )
		{
			if(m_pFreeNode != NULL)
			{
				node_type*	const pNode = m_pFreeNode;
				m_pFreeNode = m_pFreeNode->_Next;
				return pNode;
			}

			return reinterpret_cast<node_type*>(MemoryAllocator::Alloc(sizeof(node_type)));
		}

		node_type*	const Free( node_type* const pNode )
		{
			node_type* const ptr = pNode->_Next;
			MemoryAllocator::Free(reinterpret_cast<void*>(pNode));
			return ptr;
		}	

		node_type*	const RetBack( node_type* const pNode )
		{
			node_type* const ptr = pNode->_Next;
			pNode->_Next = m_pFreeNode;
			m_pFreeNode = pNode;
			return ptr;
		}

	private:
		node_type*		m_pUseNode;
		node_type*		m_pFreeNode;
	};

} //_container_

#endif