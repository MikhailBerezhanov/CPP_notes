
//--- ООП ---

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <exception>
#include <list>
#include <algorithm>			// sort, unique, partition
#include <map>
#include <set>
#include <utility>				// pair, move
#include <memory>				// shared_ptr, unique_ptr, weak_ptr, allocator
#include <new>					// bad_alloc, nothrow
#include <functional>			// библиотечные объекты-функции

#include "Text_query.h"
#include "exercises.h"

// объявление класса
class Pal;
class Disc_quote; // без спецификаторов и списока наследования
class Bulk_quote;

// Базовый класс
// 1. Если стандартный конструктор, конструктор копий, оператора присвоения копии или деструктор в базовом классе удалены
//    (= delete) или недоступны, то соответсвующая ф-ия-член в производном классе определяется как удаленная, поскольку 
//    компилятор не может использовать ф-ию-член базового класса для создания, присвоения или удаления части объекта  
//	  базового класса.
// 2. Если у базового класса недоступен или удален деструктор, то синтезируемые стандартный конструктор и конструктор копий 
//	  в производных классах определяются удаленными, посклькоу нет никакого способа удалить базовую часть производного объекта 
// 3. Компилятор не будет синтезировать удаленную ф-ию перемещения. Если использовать синтаксис = default для создания ф-ии
//	  перемещения, то это будет удаленная ф-ия в производном классе, если соответсвующая ф-ия в базовом классе будет удалена
//	  или недоступна, поскольку часть базового класса не может быть перемещена. Конструктор перемещения также будет удален, 
// 	  если деструктор базового класса окажется удален или недоступен.
//
class Quote
{
// дружественный член - ф-ия получает доступ ко всем членам класса
// ПРИМ: при наследовании дружеские отношения не сохраняются
friend void init_i(Quote& q);
friend class Pal;	// этот класс имеет доступ к членам базового класса, но не производных классов
// Открытые члены доступы и пользователю класса и вспомогательным классам
public:
	Quote() = default;
	Quote(const std::string& book, double sales_price):
		bookNo(book), price(sales_price) { init_i(*this); }

	// Базовые классы обычно должны определять виртуальный деструктор даже если они ничего не делают.
	// Выполнение оператора deleteдля указателя на базовый класс, который указывает на объект производного
	// класса, приведет к непредсказуемым последствиям, если деструктор базового класса не будет виртуальным.
	virtual ~Quote() = default;
	// ПРИМ: виртуальный деструктор отменяет синтез ф-ий перемещения (и для производный в том числе) поэтому
	// если нужны ф-ии копирования, стоит объявлять их явно, даже если они используют версию по умолчанию

	Quote(const Quote&) = default;
	Quote(Quote&&) = default;
	Quote& operator= (const Quote&) = default;
	Quote& operator= (Quote&&) = default;
	// Теперрь объекты будут почленно копироваться, присваиваться, перемещаться и удаляться
	// Кроме того, классы, производные

	std::string isbn() const { return bookNo; }

	// виртуальная ф-ия выбирается во время выполнения , а не компиляции в зависимоти от динамического типа при вызове
	// ф-ии по указателю или ссылке на объект. При наследовании может быть переопределена производным классом
	// ПРИМ: для виртуальных ф-ий аргументы по умолчанию определяются в базовом классе
	virtual double net_price(std::size_t n = 0) const { return n*price; }

// Защищенный доступ - закрыто от пользователей класса, но открыто для производный классов при наследовании
protected:
	double price = 0.0;
	static int i;	// статический член определен и существует в единственном экзмеплятре для всех проивзодных классов

// Закрытые члены не доступны никому
private:
	std::string bookNo;
};

int Quote::i = 1; 	// инициализация статического члена базового класса (без нее i в базовом классе не существует)

void init_i(Quote& q)
{ 
	// есть доступ даже к private секции
	std::cout << q.bookNo << std::endl;
	q.i = 43; 
}

