#ifndef __BST_ORDERED_INDEX_20191021232104_1442199360_H__
#define __BST_ORDERED_INDEX_20191021232104_1442199360_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <libBoost_Wheel/Bst_Ord_index_node.hpp>
#include <libBoost_Wheel/Bst_Ordered_index_args.hpp>

namespace boost{

	/*template<typename Key, typename MapT>
	struct ordered_index_map_impl : MapT
	{
		typedef typename MapT					type;
		typedef typename MapT::iterator			iterator;
		typedef typename MapT::const_iterator	const_iterator;
	};

	template<typename Key, typename MapT>
	class ordered_index_map //: ordered_index_map_impl<MapT>
	{
	public:
		typedef typename ordered_index_map_impl<Key,MapT>	map_type;
		typedef map_type			iterator;

	public:
		ordered_index_map( void )
		{

		}

		~ordered_index_map( void )
		{

		}

		template<typename Value>
		iterator insert(Value x)
		{
			return _Map.insert(x);
		}

		iterator find(const key_type& x)
		{
			return _Map.find(x);
		}

		const_iterator find(const key_type& x)const
		{
			return _Map.find(x);
		}
	private:
		map_type	_Map;
	};*/

	template<typename Iterator, typename RetType >
	struct ordered_index_iterator
	{
	public:
		ordered_index_iterator( void ) 
			: _isInit( false )
		{

		}

		ordered_index_iterator( const Iterator& Iter, const Iterator& End ) 
			: _Iter(Iter)
			, _End(End)
			, _isInit( true )
		{

		}

		ordered_index_iterator( const ordered_index_iterator& t1 ) 
			: _Iter(t1._Iter)
			, _End(t1._End)
			, _isInit( true )
		{
			
		}

		inline ordered_index_iterator& operator=( const ordered_index_iterator& t1 )
		{
			this->_Iter = t1._Iter;
			this->_End = t1._End;
			this->_isInit = t1._isInit;
			return *this;
		}

		inline ordered_index_iterator& operator++()
		{
			this->_Iter ++;
			return *this;
		}

		inline const ordered_index_iterator operator++(int)
		{
			ordered_index_iterator Old = *this;
			++(*this);
			return Old;
		}

		inline bool operator==( const ordered_index_iterator& t1 )
		{
			return this->_Iter == t1._Iter;
		}

		inline bool operator!=( const ordered_index_iterator& t1 )
		{
			return this->_Iter != t1._Iter;
		}

		inline RetType* operator->()
		{
			return &(this->_Iter->second->value());
		}

		inline operator bool ()
		{
			return  (this->_isInit && this->_Iter != this->_End) ? true : false;
		}

		inline bool operator!()
		{ 
			return (!this->_isInit || this->_Iter == this->_End) ? true : false;
		}

	private:
		bool		_isInit;
		Iterator	_Iter;
		Iterator	_End;
	};

	template<typename Super>
	struct ordered_index_node : Super
	{

	};

	struct ordered_unique_tag{};
	struct ordered_non_unique_tag{};

	template<
		typename KeyFromValue,typename Compare,
		typename SuperMeta,typename TagList,typename Category = ordered_unique_tag
	>
	class ordered_index: public SuperMeta::type
	{
	public:
		typedef typename SuperMeta::type				super;
		typedef typename super::node_type				node_type;
		typedef typename super::node_type_sptr			node_type_sptr;

		typedef typename push_front<
			typename super::index_type_list,
			ordered_index>::type						index_type_list;
		typedef KeyFromValue                            key_from_value;
		typedef typename KeyFromValue::result_type      key_type;
		typedef key_type								key_param_type;
		typedef typename node_type::value_type          value_type;
		typedef value_type								value_param_type;

		typedef TagList									tag_type;
		typedef Compare									key_compare;

		typedef typename std::multimap<key_type, node_type_sptr>		map_type;
		typedef typename map_type::iterator				map_iterator;
		typedef ordered_index_iterator<map_iterator, value_type>		iterator;
		typedef iterator								const_iterator;
		//typedef typename ordered_index_map<key_type,map_t>	map_type;
		/*typedef typename map_type::iterator					iterator;
		typedef typename map_type::const_iterator			const_iterator;*/

