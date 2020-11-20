#include "Sales_data.h"


int Sales_data::st_member = 1;	// Инициализация статического члена класса

Sales_data& Sales_data::combine(const Sales_data& rhs)
{
	// this->bookNo - тоже самое что bookNo
	if(bookNo == rhs.bookNo){	
		/*this->units_sold*/units_sold += rhs.units_sold;
		/*this->revenue*/revenue += rhs.revenue;
	}

	++rhs.access_cnt; // Несмотря на то что параметр - ссылка на константу, mutable член можно изменить

	return *this;	// текущий объект возвращается как ссылка т.е. lvalue чтобы можно было 
					// комбинировать методы:   obj.combine().avg_price(). Иначе бы возвращался 
					// бы не сам объект, а его копия!
}



double Sales_data::avg_price() const
{
	if(units_sold) return (revenue / units_sold);

	++access_cnt; // Несмотря на то что функция константная, mutable член можно изменить

	return 0;
}


Sales_data add(const Sales_data& lhs, const Sales_data& rhs)
{
	Sales_data sum = lhs;	// Копирование объектов. Вызывается копирование по-умолчанию, т.к. 
							// оно не переопределено в классе. Копируются переменные-члены класса
	sum.combine(rhs);

	return sum;				// Возвращается копия локального объекта
}

// ПРИМ: Классы ввода и вывода не допускают копирования. Передача ТОЛЬКО по ссылке. Кроме того
// чтение и запись в поток изменяют его, поэтому обе функции получают обычные ссылки, а не на константы
std::ostream& print(std::ostream& os, const Sales_data& rhs)
{
	// Т.к. функция объявлена внутри класса как дружественная, она имеет доступ 
	// к закрытым (private) членам объекта
	os << rhs.isbn() << " " << rhs.units_sold << " " << 
	rhs.revenue << " " << rhs.avg_price();

	return os;
}

std::istream& read(std::istream& is, Sales_data& item)
{
	double price = 0;
	is >> item.bookNo >> item.units_sold >> price;
	item.revenue = price * item.units_sold;

	return is;
}

bool compare_isbn(const Sales_data& o1, const Sales_data& o2) 
{ 
	return o1.isbn() < o2.isbn(); 
}



// Exercise
// Класс сначала ищет определения типов внутри своего тела, затем в глобальной области, однако тип 
// возвращаемого значения указывается раньше чем имя и принадлежность функции. Поэтому первый Type -
// из глобальной области (string), а второй и все остальные переменные из области видимости класса.
// Для использования типов, поределенный внутри класса или самого класса в качестве возвращаемого значения
// при определении функции вне класса следует использовать Exercise::

/* Type */Exercise::Type Exercise::setVal(Type parm)
{
	val = parm + initVal();
	return val;	// cast int to string
}