// Абстрактный класс (содержащий или унаследовавший без переопределения чистую виртуальную ф-ию).
// Чисто виртуальная ф-ия - ф-ия, которая не имеет определения. НЕЛЬЗЯ создать объекты абстрактного класса
// спецификатор доступа в списке наследования определяет как члены базового класса будут включены в проивзодный класс
//(открытыми , закрытыми или защищенными ). Доступ к конкретному члену базового класса определяет САМ базовый класс
class Disc_quote : public Quote
{
public:
	// Стандартные конструкторы необходимы во всей иерархии наследования если их планируется использоваться в производных объектах
	Disc_quote() = default;
	// Только САМ базовый класс может инициализировать свои члены, поэтому сперва вызывается конструктор базового класса
	Disc_quote(const std::string& book, double sales_price, std::size_t qty, double disc):
		Quote(book, sales_price), quantity(qty), discount(disc) { }

	// Когда производный класс определяет ф-ию копирования или перемещения, эта ф-ия несет ответственность за копирование
	//или перемещение всего объекта, включая члены базового класса
	Disc_quote(const Disc_quote& d): Quote(d), quantity(d.quantity), discount(d.discount) { }
	Disc_quote(Disc_quote&& dr): Quote(std::move(dr)), quantity(std::move(dr.quantity)) , discount(std::move(dr.discount)) { }
	Disc_quote& operator= (const Disc_quote& rhs){ Quote::operator=(rhs); quantity = rhs.quantity; discount = rhs.discount; }
	Disc_quote& operator= (Disc_quote&& rhs){ Quote::operator=(std::move(rhs)); quantity = std::move(rhs.quantity); discount = std::move(rhs.discount); }

	// унаследованная виртуальная -фия неявно является вирутальной. Спецификатор override показывает, что ф-ия с такой 
	// сигнатурой должна быть в базовом классе и здесь она переопределяется. Сигнатуры должны точно совпадать за исключением 
	// возвращаемого значения т.к. если в базовом классе возвращалась ссылка на его класс, здесь может быть возвращаена ссылка
	// на проивзодный класс. =0 - показывает что у ф-ии нет определения (тела) и она таким образом является чисто виртуальной.
	double net_price(std::size_t) const override = 0;

protected:
	std::size_t quantity = 0;
	double discount = 0.0;
};

// Производный класс
// Disc_quote - прямой базовый класс, Quote - косвенный базовый класс
// спецификатор final предотвращает дальнейшее наследование (класс Bulk_quote не может быть базовым)
// ПРИМ: Область видимости производного класса вложена в область видимости его базовых классов. При поиске
// необходимого члена, поиск начинается изнутрути: сначала в производном класса, затем выше по иерархии наследования
class Bulk_quote final : public Disc_quote
{
public:
#if 0
	Bulk_quote() = default;
	Bulk_quote(const std::string& book, double sales_price, std::size_t qty, double disc):
		Disc_quote(book, sales_price, qty, disc) {}
#else
	using Disc_quote::Disc_quote;	// наследует конструкторы базового класса
	// ПРИМ: обычно using делает имя видимым в текущей области видимости, однако преминительно к конструктору using
	// приводит к созданию компиляторо кода - для каждого конструктора из базового класса компилятор создает конструктор
	// в проивзодной классе с таким же списком параметров.
	// 		производный(параметры) : базовый(аргументы) {}
	// если у производного класса есть какие-нибудь собственные члены, они инициализируются по умолчанию.
#endif
	// переопределение базовой версии для реализации политики скидок
	double net_price(std::size_t) const override;

	// не переопределение виртуальной ф-ии, а объявление новой!
	double net_price() { return price; }	// скрывает net_price из базового класса

	// ПРИМ: Член производного класса, имя которого совпадает с именем члена базового класса, скрывает член базового 
	// класса и предотвращает прямой доступ к нему
	//std::size_t quantity - Скрыло бы доступ к членам Disc_quote
	//double discount;
};

