#include <iostream>
#include <memory>
#include <cstdio>

using namespace std;



int main()
{
	// Unique ptr features:
	// - doesn't have Control block.
	// - if no custom deleter provided has size == raw ptr (sizeof(void*))
	// - custom deleter is type parameter.
	// - has specialization for arrays (e.g. allocated with new[]).
	//   uses delete[] as deleter in that case. 
	// - supports operator[] (provides indexed access to the managed array)
	unique_ptr<int[]> ints_array(new int[100]()); // new[]() <- value inializatoin
	ints_array[50] = 43;
	cout << "uptr size: " << sizeof(ints_array) << endl;
	cout << ints_array[50] << endl;

	// NOTE: decltype(function) deduce function signature ( i.e. int(FILE*) ). 
	// To make deduction as function pointer - use '*' or '&function'. ( int(*)(FILE*) ) 
	unique_ptr< FILE, decltype(fclose)* > uni_file(fopen("uni_file.txt", "w+"), fclose);

	// Shared ptr custom deleter is not type but constructor parameter.
	//
	// Shared ptr uses Control block, that consists of:
	// 		ref_counter_ptr
	//		weak_ref_counter_ptr
	//		(custom_deleter)
	//		(etc)
	//
	// Such Control block is common for all objects that accesses 
	// desired resource and allocated in dynamic memory (or by user's allocator).
	// Each shared_ptr object has a ponter to that Control block. 
	//
	// That makes it's size x2 bigger than std::unique_ptr. And slows the 
	// access to pointed data. 
	//
	// Control block is created when:
	//
	// 1. shared_ptr is constructed with make_shared<>() call
	// 2. shared_ptr is constructed from unique_ptr or auto_ptr
	// 3. shared_ptr is constructed from raw pointer (new call)
	//
	// WARN: It is undefined behaviour if Control block is created more
	// than one time for shared_ptr.

	double *pi = new double;

	std::shared_ptr<double> sptr1(pi);	// Control block created

	std::shared_ptr<double> sptr2(pi); 
	// Control block for resource pi is created again. 
	// This will cause double delete of pi !!

	return 0;
}