	public:
		ordered_index():
			super(),
			key(key_from_value()),
			comp_(key_compare())
		{
			empty_initialize();
		}

		ordered_index(
			const ordered_index<KeyFromValue,Compare,SuperMeta,TagList,Category>& x):
			key(x.key),
			comp_(x.comp_)
		{

		}

		~ordered_index()
		{
		/* the container is guaranteed to be empty by now */
		}

		ordered_index<KeyFromValue,Compare,SuperMeta,TagList,Category>& operator=(
			const ordered_index<KeyFromValue,Compare,SuperMeta,TagList,Category>& x)
		{
			this->final()=x.final();
			return *this;
		}

		std::pair<iterator,bool> insert(const value_param_type& x)
		{
			return this->template final_insert_<iterator>(x);
		}

		iterator insert_(const value_param_type& v,const node_type_sptr& x)
		{
			if( super::insert_(v,x) )
				return make_iterator(_Map.insert(std::make_pair(key(v),x)));
			return make_iterator(_Map.end());
		}

		int erase(const key_param_type& x)
		{
			map_iterator iter = _Map.find(x);
			if( iter == _Map.end() )
				return 0;
			
			node_type_sptr sptr = iter->second;
			/*_Map.erase(iter);*/
			return this->final_erase_(sptr->value(), sptr);
		}

		int erase_(const value_param_type& v, const node_type_sptr& x)
		{
			if(_Map.erase(key(v)) <= 0)
				return  0;
			return super::erase_(v,x);
		}

		/*int erase_(const value_param_type& v, const node_type_sptr& x, int depth)
		{
			return super::erase_(v,x);
		}*/

		int count( const key_type& x )
		{
			return _Map.count(x);
		}

		iterator find(const key_type& x)
		{
			//return _Map.find(x);
			return make_iterator(_Map.find(x));
		}

		const_iterator find(const key_type& x)const
		{
			//return _Map.find(x);
			return make_iterator(_Map.find(x));
		}

		iterator               begin()
		{
			return make_iterator(_Map.begin());
		}

		const_iterator         begin()const
		{
			return make_iterator(_Map.begin());
		}

		iterator               end()
		{
			return make_iterator(_Map.end());
		}

		const_iterator         end()const
		{
			return make_iterator(_Map.end());
		}

		iterator make_iterator( const map_iterator& iter )
		{
			return iterator(iter, _Map.end());
		}

	protected:
		void empty_initialize()
		{
			_Map.clear();
		}
		
	private:
		map_type		_Map;
		key_from_value	key;
		key_compare		comp_;
	};

	template<
		typename KeyFromValue,typename Compare,
		typename SuperMeta,typename TagList
	>
	class ordered_index<KeyFromValue, Compare, SuperMeta, TagList, ordered_unique_tag>: public SuperMeta::type
	{
	public:
		typedef typename SuperMeta::type				super;
		/*typedef ordered_index_node<
			typename super::node_type>                  node_type;*/
		typedef typename super::node_type						node_type;
		typedef typename super::node_type_sptr			node_type_sptr;

		typedef typename push_front<
			typename super::index_type_list,
			ordered_index>::type						index_type_list;
		typedef KeyFromValue                            key_from_value;
		typedef typename KeyFromValue::result_type      key_type;
		typedef key_type								key_param_type;
		typedef typename node_type::value_type          value_type;
		typedef value_type								value_param_type;

		typedef Compare									key_compare;
		typedef TagList									tag_type;

		typedef std::map<key_type, node_type_sptr>		map_type;
		typedef typename map_type::iterator				map_iterator;
		typedef ordered_index_iterator<map_iterator, value_type>		iterator;
		typedef iterator								const_iterator;
		//typedef typename ordered_index_map<key_type,map_t>	map_type;
		/*typedef typename map_type::iterator				iterator;
		typedef typename map_type::const_iterator		const_iterator;*/