// Виртуальный ф-ии с аргументами по умолчанию должны использовать те же значения аргументов в базовом и производных классах
double Bulk_quote::net_price(std::size_t cnt = 0) const	// ПРИМ: спецификатор override - только внутри определения в классе
{
	std::cout << "[ override net_price() called] " << std::endl;

	// ПРИМ: виртуальная ф-ия производного класса может вызвать вирт.ф-ию своего базового класса
	Quote::net_price(); // вызов с аргументов по умолчанию ( без Quote::  - рекурсия)

	// доступ к статическому члену базового класса
	Quote::i = 1;
	Bulk_quote::i = 1;
	i = 1;

	if(cnt >= quantity) return cnt * (1 - discount) * price;	// quantity, discount - защищенные члены прямого базового класса

	return cnt * price;	// price - защищенный член косвеного базового класса
}

// дружественный класс (базовому)
class Pal
{
public:
	std::string f1(Quote q) { return q.bookNo; }
	//double f2(Disc_quote dq) { return dq.discount; }	// ОШИБКА нельзя создать объект - параметр абстрактного класса
	//double f3(Bulk_quote bq) { return bq.discount; }	// ОШИБКА не имеет доступ к членам производных классов
	double f4(Bulk_quote bq) { return bq.price; }		// ОК т.к. price - член базового класса
};


// ПРИМ: статический тип указателя или ссылки на базовый класс может отличаться от его динамического типа
// ПРИМ: Виртуальные ф-ии распознаются во время выполнения , только если вызов осуществляется чеез ссылку или указатель.
// (только в этих случаях динамический тип объекта может отличаться от статического)
void print_quote(Quote& q, std::size_t n)
{
	// Ф-ия будет вызываться динамически в зависимости от переданного класса (т.к. передается по ссылке) 
	// ПОЛИМОРФИЗМ
	std::cout << "bookNo: " << q.isbn() << ", price: " << q.net_price(n) << std::endl;

}

double print_total(std::ostream& os, const Quote& q, std::size_t n)
{

	double ret = q.net_price(n);

	os << "ISBN: " << q.isbn() << " # sold: " << n << " total due: " << ret << std::endl;

	return ret;
}


class Base
{
public:
	int pub_mem = 0;
protected:
	int prot_mem = 1;
private:
	int priv_mem = 2;
};

// структура по умолчанию имеет открытый тип наследования 
// класс по умолчанию имеет закрытый тип наследования 
struct Pub_Derv : public Base
{
	// имеет доступ к pub_mem и prot_mem. Они вкладываются как открытые в производный класс
	// prot_mem остается защищенной в Pub_Derv
	int f1() { return pub_mem; }
	int f2() { return prot_mem; }
};

struct Prot_Derv : protected Base
{
	int f1() { return pub_mem; }
	int f2() { return prot_mem; }
};

// закрытое наследование не затрагивает доступ в производном классе
struct Priv_Derv : private Base
{
	int f1() { return pub_mem; }
	int f2() { return prot_mem; }
};

struct Derived_from_Public : public Pub_Derv
{	
	// prot_mem остается защищенной в Pub_Derv
	// pub_mem остается остается открытым в Pub_Derv
	int use_base1() { return pub_mem; }
	int use_base2() { return prot_mem; }
};

struct Derived_from_Protected : public Prot_Derv
{	
	// prot_mem, pub_mem остаются защищенными в Prot_Derv
	int use_base1() { return pub_mem; }
	int use_base2() { return prot_mem; }
};

struct Derived_from_Private : public Priv_Derv
{
	// ОШИБКА - Base::prot_mem является закрытым в Priv_Derv
	//int use_base() { return prot_mem; } 
};




