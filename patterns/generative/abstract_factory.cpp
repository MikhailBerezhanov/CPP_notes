#include <iostream>
#include <memory>

using namespace std;

// Абстрактная фабрика — это порождающий паттерн проектирования, 
// который позволяет создавать семейства связанных объектов, 
// не привязываясь к конкретным классам создаваемых объектов.

// Абстрактная фабрика задаёт интерфейс создания всех доступных 
// типов продуктов, а каждая конкретная реализация фабрики порождает 
// продукты одной из вариаций. Клиентский код вызывает методы фабрики 
// для получения продуктов, вместо самостоятельного создания с помощью 
// оператора new. При этом фабрика сама следит за тем, чтобы создать продукт 
// нужной вариации.

// 1. Choose common interface for idenctical products
class Chair
{
public:
	virtual ~Chair() = default;

	// client logic
	virtual void color() const = 0;
	virtual void sit_on() const = 0;
private:
};

class ModernChair : public Chair
{
public:
	void color() const override { cout << "ModernChair.color"; }
	void sit_on() const override { cout << "ModernChair.sit_on"; }
private:
};

class VictorianChair : public Chair
{
public:
	void color() const override { cout << "VictorianChair.color"; }
	void sit_on() const override { cout << "VictorianChair.sit_on"; }
private:
};


class Table
{
public:
	virtual ~Table() = default;

	// client logic
	virtual int number_of_legs() const = 0;
};

class ModernTable final : public Table 
{
public:
	virtual int number_of_legs() const { return 4; }
};

class VictorianTable final : public Table 
{
public:
	virtual int number_of_legs() const { return 3; }
};


// 2. Create abstract factory that creates all product types.
// This operation must return abstract object types.

/*
* Абстрактная Фабрика гарантирует, что все продукты, которые она создает,
* имеют одинаковую вариацию и, следовательно, совместимы.
*/

/**
 * Интерфейс Абстрактной Фабрики объявляет набор методов, которые возвращают
 * различные абстрактные продукты. Эти продукты называются семейством и связаны
 * темой или концепцией высокого уровня. Продукты одного семейства обычно могут
 * взаимодействовать между собой. Семейство продуктов может иметь несколько
 * вариаций, но продукты одной вариации несовместимы с продуктами другой.
 */

class FurnitureFactory
{
public:
	virtual unique_ptr<Chair> create_chair() = 0;
	virtual unique_ptr<Table> create_table() = 0;
};


// 3. Create concrete furniture factories
/**
 * Конкретные Продукты создаются соответствующими Конкретными Фабриками.
 */
/**
 * Конкретная Фабрика производит семейство продуктов одной вариации. Фабрика
 * гарантирует совместимость полученных продуктов. Обратите внимание, что
 * сигнатуры методов Конкретной Фабрики возвращают абстрактный продукт, в то
 * время как внутри метода создается экземпляр конкретного продукта.
 */

class ModernFactory : public FurnitureFactory
{
public:
	virtual unique_ptr<Chair> create_chair() override { return unique_ptr<Chair>(new ModernChair); }
	virtual unique_ptr<Table> create_table() override { return unique_ptr<Table>(new ModernTable); }
};

/**
 * Каждая Конкретная Фабрика имеет соответствующую вариацию продукта.
 */
class VictorianFactory : public FurnitureFactory
{
public:
	virtual unique_ptr<Chair> create_chair() override { return unique_ptr<Chair>(new VictorianChair); }
	virtual unique_ptr<Table> create_table() override { return unique_ptr<Table>(new VictorianTable); }
};


/**
 * Клиентский код работает с фабриками и продуктами только через абстрактные
 * типы: Абстрактная Фабрика и Абстрактный Продукт. Это позволяет передавать
 * любой подкласс фабрики или продукта клиентскому коду, не нарушая его.
 */
void client_code(FurnitureFactory &factory)
{
	auto chair = factory.create_chair();
	auto table = factory.create_table();

	// Business logic with products;
	chair->color();
	chair->sit_on();
	cout << "Table has " << table->number_of_legs() << " legs" << endl;
}



int main(int argc, char* argv[])
{
	// Application setup
	bool modern = true;
	if(argc > 1){
		modern = false;
	}

	FurnitureFactory *pfactory = nullptr;

	if(modern /* config is set to use modern factory*/){
		pfactory = new ModernFactory;
	}
	else{
		pfactory = new VictorianFactory;
	}

	// Client code not changed , we only need to add new 
	// concrete factory to change application behavior
	client_code( *pfactory );

	delete pfactory;

	return 0;
}