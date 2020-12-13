#ifndef _QUERY_H
#define _QUERY_H

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

#include "Text_query.h"

// Иерархия классов для реализации запроса поиска слов в файле
//
// Query - интерфейс, объекты содержат указатель на конкретный тип запроса класса Query_base
// Query_base - вбстракный класс, представляющий разные запросы 
// WordQuery - производный от Query_base, представляет конкретный тип запроса (поиск слова), создает объект класса Query
// NotQuery - производный от Query_base, представялет запрос вида ~операнд, создает объект класса Query
// BinaryQuery - производный от Query_base, абстрактный класс лоигческих запросов
// AndQuery - производный от BinaryQuery, представляет запрос вида операнд1 & операнд2, создает объект класса Query
// orQuery - производный от BinaryQuery, представляет запрос вида операнд1 | операнд2, создает объект класса Query

// Интерфейс:
// 		Query q = Query("fiery") & Query("bird") | Query("wind");
//	1. оператор & создает объект класса Query, связанный с новым объектом класса AndQuery 
//	2. оператор | создает объект класса Query, связанный с новым объектом класса OrQuery
//	3. оператор ~ создает объект класса Query, связанный с новым объектом класса NotQuery
//	4. конструктор класса Query, получающий строку создает новый объект класса WordQuery
//
// Вызов ф-ии eval() объекта q (корневого класса) приводит к выхову ф-ии eval() объекта класса OrQuery, на который
// указывает q. Обработка того объекта класса OrQuery приведет к вызову ф-ии eval() для двух его операндов, что в
// свою очередь , приведет к вызову ф-ии eval() для объектов классов AndQuery и WordQuery ("wind"). Обработка класса
// AndQuery приведет к вызову ф-ий для объектов класса WordQuery ("fiery" и "bird")
//

class Query;
class WordQuery;

// абстрактный класс, определяющий ф-ии запросов (нельзя создать его объект)
class Query_base
{
	friend class Query;
protected:
	using line_no = Text_query_heap::line_no;
	virtual ~Query_base() = default;
private:
	// возвраает соответствующий запросу результат (строки, кол-во вхождение, текст строк с встречаючимся запросом)
	virtual Query_result_heap eval(const Text_query_heap&) const = 0;
	// возвращает строковое представление запроса
	virtual std::string rep() const = 0;
};

// представляет интерфейс к иерархии наследования Query_base и скрывает ее 
class Query
{
	// эти операторы должны обращаться к указателю shared_ptr
	friend Query operator~ (const Query&);
	friend Query operator| (const Query&, const Query&);
	friend Query operator& (const Query&, const Query&);
public:
	Query(const std::string&); // создает новый WordQuery

	// ф-ии итерфейса: вызывают соответсвующую ф-ию производного объекта от класса Query_base
	Query_result_heap eval(const Text_query_heap& t) const { return q->eval(t); }
	std::string rep() const { return q->rep(); } 
private:
	// Закрытый конструктор
	Query(std::shared_ptr<Query_base> query): q(query) { std::cout << "hidden Query constructor called" << std::endl; }
	// умный указатель на объект типа запроса
	std::shared_ptr<Query_base> q;
};

std::ostream& operator<< (std::ostream& os, const Query& query);

// задача этого производного класса - хранение искомого слова
class WordQuery : public Query_base
{
	friend class Query; // дружба не наследуется (нужна для вызова конструктора WordQuery)
	// все члены закрытые
public:
	WordQuery(const std::string& s): query_word(s) { std::cout << "WordQuery constructor called for "<< query_word << std::endl; }

	Query_result_heap eval(const Text_query_heap& t) const override { return t.query(query_word); }

	std::string rep() const override 
	{ 
		std::cout << "WordQuery::rep() called for '" << query_word << "'" << std::endl;
		return query_word; 
	}

	std::string query_word; // искомое слово
};

// Операнды в каждом из этих классов могут быть объекты любого из реальных классов, производных от класса
// Query_base: NotQuery может быть применен к WordQuery, как и AndQuery, OrQuery или NotQuery. Для обеспечения
// такой габкости операнды следует хранить как указатели на класс Query_base. Таким образом, можно привязать
// указатель на любой необходимый реальный класс. Однако вместо того, чтобы хранить указатель на класс Query_base
// классы будут сами использовать объект Query. ПОдобно тому, как пользовательский код упрощается при использовании
// класса интерфейса, можно упростить код собственного класса.

class NotQuery : public Query_base
{
	friend Query operator~ (const Query&);
	// все члены закрытые
	NotQuery(const Query& q): query(q) { std::cout << "NotQuery constructor called" << std::endl; }

	Query_result_heap eval(const Text_query_heap& t) const override ;
	std::string rep() const override 
	{ 
		std::cout << "NotQuery::rep() called" << std::endl;
		return "~(" + query.rep() + ")"; 
	}

	Query query; // хранимый запрос (операнд ~)
};

// абстрактный класс лог.операций над словами запроса
class BinaryQuery : public Query_base
{
protected:
	BinaryQuery(const Query& l, const Query& r, const std::string& s):
		lhs(l), rhs(r), opSym(s) { std::cout << "BinaryQuery constructor called" << std::endl; }

	// абстракный класс. eval() не определяет
	std::string rep() const override 
	{	
		std::cout << "BinaryQuery::rep() called" << std::endl; 
		return "(" + lhs.rep() + " " + opSym + " " + rhs.rep() + ")"; 
	}

	Query lhs, rhs; // левый и правый операнды (указатели)
	std::string opSym;
};


class AndQuery : public BinaryQuery
{
	friend Query operator& (const Query&, const Query&); // для доступа к закрытому конструктору
	// все члены закрытые
	AndQuery(const Query& l, const Query& r): BinaryQuery(l, r, "&") { std::cout << "AndQuery constructor called" << std::endl; }

	Query_result_heap eval(const Text_query_heap& t) const override ;
	// rep() наследуется от прямого базового класса BinaryQuery
};

class OrQuery : public BinaryQuery
{
	friend Query operator| (const Query&, const Query&);  // для доступа к закрытому конструктору
	// все члены закрытые
	OrQuery(const Query& l, const Query& r): BinaryQuery(l, r, "|") { std::cout << "OrQuery constructor called" << std::endl; }

	Query_result_heap eval(const Text_query_heap& t) const override ;
	// rep() наследуется от прямого базового класса BinaryQuery
};

#endif
