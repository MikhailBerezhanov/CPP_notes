
//--- Управление копированием ---

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <exception>
#include <list>
#include <algorithm>			// sort, unique, partition
#include <map>
#include <set>
#include <utility>				// pair, move
#include <memory>				// shared_ptr, unique_ptr, weak_ptr, allocator
#include <new>					// bad_alloc, nothrow

#include "Str_blob.h"
#include "Text_query.h"
#include "Has_ptr.h"
#include "Message.h"
#include "StrVec.h"
#include "exercises.h"


Has_ptr global;	// стандартный конструктор
static Has_ptr foo_bar(Has_ptr arg)	// 1. при передаче аргумента не по ссылке (конструктор-копирования)
{
	Has_ptr local = arg, *heap = new Has_ptr(global); // 2,3 local - инициализация копированием, *heap = global (конструктор-копирования)
	*heap = local;	// оператор присвоения (может быть синтезируемый)
	Has_ptr pa[4] = { local, *heap }; // 4,5 инициализация массива списком инициализации (конструктор-копирования)
	// ПРИМ: для инициализации pa[2], pa[3] дефолтными значениями вызывается стандартный конструктор

	return *heap; // 6. возврат значения не по ссылке (конструктор-копирования)
}

// ПРИМ: для возвращаемого значения тоже выполняется деструктор, даже если оно временное и не используется
// Указатель heap же не вызывает деструктор при выходе из функции , т.к. не является объектом класса
static Has_ptr foo_bar2(Has_ptr arg)
{
	Has_ptr *heap = new Has_ptr("123");
	return *heap;
}


static Has_ptr& foo_bar3(Has_ptr& arg)
{
	return arg;
}

static Has_ptr foo_bar4(const Has_ptr& arg)
{
	return arg;
}


static void f(numbered& s) 
{ 
	std::cout << s.mysn << std::endl;
}

void chapter13(void)
{
	// прямая инициализация
	Has_ptr hp1("qweqeqwewqe qsadfdsagdgsert467658679uhkhkyiuorlojcjbncc11111111111111111111111111111111111111111123333333333333333333333333333333333333333333333333333333333333333333333333333__0");
#if 0
	Has_ptr hp2 (hp1);	// используется синтезированный конструктор копирования
	Has_ptr hp3 = hp1;	// ПРИМ: Важное различие - при инициализации вызывает не перегруженный оператор 
	Has_ptr hp4 = hp1;	// присваивания, а конструктор копирования ( ИНИЦИАЛИЗАЦИЯ КОПИЕЙ )
	Has_ptr hp5 (hp1);	// ПРИМ: Во время инициализации копией компилятору можно (но не обязательно)
	Has_ptr hp6 (hp1);	// пропустить коструктор копирования или перемещения и создать объект прямой инициализацией
	Has_ptr hp7;
	hp7 = hp1;			// присвоение

	std::cout << "hp2.ps: ";
	hp2.show_ps();
#endif

	//foo_bar(hp1);

	//auto a = foo_bar2(hp1);

	//foo_bar3(hp1);

	//foo_bar4(hp1);


	numbered a, b = a, c = b;
	std::cout << "a: " << a.mysn << " b: " << b.mysn << " c: " << c.mysn << std::endl;
	f(a); f(b); f(c);

	hp1 = hp1;	// оператор присвоения копирования должен корректно работать при присвоении самого себя


	Has_ptr_ x;
	Has_ptr_ y("bb");

	x = y;	// счетчик ссылок у 'x' обнуляется, а у 'y' становится 2. Указатель x.ps равен указателю y.ps


	Has_ptr hp2("hello");

	swap(hp1, hp2);

	hp1.show_ps();
	hp2.show_ps();

	//std::vector<Has_ptr> hp_vec { {"q", 1}, {"w", 2}, {"e", 3}, {"r", 4}, {"t", 5}, {"y", 6} }; 

	//std::sort(hp_vec.begin(), hp_vec.end());


	StrVec svec = {"1", "2", "3"};
	svec.push_back("hello");

	int tmp = 10;
	int& r = tmp;

	std::cout << "tmp addr: " << &tmp << ", &tmp addr: " << &r << std::endl;




	// Ссылки на r-значение (можно связать только с тем объектом, который будет удален)

	// ПРИМ: l-value выражения - 
	// 1. ф-ии, возвращающие l-value ссылки
	// 2. присвоение
	// 3. индексирование
	// 4. обращение к значению
	// 5. префиксные операторы инкремента и декремента
	// 6. переменные

	// r-value выражения:
	// 1. ф-ии, возвращающие не ссылочный тип
	// 2. арифметические, реляционные, побитовые выражения
	// 3. постфиксные операторы инкремента и декремента
	// 4. конструкторы

	int i = 43;					// i - l-value
	int &ri = i;				// OK
	//int &&rr = i;				// ОШИБКА: i - это lvalue, нельзя связать ссылку на r-value c l-value
	//int &ri2 = i * 42;		// ОШИБКА: i * 42 - временное значение r-value
	const int& ri3 = i * 42; 	// ОК константная ссылка допускает присвоение к r-value
	int &&rr2 = i * 42; 		// ОК i * 42 - временное значение r-value
	//int &&rr3 = rr2;			// ОШИБКА: rr2 - как переменная является l-value
	int &&rr3 = std::move(rr2);	// ОК - move() явно приводит l-value к r-value
	// ПРИМ: приведенный выше вызов move() обещает не использовать rr2 ни для чего кроме присвоения или удаления
	// после перемещения нельзя сделать никаких предположений о значении уже перемещенного объекта

	int f();
	std::vector<int> vi(100);
	int&& _r1 = f();			// f() - r-value
	int& _r2 = vi[0];			// vi[0] - l-value
	int& _r3 = _r1;				// _r1 - l-value
	int&& _r4 = vi[0] * f();	// r-value

	//hp1 = hp2;
	hp1 = std::move(hp2);

	Foo test;
	test.sorted();

	Foo&& u = Foo();			// конструкторы - r-value
}


