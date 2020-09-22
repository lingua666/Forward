// MediaServerSample.cpp : Defines the entry point for the console application.
//

#include <libBoost_Wheel/Bst_Multi_index_container.hpp>
#include <libBoost_Wheel/Bst_Typeof.hpp>
#include <libContainer/CMyUnordered_map.hpp>
#include <map>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <dllLock/Exports.h>
#include <libLock/Lock.hpp>
#include <libCommon/macro/Macro_for.hpp>
#include <libCommon/modafx.h>
#include <typeinfo>

#define NUM1_DEC	1000000//30000000//100000000

using namespace _container_;

struct Employee_test{
	int id;
	std::string name;
	std::string  age;

	Employee_test(int id_,std::string name_,std::string age_):id(id_),name(name_),age(age_){}
};

struct id_test{};	// 索引-学号
struct name_test{};  // 索引-姓名
struct age_test{};	// 索引-年龄

typedef boost_wheel::multi_index::multi_index_container<
	Employee_test,
	boost_wheel::vector<
	boost_wheel::multi_index::ordered_unique<boost_wheel::tag<id_test>, boost_wheel::member<Employee_test, int, &Employee_test::id> >,
	boost_wheel::multi_index::ordered_non_unique<boost_wheel::tag<name_test>,boost_wheel::member<Employee_test, std::string, &Employee_test::name> >,
	boost_wheel::multi_index::ordered_non_unique<boost_wheel::tag<age_test>,boost_wheel::member<Employee_test, std::string, &Employee_test::age> >
	>
> EmployeeContainer_test;


