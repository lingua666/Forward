#ifndef __BST_ORD_INDEX_NODE_20191021232104_1442199360_H__
#define __BST_ORD_INDEX_NODE_20191021232104_1442199360_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

namespace boost{

enum ordered_index_color{red=false,black=true};
enum ordered_index_side{to_left=false,to_right=true};

template<typename Allocator>
struct ordered_index_node_impl; /* fwd decl. */

template<typename Allocator>
struct ordered_index_node_std_base
{
  typedef ordered_index_node_impl<Allocator>      pointer;
  typedef pointer                          		  const_pointer;
  typedef ordered_index_color&                    color_ref;
  typedef pointer&                                parent_ref;

  ordered_index_color& color(){return color_;}
  ordered_index_color  color()const{return color_;}
  pointer&             parent(){return parent_;}
  pointer              parent()const{return parent_;}
  pointer&             left(){return left_;}
  pointer              left()const{return left_;}
  pointer&             right(){return right_;}
  pointer              right()const{return right_;}

private:
  ordered_index_color color_; 
  pointer             parent_;
  pointer             left_;
  pointer             right_;
};

template<typename Allocator>
struct ordered_index_node_impl_base: ordered_index_node_std_base<Allocator>
{};

template<typename Allocator>
struct ordered_index_node_impl: ordered_index_node_impl_base<Allocator>
{
private:
  typedef ordered_index_node_impl_base<Allocator> super;

public:
  typedef typename super::color_ref               color_ref;
  typedef typename super::parent_ref              parent_ref;
  typedef typename super::pointer                 pointer;
  typedef typename super::const_pointer           const_pointer;

  /* interoperability with bidir_node_iterator */

  static void increment(pointer& x)
  {
    if(x->right()!=pointer(0)){
      x=x->right();
      while(x->left()!=pointer(0))x=x->left();
    }
    else{
      pointer y=x->parent();
      while(x==y->right()){
        x=y;
        y=y->parent();
      }
      if(x->right()!=y)x=y;
    }
  }

  static void decrement(pointer& x)
  {
    if(x->color()==red&&x->parent()->parent()==x){
      x=x->right();
    }
    else if(x->left()!=pointer(0)){
      pointer y=x->left();
      while(y->right()!=pointer(0))y=y->right();
      x=y;
    }else{
      pointer y=x->parent();
      while(x==y->left()){
        x=y;
        y=y->parent();
      }
      x=y;
    }
  }

  /* algorithmic stuff */

  static void rotate_left(pointer x,parent_ref root)
  {
    pointer y=x->right();
    x->right()=y->left();
    if(y->left()!=pointer(0))y->left()->parent()=x;
    y->parent()=x->parent();
    
    if(x==root)                    root=y;
    else if(x==x->parent()->left())x->parent()->left()=y;
    else                           x->parent()->right()=y;
    y->left()=x;
    x->parent()=y;
  }

  static pointer minimum(pointer x)
  {
    while(x->left()!=pointer(0))x=x->left();
    return x;
  }

  static pointer maximum(pointer x)
  {
    while(x->right()!=pointer(0))x=x->right();
    return x;
  }

  static void rotate_right(pointer x,parent_ref root)
  {
    pointer y=x->left();
    x->left()=y->right();
    if(y->right()!=pointer(0))y->right()->parent()=x;
    y->parent()=x->parent();

    if(x==root)                     root=y;
    else if(x==x->parent()->right())x->parent()->right()=y;
    else                            x->parent()->left()=y;
    y->right()=x;
    x->parent()=y;
  }

  static void rebalance(pointer x,parent_ref root)
  {
    x->color()=red;
    while(x!=root&&x->parent()->color()==red){
      if(x->parent()==x->parent()->parent()->left()){
        pointer y=x->parent()->parent()->right();
        if(y!=pointer(0)&&y->color()==red){
          x->parent()->color()=black;
          y->color()=black;
          x->parent()->parent()->color()=red;
          x=x->parent()->parent();
        }
        else{
          if(x==x->parent()->right()){
            x=x->parent();
            rotate_left(x,root);
          }
          x->parent()->color()=black;
          x->parent()->parent()->color()=red;
          rotate_right(x->parent()->parent(),root);
        }
      }
      else{
        pointer y=x->parent()->parent()->left();
        if(y!=pointer(0)&&y->color()==red){
          x->parent()->color()=black;
          y->color()=black;
          x->parent()->parent()->color()=red;
          x=x->parent()->parent();
        }
        else{
          if(x==x->parent()->left()){
            x=x->parent();
            rotate_right(x,root);
          }
          x->parent()->color()=black;
          x->parent()->parent()->color()=red;
          rotate_left(x->parent()->parent(),root);
        }
      }
    }
    root->color()=black;
  }

  static void link(
    pointer x,ordered_index_side side,pointer position,pointer header)
  {
    if(side==to_left){
      position->left()=x;  /* also makes leftmost=x when parent==header */
      if(position==header){
        header->parent()=x;
        header->right()=x;
      }
      else if(position==header->left()){
        header->left()=x;  /* maintain leftmost pointing to min node */
      }
    }
    else{
      position->right()=x;
      if(position==header->right()){
        header->right()=x; /* maintain rightmost pointing to max node */
      }
    }
    x->parent()=position;
    x->left()=pointer(0);
    x->right()=pointer(0);
    ordered_index_node_impl::rebalance(x,header->parent());
  }

