
#include "Query.h"


Query::Query(const std::string& s): 
	q(new WordQuery(s)) 
{ 
	std::cout << "Query constructor finished for '" << s << "'" << std::endl; 
}

// 
std::ostream& operator<< (std::ostream& os, const Query& query)
{
	// Query::rep() осуществляет виртуальный вызов через свой указатель Query_base

	return os << query.rep();
}

Query operator~ (const Query& operand)
{
	// std::shared_ptr<Query_base> tmp(new NotQuery(operand));
	// return Query(tmp);
	// эквивалентно
	return std::shared_ptr<Query_base>(new NotQuery(operand)); // использовать конструктор Query(), получающий указатель
}

Query operator& (const Query& lhs, const Query& rhs)
{
	return std::shared_ptr<Query_base>(new AndQuery(lhs, rhs));
}

Query operator| (const Query& lhs, const Query& rhs)
{
	return std::shared_ptr<Query_base>(new OrQuery(lhs, rhs));
}

// возвращает объединение результатов своих операндов
Query_result_heap OrQuery::eval(const Text_query_heap& t) const
{
	std::cout << "OrQuery::eval" << std::endl;

	// виртуальные вызовы через члены Query, lhs и rhs вызовы возвращают Query_result_heap для каждого операнда
	auto left = lhs.eval(t), right = rhs.eval(t);

	// копировать номера строк левого операнда в результирующий набор
	auto ret_lines = std::make_shared<std::set<line_no>>(left.begin(), left.end());

	// вставить строки из правого операнда
	ret_lines->insert(right.begin(), right.end());

	return Query_result_heap(rep(), ret_lines, left.get_file());
}

// возвращает пересечение результатов своих операндов
Query_result_heap AndQuery::eval(const Text_query_heap& t) const
{
	std::cout << "AndQuery::eval" << std::endl;

	auto left = lhs.eval(t), right = rhs.eval(t);
	auto ret_lines = std::make_shared<std::set<line_no>>();

	// алгоритм выводит эл-ты , присутствующие в обеих исходных последовательностях в результирующую
	std::set_intersection(	left.begin(), left.end(),
							right.begin(), right.end(),
							std::inserter(*ret_lines, ret_lines->begin()));

	return Query_result_heap(rep(), ret_lines, left.get_file());
}

// возвращает номера строк, не входящий в набор операнда
Query_result_heap NotQuery::eval(const Text_query_heap& t) const
{
	std::cout << "NotQuery::eval" << std::endl;

	auto result = query.eval(t);
	auto ret_lines = std::make_shared<std::set<line_no>>();

	auto beg = result.begin(), end = result.end();
	auto size = result.get_file()->size();
	// для каждой строки во входном файле поиск отсутствующих в result
	for(size_t n = 0; n != size; ++n){

		// сеил набор result уже закончился или в нем нет текущего номера строки из исходного файла
		if(beg == end || *beg != n) ret_lines->insert(n);
		else if(beg != end) ++beg; // получить следующий номер строки в result если она есть
	}

	return Query_result_heap("", ret_lines, result.get_file());
}