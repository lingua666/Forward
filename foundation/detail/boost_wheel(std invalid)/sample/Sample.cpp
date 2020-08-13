// MediaServerSample.cpp : Defines the entry point for the console application.
//

#include <libBoost_Wheel/Bst_Multi_index_container.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <dllLock/Exports.h>
#include <libLock/Lock.hpp>
#include <libCommon/macro/Macro_for.hpp>
#include <typeinfo>

namespace boost{

struct sclw1
{
	typedef sclw1 type;
	sclw1( void )
	{

	}
};

struct sclw2
{
	sclw2( void )
	{

	}
};

struct sclw3
{
	sclw3( void )
	{

	}
};

struct sclw4
{
	sclw4( void )
	{

	}
};

template<int N,typename Value,typename IndexSpecifierList,typename Allocator>
struct nth_layer_test;

template<typename T1,typename T2>
struct nth_layer_impl11 : T1::type, T2::type
{
	//typedef typename if_< (N==IndexSpecifierList::value), sclw1, nth_layer_impl<N + 1, > >::type type;
};

template<int N,typename Value,typename IndexSpecifierList,typename Allocator>
struct nth_layer_impl
{
	typedef nth_layer_impl11< at_c<IndexSpecifierList,N>, nth_layer_test<N+1, Value, IndexSpecifierList, Allocator> > type;
};

template<int N,typename Value,typename IndexSpecifierList,typename Allocator>
struct nth_layer_test
{
	//typedef typename nth_layer_impl<N, Value, IndexSpecifierList, Allocator> type;

	//typedef typename nth_layer_impl<N, nth_layer<N+1,Value,IndexSpecifierList, Allocator>, IndexSpecifierList, Allocator>	type;
	typedef typename if_< (N==IndexSpecifierList::value), sclw1, nth_layer_impl<N,Value,IndexSpecifierList,Allocator> >::type::type type;
};

/*template<int N,typename Value,typename IndexSpecifierList,typename Allocator>
struct nth_layer_impl
{
	typedef typename if_< (N==IndexSpecifierList::value), sclw1, Value >::type type;
};

template<int N,typename Value,typename IndexSpecifierList,typename Allocator>
struct nth_layer : at_c<IndexSpecifierList,N>::type, nth_layer_impl<N+1,nth_layer<N+1,Value,IndexSpecifierList,Allocator>,IndexSpecifierList,Allocator>::type
{
	//typedef typename if_< (N==IndexSpecifierList::value), sclw1, nth_layer<N+1,Value,IndexSpecifierList,Allocator> >::type type;
};*/

template<typename T, typename Allocator>
struct slw87 : nth_layer_test<0, T, T, Allocator>
{
	slw87( void )
	{

	}
};


/*template<typename T1, typename T2, typename Allocator>
struct slw8745 : nth_layer<0, T1, T2, Allocator>::type
{
	slw8745( void )
	{

	}
};*/

}


template<typename Class,typename Type,Type Class::*PtrToMember>
struct TemTest
{
	const Type& operator()(const Class& x) const
	{
		return x.*PtrToMember;
	}
};

struct Employee{
	int id;
	std::string name;
	int age;

	Employee(int id_,std::string name_,int age_):id(id_),name(name_),age(age_){}

	/*friend std::ostream& operator<<(std::ostream& os,const Employee& e)
	{
		os<<e.id<<" "<<e.name<<" "<<e.age<<std::endl;
		return os;
	}*/
};


struct id{};	// 索引-学号
struct name{};  // 索引-姓名
struct age{};	// 索引-年龄

typedef boost::multi_index_container<
	Employee,
	boost::vector<
	boost::ordered_unique<boost::tag<id>, boost::member<Employee, int, &Employee::id> >,
	boost::ordered_non_unique<boost::tag<name>,boost::member<Employee, std::string, &Employee::name> >,
	boost::ordered_non_unique<boost::tag<age>,boost::member<Employee, int, &Employee::age> >
	>
> EmployeeContainer;

typedef EmployeeContainer::nth_index<0>::type IdIndex;
typedef EmployeeContainer::nth_index<1>::type NameIndex;
typedef EmployeeContainer::nth_index<2>::type AgeIndex;