void MapTest( void )
{
	/*{
		int iNum = NUM1_DEC;
		int iTimer = 0;
		unordered_map_tp<int,int>	m;

		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			m.insert(std::make_pair(i,i));
		}
		printf("unordered_map<int,int> insert timer:%d\r\n", GetTickCount() - iTimer);

		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			m.find(iNum - 1);
		}
		printf("unordered_map<int,int> find timer:%d\r\n", GetTickCount() - iTimer);
	}

	{
		int iNum = NUM1_DEC;
		int iTimer = 0;
		unordered_map_tp<int,std::string>	m;
		char sz[100] = {0};

		int msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			__itoa__(1000000000 + i,sz,10);
			iTimer = GetTickCount();
			m.insert(std::make_pair(i,sz));
			msecond += GetTickCount() - iTimer;
		}
		printf("unordered_map<int,string> insert timer:%d\r\n", msecond);

		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			m.find(iNum - 1);
		}
		printf("unordered_map<int,string> find timer:%d\r\n", GetTickCount() - iTimer);
	}

	{
		int iNum = NUM1_DEC;
		int iTimer = 0;
		unordered_map_tp<std::string,int>	m;

		int msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			char sz[100] = {0};
			__itoa__(iNum + i,sz,10);
			iTimer = GetTickCount();
			m.insert(std::make_pair(sz,i));
			msecond += GetTickCount() - iTimer;
		}
		printf("unordered_map<string,int> insert timer:%d\r\n", msecond);

		msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			char sz[100] = {0};
			__itoa__(iNum + i,sz,10);
			iTimer = GetTickCount();
			m.find(sz);
			msecond += GetTickCount() - iTimer;
		}
		printf("unordered_map<string,int> find timer:%d\r\n", msecond);
	}

	{
		int iNum = NUM1_DEC;
		int iTimer = 0;
		unordered_map_tp<std::string,std::string>	m;

		int msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			char Uid[60] = {0};
			sprintf(Uid, "1B000001000120160530000000000ZZZ100000000000000000%d", i);
			char sz[100] = {0};
			__itoa__(iNum + i,sz,10);
			iTimer = GetTickCount();
			//m.insert(std::make_pair(Uid,sz));
			msecond += GetTickCount() - iTimer;
		}
		printf("unordered_map<std::string,std::string> insert timer:%d\r\n", msecond);

		msecond = 0;
		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			char Uid[60] = {0};
			sprintf(Uid, "1B000001000120160530000000000ZZZ100000000000000000%d", i);
			char sz[100] = {0};
			__itoa__(iNum + i,sz,10);
			iTimer = GetTickCount();
			//m.find(Uid);
			msecond += GetTickCount() - iTimer;
		}
		printf("unordered_map<std::string,std::string> find timer:%d\r\n", msecond);
	}*/


	{
		int iNum = NUM1_DEC;
		int iTimer = 0;
		std::map<int,int>	m;

		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			m.insert(std::make_pair(i,i));
		}
		printf("map<int,int> insert timer:%d\r\n", GetTickCount() - iTimer);

		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			m.find(iNum - 1);
		}
		printf("map<int,int> find timer:%d\r\n", GetTickCount() - iTimer);
	}

	{
		int iNum = NUM1_DEC;
		int iTimer = 0;
		std::map<int,std::string>	m;
		char sz[100] = {0};

		int msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			__itoa__(iNum + i,sz,10);
			iTimer = GetTickCount();
			m.insert(std::make_pair(i,sz));
			msecond += GetTickCount() - iTimer;
		}
		printf("map<int,string> insert timer:%d\r\n", msecond);

		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			m.find(iNum - 1);
		}
		printf("map<int,string> find timer:%d\r\n", GetTickCount() - iTimer);
	}

	{
		int iNum = NUM1_DEC;
		int iTimer = 0;
		std::map<std::string,int>	m;

		int msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			char sz[100] = {0};
			__itoa__(iNum + i,sz,10);
			iTimer = GetTickCount();
			m.insert(std::make_pair(sz,i));
			msecond += GetTickCount() - iTimer;
		}
		printf("map<string,int> insert timer:%d\r\n", msecond);

		msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			char sz[100] = {0};
			__itoa__(iNum + i,sz,10);
			iTimer = GetTickCount();
			m.find(sz);
			msecond += GetTickCount() - iTimer;
		}
		printf("map<string,int> find timer:%d\r\n", msecond);
	}

	{
		int iNum = NUM1_DEC;
		int iTimer = 0;
		std::map<std::string,std::string>	m;

		int msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			char Uid[60] = {0};
			sprintf(Uid, "1B000001000120160530000000000ZZZ100000000000000000%d", i);
			char sz[100] = {0};
			__itoa__(iNum + i,sz,10);
			iTimer = GetTickCount();
			m.insert(std::make_pair(Uid,sz));
			msecond += GetTickCount() - iTimer;
		}
		printf("map<std::string,std::string> insert timer:%d\r\n", msecond);

		msecond = 0;
		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			char Uid[60] = {0};
			sprintf(Uid, "1B000001000120160530000000000ZZZ100000000000000000%d", i);
			char sz[100] = {0};
			__itoa__(iNum + i,sz,10);
			iTimer = GetTickCount();
			m.find(Uid);
			msecond += GetTickCount() - iTimer;
		}
		printf("map<std::string,std::string> find timer:%d\r\n", msecond);
	}



	#define multi_index_container_t boost_wheel::multi_index::multi_index_container
	#define vector_t				boost_wheel::vector	
	#define ordered_non_unique_t	boost_wheel::multi_index::ordered_non_unique
	#define member_t				boost_wheel::member	
	#define tag_t					boost_wheel::tag

	{
		typedef multi_index_container_t<
			Employee_test,
			vector_t<
			ordered_non_unique_t<tag_t<id_test>, member_t<Employee_test, int, &Employee_test::id> > >
		> test_multi_index;

		test_multi_index con;

		int iNum = NUM1_DEC;
		int iTimer = 0;

		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			con.insert(Employee_test(i,"Joe1",""));
		}
		printf("multi_index_container<int,int> insert timer:%d\r\n", GetTickCount() - iTimer);

		/*iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			m.find(iNum - 1);
		}
		printf("map<int,int> find timer:%d\r\n", GetTickCount() - iTimer);*/
	}

	{
		typedef multi_index_container_t<
			Employee_test,
			vector_t<
			ordered_non_unique_t<tag_t<id_test>, member_t<Employee_test, int, &Employee_test::id> >,
			ordered_non_unique_t<tag_t<name_test>,member_t<Employee_test, std::string, &Employee_test::name> > >
		> test_multi_index;

		test_multi_index con;

		int iNum = NUM1_DEC;
		int iTimer = 0;
		char sz[100] = {0};

		int msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			__itoa__(iNum + i,sz,10);
			iTimer = GetTickCount();
			con.insert(Employee_test(i,sz,""));
			msecond += GetTickCount() - iTimer;
		}
		printf("multi_index_container insert timer:%d\r\n", msecond);

		test_multi_index::index< id_test >::type& indexOID = con.get< id_test >();
		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			test_multi_index::index<id_test>::type::iterator it2 = indexOID.find(i);
			if( it2 == indexOID.end() )
			{
				printf("Key:%d Not Found!!\r\n", i);
			}
		}
		printf("multi_index_container<int,string> find timer:%d\r\n", GetTickCount() - iTimer);
	}

	{
		typedef multi_index_container_t<
			Employee_test,
			vector_t<
			ordered_non_unique_t< tag_t<name_test>,member_t<Employee_test, std::string, &Employee_test::name> > >
		> test_multi_index;

		test_multi_index con;

		int iNum = NUM1_DEC;
		int iTimer = 0;

		int msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			char sz[100] = {0};
			__itoa__(iNum + i,sz,10);
			iTimer = GetTickCount();
			con.insert(Employee_test(i,sz,""));
			msecond += GetTickCount() - iTimer;
		}
		printf("multi_index_container<string,int> insert timer:%d\r\n", msecond);

		test_multi_index::index< name_test >::type& indexOfName = con.get< name_test >();
		msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			char sz[100] = {0};
			__itoa__(iNum + i,sz,10);
			iTimer = GetTickCount();
			test_multi_index::index<name_test>::type::iterator it2 = indexOfName.find(sz);
			if( it2 == indexOfName.end() )
			{
				printf("Key:%s Not Found!!\r\n", sz);
			}
			msecond += GetTickCount() - iTimer;
		}
		printf("multi_index_container<string,int> find timer:%d\r\n", msecond);
	}

	{
		typedef multi_index_container_t<
			Employee_test,
			vector_t<
			ordered_non_unique_t< tag_t<name_test>,member_t<Employee_test, std::string, &Employee_test::name> >,
			ordered_non_unique_t< tag_t<age_test>,member_t<Employee_test, std::string, &Employee_test::age> >>
		> test_multi_index;

		test_multi_index con;

		int iNum = NUM1_DEC;
		int iTimer = 0;

		int msecond = 0;
		for( int i = 0 ; i < iNum; i ++)
		{
			char Uid[60] = {0};
			sprintf(Uid, "1B000001000120160530000000000ZZZ100000000000000000%d", i);
			char sz[100] = {0};
			__itoa__(iNum + i,sz,10);
			iTimer = GetTickCount();
			con.insert(Employee_test(i,Uid,sz));
			msecond += GetTickCount() - iTimer;
		}
		printf("multi_index_container<std::string,std::string> insert timer:%d\r\n", msecond);

		msecond = 0;
		test_multi_index::index< name_test >::type& indexOfName = con.get< name_test >();
		iTimer = GetTickCount();
		for( int i = 0 ; i < iNum; i ++)
		{
			char Uid[60] = {0};
			sprintf(Uid, "1B000001000120160530000000000ZZZ100000000000000000%d", i);
			char sz[100] = {0};
			__itoa__(iNum + i,sz,10);
			iTimer = GetTickCount();
			test_multi_index::index<name_test>::type::iterator it2 = indexOfName.find(Uid);
			if( it2 == indexOfName.end() )
			{
				printf("Key:%s Not Found!!\r\n", sz);
			}
			msecond += GetTickCount() - iTimer;
		}
		printf("multi_index_container<std::string,std::string> find timer:%d\r\n", msecond);
	}

	printf("end\r\n");
}

