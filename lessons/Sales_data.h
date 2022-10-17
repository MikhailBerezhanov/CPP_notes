#ifndef _SALES_DATA_H
#define _SALES_DATA_H

#include <string>
#include <iostream>

//ПРИМ: Инкапсуляция - разделение интерфейса класса от его реализации для пользователя класса

//
// this - константный указатель на текущий объект класса. *this - получение текущего объекта. 
// Неявно передается всем нестатическим методам. Имеет тип константного указателя: Sales_data* const this

// Единственное отличие в объявлении класса структурой или классом в доступе к членам:
// struct - все члены класса до первого спецификатора доступа открыты (public)
// class  - все члены класса до первого спецификатора доступа закрыты (private)
class /*struct*/ Sales_data
{
// Дружественные функции. Могут получать доступ к закрытым членам объекта класса
friend std::ostream& print(std::ostream&, const Sales_data&);
friend std::istream& read(std::istream&, Sales_data&); 

friend class std::hash<Sales_data>;

public:
	// Определение типов используемых в классе
	typedef std::string::size_type sz;

	// Конструкторы. Кконструктор по умолчанию (синтезируемый если не объявлен ни один консруктор)
	// в данном случае необходим, т.к. используются и другие конструкторы (он не будет создан неявно)
	Sales_data() = default;	// использует значения инициализации или значения по умолчанию для заполнения членов ("", 0 , 0.0)
	//Sales_data(std::string s = ""): bookNo(s) {} // тоже является стандартным конструктором, т.к. может быть вызван без агрументов
	Sales_data(const std::string& s, unsigned n, double p):	bookNo(s), units_sold(n), revenue(p*n) {} // используется список инициализации
	explicit Sales_data(const std::string& s): bookNo(s) {} // задает один член аргументом, остальные значениями инициализации (если есть)
	explicit Sales_data(std::istream& is): Sales_data() { read(is, *this); }	// делегирующий конструктор (вызывает другой конструктор перед выполнением)
	// ПРИМ: каждый конструктор, который может быть вызван с одним агрументом, определяет неявное преобразование в тип класса
	// (конструкторы преобразования). При этом допускается лишь одно преобразование
	// МОЖНО 
	// string tmp = "999";
	// item.combine(tmp);
	//
	// Но НЕЛЬЗЯ
	// item.combine("999"); - два преобразования: const char* -> std::string -> Sales_data (при этом создается временный объект string)
	// Спецификатор конструктора explicit предотвращает неявное преобразование. С явным конструктором нельзя использовать
	// форму инициализации копированием (Sales_data s = book;), только прямая инициализация ( Sales_data s(book); )

	// Открытые Функции-члены (методы). Представляют собой интерфейс класса
	// ПРИМ: определенная внутри тела класса функция неявно становится встраиваемой (inline) 
	// эквив. std::string isbn(const Sales_data* const this) { return this->bookNo; }
	std::string isbn() const { return bookNo; }	// Константный метод класса (не изменяет объект)
												// const применяется для модификации указателя this в указатель на константу для
												// возможности использования метода как с константыми объектами, так и с обычными
	Sales_data& combine(const Sales_data&); 
	double avg_price() const;

	Sales_data& operator+= (const Sales_data& rhs);

private:
	// Закрытые Переменные-члены. Представляют собой реализацию класса
	// ПРИМ: Конструкторы инициализирует переменные в том порядке, в котором они объявлены в классе, 
	// а не по порядку из списка инициализации конструктора
	std::string bookNo;			// значение по умолчанию
	unsigned units_sold = 0;	// значение инициализации
	double revenue = 0.0;		// значение инициализации
	mutable size_t access_cnt;	// может изменяться даже в константном объекте
	sz example = 0;
	static int st_member;		// статический член класса - не принадлежит ниодному объекту, не заполняется конструктором
								// ПРИМ. нельзя инициализировать в классе (только если const или constexpr)
								// ПРИМ. статические члены класса могут использоваться как аргументы по умолчанию
	static constexpr char st_ch = '1';	// Инициализация статического константного члена класса
										// ПРИМ. Но он также должен быть объявлен в cpp файле !
};

// Функции интерфейса класса, не являющиеся его членами
Sales_data add(const Sales_data&, const Sales_data&);
std::ostream& print(std::ostream&, const Sales_data&);
std::istream& read(std::istream&, Sales_data&); 
bool compare_isbn(const Sales_data& o1, const Sales_data& o2);
bool operator== (const Sales_data& lhs, const Sales_data& rhs);
Sales_data operator+ (const Sales_data& lhs, const Sales_data& rhs);

// Спецификация класса hash для использования типа  Sales_data
namespace std 	// открытие пространства имен
{ 
 	template<>
 	struct hash<Sales_data>
 	{
 		typedef size_t result_type;
 		typedef Sales_data argument_type;	// по умолчанию этому типу требуется опрератор ==  

 		size_t operator() (const Sales_data& s) const;
 	};
}				// закрытие пространства имен


// Исключения, специфичные для класса
class out_of_stock: public std::runtime_error
{
public:
	explicit out_of_stock(const std::string& s): std::runtime_error(s) {} 
};

class isbn_mismatch: public std::logic_error
{
public:
	explicit isbn_mismatch(const std::string& s): std::logic_error(s) {}

	isbn_mismatch(const std::string& s, const std::string& lhs, const std::string& rhs):
		std::logic_error(s), left(lhs), right(rhs) {}

	const std::string left, right; 
};



typedef std::string Type;
Type initVal();

class Exercise
{
	public:
		typedef double Type;
		Type setVal(Type);
		Type initVal() { val = 1; return val; }
	private:	
		int val;
};







#endif //_SALES_DATA_H