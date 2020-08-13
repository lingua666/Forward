
#ifndef __TYPE_BASIS_20160418173305_1438860785_H__
#define __TYPE_BASIS_20160418173305_1438860785_H__

struct na
{
	typedef na type;
	enum { value = 0 };
};

template<bool bl, typename T1, typename T2>
struct if_
{
	typedef T1 type;
};

template <typename T1, typename T2>
struct if_<false, T1, T2>
{
	typedef T2 type;
};

template< bool bl, typename F1 = na, typename F2 = na >
struct eval_if
{
	typedef typename if_<bl,F1,F2>::type f_;
	typedef typename f_::type type;
};

template<
	bool C
	, typename F1
	, typename F2
>
struct eval_if_c : if_<C,F1,F2>::type
{

};

template<typename T>
struct is_const
{	
	static const bool value = false;
};

template<typename T>
struct is_const<const T>
{	
	static const bool value = true;
};


template<typename T>
struct is_const<const volatile T>
{	
	static const bool value = true;
};


template< typename T >
struct is_na
{
	static const bool value = false;
};

template<>
struct is_na<na>
{
	static const bool value = true;
};

template <typename T>
class is_class {
	template <typename U>
	static char helper(int U::*);
	template <typename U>
	static int helper(...);
public:
	static const bool value = (sizeof(helper<T>(0)) == 1);
};

template <typename T1, typename T2>
struct is_same 
{
	static const bool value = false;
};

template <typename T>
struct is_same<T, T>
{
	static const bool value = true;
};

template<typename Base, typename Derived, bool = (is_class<Base>::value && is_class<Derived>::value)>
class is_base_of 
{
	template <typename T>
	static char helper(Derived, T);
	static int helper(Base, int);
	struct Conv 
	{
		operator Derived();
		operator Base() const;
	};
public:
	static const bool value = sizeof(helper(Conv(), 0)) == 1;
};

template <typename Base, typename Derived>
struct is_base_of<Base, Derived, false> 
{
	static const bool value = is_same<Base, Derived>::value;
};

template <typename Base> 
struct is_base_of<Base, Base, true> 
{ 
	static const bool value = true; 
};

template <typename T>
struct is_pointer 
{
	static const bool value = false;
};

template <typename T> 
struct is_pointer<T*> 
{ 
	static const bool value = true; 
};

template<typename T>
struct ret_init
{
	static T init( void )
	{ return T(); }
};

template<typename T>
struct ret_init<T*>
{
	static T* init( void )
	{ return 0; }
};

template<class T> struct addr_impl_ref
{
	T & v_;

	inline addr_impl_ref( T & v ): v_( v ) {}
	inline operator T& () const { return v_; }

private:
	addr_impl_ref & operator=(const addr_impl_ref &);
};

template<class T> struct addressof_impl
{
	static inline T * f( T & v, long )
	{
		return reinterpret_cast<T*>(
			&const_cast<char&>(reinterpret_cast<const volatile char &>(v)));
	}

	static inline T * f( T * v, int )
	{
		return v;
	}
};

template<class T> T * addressof( T & v )
{
	return addressof_impl<T>::f( addr_impl_ref<T>( v ), 0 );
}

template<class T> class reference_wrapper
{ 
public:
	typedef T type;

	explicit reference_wrapper(T& t): t_(addressof(t)) {}

	operator T& () const { return *t_; }

	T& get() const { return *t_; }

	T* get_pointer() const { return t_; }

private:

	T* t_;
};

template<class T> inline reference_wrapper<T> const ref(T & t)
{ 
	return reference_wrapper<T>(t);
}

template<class T> inline reference_wrapper<T const> const cref(T const & t)
{
	return reference_wrapper<T const>(t);
}

template< typename T >
struct identity 
{
	typedef T type;
};


// Type modifiers
// TEMPLATE CLASS remove_const
template<class _Ty>
struct remove_const
{	// remove top level const qualifier
	typedef _Ty type;
};

template<class _Ty>
struct remove_const<const _Ty>
{	// remove top level const qualifier
	typedef _Ty type;
};

template<class _Ty>
struct remove_const<const _Ty[]>
{	// remove top level const qualifier
	typedef _Ty type[];
};

template<class _Ty, unsigned int _Nx>
struct remove_const<const _Ty[_Nx]>
{	// remove top level const qualifier
	typedef _Ty type[_Nx];
};

// TEMPLATE CLASS remove_volatile
template<class _Ty>
struct remove_volatile
{	// remove top level volatile qualifier
	typedef _Ty type;
};

template<class _Ty>
struct remove_volatile<volatile _Ty>
{	// remove top level volatile qualifier
	typedef _Ty type;
};

template<class _Ty>
struct remove_volatile<volatile _Ty[]>
{	// remove top level volatile qualifier
	typedef _Ty type[];
};

template<class _Ty, unsigned int _Nx>
struct remove_volatile<volatile _Ty[_Nx]>
{	// remove top level volatile qualifier
	typedef _Ty type[_Nx];
};

// TEMPLATE CLASS remove_cv
template<class _Ty>
struct remove_cv
{	// remove top level const and volatile qualifiers
	typedef typename remove_const<typename remove_volatile<_Ty>::type>::type
		type;
};

class noncopyable
{
protected:
	noncopyable() {}
	~noncopyable() {}
private:  // emphasize the following members are private
	noncopyable( const noncopyable& );
	noncopyable& operator=( const noncopyable& );
};

// TEMPLATE CLASS is_member_object_pointer
/*template<class _Ty>
struct _Is_member_object_pointer
: false_type
{	// determine whether _Ty is a pointer to member object
};

template<class _Ty1, class _Ty2>
struct _Is_member_object_pointer<_Ty1 _Ty2::*>
: _Cat_base<!_Is_memfunptr<_Ty1 _Ty2::*>::value>
{	// determine whether _Ty is a pointer to member object
};

template<class _Ty>
struct is_member_object_pointer
: _Is_member_object_pointer<typename remove_cv<_Ty>::type>
{	// determine whether _Ty is a pointer to member object
};

// TEMPLATE CLASS is_member_function_pointer
template<class _Ty>
struct is_member_function_pointer
: _Cat_base<_Is_memfunptr<typename remove_cv<_Ty>::type>::value>
{	// determine whether _Ty is a pointer to member function
};*/


#endif // __CNONCOPY_20160418173305_1438860785_H__
