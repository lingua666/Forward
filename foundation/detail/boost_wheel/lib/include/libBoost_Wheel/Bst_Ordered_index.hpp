#ifndef __BST_ORDERED_INDEX_20191021232104_1442199360_H__
#define __BST_ORDERED_INDEX_20191021232104_1442199360_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <libBoost_Wheel/Bst_Ord_index_ops.hpp>
#include <libBoost_Wheel/Bst_Ord_index_node.hpp>
#include <libBoost_Wheel/Bst_Ordered_index_args.hpp>

namespace boost_wheel{

	namespace multi_index{

		struct none_unbounded_tag{};
		struct lower_unbounded_tag{};
		struct upper_unbounded_tag{};
		struct both_unbounded_tag{};
		struct ordered_unique_tag{};
		struct ordered_non_unique_tag{};

		struct unbounded_type{};

		template<
			typename KeyFromValue,typename Compare,
			typename SuperMeta,typename TagList,typename Category
		>
		class ordered_index: protected SuperMeta::type
		{
		public:
			typedef typename SuperMeta::type				super;
			typedef std::size_t                             size_type;    

		protected:
			typedef ordered_index_node<
				typename super::node_type>                     node_type;

			typedef typename super::final_node_type            final_node_type;

		private:
			typedef typename node_type::impl_type              node_impl_type;
			typedef typename node_impl_type::pointer           node_impl_pointer;

		public:
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
			typedef bidir_node_iterator<node_type>          iterator;
			typedef iterator                                const_iterator;

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

			  std::pair<iterator,bool> insert(value_param_type x)
			  {
				  std::pair<final_node_type*,bool> p=this->final_insert_(x);
				  return std::pair<iterator,bool>(make_iterator(p.first),p.second);
			  }

			  iterator insert(iterator position,value_param_type x)
			  {
				  std::pair<final_node_type*,bool> p=this->final_insert_(
					  x,static_cast<final_node_type*>(position.get_node()));
				  return make_iterator(p.first);
			  }

			  template<typename InputIterator>
			  void insert(InputIterator first,InputIterator last)
			  {
				  iterator hint=end();
				  for(;first!=last;++first)hint=insert(hint,*first);
			  }

			  node_type* insert_(value_param_type v,node_type* x)
			  {
				  link_info inf;
				  if(!link_point(key(v),inf,Category())){
					  return node_type::from_impl(inf.pos);
				  }

				  node_type* res=static_cast<node_type*>(super::insert_(v,x));
				  if(res==x){
					  node_impl_type::link(x->impl(),inf.side,inf.pos,header()->impl());
				  }
				  return res;
			  }

			  node_type* insert_(value_param_type v,node_type* position,node_type* x)
			  {
				  link_info inf;
				  if(!hinted_link_point(key(v),position,inf,Category())){
					  return node_type::from_impl(inf.pos);
				  }

				  node_type* res=static_cast<node_type*>(super::insert_(v,position,x));
				  if(res==x){
					  node_impl_type::link(x->impl(),inf.side,inf.pos,header()->impl());
				  }
				  return res;
			  }

			  iterator erase(iterator position)
			  {
				  this->final_erase_(static_cast<final_node_type*>(position++.get_node()));
				  return position;
			  }

			  size_type erase(key_param_type x)
			  {
				  std::pair<iterator,iterator> p=equal_range(x);
				  size_type s=0;
				  while(p.first!=p.second){
					  p.first=erase(p.first);
					  ++s;
				  }
				  return s;
			  }

			  iterator erase(iterator first,iterator last)
			  {
				  while(first!=last){
					  first=erase(first);
				  }
				  return first;
			  }

			  bool replace(iterator position, const value_param_type& x)
			  {
				  return this->final_replace_(
					  x,static_cast<final_node_type*>(position.get_node()));
			  }

