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

//#include "Str_blob.h"
//#include "Sales_data.h"
#include "exercises.h"


// Наследование конструкторов при множественном наследовании
struct Base1
{
	Base1() = default;
	Base1(const std::string&){}
	Base1(int*);

	virtual ~Base1() = default;

	virtual void print() { std::cout << "Base1 print() called" << std::endl; }
};

struct Base2
{
	Base2() = default;
	Base2(const std::string&){}
	Base2(int);

	virtual ~Base2() = default;

	virtual void print() { std::cout << "Base2 print() called" << std::endl; }
};

// ПРИМ: нельза наследовать тот же конструктор (с тем же списком параметров) от более чем одного класса
// Производный класс должен определить свою версию конструктора с совпадающими параметрами.
struct D1 : public Base1, public Base2
{
	using Base1::Base1;
	using Base2::Base2;

	D1(const std::string& s): Base1(s), Base2(s){}
	D1() = default;	// требуется т.к. класс определяет собственный конструктор

	~D1() { std::cout << "~D1() called"<< std::endl; };

	virtual void print() { std::cout << "D1 print() called" << std::endl; }
};

struct D2 : public Base1
{
	~D2() { std::cout << "~D2() called"<< std::endl; };

	virtual void print() { std::cout << "D2 print() called" << std::endl; }
};

struct D3 : public Base2
{
	~D3() { std::cout << "~D3() called"<< std::endl; };

	virtual void print() { std::cout << "D3 print() called" << std::endl; }
};

struct MI2 : public D2, public D3
{
	~MI2() { std::cout << "~MI2() called"<< std::endl; };

	virtual void print() { std::cout << "MI2 print() called" << std::endl; }
};


struct A { A() { std::cout << "A() called" << std::endl; } };
struct B : public A { B() { std::cout << "B() called" << std::endl; } };
struct C : public B { C() { std::cout << "C() called" << std::endl; } };

struct X { X() { std::cout << "X() called" << std::endl; } };
struct Y { Y() { std::cout << "Y() called" << std::endl; } };
struct Z : public X, public Y { Z() { std::cout << "Z() called" << std::endl; } };
struct MI : public C, public Z { MI() { std::cout << "MI() called" << std::endl; } };




class ZooAnimal
{
public:
	double max_weight() { return 2.0; }

	int x = 0;
private:

};

// ПРИМ: Список наследования не может включать тот же базовый класс несколько раз, однако тот же
// базовый класс может унаследоваться многократно косвенным образом (через прямые базовые)
// Если тот же базовый класс наследуется несколько раз, то у объекта производного класса будет
// больше одного внутреннего объекта этого типа. Чтобы избежать такого стандартного поведения
// используется виртуальное наследование. Оно позволяет классу указать, что его базовый класс
// будет использоваться совместно (виртуальный базовый класс). Независимо от того, сколько раз
// тот же базовый виртуальный класс присутсвует в иерархии наследования, объект производного
// класса содержит только один совместно используемый внутренний объект этого вирт. базового класса.
class Bear : virtual public ZooAnimal
{
public:
	Bear() = default;
	Bear(std::string name, bool onExhibit, const char* type){}
	// переопределяет х виртуалього базового класса
	int x = 43;	
private:
};

// Спецификатор virtual в списке наследования заявляет о готовности совместно использовать единый
// экземпляр указанного базового класса в последующих производных
class Raccoon : public virtual ZooAnimal
{
public:
	Raccoon() = default;
	Raccoon(std::string name, bool onExhibit, const char* type){}

	// переопределяет х виртуалього базового класса и Вызывает неоднозначность при доступе из класса Panda
	//int x = 103;	// ПРИМ: В таком случае внутри Panda х должен быть переопределен
private:
};

class Endangered
{
public:
	Endangered() = default;

	Endangered(int lvl){}

	double max_weight(){ return 1.0; }

	enum{
		normal = 0,
		critical,
	};
private:
	
};

// Множественное наследование - Способность получать класс производный от нескольких базовых
// ПРИМ: Поиск имен осуществляется одновременно во всех прямых базовых классах. Если имя 
// находится в нескольких базовых классах, происходит ошибка неоднозначности
// ПРИМ: класс Panda наследует ZooAnimal косвенно через два своих базовых класса, но поскольку
// эти классы происходят от ZooAnimal виртуально, у класса Panda есть только одна часть ZooAnimal
class Panda : public Bear, public Raccoon, public Endangered
{
public:
	// Содержит члены класса ZooAnimal
	// ПРИМ: Части виртуальных базовых классов всегда создаются до частей обычных базовых классов
	// независимо от того, где они располагаются в иерархии наследования.

	// Содержит члены класса Bear

	// Содержит члены класса Endangered

	// Члены класса Panda


	// ПРИМ: Порядок выполнения конструкторов зависит от их расположения в списке наследования
	// ZooAnimal() -> Bear() -> Endangere() -> Panda()
	// ПРИМ: Деструкторы всегда выполняются в обратном порядке
	// ~Panda() -> ~Endangere() -> ~Bear() -> ~ZooAnimal()
	// Явная инициализация объекта обоих базовых классов
	Panda(std::string name, bool onExhibit): 
		Bear(name, onExhibit, "Panda"), 
		Endangered(Endangered::critical){} 

	// Неявное применение стандартного конструктора класса Bear
	Panda(): Endangered(Endangered::critical){}

	// При совпадении имен членов базовых классов, необходимо явно указывать какая версия используется
	double max_weight(){ return std::max( ZooAnimal::max_weight(), Endangered::max_weight() ); }
};

// Указатель или ссылка на любой из базовых классов (ZooAnimal, Bear, Endangere)
// может успешно ссылаться \ указывать на объект класса Panda
void print_b(const Bear&);
void highlight(const Endangered&);
std::ostream operator<< (std::ostream&, const ZooAnimal&);




int multi_inheritance_test ()
{
	MI mi;

	Base1* pb1 = new MI2;
	Base2* pb2 = new MI2;
	D2* pd1 = new MI2;
	D3* pd2 = new MI2;
	pb1->print();		// MI2
	pb2->print();		// MI2
	pd1->print();		// MI2
	pd2->print();		// MI2
	delete pb1;			// MI2 -> D3 -> D2
	delete pb2;			//
	delete pd1;			//
	delete pd2;			//



	Panda pan;
	// Версия члена х в производном классе Bear имеет приоритет перед совместно используемым
	// виртуальным базовым классом ZooAnimal,  поэтому будет вызван переопределенный член х
	std::cout << "pan.x: " << pan.x << std::endl;

	return 0;
}