int main(int argc, char* argv[])
{

	typedef std::map<std::string,std::string> Map_type_Test;
	typedef std::multimap<std::string,std::string> MulMap_type_Test;
	

	Map_type_Test Test;
	Test.insert(std::make_pair("0","123"));
	Test.insert(std::make_pair("1","123"));
	Test.insert(std::make_pair("2","123"));

	Map_type_Test::iterator iter = Test.find("0");
	iter ++;
	if( iter == Test.end() )
	{ 
		int i = 0;
	}

	MulMap_type_Test MulTest;
	MulTest.insert(std::make_pair("0","123"));
	MulTest.insert(std::make_pair("1","123"));
	MulTest.insert(std::make_pair("2","123"));

	MulMap_type_Test::iterator iter1 = MulTest.find("0");
	iter1 ++;
	if( iter1 == MulTest.end() )
	{ 
		int i = 0;
	}


	//printf("",boost::ordered_unique<boost::tag<id>, boost::member<Employee, int, &Employee::id> >);
	printf("EmployeeContainer:%s\n",typeid(EmployeeContainer::value_type).name()) ;

	typedef id field_t;
	EmployeeContainer con;

	/*typedef boost::multi_index_container<
		Employee,
		boost::vector<
		boost::ordered_unique<boost::tag<id>, boost::member<Employee, int, &Employee::id> > >
		> test_multi_index;*/
	typedef EmployeeContainer	test_multi_index;

	test_multi_index test_con;

	while(true)
	{
		test_con.insert(Employee(0,"Joe1",31));
		test_multi_index::index< id >::type& indexOfField = test_con.get< id >();
		indexOfField.erase(0);
		Sleep(10);
	}

	if( con.insert(Employee(0,"Joe1",31)).second )
	{
		int s = 0;
	}
	else
	{
		int s = 0;
	}

	con.insert(Employee(1,"Joe1",31));
	con.insert(Employee(2,"Robert",27));
	con.insert(Employee(3,"John",40));

	// 用名字作为索引
	EmployeeContainer::index< field_t >::type& indexOfField = con.get< field_t >();
	indexOfField.erase(2);
	EmployeeContainer::index< field_t >::type::iterator iter_t ;

	for( iter_t = indexOfField.begin(); iter_t != indexOfField.end(); ++ iter_t )
	{
		printf("%d,%s,%d\n",iter_t->id,iter_t->name.c_str(),iter_t->age) ;
	}

	EmployeeContainer::index< name >::type& indexOfName = con.get< field_t >();
	int yl = indexOfName.count("Joe1");

	// 查找名叫李四的人
	EmployeeContainer::index<name>::type::iterator it1 = indexOfName.find("Joe1");
	EmployeeContainer::index<name>::type::iterator it2 = indexOfName.find("Robert");
	IdIndex& ids = con.get<0>();
	NameIndex& ids1 = con.get<1>();
	AgeIndex& ids2 = con.get<2>();

	ids1.erase("Joe1");

	AgeIndex::iterator iter_age ;
	for( iter_age = ids2.begin(); iter_age != ids2.end(); ++ iter_age )
	{
		printf("%d,%s,%d\n",iter_age->id,iter_age->name.c_str(),iter_age->age) ;
	}

	if(it2 != indexOfName.end())
		printf("%d,%s,%d\n",it2->id,it2->name.c_str(),it2->age) ;

	printf("index_type_list:%s\n",typeid(EmployeeContainer::index_type_list).name()) ;

	printf("EmployeeContainer::index:%s\n",typeid(EmployeeContainer::index< id >::type).name()) ;

	//test
	int iy = boost::is_class<boost::sclw4>::value;
	int iu = boost::is_class<long>::value;
	int iu8 = boost::is_class< boost::is_tag<int> >::value;
	int ls = boost::is_same<int,int>::value;
	int ls2 = boost::is_same<boost::sclw4,int>::value;
	int ls3 = boost::is_same<boost::sclw4,boost::sclw4>::value;

	int yiu = boost::is_base_of<boost::sclw4, boost::sclw4>::value;
	int yiu2 = boost::is_base_of<boost::sclw3, boost::sclw4>::value;
	int yiu3 = boost::is_base_of< boost::tag_marker, boost::is_tag<int> >::value;
	int yiu4 = boost::is_base_of< boost::tag_marker, boost::tag<int> >::value;
	int yiu5 = boost::is_base_of< boost::tag_marker, boost::tag<boost::sclw3> >::value;
	int yiu6 = boost::is_base_of< boost::is_tag<int>, boost::tag<boost::sclw3> >::value;

	typedef boost::vector<boost::sclw1,boost::sclw2,boost::sclw3,boost::sclw4> lty; 

	int yuy = boost::size<lty>::type::value;

	typedef boost::sclw3 lty123;
	int ls3444 = boost::is_same<lty123,boost::sclw4>::value;

	typedef boost::vector3<
		boost::ordered_unique<boost::tag<id>, boost::member<Employee, int, &Employee::id> >,
		boost::ordered_non_unique<boost::tag<name>, boost::member<Employee, std::string, &Employee::name> >,
		boost::ordered_non_unique<boost::tag<age>, boost::member<Employee, int, &Employee::age> > > vfind;

	//printf("pop_back:%s\n",typeid(vfind).name()) ;
	printf("find_if:%s\n",typeid(boost::find_if< vfind,boost::tag<name> >::type).name()) ;

	/*int iyu = boost::sequence_size< boost::sequence_tag<vfind>::type >::value;

	printf("push_back:%s\n",typeid(boost::sequence_tag<vfind>::type).name());*/

	lty kk23;
	//boost::at_c<lty,0>::type io = 90;
	boost::slw87<lty,boost::sclw1>::type yy;

	typedef boost::vector< boost::ordered_unique<boost::member<Employee, int, &Employee::id> >,
		boost::ordered_non_unique< boost::member<Employee, std::string, &Employee::name> >,
		boost::ordered_non_unique< boost::member<Employee, int, &Employee::age> > > lty9;
	//boost::slw8745<boost::sclw4, lty9, boost::sclw1> layer;

	printf("push_front:%s\n",typeid(boost::push_front<lty,int>::type).name());
	printf("push_back:%s\n",typeid(boost::push_back<lty,int>::type).name());

	printf("pop_front:%s\n",typeid(boost::pop_front<lty>::type).name());
	printf("pop_back:%s\n",typeid(boost::pop_back<lty>::type).name());
	//test

	for( int i = 0; i < 100000000; i ++)
	{
		while(true)
		{
			HLOCK hLock = MLock_Alloc();
			MLock_Lock(hLock);
			MLock_UnLock(hLock);
			MLock_Free(hLock);
			Sleep(100);
		}
	}

	CLock	Lock;
	int time = GetTickCount();
	for( int i = 0; i < 100000000; i ++)
	{
		Lock.Lock();
		Lock.UnLock();
	}
	printf("time:%d\r\n", GetTickCount() - time);

	std::cin.get();
	return 0;
}