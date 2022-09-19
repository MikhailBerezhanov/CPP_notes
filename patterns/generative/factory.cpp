#include <iostream>
#include <string>

// Тип создаваемого объекта определяем конкретная фабрика-потомок


// Продукты обладают общими свойствами - общим интерфейсом
class IProduct
{
public:
	virtual ~IProduct() = default;
	virtual void prepare() = 0;
	virtual void work() = 0;
	virtual void rest() = 0;
};

/**
 * Конкретные Продукты предоставляют различные реализации интерфейса Продукта.
 */
class Product1 final: public IProduct
{
public:
	Product1(const std::string &name = "Product_1"): name_(name) {}

	void prepare() override { std::cout << name_ << std::endl; }
	void work() override { std::cout << name_ << ": working" << std::endl; }
	void rest() override { std::cout << name_ << ": rest" << std::endl; }
private:
	std::string name_;
};

class Product2 final: public IProduct
{
public:
	Product2(const std::string &name = "Product_2"): name_(name) {}

	void prepare() override { std::cout << name_ << std::endl; }
	void work() override { std::cout << name_ << ": working" << std::endl; }
	void rest() override { std::cout << name_ << ": rest" << std::endl; }
private:
	std::string name_;
};

// Базовый класс фабрики
// Содержит не только интерфейс создания объектов, но и некоторую бизнесс логику
// Работает не с конкретным продуктом, а с некой абстракцией - интерфейсом
class Factory
{
public:

	virtual ~Factory() = default;

	// фабричный метод создания продуктов
	// virtual IProduct *make_product() { 
	// 	// По умолчанию создадим продукт2
	// 	return new Product2;
	// };

	virtual IProduct *make_product() = 0;

	void buisness(){
		IProduct *product = this->make_product();
		product->prepare();
		product->work();
		product->rest();
		delete product;
	}

private:

};



// Конкретные фабрики переопределяют фабричный метод и
// возвращают из него собственные продукты.
class ConcreteFactory1: public Factory
{
public:
	IProduct* make_product() override { return new Product1; }
};

class ConcreteFactory2: public Factory
{
public:
	IProduct* make_product() override { return new Product2; }
};

int main()
{
	Factory *factory = new ConcreteFactory2;
	factory->buisness();
	delete factory;

	factory = new ConcreteFactory1;
	factory->buisness();
	delete factory;

	return 0;
}
