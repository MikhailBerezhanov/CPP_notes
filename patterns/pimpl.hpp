#pragma once

#include <memory>	// smart_pointers
#include <string>

class Person
{
public:

	Person(const std::string &name = "", int age = -1);	

	// Unique_ptr needs complete type to know
	// deleter type. Here (in header) struct Impl
	// is only declared and has incomplete type.
	// Person::Dtr is deduced while compiling based on
	// class member types and their Dtrs. To make
	// unique_ptr work correctly whin incomplete Impl type
	// Person::Dtr must be defined at the place, where Impl
	// defined - inside .cpp file.
	~Person();

	std::string name() const;
	int age() const;

private:

	// Implementation declaration.
	// Definition is hidden inside .cpp file.
	//
	// Note that Impl here is incomplete type!
	struct Impl;

	// Raw pointer is acceptable, but better 
	// practive is using smart_pointer
	/* Impl *pimpl = nullptr; */
	std::unique_ptr<Impl> pimpl;

};