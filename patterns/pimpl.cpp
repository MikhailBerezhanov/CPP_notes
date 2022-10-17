#include <iostream>

// All realization dependencies is 
// hidden here.
#include <vector>

#include "pimpl.hpp"

using namespace std;

// Pointer to Implementation pattern
// Advantages:
// 1. all dependency headers could be hidden 
// 2. when implementation changes, there is no need 
//    to recompile all sources that include current
//	  header - only pimpl.cpp might be recompiler.
//    Header content may stay the same.
//
// Disadvantages:
// 1. Heap space usage.


// Definition of implementation
struct Person::Impl
{
	std::string name;
	int age = -1;
	std::vector<std::string> phone_numbers;
};

// Constructs the object , initialize the pointer
Person::Person(const std::string &name, int age): pimpl(new Impl)
{
	pimpl->name = name;
	pimpl->age = age;
}

// Destuctor definition where Impl is defined
Person::~Person() = default;

inline std::string Person::name() const
{
	return pimpl->name;
}

inline int Person::age() const
{
	return pimpl->age;
}


int main()
{
	Person p1("Mik", 10);

	cout << "p1: " << p1.name() << " " << p1.age() << endl;

	return 0;
}