
//--- Динамическая память и интеллектуальные указатели ---

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
#include <utility>				// pair
#include <memory>				// shared_ptr, unique_ptr, weak_ptr, allocator
#include <new>					// bad_alloc, nothrow

#include "Str_blob.h"
#include "Text_query.h"
#include "exercises.h"

static std::shared_ptr<int> example()
{
	auto sp = std::make_shared<int>(43);

	std::cout << "sp.use_count(): " << sp.use_count() << std::endl;	// 1

	return sp;	// при возвращении счетчик ссылок увеличивается до 2, выйдет из облаасти видимсоти и снова станет 1
}
// sp выходит из области видимости, но память, на которую он указывал не освобождается

static void use_example()
{
	auto p = example();

	std::cout << "p.use_count(): " << p.use_count() << std::endl; // 1

	// дейтсвия с p
}
// p выходит из области видимости, память, на которую он указывал освобождается автоматически

bool test()
{
	int* p = new int;
	return p;
}

// --- ВАЖНО ---
// 1. Умные указатели устойчивы к исключениям, т.к. нарушается область видимости
// 2. Умные указатели могут использовать не только для доступа к динамической памяти
#if PSEUDO_CODE
void f (dest &d)
{
	connection c = connect(&d);

	std::shared_ptr<connection> p(&c, [](connection* c) { disconnect(*c);});
	// эквивалентно (end_connection() - функция завершения соединения)
	std::unique_ptr<connection, decltype(end_connection)*> p(&c, end_connection);

	// При выходе из функции ДАЖЕ В СЛУЧАЕ ИСКЛЮЧЕНИЯ соединение будет закрыто корректно
}
#endif

#include <cstring>
// str1, str2 - строковые литералы. Результат Конкатенации записывается в динамический массив
static std::unique_ptr<char[]> my_concat(const char* str1, const char* str2)
{
	size_t str1_len = 0;
	size_t str2_len = 0;
	size_t total_len = 0;

	const char* ptr = str1;

	while(*ptr++) ++str1_len;
	ptr = str2;
	while(*ptr++) ++str2_len;

	std::cout << "str1: " << str1 << std::endl;
	std::cout << "str2: " << str2 << std::endl;
	std::cout << "str1 len: " << str1_len << " str2_len: " << str2_len << std::endl;
	total_len = str1_len + str2_len + 1;
	std::cout << "total_len: " << total_len << std::endl;

	std::unique_ptr<char[]> up(new char[total_len]);

	std::memset(&up[0], 0, total_len);
	std::memcpy(&up[0], str1, str1_len);
	std::memcpy(&up[str1_len], str2, str2_len);
	up[str1_len + str2_len] = 0;

	return up;
}

// Передача умного указателя в функцию
// unique_ptr - только по значению для корректной работы. При этом из-за отсутсвия 
// конструктора копирования вызывать надо через перемещение func2(std::move(ptr))
static void func2 (std::unique_ptr<int> uptr)
{
	// Вызывается func2(std::move(ptr)) 
} // Здесь uptr удаляется

// ИЛИ 
static void func3 (int *ptr)
{
	// Вызывается func3(uptr.get()) - передачей сырого указателя
}



