#include <iostream>
#include <memory>

using namespace std;

// Одиночка — это порождающий паттерн, который 
// гарантирует существование только одного объекта 
// определённого класса, а также позволяет достучаться 
// до этого объекта из любого места программы.

class Singleton
{
public:

	Singleton(const Singleton& rhs) = delete;
	Singleton& operator= (const Singleton& rhs) = delete;

	static Singleton& GetInstance(int ival, const std::string& str)
	{
		// При многопоточном вызове данного метода, стандартом 
		// гаранитруется синхронизованная инциализация static переменной 
		// (при конкурентном доступе к obj, пока один поток проводит
		// инициадизацию остальные должны ждать завершения, причем dedlock
		// не должен возникнуть)  --  STD $6.7.4, C++11 
		static Singleton obj(ival, str);
		return obj;
	}

	void BusinessLogic()
	{
		cout << "I'm the singleton(" << m_intData << " " << m_strData << ")" << endl;
	} 

private:
	Singleton(int ival, const std::string& str)
		: m_intData(ival), m_strData(str)
	{
		// Инициализация объекта
		cout << "Created singleton(" << m_intData << " " << m_strData << ")" << endl;
	}

	int m_intData;
	std::string m_strData;
};

int main()
{
	auto& instance1 = Singleton::GetInstance(1, "a");
	auto& instance2 = Singleton::GetInstance(2, "b");
	auto& instance3 = Singleton::GetInstance(3, "c");

	/* auto instance4 = instance3; */  	// Error	
	/* instance1 = instance2; */		// Error

	instance1.BusinessLogic();
	instance2.BusinessLogic();
	instance3.BusinessLogic();

	return 0;
}
