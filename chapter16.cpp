//--- Шаблоны и обобщенное программирование ---

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


// Шаблон финкции
template <class T>	// список параметров в определении шаблона не может быть пустым (допускается typemane или class)
					// тип Т шаблона выбирается компилятором при вызове ф-ии из типов переданных аргументов
					// ПРИМ: при этом созданяет ЭКЗЕМПЛЯТР ШАБЛОНА
int compare (const T& v1, const T& v2)
{
	// не все типы могут поддерживать оба оператора < и > поэтому лучше использовать какой-то один, или стандартные
	// шаблонные ф-ии сравнения
	//if(v1 < v2) return -1;
	//if(v1 > v2) return 1;

	if(std::less<T>()(v1, v2)) return -1;	// less из <functional>
	if(std::less<T>()(v2, v1)) return 1;

	return 0;
}

// первый параметр шаблона представляет размер первого массива, второй - второго. 
// Шаблон может быть объявлен как inline и constexpr
template<unsigned N, unsigned M> inline
int compare (const char (&p1)[N], const char (&p2)[M])	// ссылка на миссив неизвестный длины( без скобок - массив ссылок )
{
	return std::strcmp(p1, p2);
}

// Шаблоы могут иметь параметры по умолчанию аналогично ф-иям
template<typename T, typename F = std::less<T>>
int compare2(const T& v1, const T& v2, F f = F())
{
	if(f(v1, v2)) return -1;
	if(f(v2, v1)) return 1;

	return 0;
}


template<typename T, typename R>
bool my_find(T beg, T end, R value)
{
	while(beg != end){
		if (*beg == value) return true;
		++beg;
	}

	return false;
}

// аналог стандартного begin() для встроенных массивов
template<typename T, std::size_t N>
T* my_begin(T (&array)[N])	// ссылка на массив неизвестного размера типа Т
{
	return &array[0];
}

template<typename T, std::size_t N> constexpr
std::size_t my_size(T (&array)[N])
{
	return N;
}



typedef double A;

template <typename A, typename B> void f (A a, B b)
{
	A tmp = a;	// еьз иммеет тип параметра шаблоана А, а не double
	//double B; // Ошибка: повторное объявление параметра шаблона B
}

// По умлчанию имя, к которому обращаются через оператор области видимости :: не является типом, а является переменной
// Если необходимо использовать тип-член параметра типа шаблона, следует явно уэто указывать через typename
template<typename T>
typename T::value_type top(const T& c)
{
	if(!c.empty()) return c.back();

	return typename T::value_type();
}

// Псевдоним типа шаблона
template<typename T> using twin = std::pair<T, T>;

void chapter16 (void)
{
	compare(1, 0); // T - int, создается экземпляр int compare (const int& , const int& )
	compare("hi", "mom"); // int compare (const char (&p1)[3], const char (&p2)[4])

	Sales_data s1, s2;
	//compare(s1, s2);	// ОШИБКА. для данного типа неопредлена операция <

	std::vector<int> vi(5, 10);
	std::list<std::string> lstr{"test1", "test2", "elem3"};
	bool ret = my_find(vi.begin(), vi.end(), 10);
	std::cout << "my_find() int vector<int>: " << ret << std::endl;
	ret = my_find(lstr.begin(), lstr.end(), "elem2");
	std::cout << "my_find() int list<string>: " << ret << std::endl;


	char arr[10];
	char* beg = my_begin(arr);
	*beg = 'c';
	std::cout << "arr.size: " << my_size(arr) << " arr.begin: " << arr[0] << std::endl;


	twin<std::string> authors; // authors - это пара <string, string>
}