void chapter15 (void)
{
	Quote q;
	//Disc_quote dq;	// ОШИБКА - абстрактный класс не позволяет создавать объекты
	Bulk_quote bq("testbook", 13.43, 1, 0.3);

	// ПРИМ: 1. преобразование из производного класса в базовый применимо только к указателю или ссылке
	//		 2. нет неявного преобразования из типа базового класса в тип проивзодного
	//		 3. при преобразовании производного в базовый член класса может быть недостпуен из=за спецификации управления доступом	    

	Quote& qr = q;		// статический тип qr - Quote& , динамический Quote (совпадает)
	Quote& qbr = bq;	// статический тип qr - Quote& , а динамический Bulk_quote 
						// (такое возможно т.к. Bulk_quote включает в себя базовый класс Quote)
	//Bulk_quote& br = q;	// ОШИБКА: не существует неявного преобразования из базового в производный класс


	qbr.net_price(10);	// вызов перегруженной в Bulk_quote ф-ии

	q = bq;	// при инициализации объекта базового типа или присвоении объекта производного типа
			// копируется, перемещеается или присваивается только часть базового класса произхводного объекта (bookNo, price)
			// Производная часть объекта игнорируется!

	print_quote(q, 1);	// без скидок
	print_quote(bq, 1);	// со скидкой .т.к. bq- объект проивзодного класса


	Pub_Derv d1;
	Priv_Derv d2;
	Prot_Derv d3;
	Derived_from_Public dd1;
	Derived_from_Private dd2;
	Derived_from_Protected dd3;

	Base* p = &d1;	// OK
	//p = &d2;		// ОШИБКА: Base закрыто  (недоступно)
	//p = &d3;		// ОШИБКА: Base защищено (недоступно)
	p = &dd1;		// OK
	//p = &dd2;		// ОШИБКА: Base закрыто  (недоступно)
	//p = &dd3;		// ОШИБКА: Base защищено (недоступно)


	// Вызов стандартного конструктора происходит по цепи иерархии наследования Bulk_quote() - Disc_quote() - Quote()
	Quote* tmp = new Bulk_quote;	
	// Благодаря виртуальному деструктору базового класса Quote удаление объекта производного класса происходит корректно
	delete tmp;


	// ПРИМ: Нельзя поместить объекты связанных наследованием типов непосредственно в контейнер, поскольку нет никакого
	// способа определить контейнер разных типов. Поэтому используют контейнеры указателей на базовый класс, где могут 
	// храниться объекты и базового и производных типов.
	std::vector<std::shared_ptr<Quote>> basket;
	basket.push_back(std::make_shared<Quote>("123", 1));
	basket.push_back(std::make_shared<Bulk_quote>("456", 1, 10, .25));
	std::cout << basket.back()->net_price(15) << std::endl;	// net_price заивисит от динамического объекта для которого вызвана


}


class Basket
{
public:
	// использует синтезируемый стандартный конструктор - ф-ии члены управления копированием

	void add_item(const std::shared_ptr<Quote>& sale) { items.insert(sale); }

	double total_receipt(std::ostream& os) const;
private:
	// ф-ия сравнения shared_ptrs необходима для упорядочения набора multiset
	static bool compare (const std::shared_ptr<Quote>& lhs, const std::shared_ptr<Quote>& rhs)
		{ return lhs->isbn() < rhs->isbn(); }

	// набор содержит несколько стратегий расценок, упорядоченных по bookNo
	std::multiset<std::shared_ptr<Quote>, decltype(compare)*> items{compare};
};

double Basket::total_receipt(std::ostream& os) const
{
	double sum = 0.0;

	// iter указывает на первый элемент в пакете эл-тов стем же bookNo. 
	// upper_bound() возвращает итератор на эл-т сразу после конца этого пакета
	for(auto iter = items.cbegin(); iter != items.cend(); iter = items.upper_bound(*iter)){

		// *iter - shared_ptr<Quote>, **iter - то что находится по умному указателю (объект Quote)
		sum += print_total(os, **iter, items.count(*iter));
	}

	os << sum;
	return sum;
}