	public:
		ordered_index():
			super(),
			key(key_from_value()),
			comp_(key_compare())
		{
			empty_initialize();
		}

		ordered_index(
			const ordered_index<KeyFromValue,Compare,SuperMeta,TagList,ordered_unique_tag>& x):
			super(x),
			key(x.key),
			comp_(x.comp_)
		{
			
		}

		~ordered_index()
		{
		/* the container is guaranteed to be empty by now */
		}

		ordered_index<KeyFromValue,Compare,SuperMeta,TagList,ordered_unique_tag>& operator=(
			const ordered_index<KeyFromValue,Compare,SuperMeta,TagList,ordered_unique_tag>& x)
		{
			this->final()=x.final();
			return *this;
		}

		std::pair<iterator,bool> insert(const value_param_type& x)
		{
			return this->template final_insert_<iterator>(x);
		}

		iterator insert_(const value_param_type& v,const node_type_sptr& x)
		{
			map_iterator iter = _Map.find( key(v) );
			if( iter != _Map.end() )
				return make_iterator(_Map.end());

			if( super::insert_(v,x) )
				return make_iterator(_Map.insert(std::make_pair(key(v),x)).first);
			return make_iterator(_Map.end());
		}

		int erase(const key_param_type& x)
		{
			map_iterator iter = _Map.find(x);
			if( iter == _Map.end() )
				return 0;

			node_type_sptr sptr = iter->second;
			/*_Map.erase(iter);*/
			return this->final_erase_(sptr->value(), sptr);
		}

		int erase_(const value_param_type& v, const node_type_sptr& x)
		{
			if(_Map.erase(key(v)) <= 0)
				return  0;
			return super::erase_(v,x);
		}

		int count( const key_type& x )
		{
			return _Map.count(x);
		}

		iterator find(const key_type& x)
		{
			//return _Map.find(x);
			return make_iterator(_Map.find(x));
		}

		const_iterator find(const key_type& x)const
		{
			//return _Map.find(x);
			return make_iterator(_Map.find(x));
		}

		iterator               begin()
		{
			return make_iterator(_Map.begin());
		}

		const_iterator         begin()const
		{
			return make_iterator(_Map.begin());
		}

		iterator               end()
		{
			return make_iterator(_Map.end());
		}

		const_iterator         end()const
		{
			return make_iterator(_Map.end());
		}

		iterator make_iterator( const map_iterator& iter )
		{
			return iterator(iter, _Map.end());
		}

	protected:
		void empty_initialize()
		{
			_Map.clear();
		}

	private:
		map_type		_Map;
		key_from_value	key;
		key_compare		comp_;
	};

	template<typename Arg1,typename Arg2=na,typename Arg3=na>
	struct ordered_unique
	{
		typedef ordered_index_args<
			Arg1,Arg2,Arg3>                                index_args;
		typedef typename index_args::tag_list_type::type tag_list_type;
		typedef typename index_args::key_from_value_type key_from_value_type;
		typedef typename index_args::compare_type        compare_type;

		//test
		typedef tag_list_type					tag_type;
		//test

		template<typename SuperMeta>
		struct index_class
		{
			typedef ordered_index<
				key_from_value_type,compare_type,
				SuperMeta,tag_list_type,ordered_unique_tag> type;
		};
	};

	template<typename Arg1,typename Arg2=na,typename Arg3=na>
	struct ordered_non_unique
	{
		typedef ordered_index_args<
			Arg1,Arg2,Arg3>                              index_args;
		typedef typename index_args::tag_list_type::type tag_list_type;
		typedef typename index_args::key_from_value_type key_from_value_type;
		typedef typename index_args::compare_type        compare_type;

		//test
		typedef tag_list_type					tag_type;
		//test

		template<typename SuperMeta>
		struct index_class
		{
			typedef ordered_index<
				key_from_value_type,compare_type,
				SuperMeta,tag_list_type,ordered_non_unique_tag> type;
		};
	};

}

#endif
