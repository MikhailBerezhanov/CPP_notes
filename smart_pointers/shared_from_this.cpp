
// Проблема:
// если передавать конструктору shared_ptr указатель this,
// то каждый раз будет создаваться новый контрольный блок,
// что приведет к неопределенному поведению.
//
// Решение:
// enable_shared_from_this - Это шаблон базового класса, который вы наследуете,
// если хотите, чтобы класс, управляемый указателями std::shared_ptr, 
// был способен безопасно создавать std::shared_ptr из указателя this.

#include <iostream>
#include <memory>
#include <vector>

using namespace std;

class Person;
class Widget;

vector<shared_ptr<Person>> processedPersons;
vector<shared_ptr<Widget>> processedWidgets;

class Person
{
public:
	Person(const std::string& name): m_name(name){}

	void process()
	{
		// Undefined behaviour

		// Этот код будет компилироваться, но он передает обычный указатель
		// (this) контейнеру указателей std::shared_ptr. Конструируемый таким образом 
		// указатель std::shared_ptr будет создавать новый управляющий блок для 
		// указываемого Person (т.е. для объекта *this).

		processedPersons.emplace_back(this);
		cout << m_name << " processed" << endl;
	}
private:
	string m_name;
};

// Производный класс порождается от Базового класса, шаблонизированного
// производным - паттерн называется Странно повторяющийся шаблон 
// ( The Curiously Recurring Template Pattern - CRTP)
//
// Шаблон std::enable_shared_from_this определяет функцию-член, которая создает
// std::shared_ptr для текущего объекта, но делает это, не дублируя управляющие блоки.
class Widget : public std::enable_shared_from_this<Widget> 
{
public:
	void process()
	{
		// Внутри себя shared_from_this ищет управляющий блок текущего объекта и создает
		// новый std::shared ptr, который использует этот управляющий блок. Дизайн функции
		// полагается на тот факт, что текущий объект имеет связанный с ним управляющий блок.
		// Чтобы это было так, должен иметься уже существующий указатель std::shared_ptr
		// (например, за пределами функции-члена, вызывающей shared_from_this ) , который 
		// указывает на текущий объект. Если такого std::shared_ptr нет 
		// (т.е. если текущий объект не имеет связанного с ним управляющего блока), 
		// результатом будет неопределенное поведение, хотя обычно 
		// shared_from_this генерирует исключение.

		processedWidgets.emplace_back( shared_from_this() );
		cout << "Widget processed" << endl;
	}
};


// Чтобы препятствовать клиентам вызывать функции-члены, в которых используется
// shared_from_this, до того как на объект будет указывать указатель std::shared_ptr,
// классы, наследуемые от std::enaЫe_shared_from_this , часто объявляют свои 
// конструкторы как private и заставляют клиентов создавать объекты путем вызова 
// фабричных функций, которые возвращают указатели std::shared_ptr.

shared_ptr<Widget> CreateWidget()
{
	return make_shared<Widget>();
}

int main()
{
	{
		auto p1 = make_shared<Person>("p1");	// 1
		shared_ptr<Person> p2(p1);				// 2

		// Each shared_ptr(this) thinks it's the only owner of the object
		p1->process();							// 1 - control block duplicate
		p2->process();							// 1 - control block duplicate

		cout << "Person use_count: " << p2.use_count() << endl;
	}
	
	{
		Widget widget1;

		/* widget1.process(); */	// throws bad_weak_ptr

		// Each shared_ptr points to the same resource 
		auto widget2 = CreateWidget();			// 1
		widget2->process();						// 2

		shared_ptr<Widget> widget3(widget2);	// 3
		widget3->process();						// 4

		cout << "Widget use_count: " << widget3.use_count() << endl;
	}
	
	return 0;
}