			  template<typename Modifier>
			  bool modify(iterator position,Modifier mod)
			  {
				  return this->final_modify_(
					  mod,static_cast<final_node_type*>(position.get_node()));
			  }

			  template<typename Modifier,typename Rollback>
			  bool modify(iterator position,Modifier mod,Rollback back)
			  {
				  return this->final_modify_(
					  mod,back,static_cast<final_node_type*>(position.get_node()));
			  }

			  void erase_(node_type* x)
			  {
				  node_impl_type::rebalance_for_erase(
					  x->impl(),header()->parent(),header()->left(),header()->right());
				  super::erase_(x);
			  }

			  void delete_all_nodes_()
			  {
				  delete_all_nodes(root());
			  }

			  void clear()
			  {
				  this->final_clear_();
			  }

			  void clear_()
			  {
				  super::clear_();
				  empty_initialize();
			  }

			  /* set operations */

			  /* Internally, these ops rely on const_iterator being the same
			  * type as iterator.
			  */

			  template<typename CompatibleKey>
			  iterator find(const CompatibleKey& x)const
			  {
				  return make_iterator(ordered_index_find(root(),header(),key,x,comp_));
			  }

			  template<typename CompatibleKey,typename CompatibleCompare>
			  iterator find(
				  const CompatibleKey& x,const CompatibleCompare& comp)const
			  {
				  return make_iterator(ordered_index_find(root(),header(),key,x,comp));
			  }

			  template<typename CompatibleKey>
			  size_type count(const CompatibleKey& x)const
			  {
				  return count(x,comp_);
			  }

			  template<typename CompatibleKey,typename CompatibleCompare>
			  size_type count(const CompatibleKey& x,const CompatibleCompare& comp)const
			  {
				  std::pair<iterator,iterator> p=equal_range(x,comp);
				  size_type n=std::distance(p.first,p.second);
				  return n;
			  }

			  template<typename CompatibleKey>
			  iterator lower_bound(const CompatibleKey& x)const
			  {
				  return make_iterator(
					  ordered_index_lower_bound(root(),header(),key,x,comp_));
			  }

			  template<typename CompatibleKey,typename CompatibleCompare>
			  iterator lower_bound(
				  const CompatibleKey& x,const CompatibleCompare& comp)const
			  {
				  return make_iterator(
					  ordered_index_lower_bound(root(),header(),key,x,comp));
			  }

			  template<typename CompatibleKey>
			  iterator upper_bound(const CompatibleKey& x)const
			  {
				  return make_iterator(
					  ordered_index_upper_bound(root(),header(),key,x,comp_));
			  }

			  template<typename CompatibleKey,typename CompatibleCompare>
			  iterator upper_bound(
				  const CompatibleKey& x,const CompatibleCompare& comp)const
			  {
				  return make_iterator(
					  ordered_index_upper_bound(root(),header(),key,x,comp));
			  }

			  template<typename CompatibleKey>
			  std::pair<iterator,iterator> equal_range(
				  const CompatibleKey& x)const
			  {
				  std::pair<node_type*,node_type*> p=
					  ordered_index_equal_range(root(),header(),key,x,comp_);
				  return std::pair<iterator,iterator>(
					  make_iterator(p.first),make_iterator(p.second));
			  }

			  template<typename CompatibleKey,typename CompatibleCompare>
			  std::pair<iterator,iterator> equal_range(
				  const CompatibleKey& x,const CompatibleCompare& comp)const
			  {
				  std::pair<node_type*,node_type*> p=
					  ordered_index_equal_range(root(),header(),key,x,comp);
				  return std::pair<iterator,iterator>(
					  make_iterator(p.first),make_iterator(p.second));
			  }

			  /* range */

