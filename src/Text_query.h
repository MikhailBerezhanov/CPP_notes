
#ifndef _TEXT_QUERY_H
#define _TEXT_QUERY_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <utility>	
#include <algorithm>			// for_each


// Результат выполнения запроса Text_query. Если слово найдено, содержит
// - кол-во раз, сколько встречается слово в тексте
// - номера строк в тексте с этим словом
// - содержимое строк с этим словом
class Query_result
{
public:
	Query_result() = default;
	Query_result(std::set<size_t>::size_type num, std::set<size_t>* set, std::string* vec):
	 lines_num(num), word_lines_set_ptr(set), lines_vec_ptr(vec) {}

	std::ostream& print(std::ostream& os) const;

private:
	std::set<size_t>::size_type lines_num = 0;
	std::set<size_t>* word_lines_set_ptr = nullptr;
	std::string* lines_vec_ptr = nullptr;
};


class Text_query
{
public:

	typedef std::map<std::string, std::set<size_t>> word_map_t;

	Text_query(const char* file_name);


	Query_result find_word(const std::string& word);

	// debug
	std::ostream& show_word_map(std::ostream& os);
	std::ostream& show_word_map(std::ostream& os, word_map_t::iterator& iter);

private:
	std::vector<std::string> lines_vec; // содержимое файла
	std::set<size_t> lines_set;	// содержит номера строк всего файла
	word_map_t word_map;	// содержит слово и номер строки, в которой есть это слово
};



// Версия класса, хранящая данные в динамической памяти (куче) для совместного доступа
// Так, данные (вектор и набор) становятся общими для любого объекта класса и умный указатель
// shared_ptr позволяет вспомогательному классу Query_result_heap обращаться к этим данным, 
// даже когда все объекты Text_query_heap уничтожены. (в отличие от статической версии)

class Query_result_heap
{

friend std::ostream& print(std::ostream& os, const Query_result_heap&);

public:
	using line_no = std::vector<std::string>::size_type;

	Query_result_heap(std::string s, 
					  std::shared_ptr<std::set<line_no>> ps,
					  std::shared_ptr<std::vector<std::string>> pv ):
				sought(s), lines_set(ps), lines_vec(pv) {}

	std::set<line_no>::iterator begin() const { return lines_set->begin(); }
	std::set<line_no>::iterator end() const { return lines_set->end(); }

	std::shared_ptr<std::vector<std::string>> get_file() const { return lines_vec; }

private:
	std::string sought;	// слово, представляющее запрос
	std::shared_ptr<std::set<line_no>> lines_set;
	std::shared_ptr<std::vector<std::string>> lines_vec;
};

class Text_query_heap
{
public:
	using line_no = std::vector<std::string>::size_type;

	Text_query_heap(std::ifstream&);

	Query_result_heap query(const std::string& word) const;

private:
	// указатель на вектор с содержимым файла (создается в куче) 
	std::shared_ptr<std::vector<std::string>> lines_vec; 
	// содержит слово и указатель на набор с номерами строк, в котором есть это слово
	std::map<std::string, std::shared_ptr<std::set<line_no>>> word_map; 
};







#endif // _TEXT_QUERY_H