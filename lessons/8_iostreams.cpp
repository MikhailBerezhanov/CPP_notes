
//--- Библиотека ввода и вывода ---

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#include "exercises.h"

static std::istream& my_print (std::istream& is)
{
	while(!is.eof())
	{
		std::string buf;
		is >> buf;
		std::cout << buf << std::endl;
	}
	 
	std::cout << "end of file" << std::endl;
	is.clear();

	return is;
}

static int my_fread(const std::string& fname)
{
	std::string str;
	std::vector<std::string> vec;

	std::ifstream file(fname);	// Открыть файл в режиме in

	if(!file.is_open()) std::cerr << "file '" << fname << "' open failed" << std::endl;

	while(getline(file, str)) vec.push_back(str);	//Помещает каждую строку в вектор
	//while(file >> str) vec.push_back(str);	//Помещает каждое слово в вектор

	for(auto &it : vec) std::cout << it << std::endl;

	file.close(); // Закрывается неявно когда удаляется объект fstream
	return vec.size();
}

void chapter8 (void)
{
	int ival;
	using std::cin;
	using std::cout;
	using std::endl;

	//my_print(cin);

	//while(cin >> ival);

	cout << "fail: " << cin.fail() << " badbit: " << cin.bad() <<
	" eof:" << cin.eof() << endl;

	//my_fread("makefile");

	// ПРИМ, по умолчанию файл открывается в режиме out | trunc (усекается)
	// Открыть файл в режиме дозаписи (не стирая содержимое).
	std::ofstream out("file", std::ofstream::app); // out установлен неявно

	std::string my_str("first_word 2nd_word 3d_word 4th_word");
	std::string word;
	std::istringstream istr(my_str);// консруктор - поток содержит копию строки
	istr.str(my_str);				// копировать строку в поток 
	istr.str();						// вернуть копию строки, которую хранит поток
	// ПРИМ. можно читать из потоко строк istr по одному слову за раз
	istr >> word;;
	cout << word << endl;
	//my_print(istr);

	std::string my_str2;
	std::istringstream istr2(my_str2);
	my_str2 = my_str;
	//istr2.str(my_str2);
	cout << "istr2 contains: " << istr2.str() << endl;
}