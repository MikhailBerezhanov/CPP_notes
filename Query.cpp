
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

Query_result_heap NotQuery::eval(const Text_query_heap& t) const
{
	return Query_result_heap("", nullptr, nullptr);
}

Query_result_heap AndQuery::eval(const Text_query_heap& t) const
{
	return Query_result_heap("", nullptr, nullptr);
}

Query_result_heap OrQuery::eval(const Text_query_heap& t) const
{
	return Query_result_heap("", nullptr, nullptr);
}