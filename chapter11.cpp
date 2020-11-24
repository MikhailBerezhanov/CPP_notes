
//--- Ассоциативные контейнеры ---

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <exception>
#include <list>
#include <algorithm>			// sort, unique, partition, for_each
#include <map>
#include <set>
#include <utility>				// pair

#include "Sales_data.h"
#include "exercises.h"

static void words_occurence ()
{
	// ПРИМ: Важно чтобы тип ключа поддерживал операцию < 
	std::map<std::string, int> words_count;	// отсортированная ( по умолчанию исп. оператор < объектов) карта с уникальными значениями 
	std::set<std::string> exclude = {"THE", "BUT", "AND", "OR", "AN"};	// ключи = значения. Только уникальные. Остортирован в алфавитном порядке

	std::string word;

	while(std::cin >> word){
		
		// перевести слово в верхний регистр 
		for(char &c : word) c = toupper(c);
		//for_each(word.begin(), word.end(), [](char &c) {c = toupper(c);});

		// подсчитать только не исключенные слова
		if(exclude.find(word) == exclude.end())
			//++words_count[word];	// Создает новую пару если такой нет, инкерементирует значение int (счетчик)
			//эквивалентно
			++words_count.insert({word, 0}).first->second;
	}
	std::cin.clear();

	// элементом карты является пара, имеющая два открытых члена: first и second (ключ и значение в данном случае)
	for(auto &c : words_count) std::cout << c.first << " occures " << c.second << " times" << std::endl;
}




static std::map<std::string, std::string> make_rules_map(std::ifstream& rules_file)
{
	std::map<std::string, std::string> rules;

	std::string key;
	std::string value;

	// прочитать первое слово в ключ, а остальную часть строки в значение
	while(rules_file >> key && getline(rules_file, value)){
		// проверить, есть ли правило для данного ключа
		if(value.size() > 1) rules[key] = value.substr(1);	// отбросить пробел
		else throw std::runtime_error("No rule for " + key);
	}

	return rules;	
}

static std::string& transform(std::string& word, std::map<std::string, std::string>& rules)
{
	auto iter = rules.find(word);

	// Заменить слово соответствующим правилом преобразования
	if(iter != rules.end()) return iter->second;

	return word;	// если для слова нет правила, вернуть его исходный вид
}

static void word_transform(std::ifstream& rules_file, std::ifstream& in_file)
{
	std::string line;
	// карта соответсвия правил замены слов
	std::map<std::string, std::string> rules = make_rules_map(rules_file);

	while(getline(in_file, line)){
		std::istringstream word_line(line);
		std::string word;
		bool first_word = true;

		while(word_line >> word){

			if(first_word) first_word = false;
			else std::cout << " ";

			std::cout << transform(word, rules);
		}
		std::cout << std::endl;
	}
}