			  template<typename LowerBounder,typename UpperBounder>
			  std::pair<iterator,iterator>
				  range(LowerBounder lower,UpperBounder upper)const
			  {
				  typedef typename if_<
					  is_same<LowerBounder,unbounded_type>::value,
					  typename if_<
					  is_same<UpperBounder,unbounded_type>::value,
					  both_unbounded_tag,
					  lower_unbounded_tag
					  >::type,
					  typename if_<
					  is_same<UpperBounder,unbounded_type>::value,
					  upper_unbounded_tag,
					  none_unbounded_tag
					  >::type
				  >::type dispatch;

				  return range(lower,upper,dispatch());
			  }

			  iterator               begin(){return make_iterator(leftmost());}
			  const_iterator         begin()const{return make_iterator(leftmost());}
			  iterator               end(){return make_iterator(header());}
			  const_iterator         end()const{return make_iterator(header());}
			  const_iterator         cbegin()const{return begin();}
			  const_iterator         cend()const{return end();}

			  bool      empty()const{return this->final_empty_();}
			  size_type size()const{return this->final_size_();}
			  size_type max_size()const{return this->final_max_size_();}

			  iterator       make_iterator(node_type* node){return iterator(node);}
			  const_iterator make_iterator(node_type* node)const
			  {return const_iterator(node);}

		protected:
			bool replace_(value_param_type v,node_type* x)
			{
				if(in_place(v,x,Category())){
					return super::replace_(v,x);
				}

				node_type* next=x;
				node_type::increment(next);

				node_impl_type::rebalance_for_erase(
					x->impl(),header()->parent(),header()->left(),header()->right());

				try{
					link_info inf;
					if(link_point(key(v),inf,Category())&&super::replace_(v,x)){
						node_impl_type::link(x->impl(),inf.side,inf.pos,header()->impl());
						return true;
					}
					node_impl_type::restore(x->impl(),next->impl(),header()->impl());
					return false;
				}
				catch(...){
					node_impl_type::restore(x->impl(),next->impl(),header()->impl());
					throw;
				}
			}

			bool modify_(node_type* x)
			{
				bool b;
				try{
					b=in_place(x->value(),x,Category());
				}
				catch(...){
					erase_(x);
					throw;
				}

				if(!b){
					node_impl_type::rebalance_for_erase(
						x->impl(),header()->parent(),header()->left(),header()->right());
					try{
						link_info inf;
						if(!link_point(key(x->value()),inf,Category())){
							super::erase_(x);
							return false;
						}
						node_impl_type::link(x->impl(),inf.side,inf.pos,header()->impl());
					}
					catch(...){
						super::erase_(x);
						throw;
					}
				}

				try{
					if(!super::modify_(x)){
						node_impl_type::rebalance_for_erase(
							x->impl(),header()->parent(),header()->left(),header()->right());
						return false;
					}
					else return true;
				}
				catch(...){
					node_impl_type::rebalance_for_erase(
						x->impl(),header()->parent(),header()->left(),header()->right());

					throw;
				}
			}

			bool modify_rollback_(node_type* x)
			{
				if(in_place(x->value(),x,Category())){
					return super::modify_rollback_(x);
				}

				node_type* next=x;
				node_type::increment(next);

				node_impl_type::rebalance_for_erase(
					x->impl(),header()->parent(),header()->left(),header()->right());

				try{
					link_info inf;
					if(link_point(key(x->value()),inf,Category())&&
						super::modify_rollback_(x)){
							node_impl_type::link(x->impl(),inf.side,inf.pos,header()->impl());
							return true;
					}
					node_impl_type::restore(x->impl(),next->impl(),header()->impl());
					return false;
				}
				catch(...){
					node_impl_type::restore(x->impl(),next->impl(),header()->impl());
					throw;
				}
			}

		private:
			node_type* header()const{return this->final_header();}
			node_type* root()const{return node_type::from_impl(header()->parent());}
			node_type* leftmost()const{return node_type::from_impl(header()->left());}
			node_type* rightmost()const{return node_type::from_impl(header()->right());}

