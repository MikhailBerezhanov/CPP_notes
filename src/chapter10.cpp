
//--- Обобщенные алгоритмы ---

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <forward_list>
#include <deque>
#include <array>
#include <algorithm>			// sort, unique, partition
#include <functional>			// bind
#include <numeric>				// fill_n
#include <iterator>				// istream iterators

#include "exercises.h"

#define CHAPTER_NAME	"[ Chapter10 ] "

// функция сравнения, используемая при сортировке слов по длине
static bool is_shorter(const std::string& s1, const std::string& s2)
{
	return s1.size() < s2.size();
}

// представление набора слов в алфавитном порядке и удаление дубликатов
static void elim_dups(std::vector<std::string>& words)
{
	// сортировка слов в алфовитном порядке позволяет найти дубликаты (по умолчанию использует оператор < )
	std::sort(words.begin(), words.end());

	// функция unique() переупорядочивает исходный диапазон так, чтобы каждое слово присутсвовало только один раз в 
	// начальной части диапазона, и возвращает итератор на эл-т, следующий после диапазона уникальных значений
	auto end_unique = std::unique(words.begin(), words.end());

	// для удаления не уникальных эл-тов используется erase()
	words.erase(end_unique, words.end());

	// вывести полученный вектор. слова должны быть упорядочены по размеру, а затем в алфавитном порядке
	// в переделах каждого размера. stable_sort обеспечивает превоначальный порядок сортировки среди равных эл-тов
	// в качестве 3его параметра (предиката) используется функция, определенная пользователем
	//std::stable_sort(words.begin(), words.end(), is_shorter);

	// эквивалентно с использованием лямбда-выражения []()->тип {}
	// ПРИМ: [] - список захвата локальных переменных функции, () - список параметров лямбда-функции, *опционально*
	// -> тип_возвращаемого значения, {} - тело лямбда-функции. Если внутри тела один return, то тип возвращаемого
	// значения опередляется неявно и его можно опустить. 
	// Если return больше одного, а тип явно не указан, то неявно возвращаться будет void.
	  
	std::stable_sort(words.begin(), words.end(), [](const std::string& s1, const std::string& s2)
													{return s1.size() < s2.size();});

	for(const auto& s : words) std::cout << s << " ";
	std::cout << std::endl;
}


static bool more_than_5_symb (std::string& s)
{
	return s.size() >= 5;
}

// Поиск и вывод строк больше или равных заданного размера sz
static bool find_more_than_sz_symb_strings (std::vector<std::string>& words, std::string::size_type sz)
{
	// получить итератор на первый эл-т, размер которого >= sz
	// ПРИМ. некоторые алогритмы принимают только унарные предикаты, поэтому в таких случаях используют
	// лямбда-выражения с списком захвата для передачи дополнительных параметров в функцию предиката
	auto predicate = [sz](std::string& s){return s.size() >= sz;};
	auto wc = std::find_if(words.begin(), words.end(), predicate);

	//auto count = words.end() - wc;
	// эквивалентно
	auto count = std::count_if(words.begin(), words.end(), predicate);

	std::cout << " found '" << count << "' words with size >= " << sz << std::endl;

	std::for_each(wc, words.end(),[](std::string& s)->void {std::cout << s << " ";});
	std::cout << std::endl;
}

static void lambda_test()
{
	int local_var = 10;

	// ПРИМ. для изменения значения лок. переменной внутри лямбда-функции необходима передача по ссылке
	// или указание mutable
	//auto lambda = [&]()->bool				//эквивалентно - неявная передача по ссылке
	auto lambda = [&local_var]()->bool 		//эквивалентно - явная передача по ссылке
	{ 	if(local_var > 0){
			local_var--;
			if(!local_var) return true;
		}
		else if(!local_var) return true;
		else return false;
	};

	// изменение лок. переменной приведет к изменению ее значения и в лямбда-функции, захватывающей ее по ссылке
	local_var = 50; 
	for(int i = 0; i < 5; ++i) lambda();

	std::cout << "local var value: " << local_var << std::endl;

	// сохраняет значение списка захвата во время объявления (сама лок. переменная в теле не изменяется)
	auto lambda2 = [local_var]() mutable {return --local_var;};	// использование ключ. слова для изменения переменной

	local_var = 0;

	std::cout << "local var value: " << local_var << " lambda2 var value: " << lambda2() << std::endl;
}

