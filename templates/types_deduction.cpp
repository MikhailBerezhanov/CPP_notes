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
void func_copy(T param){}	// by value

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



	return 0;
}