struct Employee{
	int id;
	std::string name;
	int age;

	Employee(int id_,std::string name_,int age_):id(id_),name(name_),age(age_){}

	std::string	Get_name(void)
	{
		return name;
	}

	int	Get_age(void)
	{
		return age;
	}

	int	Get_id(void)
	{
		return id;
	}

	/*friend std::ostream& operator<<(std::ostream& os,const Employee& e)
	{
		os<<e.id<<" "<<e.name<<" "<<e.age<<std::endl;
		return os;
	}*/
};


struct id{};	// 索引-学号
struct name{};  // 索引-姓名
struct age{};	// 索引-年龄

typedef SmartPTR<Employee>	Employee_sptr;

typedef boost_wheel::multi_index::multi_index_container<
	Employee,
	boost_wheel::vector<
	boost_wheel::multi_index::ordered_unique<boost_wheel::tag<id>, boost_wheel::member<Employee, int, &Employee::id> >,
	boost_wheel::multi_index::ordered_non_unique<boost_wheel::tag<name>,boost_wheel::member<Employee, std::string, &Employee::name> >,
	boost_wheel::multi_index::ordered_non_unique<boost_wheel::tag<age>,boost_wheel::member<Employee, int, &Employee::age> >
	>
> EmployeeContainer;

typedef EmployeeContainer::nth_index<0>::type IdIndex;
typedef EmployeeContainer::nth_index<1>::type NameIndex;
typedef EmployeeContainer::nth_index<2>::type AgeIndex;