			void empty_initialize()
			{
				header()->color()=red;
				/* used to distinguish header() from root, in iterator.operator++ */

				header()->parent()=node_impl_pointer(0);
				header()->left()=header()->impl();
				header()->right()=header()->impl();
			}

			struct link_info
			{
				link_info():side(to_left){}

				ordered_index_side side;
				node_impl_pointer  pos;
			};

			bool link_point(key_param_type k,link_info& inf,ordered_unique_tag)
			{
				node_type* y=header();
				node_type* x=root();
				bool c=true;
				while(x){
					y=x;
					c=comp_(k,key(x->value()));
					x=node_type::from_impl(c?x->left():x->right());
				}
				node_type* yy=y;
				if(c){
					if(yy==leftmost()){
						inf.side=to_left;
						inf.pos=y->impl();
						return true;
					}
					else node_type::decrement(yy);
				}

				if(comp_(key(yy->value()),k)){
					inf.side=c?to_left:to_right;
					inf.pos=y->impl();
					return true;
				}
				else{
					inf.pos=yy->impl();
					return false;
				}
			}

			bool link_point(key_param_type k,link_info& inf,ordered_non_unique_tag)
			{
				node_type* y=header();
				node_type* x=root();
				bool c=true;
				while (x){
					y=x;
					c=comp_(k,key(x->value()));
					x=node_type::from_impl(c?x->left():x->right());
				}
				inf.side=c?to_left:to_right;
				inf.pos=y->impl();
				return true;
			}

			bool lower_link_point(key_param_type k,link_info& inf,ordered_non_unique_tag)
			{
				node_type* y=header();
				node_type* x=root();
				bool c=false;
				while (x){
					y=x;
					c=comp_(key(x->value()),k);
					x=node_type::from_impl(c?x->right():x->left());
				}
				inf.side=c?to_right:to_left;
				inf.pos=y->impl();
				return true;
			}

			bool hinted_link_point(
				key_param_type k,node_type* position,link_info& inf,ordered_unique_tag)
			{
				if(position->impl()==header()->left()){ 
					if(size()>0&&comp_(k,key(position->value()))){
						inf.side=to_left;
						inf.pos=position->impl();
						return true;
					}
					else return link_point(k,inf,ordered_unique_tag());
				} 
				else if(position==header()){ 
					if(comp_(key(rightmost()->value()),k)){
						inf.side=to_right;
						inf.pos=rightmost()->impl();
						return true;
					}
					else return link_point(k,inf,ordered_unique_tag());
				} 
				else{
					node_type* before=position;
					node_type::decrement(before);
					if(comp_(key(before->value()),k)&&comp_(k,key(position->value()))){
						if(before->right()==node_impl_pointer(0)){
							inf.side=to_right;
							inf.pos=before->impl();
							return true;
						}
						else{
							inf.side=to_left;
							inf.pos=position->impl();
							return true;
						}
					} 
					else return link_point(k,inf,ordered_unique_tag());
				}
			}

			bool hinted_link_point(
				key_param_type k,node_type* position,link_info& inf,ordered_non_unique_tag)
			{
				if(position->impl()==header()->left()){ 
					if(size()>0&&!comp_(key(position->value()),k)){
						inf.side=to_left;
						inf.pos=position->impl();
						return true;
					}
					else return lower_link_point(k,inf,ordered_non_unique_tag());
				} 
				else if(position==header()){
					if(!comp_(k,key(rightmost()->value()))){
						inf.side=to_right;
						inf.pos=rightmost()->impl();
						return true;
					}
					else return link_point(k,inf,ordered_non_unique_tag());
				} 
				else{
					node_type* before=position;
					node_type::decrement(before);
					if(!comp_(k,key(before->value()))){
						if(!comp_(key(position->value()),k)){
							if(before->right()==node_impl_pointer(0)){
								inf.side=to_right;
								inf.pos=before->impl();
								return true;
							}
							else{
								inf.side=to_left;
								inf.pos=position->impl();
								return true;
							}
						}
						else return lower_link_point(k,inf,ordered_non_unique_tag());
					} 
					else return link_point(k,inf,ordered_non_unique_tag());
				}
			}

