#include <iostream>
#include <memory>
#include <string>

using namespace std;

// Декоратор — это структурный паттерн, который позволяет добавлять объектам новые 
// поведения на лету, помещая их в объекты-обёртки.
//
// Декоратор позволяет оборачивать объекты бесчисленное количество раз благодаря тому, 
// что и обёртки, и реальные оборачиваемые объекты имеют общий интерфейс.

// Применимость

// 1. Когда вам нужно добавлять обязанности объектам на лету, незаметно для кода, 
// который их использует.
//
//  Объекты помещают в обёртки, имеющие дополнительные поведения. Обёртки и сами 
//  объекты имеют одинаковый интерфейс, поэтому клиентам без разницы, с чем работать 
//  — с обычным объектом данных или с обёрнутым.

// 2. Когда нельзя расширить обязанности объекта с помощью наследования.
//
//  Во многих языках программирования есть ключевое слово final, которое может заблокировать 
//  наследование класса. Расширить такие классы можно только с помощью Декоратора.

/**
 * Базовый интерфейс Компонента определяет поведение, которое изменяется
 * декораторами.
 */
struct IComponent
{
	virtual ~IComponent() = default;
	virtual string operation() = 0;
};

/**
 * Конкретные Компоненты предоставляют реализации поведения по умолчанию. Может
 * быть несколько вариаций этих классов.
 */
struct ConcComponent : IComponent
{
	string operation() override
	{
		return "ConcreteComponent";
	}
};

/**
 * Базовый класс Декоратора следует тому же интерфейсу, что и другие компоненты.
 * Основная цель этого класса - определить интерфейс обёртки для всех конкретных
 * декораторов. Реализация кода обёртки по умолчанию может включать в себя поле
 * для хранения завёрнутого компонента и средства его инициализации.
 */
class BaseDecorator : public IComponent
{
public:
	BaseDecorator(const shared_ptr<IComponent>& component): m_component(component) {} 

	/**
   * Декоратор делегирует всю работу обёрнутому компоненту.
   */
	string operation() override
	{
		return m_component->operation();
	}

protected:
	shared_ptr<IComponent> m_component;
};

/**
 * Конкретные Декораторы вызывают обёрнутый объект и изменяют его результат
 * некоторым образом.
 */
class ConcreteDecoratorA : public BaseDecorator
{
public:
	ConcreteDecoratorA(const shared_ptr<IComponent>& component): BaseDecorator(component) {}

	/**
   * Декораторы могут вызывать родительскую реализацию операции, вместо того,
   * чтобы вызвать обёрнутый объект напрямую. Такой подход упрощает расширение
   * классов декораторов.
   */
	string operation() override
	{
		return "ConcreteDecoratorA(" + BaseDecorator::operation() + ")";
	}
};

/**
 * Декораторы могут выполнять своё поведение до или после вызова обёрнутого
 * объекта.
 */
class ConcreteDecoratorB : public BaseDecorator
{
public:
	ConcreteDecoratorB(const shared_ptr<IComponent>& component): BaseDecorator(component) {}

	string operation() override
	{
		return "[" + BaseDecorator::operation() + "]ConcreteDecoratorB";
	}
};


void ClientCode(IComponent& component)
{
	cout << "RESULT: " << component.operation() << endl;
}

int main()
{
	auto simpleComponent = std::make_shared<ConcComponent>();
	auto wrapperA = std::make_shared<ConcreteDecoratorA>(simpleComponent);
	auto wrapperB = std::make_shared<ConcreteDecoratorB>(wrapperA);

	ClientCode(*wrapperB);

	return 0;
}
