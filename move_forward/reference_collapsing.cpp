#include <iostream>
#include <utility>
#include <string>

// using namespace std;

// Reference collapsing apperas is 4 contexts:
// 1. template instantiation
// 2. auto deduction
// 3. typedef \ using =
// 4.
//
// Collapsing takes place when 'reference to reference' appears.
// Possible ref_to_ref cases:
// - lvalue ref to lvalue ref  ->  lvalue ref result
// - lvalue ref to rvalue ref  ->  lvalue ref result
// - rvalue ref to lvalue ref  ->  lvalue ref result
// - rvalue ref to rvalue ref  ->  rvalue ref result
//
// Rules of collapsing: if lvalue is a part of ref_to_ref , result is lvalue ref. 
// Otherwise (rvalue ref to rvalue ref) - rvalue ref.

// return value is r-value 
std::string make_string()
{
	return "result";
}

std::string str;			// lvalue
std::string &str_ref = str;	// lvalue ref 

// C++11 
namespace cpp11
{
	// std::move implementation
	// Пример определения ф-ии std::move() преобразующей объект в r-value 
	// (объект может быть как r-value, так и l-value)
	template<typename T>
	typename std::remove_reference<T>::type&& my_move(T &&param)
	{
		// ПРИМ: оператор static_cast поддерживает приведение l-value 
		// к ссылке на r-value и делает это ЯВНО
		return static_cast< typename std::remove_reference<T>::type&& >(param);
	}

	void my_move_tests()
	{
		// passing l-value will instantiate template parameter as T&
		// by universal reference deduction rules.
		my_move(str);
/*
	Reference collapsing will work and make string& && - lvalue ref

	string&& my_move(string& &&param)        ->  	string&& my_move(string &param)
	{												{
		return static_cast< string&& >(param);			return static_cast< string&& >(param);
	}												}
*/

		// passing r-value will instantiate template parameter as T
		// by universal reference deduction rules.
		my_move(::make_string());
/*
	string&& my_move(string &&param)
	{
		return static_cast< string&& >(param);
	}
*/
	}


	// std::forward implementation
	template <typename T>
	T&& my_forward(typename std::remove_reference<T>::type &param)
	{
		return static_cast< T&& >(param);
	}

	template <typename T>
	void my_forward_tests(T &&param)
	{
		// 1. passing l-value. template parameter will be instantiated as T&
		my_forward<T>(param);
/*
	string& && my_forward( remove_reference< string& > &param ) { ... }
	
				|
				v

	string& && my_forward(string &param) 	 -> 	string& my_forward(string& param)
	{												{
		return static_cast< string& && >(param);		return static_cast< string& >(param);
	}												}
*/
		// 2. passing l-value ref. Same behaviour as above example.

		// 3. passing r-value. Template parameter will be instantiated as T.
		// my_forward_tests(::make_string());
/*
	string&& my_forward(string &param)
	{
		return static_cast< string&& >(param);   -  converts lvalue to rvalue
	}

*/

		// 4. passing r-value ref
		// my_forward(std::move(str));
/*
	string&& my_forward(string &param)
	{
		return static_cast< string&& >(param);
	}
*/
	}


}

// C++ 14
namespace cpp14
{
	template<typename T>
	decltype(auto) my_move(T &&param)
	{
		return static_cast< std::remove_reference_t<T>&& >(param);
	}

	template<typename T>
	decltype(auto) my_forward(std::remove_reference_t<T> &param)
	{
		return static_cast< T&& >(param);
	}
}



int main()
{
	using namespace cpp11;

	my_move_tests();

	my_forward_tests(str);				// 1. lvalue passing
	my_forward_tests(str_ref);			// 2. lvalue ref passing
	my_forward_tests(::make_string);	// 3. rvalue passing
	my_forward_tests(std::move(str));	// 4. rvalue ref passing

	return 0;
}



