void EmployeeContainer_Test( void )
{
		typedef boost_wheel::multi_index::multi_index_node_type<
		Employee,
		boost_wheel::vector<
		boost_wheel::multi_index::ordered_unique<boost_wheel::tag<id>, boost_wheel::member<Employee, int, &Employee::id> >,
		boost_wheel::multi_index::ordered_non_unique<boost_wheel::tag<name>,boost_wheel::member<Employee, std::string, &Employee::name> >,
		boost_wheel::multi_index::ordered_non_unique<boost_wheel::tag<age>,boost_wheel::member<Employee, int, &Employee::age> >
		>
	>::type node_type_;

	printf("multi_index_node_type:%s\n",typeid(node_type_).name()) ;

	node_type_ slw_;
	slw_.color()=boost_wheel::multi_index::red;

	boost_wheel::multi_index::multi_index_node_type<Employee,
		boost_wheel::vector<
		boost_wheel::multi_index::ordered_unique<boost_wheel::tag<id>, boost_wheel::member<Employee, int, &Employee::id> > >
		>::type ldw;

	typedef std::map<std::string,std::string> Map_type_Test;
	typedef std::multimap<std::string,std::string> MulMap_type_Test;

	EmployeeContainer::node_type llwe;
	

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

	printf("EmployeeContainer::node_type:%s\n",typeid(EmployeeContainer::node_type).name()) ;
	printf("EmployeeContainer::iterator:%s\n",typeid(EmployeeContainer::iterator).name()) ;

	typedef boost_wheel::multi_index::multi_index_container<
		Employee,
		boost_wheel::vector<
		boost_wheel::multi_index::ordered_unique<boost_wheel::tag<id>, boost_wheel::member<Employee, int, &Employee::id> > >
		> test_multi_index;
	//typedef EmployeeContainer	test_multi_index;

	test_multi_index test_con;

	/*while(true)
	{
		test_con.insert(Employee(0,"Joe1",31));
		test_multi_index::index< id >::type& indexOfField = test_con.get< id >();
		test_con.erase(0);
		Sleep(10);
	}*/

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
	EmployeeContainer::index< name >::type& indexOfName = con.get< name >();
	int yl = indexOfName.count("Joe1");

	EmployeeContainer::index<name>::type::iterator it1 = indexOfName.find("Joe1");
	indexOfName.erase("Joe1");
	EmployeeContainer::index< field_t >::type& indexOfField = con.get< field_t >();
	//indexOfField.erase(1);
	EmployeeContainer::index< field_t >::type::iterator iter_t ;

	for( iter_t = indexOfField.begin(); iter_t != indexOfField.end(); ++ iter_t )
	{
		printf("%d,%s,%d\n",iter_t->id,iter_t->name.c_str(),iter_t->age) ;
	}



	// 查找名叫李四的人
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
	int iu = is_class<long>::value;
	int iu8 = is_class< boost_wheel::is_tag<int> >::value;
	int ls = is_same<int,int>::value;

	int yiu3 = is_base_of< boost_wheel::tag_marker, boost_wheel::is_tag<int> >::value;
	int yiu4 = is_base_of< boost_wheel::tag_marker, boost_wheel::tag<int> >::value;


	typedef boost_wheel::vector3<
		boost_wheel::multi_index::ordered_unique<boost_wheel::tag<id>, boost_wheel::member<Employee, int, &Employee::id> >,
		boost_wheel::multi_index::ordered_non_unique<boost_wheel::tag<name>, boost_wheel::member<Employee, std::string, &Employee::name> >,
		boost_wheel::multi_index::ordered_non_unique<boost_wheel::tag<age>, boost_wheel::member<Employee, int, &Employee::age> > > vfind;

	//printf("pop_back:%s\n",typeid(vfind).name()) ;
	printf("find_if:%s\n",typeid(boost_wheel::find_if< vfind,boost_wheel::tag<name> >::type).name()) ;

	/*int iyu = boost::sequence_size< boost::sequence_tag<vfind>::type >::value;

	printf("push_back:%s\n",typeid(boost::sequence_tag<vfind>::type).name());*/
};

typedef boost_wheel::multi_index::multi_index_container<
	Employee_sptr,
	boost_wheel::vector<
	boost_wheel::multi_index::ordered_unique<boost_wheel::tag<id>, boost_wheel::member<Employee, int, &Employee::id> >,
	boost_wheel::multi_index::ordered_non_unique<boost_wheel::tag<name>,boost_wheel::member<Employee, std::string, &Employee::name> >,
	boost_wheel::multi_index::ordered_non_unique<boost_wheel::tag<age>,boost_wheel::member<Employee, int, &Employee::age> >
	>
> EmployeeContainer_sptr;