void chapter12 (void)
{

	// ПРИМ: Интеллектуальные указатели гарантирует освобождение памяти при выходе из зоны видимости 

	std::shared_ptr<std::string> sp;	// создает пустой указатель на string (позволяет нескольким указателям указывать на тот же объект)
	std::unique_ptr<int> up;			// создает пустой уникальный указатель int

	auto p = std::make_shared<int>(42);	// выделяет динамическую память для объекта и возвращает инт. указатель shared_ptr
	std::shared_ptr<int> q;
	*p = 10;
	p.get();		// Возвращает указатель, хранимый указателем sp
	std::cout << "p.get(): " << p.get() << std::endl;	// адрес
	std::cout << "*p.get(): " << *p.get() << std::endl;	// значение
	p.unique();		// Возвращает true если use_count равно 1 (только один указатель на объект)
	p.use_count();	// Возвращает кол-во объектов совместно использующих указатель p
	std::shared_ptr<int> p2(q);	// p2 - копия q. Инткремент счетчика ссылок q
	p = q;			// Происходит декремент счетчика ссылок p и инкремент счетчика q. Если счетчик p = 0, происходит память его объекта освобождается

	use_example();

	Str_blob b1;
	//b1.front();

	{
		Str_blob b2 = {"a", "b", "c"}; // b2 инкремент счетчика ссылок - 1
		b1 = b2; // b2 инкремент счетчика ссылок - 2, b1 - декремент, удаляется
		b2.push_back("ass");

		std::cout << "Str_blob use count: " << b2.use_count() << std::endl;
	}// b2 декремент счетчика ссылок - 1

	std::cout << "Str_blob use count: " << b1.use_count() << std::endl;
	std::cout << "b1 size: " << b1.size() << std::endl; 


	int* i1 = new int;	// создает неинициализированный объект в динамической памяти
	const int* i2 = new const int(43);		// инициализирует число как 43. при неудаче передает исключение std::bad_alloc
	auto pd = new auto(3.14);				// выводит тип из значения инициализации	
	int* i3 = new (std::nothrow) int(1024);	// при неудаче вернет nullptr (размещающий оператор new)

	// Нет автоматического освобождения памяти, только явное
	delete i1;
	delete i2;
	delete i3;
	delete pd;	// delete допускает освобождение nullptr

	test();


	//std::shared_ptr<int> sp1 = new int(1024);	// ОШИБКА требуется прямая инициализация т.к. конструктор shared_ptr explicit
	std::shared_ptr<int> sp2(new int(1024));

	// ПРИМ: по умолчанию используется delete для освобождения объекта, на который указывает умный указатель
	// но такое поведение можно изменить, добавиви вторым параметром прри инициализации новую функцию освобождения
	std::shared_ptr<int> sp3(new int[10], [](int* arr){ delete[] arr; } );	// указывает на массив, освобождаться должен тоже массив 
	// shared_ptr не поддерживает индексаци по массиву. Вместо этого следует использовать метод get()
	for(int i = 0; i != 10; ++i){
		*(sp3.get() + i) = i;		// доступ к эл-там динамического массива через shared_ptr
	}


	int* q2 = nullptr;	// должен указывать на область динамической памяти или быть нулевым, т.к. умный указатель исп delete

	sp3.reset();	// если sp3 единственный указатель на объект, то функция reset() освободит существующий объект,
	sp2.reset(q2);	// если передан указатель встроенного типа, то sp будет указывать на q, декрементировав счетчик ссылок
	sp2.reset(new int, [](int* i){ delete i;});	// так же можно передать собственную функцию освобождения


	auto sp4 = std::make_shared<int>();
	auto ptr = sp4.get();
	//delete ptr;	// двойное освобождение. т.к. умный указатель при выходе из ф-ии сделает еще один delete тому же адресу 


	// unique_ptr владеет объектом, на который указывает. не поддерживает копирования и присваивания
	std::unique_ptr<std::string> up1(new std::string("heap string")); 

	up1 = nullptr;	// удаляет объект, на который указывает и обнуляет указатель
	auto ptr2 = up1.release();	// прекращает контроль содержимого, возвращает содержимое указателя и обнуляет указатель
	up1.reset();	// удаляет объект, на который указывает (в данном случае ничего не освободит)
	delete(ptr2);

	// ПРИМ: при возвращении unique_ptr через return копирование возможно, но используется особый его тип
	// unique_ptr может использоваться для работы с динамическим массивом (автоматически вызывает delete[])
	std::unique_ptr<int[]> up2(new int[10]{1,2,3});
	// также поддерживает индексацию в отличие от shared_ptr
	up2[3] = 43;


	// ПРИМ: Deleter является частью типа у unique_ptr и частью конструктора у shared_ptr 
	auto lambda = [q2] (FILE* fp) { if(fp) fclose(fp); }

	// Допусткается в качестве deleter'a использовать функтор (лямбда функцию). Если она имеет
	// список захвата, наобходимо в качесте типо использовать шаблон std::function<>
	std::unique_ptr<FILE, std::function<void(FILE*)> ex_uptr (fopen("test"), lambda);


	// weak_ptr не контролирует продолжительность существования объекта, на который указывает. он только указывает 
	// на объект, который контролирует shared_ptr. Пивыязка weak_ptr к shared_ptr не имезняет счетчик ссылок.
	auto sp5 = std::make_shared<int>(43);
	std::weak_ptr<int> wp(sp5);

	wp.reset();		// Обнуляет указатель
	wp = sp5;		// Присваивает указатель типа shared_ptr или weak_ptr
	wp.use_count();	// Возвращает кол-во указателеть shared_ptr, с которым связан
	wp.expired();	// Возвращает true, когда shared_ptr.use_count == 0 (больше нет указателей на объект)
	wp.lock();		// Возвращает нулевой указатель shared_ptr если нет указателей на объект, в противном случае - на объект 


	std::unique_ptr<char[]> cp = my_concat("Hello ", "world!");
	for(int i = 0; cp[i]; ++i) std::cout << cp[i];
	std::cout << std::endl;


	std::allocator<std::string> alloc;	// объект, способный резервировать строки (ПАМЯТЬ НЕ ЗАПОЛНЕНА)
	auto a = alloc.allocate(10);	// резервирует 10 строк, но не инициализирует их
	for(int i = 0; i != 10; ++i) alloc.construct(a++, "test");	// передает аргументы конструктору объекта
	// ПРИМ: перед deallocateстоит вызывать destroy для всех объектов, созданных в области памяти
	for(int i = 0; i != 10; ++i) alloc.destroy(--a);	// вызывает деструктор для объекта. (a - указывает на элемент после последнего заполненного)
	alloc.deallocate(a, 10);
	// ПРИМ: чтобы использовать память, возвращенную allocate в ней следует предварительно создать объект


	Text_query tq("trans.txt");
	tq.show_word_map(std::cout) << std::endl;
	tq.find_word("where2").print(std::cout) << std::endl;


	std::ifstream in_file("trans.txt");
	Text_query_heap tqh(in_file);
	auto res = tqh.query("where");
	print(std::cout, res);
}

