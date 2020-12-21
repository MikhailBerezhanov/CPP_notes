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



// ПРИМ: Когда два или более отдельно откомпилированных файла используют тот же шаблон с теми же аргументами,
// создание экземпляра этого шаблона осуществляется в каждом из этих файлов! Этих затрат можно избежать за счет
// явного создания экземпляра
extern template class Blob<std::string>;	// Объявление
// Когда компилятор встретит объявление шаблона, он не будет создавать код его экземпляра в этом файле. Предполагается
// что данный экземпляр уже создан в другом файле и связан с текущим во время компиляции.
// Для каждого объявления экземпляра где-нибудь в программе должно быть определение явного создания экземпляра.
// (определение находится в Str_blob.cpp)

template<typename T> T fobj (T p1, T p2) { return p1; }	// аргументы копируются
template<typename T> T fref (const T& r1, const T& r2) { T tmp; return tmp; } // аргументы - ссылки
// обычные преобразования применимы к обычным аргументам (тип которых не является параметром шаблона)
// в качестве os может быть использован как ostream, так и ofstream и тп для одного экземпляра
template<typename T> inline 
std::ostream& print (std::ostream& os, const T& obj) 
{ 
	return os << obj; 
}

template<typename T> T calc(T t, int i) { return t; }
template<typename T> T fcn(T t1, T t2) { return t1; } 

// T1 - не может быть выведен, он отсутствует в списке параметров ф-ии
template<typename T1, typename T2, typename T3> T1 sum (T2 v1, T3 v2) { return v1 + v2;}

// Замыкающие типы возвращаемого значения
template<typename It>
auto func(It beg, It end) -> decltype(*beg)
{
	return *beg; // нужно вернуть ссылку на тип значения, хранящегося по итератору
}

// СТАНДАРТНЫЕ ШАБЛОНЫ ТРЕНСФОРМАЦИИ ТИПА (у шаблона remove_reference один параметр типа шаблона и открытый член type)
// Возвращает эл-т по значению , а не по ссылке (копию)
template<typename It>
auto func2(It beg, It end) -> typename std::remove_reference<decltype(*beg)>::type
{
	return *beg;	// вернет копию эл-та
}

template<typename It>
auto func3(It beg, It end) ->  decltype(*beg + 0)	// эл-ты контейнера должны поддерживать операцию + с int
{
	return *beg;	// вернет копию эл-та
}


template<typename T> void f1(T&){};		// Аргумент только l-value
template<typename T> void f2(const T&){};	// Аргумент константный (или нет) объект, r-value или l-value
template<typename T> void f3(T&&){};		// Аргумент r-value или l-value (сворачивание ссылок)
// ПРИМ: Шаблону с параметром ссылки r-value МОЖНО передавать аргумент l-value . При этом происходит сворачивание
// ссылок. Тип Т выводится как int& ,а не просто int , что приводит к появлению T& && что преобразуется в T& - l-value
// X& &, X& &&, X&& & сворачиваются в X&
// X&& && сворачивается в X&&
// ПРИМ: Сворачивание ссылок применимо только тогда, когда ссылка на ссылку создается косвенно, как в псевдониме
// типа или параметре шаблона. Всвязи с этим возможны проблемы написания правильного кода ф-ии:
template<typename T> 
void f4(T&& val)
{
	T t = val; 			// копировать или привязать ссылку?
	t = fcn(t);			// изменит ли присвоение только t или еще и val ?
	if (val == t) {} 	// всегда истинно если Т - ссылка l-value
}

// Пример определения ф-ии std::move() преобразующей объект в r-value (объект может быть как r-value, так и l-value)
template<typename T>
typename std::remove_reference<T>::type&& move(T&& t)
{
	// ПРИМ: оператор static_cast поддерживает приведение l-value к ссылке на r-value и делает это ЯВНО
	return static_cast<typename std::remove_reference<T>::type&&>(t);
}


