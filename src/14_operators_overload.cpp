
//--- Перегрузка операторов и преобразований ---

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
#include <functional>			// библиотечные объекты-функции

#include "Str_blob.h"
#include "Text_query.h"
#include "Has_ptr.h"
#include "Message.h"
#include "StrVec.h"
#include "MyString.h"
#include "exercises.h"


int add(int i, int j) {  return i +j; }


void chapter14(void)
{


	MyString s = "hello Dasha";
	MyString s2 = s;	// Инициализация копированием (вызывается конструктор копирования)

	MyString s3 = "BADABASH";	
	//s = s3;			// Присвоение копирования (переменная s3 - l-value)
	//s = "KAKULYA";	// Присвоение перемещения (литерал - r-value)
	// ИЛИ
	s = std::move(s3);	// Присвоение перемещения (move делает из l-value - r-value), НО s3 уже пользоваться нельзя

	//std::cin >> s;

	//s = s + s2;
	s[0] = 'W';
	std::cout << "My string (size= "<< s.size() <<") is: " << s << std::endl;;

	s2.show(std::cout);
	std::cout << std::endl;


	ReadString rs;	// объект-финукция
	std::string str;
	std::vector<std::string> svec;
	while (rs(str)){
		std::cout << "ReadString: " << str << std::endl;
		svec.push_back(str);
	}	
	size_t sz = 0;
	std::stable_sort(svec.begin(), svec.end(), [sz](const std::string& a, const std::string& b){ return a.size() < b.size();} );
	// ПРИМ: Лямбда-фии - это безымянные объекты - функции класса. В данном случае выглядел бы такой класс примерно так:
	class ShorterString
	{
	public:
		ShorterString(size_t n):sz(n) {} // параметр для каждой захваченной по значению переменной

		// по-умолчанию лямбда ф-ии не могут изменять свои захваченные переменные. поэтому ф-ия - const
		// есди лямбда выражение будет mutable, то ф-ия не будет const.
		bool operator() (const std::string& a, const std::string& b) const { return a.size() < b.size(); }

		// Если у лямбда-выражения пустое тело захвата, в классе отсутсвует члены-переменные. Если в тело захвата передаются
		// переменные по значению, то в классе появиляются переменные-члены, инициализирующиеся конструктором
	private:
		size_t sz;	// переменная-член для каждой захваченной по значению переменной из тела захвата лямбда-выражения
	};
	// Эквивалентный вызов
	std::stable_sort(svec.begin(), svec.end(), ShorterString(sz));


	// Применение библиотечного объекта-фи-ии из <functional> с алгоритмами
	std::sort(svec.begin(), svec.end(), std::greater<std::string>());

	// ПРИМ: также библиотека <functional> определяет класс function позволящий хранить разные вызываемые объекта 
	// с одинаковой сигнатурой

	auto mod = [](int i, int j) -> int { return i % j; }; // лямбда-выражение, создающий безымянный класс объекта-ф-ии

	struct div	// класс объекта-ф-ии
	{
		int operator() (int de, int di) { return de / di; }
	};

	// При необходимости можно составить массив или карту необходимых вызываемых объектов при помощи шаблона function
	std::function<int(int, int)> f1 = add;		// обычная ф-ия
	std::function<int(int, int)> f2 = div();	// лямбда-выражение
	std::function<int(int, int)> f3 = mod;		// объект-ф-ия

	// s пребразуется в int (пользовательским преобразованием), а затем а double (встроенным преобразованием)
	// явное преобразование требуется т.к. пользовательское преобразование в int объявлено как explicit
	std::cout << "sum with pi: " << static_cast<int>(s) + 3.14 << std::endl;

	// Не смотря на то, что пользовательское преобразование в bool опеределно как explicit, в лог.выражениях
	// компилятор вызывает его неявно
	if(s && !s) {}
	auto tmp = s ? 1 : 0;

	//bool BUT = s;	// ОШИБКА: не лог. выражение. преобразование должно вызывать ЯВНО
	const int tmp2 = s;




	MyStringNew sn("String with New and Delete[]");
	MyStringNew sn2 = sn;
	sn2 = "BAZOOKA";

	sn2.show(std::cout);
	std::cout << std::endl;
}	