void EmployeeContainer_sptr_Test( void )
{
	EmployeeContainer_sptr con;

	Employee_sptr sptr0( new Employee(Employee(0,"Joe1",31)) );
	Employee_sptr sptr1( new Employee(Employee(1,"Joe1",31)) );
	Employee_sptr sptr2( new Employee(Employee(2,"Robert",27)) );
	Employee_sptr sptr3( new Employee(Employee(3,"John",40)) );

	while(true)
	{
		EmployeeContainer_sptr con1;

		{
			Employee_sptr sptr00( new Employee(Employee(2,"Robert",27)) );
			Employee_sptr sptr11( new Employee(Employee(1,"1Joe1",31)) );
			Employee_sptr sptr22( new Employee(Employee(10,"Joetest",131)) );

			con1.insert(sptr22);
			std::pair<EmployeeContainer_sptr::iterator,bool> liss = con1.insert(sptr22);
			if( liss.second )
			{
				int s = 0;
			}
			else
			{
				EmployeeContainer_sptr::iterator ier = liss.first;
				printf("%d,%s,%d\n",(*ier)->id,(*ier)->name.c_str(),(*ier)->age) ;
				int s = 0;
			}

			con1.insert(sptr00);
			con1.insert(sptr11);
			con1.insert(sptr3);
		}

		EmployeeContainer_sptr::index< name >::type& indexOfName = con1.get< name >();
		EmployeeContainer_sptr::index<name>::type::iterator it2 = indexOfName.find("Joetest");
		if(it2 != indexOfName.end())
			printf("%d,%s,%d\n",(*it2)->id,(*it2)->name.c_str(),(*it2)->age) ;

		int iy = con1.size();
		con1.clear();
		indexOfName.erase("Joetest");
		Sleep(10);
	}

	if( con.insert(sptr0).second )
	{
		int s = 0;
	}
	else
	{
		int s = 0;
	}

	con.insert(sptr1);
	con.insert(sptr2);
	con.insert(sptr3);

	// 查找名叫李四的人
	EmployeeContainer_sptr::index< name >::type& indexOfName = con.get< name >();
	int yl = indexOfName.count("Joe1");

	EmployeeContainer_sptr::index<name>::type::iterator it2 = indexOfName.find("Robert");
	if(it2 != indexOfName.end())
		printf("%d,%s,%d\n",(*it2)->id,(*it2)->name.c_str(),(*it2)->age) ;

};

typedef boost_wheel::multi_index::multi_index_container<
	Employee*,
	boost_wheel::vector<
	boost_wheel::multi_index::ordered_unique<boost_wheel::tag<id>, boost_wheel::member<Employee, int, &Employee::id> >,
	boost_wheel::multi_index::ordered_non_unique<boost_wheel::tag<name>,boost_wheel::member<Employee, std::string, &Employee::name> >,
	boost_wheel::multi_index::ordered_non_unique<boost_wheel::tag<age>,boost_wheel::member<Employee, int, &Employee::age> >
	>
> EmployeeContainer_point;

void EmployeeContainer_point_Test( void )
{
	EmployeeContainer_point con;

	Employee* sptr0 = new Employee(Employee(0,"Joe1",31)) ;
	Employee* sptr1 = new Employee(Employee(1,"Joe1",31)) ;
	Employee* sptr2 = new Employee(Employee(2,"Robert",27)) ;
	Employee* sptr3 = new Employee(Employee(3,"John",40)) ;

	/*while(true)
	{
		Employee* sptr22 = new Employee(Employee(10,"Joetest",131)) ;
		if( con.insert(sptr22).second )
		{
			int s = 0;
		}
		else
		{
			int s = 0;
		}

		EmployeeContainer_point::index< name >::type& indexOfName = con.get< name >();
		EmployeeContainer_point::index<name>::type::iterator it2 = indexOfName.find("Joetest");
		if(it2 != indexOfName.end())
			printf("%d,%s,%d\n",(*it2)->id,(*it2)->name.c_str(),(*it2)->age) ;

		indexOfName.erase("Joetest");
		delete sptr22;
		Sleep(10);
	}*/

	if( con.insert(sptr0).second )
	{
		int s = 0;
	}
	else
	{
		int s = 0;
	}

	con.insert(sptr1);
	con.insert(sptr2);
	con.insert(sptr3);

	// 查找名叫李四的人
	EmployeeContainer_point::index< name >::type& indexOfName = con.get< name >();


	EmployeeContainer_point::index< name >::type::iterator it321 = indexOfName.find("John");
	if( it321 != indexOfName.end() )  
	{
		Employee* tmmtsptr0 = new Employee(Employee(3,"Eohn",40)) ;
		bool ls = indexOfName.replace(it321, tmmtsptr0);
		int i = 0;

		{
			for( it321 = indexOfName.begin(); it321 != indexOfName.end(); ++ it321 )
			{
				printf("%d,%s,%d\n",(*it321)->id,(*it321)->name.c_str(),(*it321)->age) ;
			}
		}

		{
			EmployeeContainer_point::index< id >::type& indexOfid = con.get< id >();
			for(EmployeeContainer_point::index< id >::type::iterator it321 = indexOfid.begin(); it321 != indexOfid.end(); ++ it321 )
			{
				printf("%d,%s,%d\n",(*it321)->id,(*it321)->name.c_str(),(*it321)->age) ;
			}
		}
	}

	int yl = indexOfName.count("Joe1");

	EmployeeContainer_point::index<name>::type::iterator it2 = indexOfName.find("Robert");
	if(it2 != indexOfName.end())
		printf("%d,%s,%d\n",(*it2)->id,(*it2)->name.c_str(),(*it2)->age) ;
}