// Перенаправление (передача параметров шаблона внутренним ф-иям)
template<typename F, typename T1, typename T2>
void flip1(F f, T1 t1, T2 t2)
{
	f(t2, t1);
}
// Этот шаблон работает пока он не использует для вызова ф-ию со ссылочным параметром
void ff(int v1, int& v2)
{
	std::cout << v1 << " " << ++v2 << std::endl;
}
// flip1(ff, i, 43);  - ОШИБКА: не изменяется i , изменяется его локальная копия
// Проблема в том, что ff изменяет свой второй аргумент, а параметр шаблона t1 - простой не ссылочный тип
// при передаче аргумента вызовется копирование и исходный аргумент изменен не будет
// (ссылочный параметрр ф-ии ff() будет связано с t1 , а не с входной переменной)
// Благодаря сворачиванию ссылок можно сохранить принадлежность аргументов в l-value и r-value
template<typename F, typename T1, typename T2>
void flip2(F f, T1&& t1, T2&& t2)
{
	f(t2, t1);
}
// Однако это не решает проблему, если потребуется использовать ф-ию , принимающую ссылки на r-value:
void g(int&& v1, int&& v2)
{
	std::cout << v1 << " " << v2 << std::endl;
}
// Так как в таком случае t1 внутри ф-ии шаблона является l-value (как переменная) и в ф-ию g() не удастся передать
// l-value объект в параметр ссылки на r-value
// flip2(g, i, 43);  -  ОШИБКА: нельзя инициализировать int&& из l-value 

// Так, чтобы передать ф-ии параметры способом, сохраняющим типы первоначальных аргументов, можно использовать
// ф-ию std::forward() из <utility>. Обычно эту ф-ию испоьзуют для передачи параметра ф-ии, который определен
// как ссылка r-value, параметру типа шаблона. Благодаря сворачиванию ссылок для типа возвращаемого значения
// ф-ия forward() сохраняет характер l-value или r-value переданного аргумента
template<typename T> 
void external_func(T&& arg)
{
	// благодаря сворачиванию ссылок тип Т будет либо просто Т, либо T&
	// таким образом, forward() вернет либо T&& (r-value) либо T&(l-value) соответственно
	internal_func(std::forward<T>(arg));	
} 


// Перегрузка шаблонных ф-ий
template<typename T> void foo(T t) { std::cout << "foo(T) called"<< std::endl; }
template<typename T> void foo(const T* t) { std::cout << "foo(const T*) called"<< std::endl; }
template<typename T> void goo(T t) { std::cout << "goo(T) called"<< std::endl; }
template<typename T> void goo(T* t) { std::cout << "goo(T*) called"<< std::endl; }


// Шаблон с переменным кол-вом аргументов
template<typename T, typename... Args>		// Args - пакет параметров шаблона
void woo(const T& t, const Args& ... rest)	// rest - пакет параметров ф-ии  (развертывание Args)
{
	std::cout << "woo() Args number: " << sizeof...(Args) << std::endl;
	std::cout << "woo() rest number: " << sizeof...(rest) << std::endl;
}
// Перенаправление переменного кол-ва аргументов
template<typename T, typename... Args>		
void woo2(const T& t, Args& ... args)	// развертывание Args в список ссылок r-value
{
	// чтобы forward() применялась к каждому аргументу, а не ко всему списку необходимы скобки для args
	work(std::forward<Args>(args)...);  
}
// ПРИМ: когда не шаблонная фи-я обеспечивает одинаково хорошее соответствие с щаблонной, выбирается не шаблонная версия


// Специализация шаблона
// специальная версия compare для обработки символьный массивов (иначе сравнивались бы значения указателей, а не strcmp)
// ПРИМ: при определении специализации типы параметров ф-ии должны совпадать с соотв. типами ранее объявленного шаблона
// template<typename T> int compare (const T& v1, const T& v2) В данным случае Т - const char*
template<>
int compare (const char* const &p1, const char* const &p2)
{
	return std::strcmp(p1, p2);
}
// ПРИМ: специализация создает экземпляр шаблона, а не перегружает его. Выбираться будет более СПЕЦИАЛИЗИРОВАННА версия
// ПРИМ: специализация должна быть в области одной области видимости с оригинальным объявлением шаблона и ПЕРЕД
// любым использование этого шаблона (иначе будет создан экземпляр из оригинала)

// Частичная специализация
template<class T> struct remove_reference	// наиболее общий шаблон
{
	typedef T type;
};

template<class T> struct remove_reference<T&>	// l-value ссылки
{
	typedef T type;
};

template<class T> struct remove_reference<T&&>	// r-value ссылки
{
	typedef T type;
};


