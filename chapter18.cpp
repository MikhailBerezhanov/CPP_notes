//--- Исключения, пространства имен, множественное наследование ---

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <vector>
#include <exception>
#include <list>
#include <algorithm>			// sort, unique, partition
#include <map>
#include <set>
#include <utility>				// pair, move
#include <memory>				// shared_ptr, unique_ptr, weak_ptr, allocator
#include <new>					// bad_alloc, nothrow
#include <functional>			// библиотечные объекты-функции

#include "Str_blob.h"
#include "Sales_data.h"
#include "exercises.h"


// ВАЖНО: необработанные исключения завершают процесс
// ПРИМ: при вызове throw запускаются деструкторы локальных объектов. 
// Деструкторы не должны пораждать исключения, которые они сами не обрабатывают
// Передавать в throw локальный объект - бессмысленно, т.к. к моменту обработки он будет удален

void func(bool b)
{
	std::range_error r("error");
	if(b){
		throw r;
	}
	else{
		std::exception* p = &r;
		throw *p;
	}
}

// ПРИМ: noexcept имеет два значения - спецификатор когда стоит в определении ф-ии и оператор
void f() noexcept;	// обещает, что не будет передавать исключений
void g() noexcept(true); // эквивалентно (не будет передавать исключения)
void g2() throw();	// пустой список пораждаемых исключений
// спецификатор noexcept f2 зависит от спецификатора f
void f2() noexcept(noexcept(f()));	// noexcept(f) - оператор возвращающий  bool


class Base
{
public:
	virtual double f1(double) const noexcept;
	virtual int f2() noexcept(false) = 0;
	virtual void f3();
};

class Derived: public Base
{
public:
	//double f1(double) const; // ОШИБКА: Base::f1() обещает не передавать исключения. И производный класс не должен
	int f2() noexcept(false);	// ОК - та же спецификация что и у базового
	void f3() noexcept;	// когда базовый класс передает исключения, в производном вирт.ф-ия должна быть ограничена
};


// ПРИМ: Глобально пространство имен определяется неявно и доступ к нему осуществляется через ::
namespace test
{
	int i = 10;
	void func(int arg);
	// вложенное пр-во имен
	namespace in
	{
		int i = 43;
	}

}

// определение ф-ии вне пр-ва имен
void test::func(int arg) { return; }

// снова открыть и добавить (встроить) в уже существующее пр-во
namespace test
{
	#include "namespace.h"
}

// Безымянное пр-во имен (у каждого файла свое и не может быть разобщено между разными файлами)
// ПРИМ: определяет сущности локальные для каждого файла (аналог static)
namespace
{
	int p;
}

// Псевдоним пр-ва имен
namespace t_in = test::in;
// Псевдоним типа
using SD = Sales_data;


namespace excersize
{
	int ivar = 0;
	double dvar = 0;
	const int limit = 1000;
}

int ivar = 1;

// обявления using для всех членов пр-ва имен (добавят в глобальную область члены пр-ва)
//using excersize::ivar; ОШИБКА уже есть такая переменная в глобальной области видимости
using excersize::dvar;
using excersize::limit;

void manip()
{
	// pos2
	double dvar = 3.14;
	int iobj = limit + 1;
	++ivar;
	++::ivar;
}



template<typename T> void swap(T l, T r)
{
	using std::swap;	// без определения using произошла бы бесконечная рекурсия
	std::cout << "my swap() called" << std::endl;
	swap(l, r);
}


void chapter18 (void)
{

	try {
		func(false);// При возникновении исключения контекс перемещается на соответсвующий catch и продолжается за ним
	}
	// Передавая исключение по ссылке можно менять его данные. Иначе - изменения затронут лишь локальную копию
	catch(const std::range_error e) {
		std::cerr << "range_error: " << e.what() << '\n';	// what() НЕ передает исключения
	}
	catch(const std::exception& e) {
		std::cerr << "exception occured: " << e.what() << '\n';
		
	}
	catch(...)
	{
		throw; // передать исключение дальше
	}
	// после соответсвующей метки catch выполнение продолжится отсюда
	// ПРИМ: тип параметра блока catch должен четко соответсвовать throw . Допустимы преобразования:
	// - из неконст типа  в конст
	// - из производного типа в базовый
	// - массив и ф-ия преобразуются в соотв тип указателя
	// ПРИМ: в наборе директив catch с типами, связанными наследованием, обработчик для более производных типов
	// следует располагать прежде наимее производных (базовых)


	test::i = 20;
	test::in::i = 120;	// вложенное пр-во
	test::d = 5.0;		// встроенное пр-во 
	p = 9;				// безымянное пр-во


	t_in::i = 130;
	// Директива using . Дает доступ ко всему пр-ву имен
	// ПРИМ: объявляется в глобальной, локальной или области видимости пр-ва имен. Не может быть внутри класса
	using namespace test;	// с этой строчки программе становится достпуно пр-во имен
	i = 30;
	d = 4.43;

	// Объявление using (локальный псевдоним для члена пр-ва имен) Не дает доступ ко всему пр-ву имен.
	using std::string;
	string s;


	// ПРИМ: Когда объект класса передается ф-ии, компилятор ищет пр-во имен, в котором определяется
	// класс аргумента в дополнение к обычному поиску области видимости (справедливо также для указателя и ссылки)
	operator<<(std::cout, "chapter10 operator<<\n");
	// Иначе пришлось бы вызывать std::operator<< или using std::operator<<

	string s2;
	swap(s, s2);	// вызовет std::swap , так как string и пр-ва имен std
	swap(test::i, test::in::i);	// вызовет мой swap, т.к. для test swap() не определен, а в глобальной области есть
}



