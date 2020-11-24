
#include "Text_query.h"


Text_query::Text_query(const char* file_name)
{
	std::ifstream in_file(file_name);

	size_t curr_line_num = 1;

	if(in_file.is_open()){
		std::string line;

		while(getline(in_file, line)) {

			std::istringstream ss(line);

			// Заполнить набор номеров строки
			lines_set.insert(curr_line_num);

			// Заполнить карту слов
			std::string word;
			while(ss >> word) word_map[word].insert(curr_line_num);

			lines_vec.push_back(line);
			++curr_line_num;
		}
	}
}


std::ostream& Text_query::show_word_map(std::ostream& os)
{

	for(auto iter = word_map.begin(); iter != word_map.end(); ++iter){
		os << "'"<< iter->first << "' : ";
		for_each(iter->second.begin(), iter->second.end(), [&os] (size_t val) { os << val << ", ";} );
		os << std::endl;
	}


	return os;
}

std::ostream& Text_query::show_word_map(std::ostream& os, word_map_t::iterator& iter)
{

	os << "'"<< iter->first << "' : ";
	for_each(iter->second.begin(), iter->second.end(), [&os] (size_t val) { os << val << ", ";} );
	os << std::endl;


	return os;
}

Query_result Text_query::find_word(const std::string& word)
{
	auto iter = word_map.find(word);

	if(iter == word_map.end()) {
		Query_result res;
		return res;
	}

	//this->show_word_map(std::cout, iter); 

	Query_result res(iter->second.size(), &iter->second, &lines_vec[0]);
	return res;
}

std::ostream& Query_result::print(std::ostream& os) const
{
	if(!lines_num && !word_lines_set_ptr) {
		os << "word not found";
	}
	else{
		os << "Occures " << lines_num << " times, in lines:" << std::endl;

		for(auto iter = word_lines_set_ptr->begin(); iter != word_lines_set_ptr->end(); ++iter){
			os << "[" << *iter << "] " << lines_vec_ptr[*iter-1] << std::endl;
		}
	}

	return os;
}



// ---------------------------------------------------------------------------------------------

Text_query_heap::Text_query_heap(std::ifstream& in_file): lines_vec(new std::vector<std::string>)
{
	std::string line;

	while(getline(in_file, line)) {

		lines_vec->push_back(line);

		line_no n = lines_vec->size() - 1;	// номер текущей строки

		// Заполнить карту слов
		std::istringstream ss(line);
		std::string word;
		while(ss >> word){
			// line_set - ссылка на shared_ptr<set>, НЕ КОПИЯ, чтобы можно было изменять сам указатель
			auto& line_set = word_map[word]; 

			// еще не выделена память для набора line_set = nullptr при создании
			if(!line_set) 
				line_set.reset(new std::set<line_no>);

			line_set->insert(n);	// вставить номер строки, в котором встречается слово
		}

	}
}

Query_result_heap Text_query_heap::query(const std::string& word) const
{
	// пустой набор 
	static std::shared_ptr<std::set<line_no>> nodata(new std::set<line_no>);

	auto ret = word_map.find(word);
	if(ret == word_map.end()) return Query_result_heap(word, nodata, lines_vec);
	else return Query_result_heap(word, ret->second, lines_vec);
}

static std::string make_plural(size_t ctr, const std::string& word, const std::string& ending = "s")
{
    return (ctr > 1) ? word + ending : word;
}

std::ostream& print(std::ostream& os, const Query_result_heap& qr)
{
	os << qr.sought << " occures " << qr.lines_set->size() << " "
	   << make_plural(qr.lines_set->size(), "time", "s") << std::endl;

	// вывести кажду строку, в которой присутсвует слово. num - номер строки из набора, начинается с 0
	for(auto num : *qr.lines_set){
		os << "\tline " << num + 1 << ": " << *(qr.lines_vec->begin() + num) << std::endl;
	}

	return os;
}