/*#define MULTI_INDEX_DECL_string11( name )  #name

#define MULTI_INDEX_DECL_string1( name ) MULTI_INDEX_DECL_string11(name)


#define MULTI_INDEX_DECL_test1( name )  "12345"

#define MULTI_INDEX_DECL_test( name ) MULTI_INDEX_DECL_test1 ## name

#define MULTI_INDEX_DECL_test_string11( name )  MULTI_##name

#define MULTI_INDEX_DECL_test_string1( name ) MULTI_INDEX_DECL_test_string11(name)*/

namespace MULTI_INDEX
{
	TYPEDEF_MULTI_INDEX_DECL(map_tp, _Map, Employee*, \
		INDEX_UNIQUE_ELEMENT(Employee, int, id),\
		INDEX_NON_UNIQUE_ELEMENT(Employee, std::string, name));
}

bool Multi_Index_Test_Find(UInt64 uCount, UInt64 uPos, Employee* p )
{
	printf("Multi_Index_Test_Find(%lld,%lld) age:%d, id:%d, name:%s\r\n", uCount, uPos, p->age, p->id, p->name.c_str());
	int i = 0;
	return true;
}

void Multi_Index_Test( void )
{
	MULTI_INDEX::map_tp dle;

	Employee* sptr0 = new Employee(Employee(0,"Joe1",31)) ;
	Employee* sptr1 = new Employee(Employee(1,"Joe1",31)) ;
	Employee* sptr2 = new Employee(Employee(2,"Robert",27)) ;
	Employee* sptr3 = new Employee(Employee(3,"John",40)) ;

	MULTI_INDEX::MultiIndex_Insert(sptr0);
	MULTI_INDEX::MultiIndex_Insert(sptr1);
	MULTI_INDEX::MultiIndex_Insert(sptr2);
	MULTI_INDEX::MultiIndex_Insert(sptr3);

	//MULTI_INDEX::MultiIndex_RemoveFrom_name("Robert");
	Employee* spt2 = MULTI_INDEX::MultiIndex_FindFrom_name("Robert");
	printf("age:%d, id:%d, name:%s\r\n",spt2->age, spt2->id, spt2->name.c_str());

	UInt64 uc = MULTI_INDEX::MultiIndex_FindsFrom_name("Joe1", Multi_Index_Test_Find);
	printf("MultiIndex_FindsFrom_name:%lld\r\n", uc);

	uc = MULTI_INDEX::MultiIndex_Count_name("Joe1");
	printf("MultiIndex_Count_name:%lld\r\n", uc);


	MULTI_INDEX::map_tp* pow = &MULTI_INDEX::_Map;

	int iw = 0;
	long iwl = 0;
	char sw;

	/*char* slw = MULTI_INDEX_DECL_test_string1( INDEX_DECL_test( (int) ) );
	printf("%s\r\n", slw);*/

	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(dle)).name());
	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(dle)).name());
	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(iw)).name());
	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(iw)).name());
	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(iwl)).name());
	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(iwl)).name());

	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(sw)).name());
	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(sw)).name());
	//printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(slw)).name());
	//printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(slw)).name());

	//printf("typeid:%s\r\n", typeid((boost_wheel::ref(slw))::type).name()); 

	/*char* ps = MULTI_INDEX_DECL_string1( boost_wheel::multi_index::multi_index_container<\
		type,\
		boost_wheel::vector<\
		MACRO_PP_SEQ_FOR_EACH_I(MULTI_INDEX_MEMBER_GET_FIELD, _, MACRO_PP_VARIADIC_TO_SEQ(\

		))\
		> );*/

	/*typedef boost_wheel::multi_index::multi_index_container<
		Employee*,
		boost_wheel::vector<
		boost_wheel::multi_index::ordered_unique<boost_wheel::tag<id>, boost_wheel::member<Employee, int, &Employee::id> >,
		boost_wheel::multi_index::ordered_non_unique<boost_wheel::tag<name>,boost_wheel::member<Employee, std::string, &Employee::name> >,
		boost_wheel::multi_index::ordered_non_unique<boost_wheel::tag<age>,boost_wheel::member<Employee, int, &Employee::age> >
		>
	> EmployeeContainer_point;*/

	/*char* ps = MULTI_INDEX_DECL_string1( MULTI_INDEX_TYPEDEF_DECL(map_tp, Employee*, \
		INDEX_UNIQUE_ELEMENT(Employee, int, id),
		INDEX_NON_UNIQUE_ELEMENT(Employee, std::string, name) ) );*/

	/*char* ps = MULTI_INDEX_DECL_string1( TYPEDEF_MULTI_INDEX_DECL(map_tp, _Map, Employee*, \
		INDEX_UNIQUE_ELEMENT(Employee, int, id),
		INDEX_NON_UNIQUE_ELEMENT(Employee, std::string, name) ) );

	printf("%s\r\n", ps);*/
}

