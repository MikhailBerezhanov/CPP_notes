#include <iostream>

using namespace std;


// Template of functions

// 1. ParamType is reference (l-value or r-value but not universal) or pointer.
// 		- argument's reference is omitted while type deduction 
//
template <typename T>
void func_lhs_ref(T &param) 	// ParamType is T& may be noq equal with T
{
}

// same rules
template <typename T>
void func_cptr(T *param){}

// 2. ParamType is universal reference
//		- argument is l-value -> T and ParamType will be l-value references both.
//		- argument is r-value , same rules as for 1.	
//
template <typename T>
void func_uni_ref(T &&param){}

// 3. ParamType neither reference, nor pointer.
// 		- argument's reference is omitted while type deduction
//		- argument's const (volatile) is omitted
//
template<typename T>
void func_copy(T param)	// by value
{
	cout << param << endl;
}

// Templates with C-arrays
// 
template<typename T, std::size_t size>
constexpr std::size_t size_of_array( T (&) [size] ) noexcept
{
	return size;
}

void arrays_deduction_feature()
{
	const char arr[13] = "abcrfgh";

	func_copy(arr);		// T - const char*  - array is converted to pointer
	func_copy(&arr[3]);	// T - const char*
	func_copy(arr[3]);	// T - char

	// BUT
	func_lhs_ref(arr);	// T - const char [13], ParamType -  const char (&) [13]
						// (reference to array of specified size)

	cout << "size_of_array: " << size_of_array(arr) << endl;
}



int main()
{
	int a = 43;			// l-value  (address can be taken)
	const int ca = 100;	// l-value
	const int &ra = a;	// l-value reference (address can be taken)

	// 1. ------------------------------------------------------------
	func_lhs_ref(a);	// T - int, 		ParamType - int&
	func_lhs_ref(ca);	// T - const int, 	ParamType - const int&
	// NOTE: reference of variable will be omitted
	func_lhs_ref(ra);	// T - const int, 	ParamType - const int&

	// It is safe to pass const variables as  T& params
	// because T will be instantiated as const int. 

	func_cptr(&a);		// T - int,			ParamType - const int*
	func_cptr(&ca);		// T - const int,	ParamType - const int*
	func_cptr(&ra);		// T - const int,	ParamType - const int*

	// func_lhs_ref(15);	// error: cannot bind nonconst l-value reference to an r-value

	// 2. ------------------------------------------------------------
	func_uni_ref(a);	// T - int&, 		ParamType - int&
	func_uni_ref(ca);	// T - const int&,	ParamType - const int&
	func_uni_ref(ra);	// T - const int&,	ParamType - const int&
	func_uni_ref(15);	// T - int,			ParamType - int&&	(15 is r-value)


	// 3. ------------------------------------------------------------
	func_copy(a);		// T - int,			ParamType - int
	func_copy(ca);		// T - int, 		ParamType - int
	func_copy(ra);		// T - int, 		ParamType - int

	const char* const ccptr = "const const pointer";
	// constance of pointer itself will be omitted, but constance of
	// pointed data will stay. ccptr is copied into the function.
	func_copy(ccptr);	// T - const char*, ParamType - const char*


	// ---------------------------------------------------------------
	// 'auto'  deduction works as template deduction (auto as ParamType)
	// 		template<typename T>
	// 		void auto_wrapper(auto_value param) 
	//
	// except for = {} - initialization. auto counts {} as std::initializer_list<>
	//

	auto x = 10;		// ParamType - T , 					=> auto_value - int
	const auto &rx = x;	// ParamType - const T&, T - int  	=> auto_value - const int&

	auto y = rx;		// ParamType - T , auto_value - int (copy) - const reference is omitted
	y = 11;				// y is a copy, can be changed 

	auto &rrx = rx;		// ParamType - T&, T - const int 	=> auto_value - const int&
	// rrx = 12;			// Error - readonly 
	cout << "rrx: " << rrx << ", rx: " << rx << ", x: " << x << endl;

	// auto &arx = 100;		// ParamType - T&, T - int 	=> auto_value - int&	- Error
	auto &&rvx = 100;	// r-value 

	auto i = {999};		// Warning:  i is initializer list
	// i = 10; 			// Error
	// BUT 
	auto ii{333}; 		// OK. auto_value - int 
	cout << "ii: " << ii << endl;


	arrays_deduction_feature();

	return 0;
}