void chapter10 (void)
{
	std::vector<int> vi {1, 50, 70, 99};

	// 3ий параметр accumulate задает тип возвращаемого значения ( в данном случае int)
	std::cout << CHAPTER_NAME "accumulate int: " << std::accumulate(vi.begin(), vi.end(), 0) << std::endl;

	std::vector<double> vd {1.3, 50.4, 70.5, 99.6};
	std::cout << CHAPTER_NAME "accumulate double: " << std::accumulate(vd.begin(), vd.end(), 0) << std::endl;

	// equal использует по умолчанию оператор == для сравнения эл-тов контейнеров. Типы необязательно должны совпадать
	// если их можно преобразовать к одному. Главное чтобы была поддержка оператора == для типа
	std::cout << CHAPTER_NAME "equal vi and vd: " << std::equal(vi.begin(), vi.end(), vd.begin()) << std::endl;

	// Обнуление последовательности чисел
	std::fill_n(vi.begin(), vi.size(), 0);
	std::fill_n(vd.begin(), vd.size(), 0);
	std::cout << CHAPTER_NAME "equal vi and vd: " << std::equal(vi.begin(), vi.end(), vd.begin()) << std::endl;

	vi.clear();			// сделать вектор пустым
	std::cout << CHAPTER_NAME  "vi size: " << vi.size() << std::endl;
	std::list<int> lst{1, 2, 3, 4, 5, 6};	// пустой двусвязный список по умолчанию

	// копирование эл-тов из одного контейнера в другой. 
	// ПРИМ: важно чтобы контейнер назначения имел размер НЕ МЕНЬШЕ исходного
	std::copy(lst.cbegin(), lst.cend(), vi.begin());

	for(auto& i : vi) std::cout << i << " ";
	std::cout << std::endl;


	std::vector<std::string> vs{"aa", "bbb", "bb", "aaa", "keykey", "key", "key", "aa", "cc", "ccc", "c", "bb", "dddddd", "tttttt"};
	elim_dups(vs);
	find_more_than_sz_symb_strings(vs, 3);


	// делит контейнер так, чтобы значения, для которых предикат возвращает true, располагались в начале последовательности
	// а для false - в конце. Возвращает итератор на следующий эл-т после последнего, для которого предикат возвратил true
	// ПРИМ. предикат применяется к каждому эл-ту диапазона
	auto end_it = std::partition(vs.begin(), vs.end(), more_than_5_symb);

	auto it = vs.begin();

	while(it != vs.end()) std::cout << *it++ << " ";
	std::cout << std::endl;

	// Вывод эл-тов, у которых больше и ровно 5 символов
	//it = vs.begin();
	//while(it != end_it) std::cout << *it++ << " ";

	// Эквивалентно
	for_each(vs.begin(), end_it, [](const std::string& s){std::cout << s << " ";});
	std::cout << std::endl;


	lambda_test();


//----------- ТИПЫ ИТЕРАТОРОВ -----------

	// ### Итератор вставки. Связан с контейнером и применяется для вставки эл-тов в контейнер
	// ПРИМ: адаптер inserter - адаптер итератора
	std::list<int> lst2;
	std::back_inserter(lst2);			// создает итератор вставки в конец ( исп. push_back() )
	std::front_inserter(lst2);			// создает итератор вставки в начало ( исп. push_front() )
	std::inserter(lst2, lst2.begin());	// создает итератор вставки в любое место ( исп. insert() )

	std::vector<int> vii{1,1,1,4,4,6,7,8,8,8,9};

	// Список изначально пуст, но благодаря итератору встаки мы можем копировать в него значения
	std::unique_copy(vii.cbegin(), vii.cend(), std::front_inserter(lst2));
	std::cout << CHAPTER_NAME  "lst2 after unique_copy (front_inserter): ";
	for_each(lst2.cbegin(), lst2.cend(), [](const int& i){std::cout << i << " ";});
	std::cout << std::endl;

	std::list<int> lst3;
	std::list<int> lst4;

	std::unique_copy(vii.cbegin(), vii.cend(), std::back_inserter(lst3));
	std::cout << CHAPTER_NAME  "lst3 after unique_copy (back_inserter): ";
	for_each(lst3.cbegin(), lst3.cend(), [](const int& i){std::cout << i << " ";});
	std::cout << std::endl;

	std::unique_copy(vii.cbegin(), vii.cend(), std::inserter(lst4, lst4.begin()));
	std::cout << CHAPTER_NAME  "lst4 after unique_copy (inserter): ";
	for_each(lst4.cbegin(), lst4.cend(), [](const int& i){std::cout << i << " ";});
	std::cout << std::endl;


	// ### Потоковый итератор. Может быть связан с потоком ввода или вывода и применяется для перебора связанного потока
	
	std::istream_iterator<int> in_it(std::cin);		// читает целые числа из потока ввода
	std::istream_iterator<int> eof_it;				// по умолчанию инициализируется как итератор указывающий на конец файла

	//std::vector<int> vec;
	//while(in_it != eof_it) vec.push_back(*in_it++); // пока не пришел символ конца файла или ощибка потока
	// эквивалентно циклу
	std::vector<int> vec(in_it, eof_it);			// инициализация диапазоном итераторов

	std::ostream_iterator<int> out_vec_it(std::cout, " ");	

	std::sort(vec.begin(), vec.end());
	std::cout << CHAPTER_NAME  "vector from cin: ";
	std::unique_copy(vec.begin(), vec.end(), out_vec_it);	// пишет значения в поток вывода
	// эквивалетно
	//for_each(vec.cbegin(), vec.cend(), [](const int& i){std::cout << i << " ";});
	std::cout << std::endl;


	std::ostream_iterator<std::string> out_it(std::cout, " ");	// пишет значения в поток вывода сопровождаемые символом " "
	// пишет в поток вывода
	out_it = "hello";
	out_it = "from";
	out_it = "chapter10\n";

	// Работают ,но ничего не делают
	// *out_it, ++out_it, out_it++ 


	// ### Реверсивный итератор. Перемещается назад, а не вперед. Есть у всех библиотечных контейнеров кроме forward_list

	std::vector<int> vec2 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	std::cout << CHAPTER_NAME  "reverse vector2: ";
	// rbegin() - получение итетора, указывающего на последний эл-т,
	// rend() - получение итетора, указывающего на эл-т перед первым
	// ++r_iter - декремент реверсивного итератора
	for(auto r_iter = vec2.crbegin(); r_iter != vec2.crend(); ++r_iter) std::cout << *r_iter << " ";
	std::cout << std::endl;

	// сортировка в обратном порядке
	std::sort(vec2.rbegin(), vec2.rend());
	std::cout << CHAPTER_NAME  "reverse-sorted vector2: ";
	for_each(vec2.cbegin(), vec2.cend(), [](const int& i){std::cout << i << " ";});
	std::cout << std::endl;

	std::string line("FIRST,MIDDLE,LAST");
	// найти последний элемент в строке, разделенной запятыми
	auto rcomma = std::find(line.crbegin(), line.crend(), ',');
	// для восстановления реверсивного итератора используется ф-ия base()
	std::cout << std::string(line.crbegin(), rcomma) << std::endl;		// "TSAL"
	std::cout << std::string(rcomma.base(), line.cend()) << std::endl;	// "LAST"


	// ### Итератор перемещения. Перемещает эл-ты, а не копирует



//----------- КАТЕГОРИИ ИТЕРАТОРОВ -----------
//
//	1. Итераторы ввода - позволяют читать, но записи не гарантируют.
//						поддерживают:	==, !=, ++, (*) только справа от =, ->
//						пример: istream_iterator, find(), accumulate()
//
//	2. Итераторы вывода - позволяет записывать, но чтения не гарантирует. 
// 	ПРИМ: Значение итератору ввода можно присвоить только однажды. Подобно итераторам ввода,
//	итераторы вывода можно использовать только для однопроходных алгоритмов.
//						поддерживают:	++, (*) только слева от =
//						пример: ostream_iterator
//
//	3. Прямой итератор - позволяет читать и записывать данные в последовательность. 
//						Перемещаются только в одном направлении. 
//						поддерживают: все операции итераторов ввода и вывода. Кроме того,
//									  позволяют читать и записывать значение в тот же э-т
//									  несколько раз. Сохраненное состояние прямого итератора
//									  можно использовать и работабт в нескольких проходах										
//
//	4. Двунаправленный итератор - позволяет читать и записывать данные в последовательность
//								в обоих направлениях. Также поддерживает --
//
//	5. Итератор прямого доступа - обеспечивает доступ к любой позиции последовательности в любой момент.
//						поддерживает - операторы сравнения <, <=, >, >= (сравнение относительных позиций)
//									   операторы арифметического сложения и вычитания +, -, +=, -= (перемещение)
//									   оператор вычитания (-) (дистанция между двумя итераторами)
//									   оператор индексирования iter[] ( *(iter + n) )
//
//
// 	ПРИМ: Разные алгоритмы требуют разные категории итераторов.
//
//

}