			void delete_all_nodes(node_type* x)
			{
				if(!x)return;

				delete_all_nodes(node_type::from_impl(x->left()));
				delete_all_nodes(node_type::from_impl(x->right()));
				this->final_delete_node_(static_cast<final_node_type*>(x));
			}

			bool in_place(value_param_type v,node_type* x,ordered_unique_tag)
			{
				node_type* y;
				if(x!=leftmost()){
					y=x;
					node_type::decrement(y);
					if(!comp_(key(y->value()),key(v)))return false;
				}

				y=x;
				node_type::increment(y);
				return y==header()||comp_(key(v),key(y->value()));
			}

			bool in_place(value_param_type v,node_type* x,ordered_non_unique_tag)
			{
				node_type* y;
				if(x!=leftmost()){
					y=x;
					node_type::decrement(y);
					if(comp_(key(v),key(y->value())))return false;
				}

				y=x;
				node_type::increment(y);
				return y==header()||!comp_(key(y->value()),key(v));
			}

			template<typename LowerBounder,typename UpperBounder>
			std::pair<iterator,iterator>
				range(LowerBounder lower,UpperBounder upper,none_unbounded_tag)const
			{
				node_type* y=header();
				node_type* z=root();

				while(z){
					if(!lower(key(z->value()))){
						z=node_type::from_impl(z->right());
					}
					else if(!upper(key(z->value()))){
						y=z;
						z=node_type::from_impl(z->left());
					}
					else{
						return std::pair<iterator,iterator>(
							make_iterator(
							lower_range(node_type::from_impl(z->left()),z,lower)),
							make_iterator(
							upper_range(node_type::from_impl(z->right()),y,upper)));
					}
				}

				return std::pair<iterator,iterator>(make_iterator(y),make_iterator(y));
			}

			template<typename LowerBounder,typename UpperBounder>
			std::pair<iterator,iterator>
				range(LowerBounder,UpperBounder upper,lower_unbounded_tag)const
			{
				return std::pair<iterator,iterator>(
					begin(),
					make_iterator(upper_range(root(),header(),upper)));
			}

			template<typename LowerBounder,typename UpperBounder>
			std::pair<iterator,iterator>
				range(LowerBounder lower,UpperBounder,upper_unbounded_tag)const
			{
				return std::pair<iterator,iterator>(
					make_iterator(lower_range(root(),header(),lower)),
					end());
			}

			template<typename LowerBounder,typename UpperBounder>
			std::pair<iterator,iterator>
				range(LowerBounder,UpperBounder,both_unbounded_tag)const
			{
				return std::pair<iterator,iterator>(begin(),end());
			}

			template<typename LowerBounder>
			node_type * lower_range(node_type* top,node_type* y,LowerBounder lower)const
			{
				while(top){
					if(lower(key(top->value()))){
						y=top;
						top=node_type::from_impl(top->left());
					}
					else top=node_type::from_impl(top->right());
				}

				return y;
			}

			template<typename UpperBounder>
			node_type * upper_range(node_type* top,node_type* y,UpperBounder upper)const
			{
				while(top){
					if(!upper(key(top->value()))){
						y=top;
						top=node_type::from_impl(top->left());
					}
					else top=node_type::from_impl(top->right());
				}

				return y;
			}

		private:
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
			struct node_class
			{
				typedef ordered_index_node<SuperMeta> type;
			};

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
			struct node_class
			{
				typedef ordered_index_node<SuperMeta> type;
			};

			template<typename SuperMeta>
			struct index_class
			{
				typedef ordered_index<
					key_from_value_type,compare_type,
					SuperMeta,tag_list_type,ordered_non_unique_tag> type;
			};
		};

	}

}

#endif