namespace MULTI_INDEX_LOCK
{
	TYPEDEF_LOCK_MULTI_INDEX_DECL(map_tp, _Map, Employee*, \
		INDEX_UNIQUE_ELEMENT(Employee, int, id),\
		INDEX_NON_UNIQUE_ELEMENT(Employee, std::string, name));
}

void Multi_Index_Lock_Test( void )
{
	MULTI_INDEX_LOCK::map_tp dle;

	Employee* sptr0 = new Employee(Employee(0,"Lock_Joe1",31)) ;
	Employee* sptr1 = new Employee(Employee(1,"Lock_Joe1",31)) ;
	Employee* sptr2 = new Employee(Employee(2,"Lock_Robert",27)) ;
	Employee* sptr3 = new Employee(Employee(3,"Lock_John",40)) ;

	MULTI_INDEX_LOCK::MultiIndex_Insert(sptr0);
	MULTI_INDEX_LOCK::MultiIndex_Insert(sptr1);
	MULTI_INDEX_LOCK::MultiIndex_Insert(sptr2);
	MULTI_INDEX_LOCK::MultiIndex_Insert(sptr3);

	//MULTI_INDEX::MultiIndex_RemoveFrom_name("Robert");
	Employee* spt2 = MULTI_INDEX_LOCK::MultiIndex_FindFrom_name("Lock_Robert");
	printf("age:%d, id:%d, name:%s\r\n",spt2->age, spt2->id, spt2->name.c_str());

	UInt64 uc = MULTI_INDEX_LOCK::MultiIndex_FindsFrom_name("Lock_Joe1", Multi_Index_Test_Find);
	printf("MultiIndex_FindsFrom_name:%lld\r\n", uc);

	uc = MULTI_INDEX_LOCK::MultiIndex_Count_name("Lock_Joe1");
	printf("MultiIndex_Count_name:%lld\r\n", uc);

	int iw = 0;
	long iwl = 0;
	char sw;

	/*char* slw = MULTI_INDEX_DECL_test_string1( INDEX_DECL_test( (int) ) );
	printf("%s\r\n", slw);*/

	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(dle)).name());
	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(dle)).name());
	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(iw)).name());
	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(iw)).name());
	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(iwl)).name());
	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(iwl)).name());

	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(sw)).name());
	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(sw)).name());
	/*printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(slw)).name());
	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(slw)).name());*/

	/*char* ps = MULTI_INDEX_DECL_string1( TYPEDEF_LOCK_MULTI_INDEX_DECL(map_tp, _Map, Employee*, \
		INDEX_UNIQUE_ELEMENT(Employee, int, id),
		INDEX_NON_UNIQUE_ELEMENT(Employee, std::string, name) ) );

	printf("%s\r\n", ps);*/
}

struct Employee_func {
	int id;
	std::string name;
	int age;

	Employee_func(int id_, std::string name_, int age_) :id(id_), name(name_), age(age_) {}

	const std::string&	Get_name(void)
	{
		return name;
	}

	const int&	Get_age(void)
	{
		return age;
	}

	const int&	Get_id(void)
	{
		return id;
	}

	/*friend std::ostream& operator<<(std::ostream& os,const Employee& e)
	{
	os<<e.id<<" "<<e.name<<" "<<e.age<<std::endl;
	return os;
	}*/
};

namespace MULTI_INDEX_FUNC_LOCK
{
	CLock _Lock;

	TYPEDEF_LOCK_MULTI_INDEX_DECL(map_tp, _Map, Employee_func*, \
		FUNC_INDEX_UNIQUE_ELEMENT(Employee_func, const int&, Get_id), \
		FUNC_INDEX_NON_UNIQUE_ELEMENT(Employee_func, const std::string&, Get_name));
}

bool Multi_Index_Func_Test_Find(UInt64 uCount, UInt64 uPos, Employee_func* p)
{
	printf("Multi_Index_Func_Test_Find(%lld,%lld) age:%d, id:%d, name:%s\r\n", uCount, uPos, p->age, p->id, p->name.c_str());
	int i = 0;
	return true;
}