void chapter11 (void)
{
	//words_occurence();

	// ключ - фамилия семьи, значение - веток имен детей
	std::map<std::string, std::vector<std::string>> family { {"Berezhanovs", {"Mik","Petr","Irina"}} };
	family["Trololo"] = {"Vasya", "Vova"};

	// ПРИМ: если требуется нестандартная операция сравнения для сортировке, ее можно передать при определении контейнера
	// в виде УКАЗАТЕЛЯ на функцию
	std::multiset<Sales_data, decltype(compare_isbn)* > bookstore(compare_isbn);// набор неуникальных значений , отсортированный по ISBN
	std::multimap<Sales_data, int, bool (*)(const Sales_data& o1, const Sales_data& o2)> bookstore2(compare_isbn);// аналогично, но карта

	// ПРИМ: не все итераторы поддерживают оператор < 
	std::map<std::vector<int>::iterator, int> m1;
	std::map<std::list<int>::iterator, int> m2;


	std::pair<std::string, std::string> anon;	// содержит две строки
	auto temp = std::make_pair(10, "temp");		// возвращает пару, инициализированную значениями. Тип пары выводится из типов значений
	std::cout << "temp.first: " << temp.first << " temp.second: " << temp.second << std::endl;


	// ПРИМ: для карт value_type - pair<const key_type, mapped_type>  -  такой же тип имеет и итератор по map
	std::set<int>::value_type v1;				// v1 - int
	std::set<int>::key_type v2;					// v2 - int
	std::map<std::string, int>::value_type v3;	// v3 - pair<const string, int>  - Ключ неизменяем
	std::map<std::string, int>::key_type v4;	// v4 - string
	std::map<std::string, int>::mapped_type v5;	// v5 - int		определен только для map, miltimap, unordered map, unorderred_multimap


	auto map_it = family.begin();
	// *map_it - ссылка на объект типа пары
	std::cout << map_it->first << " ";
	for_each(map_it->second.begin(), map_it->second.end(), [](const std::string& s){std::cout << s << " ";});
	std::cout << std::endl;
	//map_it->first = "new key"; // ОШИБКА: ключ - константа, аналогично итератор набор set - контантен, только чтение
	map_it->second = {"HEYAA", "qwerty"};

	std::vector<char> v;
	std::multiset<char> c;

	std::copy(v.begin(), v.end(), std::inserter(c, c.end()));
	//std::copy(v.begin(), v.end(), std::back_inserter(c));	// ОШИБКА map не имеет метода push_back()
	std::copy(c.begin(), c.end(), std::inserter(v, v.end()));
	std::copy(c.begin(), c.end(), std::back_inserter(v));

	std::multiset<char>::iterator it2 = c.begin();
	std::multiset<Sales_data>::iterator it3 = bookstore.begin();


	// ПРИМ: для контейнеров с уникальными эл-тами inrest возвращает пару с итератором и флагов был ли эл-т вставлен
	// у контейнеров, допускающих совпадение ключей, insert() возвращает тератор на новый эл-т
	auto ret = family.insert( {"Berezhanovs", {"Balodya", "Misha", "Kolya"}} );
	if(!ret.second) std::cout << ret.first->first << " already exists in map. not insertred" << std::endl;


	// Поиск по ассоциативным контейнера осуществялется встроенными методами
	family.find("Berezhanovs");			// Возвращает итератор на первый эл-т с ключем или итератор после конца если не найден
	family.count("Berezhanovs");		// Возврашает кол-во эл-тов с заданным ключем
	family.lower_bound("Berezhanovs");	// Возвращает тератор на первый эл-т, значение ключа которого не меньше заданного (только для упорядоченных)
	family.upper_bound("Berezhanovs");	// Возвращает тератор на первый эл-т, значение ключа которого больше заданного (только для упорядоченных)
	family.equal_range("Berezhanovs");	// Возврашает пару итераторов, обозначающих эл-ты с заданным ключем. Если не найдено, то end()
	// lower_bound, upper_bound, equal_range - возвращают итератор на позицию, куда эл-т мог бы быть вставлен при его отсутствии, в том числе end

	std::multimap<std::string, int> authors = { {"C", 4}, {"A", 1}, {"B", 0}, {"A", 2}, {"B", 3} };

	// ПРИМ: Равенство нижней границы верхней означает отсутствие эл-та в контейнере
	for(auto beg = authors.lower_bound("A"),
			 end = authors.upper_bound("A");
			 beg != end;
			 ++beg)	std::cout << beg->second << std::endl;

	// Эквивалентно
	for (auto pos = authors.equal_range("B"); pos.first != pos.second; ++pos.first)
		std::cout << pos.first->second << std::endl;

	auto iter = authors.find("C");
	if (iter != authors.end()) authors.erase(iter);
	std::cout << "authors: ";
	for(auto &i : authors) std::cout << i.first << ":" << i.second << " ";
	std::cout << std::endl;


	std::ifstream rules_file("rules.txt");
	std::ifstream trans_file("trans.txt");

	if(rules_file.is_open() && trans_file.is_open()) word_transform(rules_file, trans_file);
}