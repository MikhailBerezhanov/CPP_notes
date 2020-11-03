#ifndef _SALES_DATA_H
#define _SALES_DATA_H

#include <string>
#include <iostream>

//ПРИМ: Инкапсуляция - разделение интерфейса класса от его реализации для пользователя класса

//
// this - константный указатель на текущий объект класса. *this - получение текущего объекта. Неявно передается всем методам
// Sales_data* const this

// Единственное отличие в объявлении класса структурой или классом в доступе к членам:
// struct - все члены класса до первого спецификатора доступа открыты (public)
// class  - все члены класса до первого спецификатора доступа закрыты (private)
class /*struct*/ Sales_data
{
// Дружественные функции. Могут получать доступ к закрытым членам объекта класса
friend std::ostream& print(std::ostream&, const Sales_data&);
friend std::istream& read(std::istream&, Sales_data&); 

public:
	// Конструкторы. Кконструктор по умолчанию (синтезируемый если не объявлен ни один консруктор)
	// в данном случае необходим, т.к. используются и другие конструкторы (он не будет создан неявно)
	Sales_data() = default;	// использует значения инициализации или значения по умолчанию для заполнения членов ("", 0 , 0.0)
	Sales_data(const std::string& s, unsigned n, double p):	bookNo(s), units_sold(n), revenue(p*n) {}
	Sales_data(const std::string& s): bookNo(s) {} // задает один член аргументом, остальные значениями инициализации (если есть)
	Sales_data(std::istream&);	// будет определен позже

	// Открытые Функции-члены (методы). Представляют собой интерфейс класса
	// ПРИМ: определенная внутри тела класса функция неявно становится встраиваемой (inline) 
	// эквив. std::string isbn(const Sales_data* const this) { return this->bookNo; }
	std::string isbn() const { return bookNo; }	// Константный метод класса (не изменяет объект)
												// const применяется для модификации указателя this в указатель на константу для
												// возможности использования метода как с константыми объектами, так и с обычными
	Sales_data& combine(const Sales_data&);
	double avg_price() const;
private:
	// Закрытые Переменные-члены. Представляют собой реализацию класса
	std::string bookNo;			// значение по умолчанию
	unsigned units_sold = 0;	// значение инициализации
	double revenue = 0.0;		// значение инициализации
	mutable size_t access_cnt;	// может изменяться даже в константном объекте
};


// Функции интерфейса класса, не являющиеся его членами
Sales_data add(const Sales_data&, const Sales_data&);
std::ostream& print(std::ostream&, const Sales_data&);
std::istream& read(std::istream&, Sales_data&); 











#endif //_SALES_DATA_H