#include <iostream>
#include <vector>
#include <string>

using namespace std;


// •std::move выполняет безусловное приведение к rvalue. 
// 		Сама по себе эта функция не перемещает ничего.
// •std::forward приводит свой аргумент к rvalue только тогда, 
// 		когда этот аргумент связан с rvalue.
// •Ни std::move, ни std::forward не выполняют никаких действий 
// 		времени выполнения.

// Universal reference criterias:
// 1. construction - T&&
// 2. type deduction must take place

// Main feature of universal referece is 
// direct passing \ perfect forwarding - Прямая передача аргумента.
// Internal functions or constructors get exactly input types of arguments.
// preventing creation of temp objects or copying => more effective calls.

class Person
{
public:

	Person() = default;

	Person(const std::string& s): name(s) { cout << "Person copy of string called" << endl; }

	Person(std::string &&s): name(std::move(s)) { cout << "Person move of string called" << endl; }

	Person(const char *s): name(s) { cout << "Person from literal called" << endl; }

	// r-value ref but rhs is l-value! Use std::move for correct
	// move semantic work.
	Person(Person &&rhs): name(std::move(rhs.name)) {}

	// Using universal reference
	// Advantages of using  uni ref:
	// 1. less code 
	// 2. direct argumets passing
	template <typename T>
	void set_name(T&& new_name)
	{
		// may use
		// - copy assignment (lvalue argument)
		// - move assignment (rvalue argument)
		// - assignment from literal (const char* argument)
		name = std::forward<T>(new_name);
	}

	// Using standart way (copy and move versions)
	void set_name2(const std::string &new_name)
	{
		cout << "set_name2::copy assignment" << endl;
		name = new_name;
	}

	void set_name2(std::string &&new_name)
	{
		cout << "set_name2::move assignment" << endl;
		name = std::move(new_name);
	}

private:
	std::string name;
};

// r-value ref, not universal ref
void func(Person &&param);	// no type deduction 

// r-value ref, not universal ref
template <typename T>
void func1(std::vector<T> &&param){}	// no uni ref construction


// universal reference
template <typename T>
void uni_ref_func(T &&param)
{
	// NOTE: param is l-value anyway 
	// (even if passed as r-value with std::move())


	// To save input type of param while passing to
	// the another internal function use std::forward.
	// If not - param will be passed as l-value.

	Person p1(param);	// uses always copying!

	Person p2(std::forward<T>(param));

	// internal_function( std::forward<T>(param) );
}



void uni_ref_test()
{
	std::vector<int> vec;
	std::string text;

	// func1(vec); // ERROR: lvalue passed to rvalue ref
	auto &&var1 = vec;				// universal ref
	auto &&var2 = std::move(vec);	// universal ref

	cout << "1. passing lvalue to uni ref" << endl;
	uni_ref_func(text);				// param - lvalue ref

	cout << "2. passing rvalue to uni ref" << endl;
	uni_ref_func(std::move(text));	// param - rvalue ref

	cout << "3. passing literal to uni ref" << endl;
	uni_ref_func("qwerty");	

	// Universal ref (auto&&) feature for lambdas in C++14
	auto calculate_execution_time = [](auto&& func, auto&&... params)
	{
		// start timer
		std::forward<decltype(func)>(func)( std::forward<decltype(params)>(params)... );
		// stop timer
	};

	cout << "4. passing literal to uni ref<const char*>" << endl;

	// NOTE: literal will be passed to the Person's (std::string) 
	// constructor directly without creating temp objects and copying.
	calculate_execution_time(uni_ref_func<const char*>, "abcdef");

	cout << "5. passing literal to uni ref<string>" << endl;	

	// NOTE: 
	// 1. temp string object will be constructed from literal 
	// 2. temp string object is rvalue and will be moved 
	// 3. temp string objecct destructor will be called
	calculate_execution_time(uni_ref_func<string>, "abcdef");
}


void set_person_name_test()
{
	Person p;

	std::string local_name{"mik"};

	// Using universal reference approach

	p.set_name(local_name);	// lvalue
	p.set_name(std::move(local_name));

	cout << "local_name after move: " << local_name << endl;
	local_name = "mik";

	p.set_name("Mikhail");	// direct literal passing (const char*)

	// Using overloads approach

	p.set_name2(local_name);
	p.set_name2(std::move(local_name));

	// Данный вызов set_name2 повлечет за собой выполнение 
	// - одного конструктора std::string (для создания временного объекта), 
	// - одного перемещающего оператора присваивания std::string 
	//   (для перемещения new_name в p.name ) и 
	// - одного деструктора std::string (для уничтожения временного объекта). 
	//
	// Это практически наверняка более дорогостоящая последовательность операций, 
	// чем вызов только одного оператора присваивания std::string, принимающего 
	// указатель const char * .
	p.set_name2("QWERTY");

}


int main(int argc, char *argv[])
{
	uni_ref_test();

	set_person_name_test();

	return 0;
}