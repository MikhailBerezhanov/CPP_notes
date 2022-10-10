#include <iostream>
#include <string>
#include <vector>
#include <array>

using namespace std;

// Universal reference works with not all types.
// Problematic cases:

// 1. 
// 0 or NULL will be deduced as int instead of pointers
// (use 'nullptr' for correct pointer deduction)


// 2.
// - {..} initializer list  - compilers are forbidden to deduce 
// template parameters if std::initializer_list passed.
// (NOTE: not for 'auto' though) 

void vec_func(const std::vector<int> &vec)
{

}

template<typename T>
void fwd(T &&param)
{
	vec_func(std::forward<T>(param));
}

void ilist_uni_ref_deduction_demo()
{
	vec_func({1, 2, 3});	// OK

	/* fwd({1, 2, 3}); */	// ERROR - couldn’t deduce template parameter ‘T’

	// BUT 
	auto ilist = {1, 2, 3};	// std::initializer list
	fwd(ilist);				// OK

	fwd(std::initializer_list<int>({1, 2, 3}));
}

// 3. Integer static const and static constexpr members without definition.
struct A
{
	// NOTE: static const integral members don't need definition.
	// If so they don't have physical address and don't consume memory,
	// their values just inlined as macroses.
	//
	// If such members has definition, they are placed in memory and
	// has physical addresses. Pointer in such case can be taken. 

	static const size_t m_size = 100;		// declaration
	static constexpr int m_counter = 999;	// declaration


	// Arrays in-class initialization (will consume the memory)
	static constexpr const char* arr[] = {"adc", "defg", "hkkl", "HELLO", "WoRlD"};

	static constexpr std::array<double, 5> doubles = {1.1, 2.2, 3.3, 4.4, 5.5};
};

// Definition => placing in memory 
constexpr int A::m_counter; 

// No definition
/* const size_t A::m_size; */ 
 
template<typename T>
void fwd2(T &&param) {}

void static_const_members_demo()
{
	cout << "A size: " << sizeof(A) << endl;
	cout << "A::m_size: " << A::m_size << endl;	// Value can be used
	
	// Compiles OK, but Linkage error - no address could be taken
	/* const size_t *sptr = &A::m_size; */

	const int *iptr = &A::m_counter; // OK - m_counter has definition
	cout << "A::m_counter: " << *iptr << endl;

	fwd2(100);			// OK - rvalue passed

	/* fwd2(A::m_size); */	// ERROR - linkage error

	// Т.к. передается по факту lvalue, по правиллам вывода типа универсальной
	// ссылки , параметр шаблона выводится как lvalue ссылка T& (size_t&).
	//
	// Cсылки в коде, сгенерированном компилятором, обычно рассматриваются 
	// как указатели. В бинарном коде программы указатели и ссылки, по сути, 
	// представляют собой одно и то же. На этом уровне можно считать, что 
	// ссылки - это просто указатели, которые автоматически разыменовываются. 
	// В таком случае передача A::m_size по ссылке фактически представляет 
	// собой то же, что и передача по указателю, а раз так, то должна иметься 
	// память, на которую этот указатель указывает.

	fwd2(A::m_counter);	// OK - lvalue passed, that has an address


	// TEST
	for(size_t i = 0; i < sizeof(A::arr) / sizeof(A::arr[0]); ++i){
		cout << A::arr[i] << " ";
	}
	cout << endl;

	const char * const *cptr = &A::arr[0];	//
}

int main()
{
	// 2. std::initializer list is forbidden for template parameter deduction
	ilist_uni_ref_deduction_demo();

	// 3. integral static const \ static constexpr without definition 
	static_const_members_demo();
	
	// 4.


	

	return 0;
}
