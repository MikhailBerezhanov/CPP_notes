#include <iostream>
#include <string>
#include <vector>


// Build complex object by parts. 
// Uses different methods for differet parts
// instead of huge constructor

/**
 * Интерфейс Строителя объявляет создающие методы для различных частей объектов Продуктов.
 */
class IBuilder
{
public:
	virtual ~IBuilder() = default;
	virtual void build_step_A() const = 0;
	virtual void build_step_B() const = 0;
	virtual void build_step_C() const = 0;
};


class Product1
{
public:
	std::vector<std::string> parts;

	void list_parts() const
	{
		for(const auto &s : parts){
			std::cout << s << "\n";
		}
		std::cout << std::endl;
	}
};

class ConcreteBuilder1: public IBuilder
{
public:
	ConcreteBuilder1(){ this->reset(); }
	~ConcreteBuilder1(){ if(product) delete product; }

	void reset(){ product = new Product1; }

	void build_step_A() const override { this->product->parts.push_back("partA"); }
	void build_step_B() const override { this->product->parts.push_back("partB"); }
	void build_step_C() const override { this->product->parts.push_back("partC"); }

	 /**
     * Конкретные Строители должны предоставить свои собственные методы
     * получения результатов. Это связано с тем, что различные типы строителей
     * могут создавать совершенно разные продукты с разными интерфейсами.
     * Поэтому такие методы не могут быть объявлены в базовом интерфейсе
     * Строителя (по крайней мере, в статически типизированном языке
     * программирования). Обратите внимание, что PHP является динамически
     * типизированным языком, и этот метод может быть в базовом интерфейсе.
     * Однако мы не будем объявлять его здесь для ясности.
     *
     * Как правило, после возвращения конечного результата клиенту, экземпляр
     * строителя должен быть готов к началу производства следующего продукта.
     * Поэтому обычной практикой является вызов метода сброса в конце тела
     * метода getProduct. Однако такое поведение не является обязательным, вы
     * можете заставить своих строителей ждать явного запроса на сброс из кода
     * клиента, прежде чем избавиться от предыдущего результата.
     */

    /**
     * Please be careful here with the memory ownership. Once you call
     * GetProduct the user of this function is responsable to release this
     * memory. Here could be a better option to use smart pointers to avoid
     * memory leaks
     */
	Product1* get_product() {
		Product1 *tmp = this->product;
		this->reset();
		return tmp;
	}

private:
	// Будем хранить указатель на продукт, а не сам продукт т.к. объект может
	// быть потенциально большим по памяти и его возвращение как копии не ОК
	Product1 *product = nullptr;

	
};

/**
 * Директор отвечает только за выполнение шагов построения в определённой
 * последовательности. Это полезно при производстве продуктов в определённом
 * порядке или особой конфигурации. Строго говоря, класс Директор необязателен,
 * так как клиент может напрямую управлять строителями.
 */
class Director
{
public:

	 /**
     * Директор работает с любым экземпляром строителя, который передаётся ему
     * клиентским кодом. Таким образом, клиентский код может изменить конечный
     * тип вновь собираемого продукта.
     */
	void set_builder(IBuilder *b){ this->builder = b; }

	 /**
     * Директор может строить несколько вариаций продукта, используя одинаковые
     * шаги построения.
     */

	void produce_chip_product(){
		this->builder->build_step_A();
	}

	void produce_heavy_product(){
		this->builder->build_step_A();
		this->builder->build_step_B();
		this->builder->build_step_B();
		this->builder->build_step_B();
		this->builder->build_step_C();
	}

private:
	IBuilder *builder;
};


void client_code(Director &director)
{
	ConcreteBuilder1 builder = ConcreteBuilder1;
	director.set_builder(builder);

	std::cout << "Cheap product: \n";
	director.produce_chip_product();
	Product1 *p = builder->get_product();
	p->list_parts();
	delete p;

	std::cout << "Heavy product: \n";
	director.produce_heavy_product();
	p = builder->get_product();
	p->list_parts();
	delete p;

	std::cout << "Custom product: \n";
	builder->build_step_B();
	builder->build_step_A();
	builder->build_step_C();
	p = builder->get_product();
	p->list_parts();
	delete p;

	delete builder;
}


int main()
{
	Director director;
	client_code(director);

	return 0;
}