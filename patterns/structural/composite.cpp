#include <iostream>
#include <memory>
#include <list>
#include <string>

using namespace std;

// Компоновщик — это структурный паттерн проектирования, 
// который позволяет сгруппировать множество объектов в
// древовидную структуру, а затем работать с ней так, 
// как будто это единичный объект.

// Применимость

// 1.Когда вам нужно представить древовидную структуру объектов.
// 
//  Паттерн Компоновщик предлагает хранить в составных объектах 
//  ссылки на другие простые или составные объекты. Те, в свою очередь, 
//  тоже могут хранить свои вложенные объекты и так далее. В итоге вы 
//  можете строить сложную древовидную структуру данных, используя всего 
//  две основные разновидности объектов.

// 2. Когда клиенты должны единообразно трактовать простые и составные объекты.
// 
//  Благодаря тому, что простые и составные объекты реализуют общий интерфейс,
//  клиенту безразлично, с каким именно объектом ему предстоит работать.


struct IProduct
{
	virtual ~IProduct() = default;

	virtual int get_sum() const = 0;

	virtual bool is_composite() const { return false; }
};

/**
 * Класс Лист представляет собой конечные объекты структуры. Лист не может иметь
 * вложенных компонентов.
 *
 * Обычно объекты Листьев выполняют фактическую работу, тогда как объекты
 * Контейнера лишь делегируют работу своим подкомпонентам.
 */
// Basic product  ( Tree leaf )
class Product : public IProduct
{
public:
	Product(const string& name, int price): m_name(name), m_price(price) {}

	// just returns it's value
	int get_sum() const override { return m_price; }

protected:
	string m_name;
	int m_price = 0;
};

/**
 * Класс Контейнер содержит сложные компоненты, которые могут иметь вложенные
 * компоненты. Обычно объекты Контейнеры делегируют фактическую работу своим
 * детям, а затем «суммируют» результат.
 */
// Container of products
class Box : public IProduct
{
public:

	bool is_composite() const override { return true; }

	void add(const shared_ptr<IProduct>& product)
	{
		m_childs.push_back(product);
	}

	void remove(const shared_ptr<IProduct>& product)
	{
		m_childs.remove(product);
	}

	/**
   * Контейнер выполняет свою основную логику особым образом. Он проходит
   * рекурсивно через всех своих детей, собирая и суммируя их результаты.
   * Поскольку потомки контейнера передают эти вызовы своим потомкам и так
   * далее, в результате обходится всё дерево объектов.
   */
	// Recursivly calling get_sum of container components
	int get_sum() const override 
	{ 
		int res = 0;

		for(const auto& item : m_childs){
			res += item->get_sum();
		}

		return res; 
	}

private:
	// contains another products: basic or boxes
	list< std::shared_ptr<IProduct> > m_childs;
};

// Products can be extended
class SpecialProduct : public Product
{
public:
	SpecialProduct(const string& name, int price): Product(name, price) {}

	// special logic with discount
	int get_sum() const override { return m_discount * m_price; }

protected:
	double m_discount = 0.5;
};


/**
 * Клиентский код работает со всеми компонентами через базовый интерфейс.
 */
void ClientCode(IProduct& complex_product)
{
	cout << "complex_product sum: " << complex_product.get_sum() << endl;
}

int main()
{
	// Creating complex product

	auto root_box = make_unique<Box>();

	auto productA = make_shared<Product>("A", 130);

	root_box->add(productA);

	auto spec_product = make_shared<SpecialProduct>("Spec", 130);
	auto productB = make_shared<Product>("B", 1000);
	auto box1 = make_shared<Box>();

	box1->add(spec_product);
	box1->add(productB);

	root_box->add(box1);

	ClientCode(*root_box);

	return 0;
}