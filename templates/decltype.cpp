#include <iostream>
#include <vector>

using namespace std;


// We want function that returns type as containers operattor[].
// For most STL containers operator[] returns T&  lvalue reference,
// BUT not for std::vector<bool> for example!

// C++11  (trailing return type)
template<typename Container, typename Idx>
auto func1(Container &c, Idx i) -> decltype( c[i] )
{
	// do smth with c[i]
	return c[i];

	// NOTE: it is not auto - deduction, but decltype works!
	// auto deduction would ommit reference in that case, because
	// ParamType is simple T (copy)
}

// C++14 feature (auto deduction)
template<typename Container, typename Idx>
auto func2(Container &c, Idx i)		// NOTE: rvalue couldn't be passed as non-const lvalue ref
{
	return c[i];	// auto deduction works ! returns rvalue copy !

	// this is incorrect version , cuz 'auto' will ommit reference type
}

// CORRECT WAY: decltype(auto)
// using universal reference to let rvalue containers be passed
template<typename Container, typename Idx>
decltype(auto) func3(Container &&c, Idx i)	
{
	return std::forward<Container>(c)[i];	// forward 
}


int generate_rvalue(){ return 10; }

int main()
{
	int x = 10;			// decltype(x) - int
	const int& rx = x;	// decltype(rx)- const int&
	decltype(generate_rvalue());	// int&&
	decltype(43);		// int&&

	// NOTE: decltype( (x) ) - int& ! '()' - is expression !
	decltype( (x) );	// int&

	std::vector<int> vec{100, 1, -1};

	func1(vec, 0) = 90;
	cout << vec.at(0) << endl;

	// func2(vec, 0) = -123;	// ERROR: lvalue required , but returned copy of element as rvalue 
	// cout << vec.at(0) << endl;

	func3(vec, 0) = 135;
	cout << vec.at(0) << endl;

	return 0;
}