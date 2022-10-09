#include <iostream>
#include <string>

using namespace std;

// explicit keyword test


class Person
{
public:

	// Без explicit допускается одно неявное преобразование типа в int
	explicit Person(int age): name_(""), age_(age) 
	{
		cout << "Person(int age)" << endl;
	}

	// Без explicit допускается одно неявное преобразование типа в std::string
	// (например передача литерала const char*)
	Person(const std::string &name): name_(name), age_(0)
	{
		cout << "Person(const std::string &)" << endl;
	}

	Person(const Person &rhs): name_(rhs.name_), age_(rhs.age_)
	{
		cout << "Person(const Person&)" << endl;
	}

	Person(Person &&rhs) noexcept : name_(std::move(rhs.name_)), age_(std::move(rhs.age_))
	{
		cout << "Person(Person&&)" << endl;
	}

	// NOTE: because of explicit keyword in Person(int) Ctr const char* arguments 
	// in assigment operations wouldn't be tried to be converted to int to be
	// used in Person(int) Ctr for temp object. Without explicit keyword it will.
	//
	// Person p1 = "mik";

	Person& operator= (const Person &rhs)
	{
		cout << "Person::operator= (const Person&)" << endl;

		this->name_ = rhs.name_;
		this->age_ = rhs.age_;

		return *this;
	}

	Person& operator= (Person &&rhs)
	{
		cout << "Person::operator= (Person&&)" << endl;

		// self-assinment protection
		if(this != &rhs){
			this->name_ = std::move(rhs.name_);
			this->age_ = std::move(rhs.age_);
		}

		return *this;
		// rhs dtr will be called here
	}

	bool is_same(const Person &another)
	{
		// return this == &another;
		return this->name_ == another.name_;
	}

private:
	string name_;
	int age_ = 0;
};



int main()
{
	// Person contructor from string called
	Person p("abuga");	// OK

	// ERROR - требуется два неяных преобразования:
	// const char* -> std;:string -> Person
	//Person p1 = "mik";  
	Person p1 = std::string("mik");

	// Если конструктор помечен explicit то используется: 
	// 1. неявный вызов конструктора Person(int) для создания временного объекта
	// 2. присваивание перемещения Person::operator=(Person&&) для инициализации p2 
	//	  созданным временным объектом.
	//
	// Без explicit применяется оптимизация инициализации 
	// - вызов Person(int)

	//Person p2 = 10;   `// ERROR конструктор explicit
	Person p2 = Person(10);	// OK and optimizatin works - just Person(int) called

	double dval = 10.143;
	short sval = 123;
	long long lval = 55555;

	// OK - convertion to int will be executed first, than constructor called
	Person p3{dval};	// narrowing conversion warning
	Person p4{sval};
	Person p5{lval};	// narrowing conversion warning
	

	p5.is_same(p4);		// OK

	//p5.is_same("bob");	// ERROR трубется более одного неявного преобразования:
						// const char* -> std::string -> Person 
	std::string name = "bob";
	cout << p5.is_same(name) << endl;	// OK - одно неявное преобразование std::string -> Person

	//p5.is_same(1043);	// ERROR - Одно неявное преобразование int -> Person , НО
						// конструктора помечен как explicit и не допускает применения
						// преобразований. 
	return 0;
}