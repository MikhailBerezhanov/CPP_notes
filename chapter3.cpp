
//--- Типы string, vector и массивы ---

#include <iostream>
#include <string>
#include <cctype>
#include <vector>

#include "exercises.h"

static void binary_search(std::vector<int> &vec, int sought)
{
	auto left = vec.begin();
	auto right = vec.end() - 1;
	auto mid = left + (right - left) / 2;

	bool found = false;

	while(mid != right)
	{
		if(*mid == sought){
			found = true;
			break;
		} 

		if(sought < *mid) right = mid;
		else left = mid + 1;

		mid = left + (right - left) / 2;
	}

	std::cout << "Found: " << found << std::endl;
}

void chapter3 (void)
{
	std::string str;
	// Блокирует поток до введения строки или конца файла
	if(std::cin >> str)
	{
		decltype(str.size()) size = 0;	// имеет тип  std::string::size_type
		size = str.size();

		for(auto c : str) {
			std::cout << ' ' << c << std::endl; 
			c = 'a';	// изменяет значение управляющей переменной, но не строки. для изменения
						// элемента строки нужно использовать ссылку &c в for
		}

		for(char &c : str) c = toupper(c);

		std::cout << "string '" << str << "' size:" << size << std::endl;
	} 

	const std::string s;
	for(auto &c : s){ //c = 0;	ОШИБКА: нельзя изменять строку (auto - const char)
	}


	using std::vector;
	vector<std::string> svec(10, "hello");	// содержит 10 элементов "hello"
	vector<int> ivec{1, 2, 3};				// содержит 3 элемента
	vector<std::string> svec2{10};			// содержит 10 элементов пустых строк, т.к. int не преобразуется в string
	svec2.push_back("ika");
	vector<int>::size_type svec2_size = svec2.size();
	std::cout << "svec2_size : " << svec2_size << std::endl;

	// Итераторы (end - указывает на эл-нт после конца контейнера (несуществующий))
	auto b = svec2.begin(), e = svec2.end();
	if(b != e) {
		*b = 'e';	// ПРИМ. *b возвращает ссылку на эл-т, обозначенный итератором
		std::cout << "svec2 begin iter: '" << *b << "'" << std::endl;
	}
	// ПРИМ. большинство итераторов не имеют оператора < поэтому используется == или != для сравнения
	for(auto iter = svec.begin(); iter != svec.end(); iter++){
		std::cout << *iter << std::endl;
		iter->empty();
		*iter++;
		//(*iter)++;		ОШИБКА
		//++*iter;			ОШИБКА
		//*iter.empty(); 	ОШИБКА
		//iter++->size(); 	Допустимо, но итератор будет указывать за пределы контейнера в данном случае
	}

	vector<int> __vec{1, 2, 3, 4, 5};
	binary_search(__vec, 1);
}

