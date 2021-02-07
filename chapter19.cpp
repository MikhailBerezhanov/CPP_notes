//--- Специализированные инструменты и технологии ---

#include <iostream>
#include <string>
#include <memory>				// shared_ptr, unique_ptr, weak_ptr, allocator
#include <new>					// bad_alloc, nothrow
#include <typeinfo>				// typeid

#include "exercises.h"


// Операторы new и delete можно перегрузить. Они имеют 8 версий:
#if PSEUDOCODE
// версии, способные передавать исключения
void* operator new(size_t);
void* operator new[](size_t);
void operator delete(void*) noexcept;	// освобождает память зарезервированную объектом (после деструктора)
void operator delete[](void*) noexcept;	//

// версии, обещающие не передавать исключений
void* operator new(size_t, nothrow_t&) noexcept;
void* operator new[](size_t, nothrow_t&) noexcept;
void operator delete(void*, nothrow_t&) noexcept;
void operator delete[](void*, nothrow_t&) noexcept;

// при вызове функции operator new [] ( ) передается количество байтов,
// необходимых для хранения массива заданного количества элементов


// ПРИМ: Заголовок new определяет также константны йобъект nothrow, который пользователи
// могуг передавать как сигнал, что необходима версия оператора new, не передающего исключения

// Размещающий оператор - вызванный с одним параметров не резервирует память
// (создает объект в заданной адресом предварительно зарезервированной области памяти)
void * operator new (size_t , void*); // эта в ерсия не может быть переопределена

#endif


class BaseT
{
public:
	BaseT() = default;
	virtual ~BaseT() = default;
	virtual void func(int a) { std::cout << a << std::endl; }
private:
};

class DerivedT: public BaseT
{
public:
	DerivedT(): BaseT() {}
	virtual void func(int a) { std::cout << a*10 << std::endl; }
};


void dyn_cast1(BaseT* pb)
{
	// При неудаче приведения к типу указателя оператор возвращает 0
	if ( DerivedT* pd = dynamic_cast<DerivedT*>(pb) ){
		pd->func(5);
	}
	else{
		std::cout << "dynamic cast failed" << std::endl;
	}
}

void dyn_cast2(BaseT& rb)
{
	// При приведении к типу ссылки возбуждается исключение bad_cast
	try{
		DerivedT& d = dynamic_cast<DerivedT&>(rb);
	}
	catch(const std::bad_cast& e){
		std::cerr << e.what() << std::endl;
	}
}

// Перечисление с неограниченной зоной видимостью
enum values
{
	one,
	two,
	three
};

//  Перечисление с ограниченной зоной видимостью
enum class enum_class
{
	one = 1,
	two,
	three
};

enum class test: unsigned long long
{
	one,
};


void chapter19 (void)
{
	BaseT b;

	dyn_cast1(&b);
	
	dyn_cast2(b);

	DerivedT* dp = new DerivedT;
	BaseT* bp = dp;

	if(typeid(*bp) == typeid(*dp)){
		std::cout << "*bp type is *dp type: " << typeid(*bp).name() << std::endl;
	}

	if(typeid(*bp) == typeid(DerivedT)){
		std::cout << "*bp type is Derived: " << typeid(*bp).name() << std::endl;
	}

	if(typeid(bp) == typeid(DerivedT)){
		// bp - BaseT*
	}
	else{
		std::cout << "bp type is not Derived: " << typeid(bp).name() << std::endl;
	}


	values e1 = two;
	int i = one;

	enum_class e2 = enum_class::one;
}