  static pointer rebalance_for_erase(
    pointer z,parent_ref root,pointer& leftmost,pointer& rightmost)
  {
    pointer y=z;
    pointer x=pointer(0);
    pointer x_parent=pointer(0);
    if(y->left()==pointer(0)){    /* z has at most one non-null child. y==z. */
      x=y->right();               /* x might be null */
    }
    else{
      if(y->right()==pointer(0)){ /* z has exactly one non-null child. y==z. */
        x=y->left();              /* x is not null */
      }
      else{                       /* z has two non-null children.  Set y to */
        y=y->right();             /* z's successor. x might be null.        */
        while(y->left()!=pointer(0))y=y->left();
        x=y->right();
      }
    }
    if(y!=z){
      z->left()->parent()=y;   /* relink y in place of z. y is z's successor */
      y->left()=z->left();
      if(y!=z->right()){
        x_parent=y->parent();
        if(x!=pointer(0))x->parent()=y->parent();
        y->parent()->left()=x; /* y must be a child of left */
        y->right()=z->right();
        z->right()->parent()=y;
      }
      else{
        x_parent=y;
      }

      if(root==z)                    root=y;
      else if(z->parent()->left()==z)z->parent()->left()=y;
      else                           z->parent()->right()=y;
      y->parent()=z->parent();
      ordered_index_color c=y->color();
      y->color()=z->color();
      z->color()=c;
      y=z;                    /* y now points to node to be actually deleted */
    }
    else{                     /* y==z */
      x_parent=y->parent();
      if(x!=pointer(0))x->parent()=y->parent();   
      if(root==z){
        root=x;
      }
      else{
        if(z->parent()->left()==z)z->parent()->left()=x;
        else                      z->parent()->right()=x;
      }
      if(leftmost==z){
        if(z->right()==pointer(0)){ /* z->left() must be null also */
          leftmost=z->parent();
        }
        else{              
          leftmost=minimum(x);      /* makes leftmost==header if z==root */
        }
      }
      if(rightmost==z){
        if(z->left()==pointer(0)){  /* z->right() must be null also */
          rightmost=z->parent();
        }
        else{                   /* x==z->left() */
          rightmost=maximum(x); /* makes rightmost==header if z==root */
        }
      }
    }
    if(y->color()!=red){
      while(x!=root&&(x==pointer(0)|| x->color()==black)){
        if(x==x_parent->left()){
          pointer w=x_parent->right();
          if(w->color()==red){
            w->color()=black;
            x_parent->color()=red;
            rotate_left(x_parent,root);
            w=x_parent->right();
          }
          if((w->left()==pointer(0)||w->left()->color()==black) &&
             (w->right()==pointer(0)||w->right()->color()==black)){
            w->color()=red;
            x=x_parent;
            x_parent=x_parent->parent();
          } 
          else{
            if(w->right()==pointer(0 )
                || w->right()->color()==black){
              if(w->left()!=pointer(0)) w->left()->color()=black;
              w->color()=red;
              rotate_right(w,root);
              w=x_parent->right();
            }
            w->color()=x_parent->color();
            x_parent->color()=black;
            if(w->right()!=pointer(0))w->right()->color()=black;
            rotate_left(x_parent,root);
            break;
          }
        } 
        else{                   /* same as above,with right <-> left */
          pointer w=x_parent->left();
          if(w->color()==red){
            w->color()=black;
            x_parent->color()=red;
            rotate_right(x_parent,root);
            w=x_parent->left();
          }
          if((w->right()==pointer(0)||w->right()->color()==black) &&
             (w->left()==pointer(0)||w->left()->color()==black)){
            w->color()=red;
            x=x_parent;
            x_parent=x_parent->parent();
          }
          else{
            if(w->left()==pointer(0)||w->left()->color()==black){
              if(w->right()!=pointer(0))w->right()->color()=black;
              w->color()=red;
              rotate_left(w,root);
              w=x_parent->left();
            }
            w->color()=x_parent->color();
            x_parent->color()=black;
            if(w->left()!=pointer(0))w->left()->color()=black;
            rotate_right(x_parent,root);
            break;
          }
        }
      }
      if(x!=pointer(0))x->color()=black;
    }
    return y;
  }

  static void restore(pointer x,pointer position,pointer header)
  {
    if(position->left()==pointer(0)||position->left()==header){
      link(x,to_left,position,header);
    }
    else{
      decrement(position);
      link(x,to_right,position,header);
    }
  }

#if defined(BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING)
  /* invariant stuff */

  static std::size_t black_count(pointer node,pointer root)
  {
    if(node==pointer(0))return 0;
    std::size_t sum=0;
    for(;;){
      if(node->color()==black)++sum;
      if(node==root)break;
      node=node->parent();
    } 
    return sum;
  }
#endif
};

}

#endif
