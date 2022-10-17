
//--- Последовательные контейнеры ---

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <forward_list>
#include <deque>
#include <array>

#include "Sales_data.h"
#include "exercises.h"

static void read_str_to_deque()
{
	using std::string; using std::deque; using std::cin; using std::cout; using std::endl; using std::list;

	string word;
#ifdef DEQUE
	cout << "[ " << __func__ << "() ] --- Using deque as container" << endl;
	deque<string> c;
#else
	cout << "[ " << __func__ << "() ] --- Using list as container" << endl;
	list<string> c;
#endif

	auto iter = c.begin();
	// ПРИМ insert, emplace выполняют операции с эт-ом ПЕРЕД, обозначенным итератором
	// ПРИМ при изменении контейнера (добавление \ удаление эл-тов) его итераторы становятся недействительными
	// (инвалидация итераторов) поэтому в теле цикла необходимо обновлять интератор, а не кэшировать их заранее
	// данный цикл аналогичен вызовам push_front()
	while(cin >> word) iter = c.insert(iter, word);	// insert возвращает итератор на добавленный эл-т

	cin.clear();
	cout << "-------------------------------------------" << endl;
	cout << "container size: " << c.size() << endl;


	auto tmp = c.end();
	auto t = c.erase(tmp, tmp);	// удалить диапазон между двумя итераторами (в данном случае ничего не удаляется)
						
	for(auto &it : c) cout << it << endl;
}

// параметр - ссылка на массив из 12 эл-тов ттипа int
static void arr_remove_elems(int(&arr)[12])
{
	std::vector<int> vi(std::begin(arr), std::end(arr));
	std::list<int> li(std::begin(arr), std::end(arr));

	auto v_it = vi.begin();
	auto l_it = li.begin();

	// удалить из вектора четные эл-ты
	while(v_it != vi.end()){
		if(*v_it % 2) ++v_it;		// переход на следующий эл-т
		else v_it = vi.erase(v_it);	// возвращает итератор на эл-т сразу после последнего удаленного
	}
	std::cout << "Vector without even elements: " << std::endl;
	for(auto &it : vi) std::cout << it << std::endl;

	// удалить из списка нечетные эл-ты
	while(l_it != li.end()){
		if(*l_it % 2) l_it = li.erase(l_it);
		else ++l_it;
	}
	std::cout << "List without odd elements: " << std::endl;
	for(auto &it : li) std::cout << it << std::endl;
}

void chapter9 (void)
{
	std::list<std::list<int>> example;

	std::list<int> l;
	l.emplace(l.begin(), 12);
	std::cout << "list size: " << l.size() << std::endl;

	std::array<char, 10> carr = {'a', 0x31, 'b', 0x43};
	std::array<char, 10>::size_type idx = 0;
	carr[idx] = 'v';

	std::vector<int> vec1;			// Инициализация стандартным конструкторм (пустой вектор)
	std::vector<int> vec2(10);		// Создание 10 элементов, инициализация по умолчанию (0)
	std::vector<int> vec3(10, 43);	// Создание 10 эл-в и их инициализация значением 43
	std::vector<int> vec4(vec3);	// Конструктор копирования (типы должны совпадать)
	std::vector<int> vec5 = {1, 2, 3, 4, 5};	// Инициализация списком инициализации
	std::vector<int> vec6(vec5.begin(), vec5.end());	// Конструктор получающий два интератора для копирования (диапазон)


	
	std::list<int> la{1, 2, 3, 4, 5};
	std::vector<double> va(la.begin(), la.end());

	// ОШИБКА сравнения котнейнеров разных типов
	//if(la == va) std::cout << "Container copy succeed" << std::endl;
	auto it_vec = va.begin();
	for(auto it_list = la.begin(); it_list != la.end() && it_vec != va.end(); ++it_list, ++it_vec)
	{
		if(*it_list != *it_vec) std::cout << "Container copy failed" << std::endl;
	}

	std::vector<char> vt;
	//vt[0] = 'a';		ОШИБКА. Вектор пуст, нет элементов
	try{
		vt.at(0) = 'a';	// возбудит исключение, но не крах программы
	}
	catch(std::out_of_range err){
		std::cout << "[ TEST ] Exception occured: " << err.what() << std::endl;
	}

	if(!vt.empty()){
		vt.front() = 'a';	// требуют проверки наличия эл-тов в контейнере
		*vt.begin() = 'a';
	}
	

	vt.insert(vt.begin(), 10, 'a');	// вставить 10 элементов 'a' перед итератором begin()
	std::vector<Sales_data> vsales;
	vsales.emplace(vsales.begin(), "666");	// вызывает конструктор класса объекта, создает объект в контейнере
	vsales.emplace_back();	// используется конструктор Sales_data по умолчанию
	vsales.emplace(vsales.end(), "999", 25, 15.99);	// аргументы должны соответствовать конструктору типа эл-та


	//read_str_to_deque();


	int ai[] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89};
	arr_remove_elems(ai);

	// ПРИМ: вектор резервирует память для добавления новых элементов. Если зарезервированное место заканчивается
	// происходит новое резервирование и перенос вектора. 
	std::cout << "vec size: " << vt.size() << ", vec capacity: " << vt.capacity() << std::endl;
	vt.reserve(50);	// резервирование места по крайней мере для 50 эл-тов
	vt.resize(20);	// изменение размера контейнера. новые эл-ты инициализируется по умолчанию
	std::cout << "after resize: "<< "vec size: " << vt.size() << ", vec capacity: " << vt.capacity() << std::endl;
	vt.capacity();	// емкость вектора ( кол-во эл-тов, кторое может иметь контейнер прежде чем произойдет новое резервирование)
	vt.shrink_to_fit();	// Запрос на уменьшение емкости в соотвествии с фактическим размером
	std::cout << "after shrink_to_fit: " << vt.size() << ", vec capacity: " << vt.capacity() << std::endl;
	vt.resize(82);
	std::cout << "after resize: "<< "vec size: " << vt.size() << ", vec capacity: " << vt.capacity() << std::endl;
}