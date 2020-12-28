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
		std::cerr << "exception error: " << e.what() << '\n';
		
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

}