void Multi_Index_Func_Lock_Test(void)
{
	int is = sizeof(_pool_::MemPool<>);
	int is2 = sizeof(_string_v20_::string);
	int is3 = sizeof(std::string);

	MULTI_INDEX_FUNC_LOCK::map_tp dle;

	Employee_func* sptr0 = new Employee_func(Employee_func(0, "Lock_Joe1", 31));
	Employee_func* sptr1 = new Employee_func(Employee_func(1, "Lock_Joe1", 31));
	Employee_func* sptr2 = new Employee_func(Employee_func(2, "Lock_Robert", 27));
	Employee_func* sptr3 = new Employee_func(Employee_func(3, "Lock_John", 40));

	Employee_func* sptr4 = new Employee_func(Employee_func(4, "Aock_John", 42));

	MULTI_INDEX_FUNC_LOCK::MultiIndex_Insert(sptr0);
	MULTI_INDEX_FUNC_LOCK::MultiIndex_Insert(sptr1);
	MULTI_INDEX_FUNC_LOCK::MultiIndex_Insert(sptr2);
	MULTI_INDEX_FUNC_LOCK::MultiIndex_Insert(sptr3);

	bool isF = MULTI_INDEX_FUNC_LOCK::MultiIndex_Replace_Get_name("Lock_Joe1", sptr4);

	//MULTI_INDEX::MultiIndex_RemoveFrom_name("Robert");
	Employee_func* spt2 = MULTI_INDEX_FUNC_LOCK::MultiIndex_FindFrom_Get_name("Lock_Robert");
	printf("age:%d, id:%d, name:%s\r\n", spt2->age, spt2->id, spt2->name.c_str());

	UInt64 uc = MULTI_INDEX_FUNC_LOCK::MultiIndex_FindsFrom_Get_name("Lock_Joe1", Multi_Index_Func_Test_Find);
	printf("MultiIndex_FindsFrom_name:%lld\r\n", uc);

	uc = MULTI_INDEX_FUNC_LOCK::MultiIndex_Count_Get_name("Lock_Joe1");
	printf("MultiIndex_Count_name:%lld\r\n", uc);

	MULTI_INDEX_FUNC_LOCK::TYPE_MULTI_INDEX(Get_name)& indexOfName = MULTI_INDEX_FUNC_LOCK::_Map.get< MULTI_INDEX_FUNC_LOCK::TAG_MULTI_INDEX(Get_name) >();
	for (MULTI_INDEX_FUNC_LOCK::ITERATOR_MULTI_INDEX(Get_name) name_iter = indexOfName.begin(); name_iter != indexOfName.end(); name_iter++)
	{
		printf("%s:%d\r\n", (*name_iter)->name.c_str(), (*name_iter)->id);
	}

	int iw = 0;
	long iwl = 0;
	char sw;

	/*char* slw = MULTI_INDEX_DECL_test_string1( INDEX_DECL_test( (int) ) );
	printf("%s\r\n", slw);*/

	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(dle)).name());
	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(dle)).name());
	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(iw)).name());
	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(iw)).name());
	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(iwl)).name());
	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(iwl)).name());

	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(sw)).name());
	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(sw)).name());
	/*printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(slw)).name());
	printf("typeid:%s\r\n", typeid(BOOST_WHEEL_TYPEOF(slw)).name());*/

	/*char* ps = MULTI_INDEX_DECL_string1( TYPEDEF_LOCK_MULTI_INDEX_DECL(map_tp, _Map, Employee_func*, \
	INDEX_UNIQUE_ELEMENT(Employee_func, int, id),
	INDEX_NON_UNIQUE_ELEMENT(Employee_func, std::string, name) ) );

	printf("%s\r\n", ps);*/

	MULTI_INDEX_FUNC_LOCK::MultiIndex_Clear();

	delete sptr0;
	delete sptr1;
	delete sptr2;
	delete sptr3;
	delete sptr4;
}

int main(int argc, char* argv[])
{
	Multi_Index_Func_Lock_Test();

	boost_wheel::member_func< Employee, std::string, &Employee::Get_name > slw;
	Employee Emp(0, "Lock_Joe1", 31);
	std::string uy = slw(Emp);

	/*int v = const_member_base< TestOb, _string_type, &TestOb::_Name>::value;
	int v1 = const_member_base< TestOb, _string_type, &TestOb::GetName >::value;*/

	/*Multi_Index_Test();
	Multi_Index_Lock_Test();
	EmployeeContainer_point_Test();
	EmployeeContainer_sptr_Test();*/
	Multi_Index_Func_Lock_Test();

	MapTest();
	while (true)
	{
		Sleep(1000);
	}

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