#include <iostream>
#include <string>
#include <vector>
#include <array>

using namespace std;

// • Прямая передача неудачна, когда вывод типа не удается выполнить или 
// когда он выводит неверный тип.
//
// • Разновидностями аргументов, которые приводят к неудачам при прямой передаче:
// 1. нулевые указатели, выраженные как О или NULL,
// 2. инициализаторы в фигурных скобках, 
// 3. целочисленные члены-данные, объявленные как static const, static constexpr 
//	  и не имеющие определений, 
// 4. имена шаблонов и перегруженных функций,
// 5. битовые поля.


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
void fwd2(T &&param)
{
	vec_func(std::forward<T>(param));
}

void ilist_uni_ref_deduction_demo()
{
	vec_func({1, 2, 3});	// OK

	/* fwd2{1, 2, 3}); */	// ERROR - couldn’t deduce template parameter ‘T’

	// BUT 
	auto ilist = {1, 2, 3};	// std::initializer list
	fwd2(ilist);				// OK

	fwd2(std::initializer_list<int>({1, 2, 3}));
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
void fwd3(T &&param) {}

void static_const_members_demo()
{
	cout << "A size: " << sizeof(A) << endl;
	cout << "A::m_size: " << A::m_size << endl;	// Value can be used
	
	// Compiles OK, but Linkage error - no address could be taken
	/* const size_t *sptr = &A::m_size; */

	const int *iptr = &A::m_counter; // OK - m_counter has definition
	cout << "A::m_counter: " << *iptr << endl;

	fwd3(100);			// OK - rvalue passed

	/* fwd3(A::m_size); */	// ERROR - linkage error

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

	fwd3(A::m_counter);	// OK - lvalue passed, that has an address


	// TEST
	for(size_t i = 0; i < sizeof(A::arr) / sizeof(A::arr[0]); ++i){
		cout << A::arr[i] << " ";
	}
	cout << endl;

	const char * const *cptr = &A::arr[0];	//
}


// 4.
// If internal function accepts function pointer, and we're trying
// to pass overloaded function - external universal reference function
// couldn't deduce correctly.
// Function pointers with overloaded functions
void func_with_callback( int (*cb) (int) ) { }

int worker(int p) { return p; }
int worker(int p1, int p2) { return p1 * p2; }

template<typename T>
void fwd4(T &&param)
{
	func_with_callback(std::forward<T>(param));
}

void overloaded_function_pointer_demo()
{
	func_with_callback(worker);	// OK - func_with_callback() signature helps
								// to choose correct overload version int(int)

	/* fwd4(worker); */	// ERROR - uni ref can accept any overload version
						// Which one to choose?

	// Solution - help overload deductoin by casting function type

	typedef int(*cb_t)(int);
	fwd4( static_cast<cb_t>(worker) );	// OK - help overload choosing
	// or
	using cb_type = int(*)(int);
	cb_type cb_ptr = worker;
	fwd4( cb_ptr );
}


// 5.
// Bit fields couldn't be deduced correctly
struct IPv4_Header
{
	// 4 bytes 
	std::uint32_t  	version : 4,
					IHL : 4,
					DSCP : 6,
					ECN : 2,
					length : 16;
};

void internal_func(std::size_t len) {}

template<typename T>
void fwd5(T &&param)
{
	internal_func(std::forward<T>(param));
}


void bit_fields_demo()
{
	IPv4_Header ipv4;
	cout << "IPv4_Header size: " << sizeof(IPv4_Header) << endl;

	internal_func(ipv4.length);	// OK

	/* fwd5(ipv4.length); */	// ERROR - cannot bind bit-field

	// Cтандарт С++ : "неконстантная ссылка не может быть привязана к битовому полю': 
	// Тому есть превосходная причина. Битовые поля могут состоять из произвольных 
	// частей машинных слов (например, биты 3-5 из 32-битного int ) , но 
	// непосредственно их адресовать нет никакой возможности. Cсылки и указатели 
	// представляют собой одно и то же на аппаратном уровне, и просто так же, 
	// как нет никакого способа создать указатели на отдельные биты (С++ утверждает, 
	// что наименьшей сущностью, которую вы можете адресовать, является char) , 
	// нет никакого способа связать ссылку с произвольными битами.

	// Solution - use copy of the bit-field 
	auto len = static_cast<std::size_t>(ipv4.length);
	fwd5(len);


	// NOTE: by rules, const reference CAN be bind with bit-field.
	// Actually it binds with internally created copy.
	const auto &len_ref = ipv4.length;
}



int main()
{
	// 2. std::initializer list is forbidden for template parameter deduction
	ilist_uni_ref_deduction_demo();

	// 3. integral static const \ static constexpr without definition 
	static_const_members_demo();
	
	// 4. function pointer with overloads
	overloaded_function_pointer_demo();

	// 5. bit fields
	bit_fields_demo();

	return 0;
}