void chapter16 (void)
{
	compare(1, 0); // T - int, создается экземпляр int compare (const int& , const int& )
	compare("hi", "mom"); // int compare (const char (&p1)[3], const char (&p2)[4])

	Sales_data sd1, sd2;
	//compare(sd1, sd2);	// ОШИБКА. для данного типа неопредлена операция <

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


	// ПРИМ: Процесс определения аргументов шаблона по аргументам ф-ии называется дедукцией аргумента шаблона
	// Вместо преобразований аргументов компилятор создает новые экземпляры. Допустим лишь 2 преобразования:
	// 1. Преобразование констант - параметр ф-ии, являющийся ссылкой (или указателем) на константу, может быть 
	// 	  передан как ссылка (или указатель) на не константный объект.
	// 2. Преобразование массива или ф-ии в указатель - если тип параметра ф-ии не будет ссылочным, то к аргументам 
	//	  типа массива или ф-ии будет применено обычное преобразование указателя. Аргумент типа массива будет 
	//	  преобразован в указатель на его первый эл-т, а аргумент типа ф-ии - в указатель на тип ф-ии.
	std::string s1("a value");
	const std::string s2("another value");
	fobj(s1, s2); // const игнорируется. вызов fobj(string, string)
	fref(s1, s2); // использует допустимое преобразование в константу для s1. fref(const string& , const string&)
	int a[10], b[42];
	fobj(a, b);	  // вызов fobj(int* , int*)
	//fref(a, b);	  // ОШИБКА: типы массивов не совпадают

	char c;
	double d;
	float f;
	calc(c, 'c');	// Вызов calc(char, int). 'c' преобразуется в int
	calc(d, f);		// Вызов calc(double, int). f преобразуется в int
	fcn(c, 'c');	// Вызов fcn(char, char)
	//fcn(d, f);	// ОШИБКА: вызов шаблона ф-ии с разными типами аргументов fcn(double, float)


	// Определение явного аргумента шаблона Т1 определяется явно, Т2 и Т3 выводятся из типов аргументв
	// ПРИМ: список явных аргументов имеет тот же порядок, что и в шаблоне (слева направо)
	auto val3 = sum<long long>(1024, 3.14); // long long sum(int, double)

	//compare(f, d);	// ОШИБКА: типы аргументов не совпадают
	compare<double>(f, d); // вызывает compare(const double& , const double& )
	//std::max(1024, d);
	std::max<double>(1024, d);	// 1024 преобразуется в double

	std::vector<std::string> vs{"hello", "world"};
	func(s1.begin(), s1.end()) = 'b';	// возвращает char&
	func(vs.begin(), vs.end()) = "goodbie";	// возвращает string&
	std::for_each(vs.begin(), vs.end(), [](const std::string& s){ std::cout << s << " "; });
	std::cout << std::endl;

	auto x = func3(std::begin(a), std::end(a));


	// Сворачивание ссылок
	// Параметру ф-ии, являющемуся ссылкой на r-value на тип параметра шаблона (T&&), может быть
	// передан аргумент любого типа. Когда такому параметру передается l-value, экземпляр ф-ии создается с 
	// параметром T&
	int i = 0;
	const int ci = 10;
	f3(i);	// аргумент - l-value, параметр шаблона T - int&
	f3(ci);	// аргумент - l-value, параметр шаблона T - const int&
	f3(i = ci);

	// Перегруженные шаблоны ф-ий
	int* p = &i;
	const int* p2 = &ci;
	goo(42);	// goo(T)
	goo(p);		// goo(T*)
	goo(ci);	// goo(T)
	goo(p2);	// неявное преобразование аргумента к const допустимо goo(const T*)  (из goo(T*))
	foo(42);	// foo(T) : T - int
	foo(p);		// foo(T) : T - int*
	foo(ci);	// foo(T) : T - const int
	foo(p2);	// foo(const T*) - отбросить const шаблон не может. только добавить в тип Т


	// Шаблон с переменным кол-вом аргументов
	woo(i, s1, 42, d);	// 3 параметра в пакете
	woo(s1, 42, "hi");	// 2 параметра в пакете
	woo(d, s1);			// 1 параметра в пакете
	woo("hi");			// пустой пакет
}