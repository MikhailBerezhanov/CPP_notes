#include <iostream>
#include <memory>

using namespace std;

// Заместитель — это структурный паттерн проектирования, 
// который позволяет подставлять вместо реальных объектов 
// специальные объекты-заменители. Эти объекты перехватывают 
// вызовы к оригинальному объекту, позволяя сделать что-то до 
// или после передачи вызова оригиналу.

// Применимость

// 1. Ленивая инициализация (виртуальный прокси). Когда у вас есть 
// тяжёлый объект, грузящий данные из файловой системы или базы данных.
//
//  Вместо того, чтобы грузить данные сразу после старта программы, можно 
//  сэкономить ресурсы и создать объект тогда, когда он действительно понадобится.

// 2. Защита доступа (защищающий прокси). Когда в программе есть разные типы 
// пользователей, и вам хочется защищать объект от неавторизованного доступа. 
// Например, если ваши объекты — это важная часть операционной системы, а 
// пользователи — сторонние программы (хорошие или вредоносные).
//
//  Прокси может проверять доступ при каждом вызове и передавать выполнение
//  служебному объекту, если доступ разрешён.

// 3. Локальный запуск сервиса (удалённый прокси). Когда настоящий сервисный 
// объект находится на удалённом сервере.
//
//  В этом случае заместитель транслирует запросы клиента в вызовы по сети в 
//  протоколе, понятном удалённому сервису.

// 4. Логирование запросов (логирующий прокси). Когда требуется хранить историю 
// обращений к сервисному объекту.
//
//  Заместитель может сохранять историю обращения клиента к сервисному объекту.

// 5. Кеширование объектов («умная» ссылка). Когда нужно кешировать результаты 
// запросов клиентов и управлять их жизненным циклом.
//
//  Заместитель может подсчитывать количество ссылок на сервисный объект, 
//  которые были отданы клиенту и остаются активными. Когда все ссылки освобождаются, 
//  можно будет освободить и сам сервисный объект (например, закрыть подключение к 
//  базе данных).
//
//  Кроме того, Заместитель может отслеживать, не менял ли клиент сервисный объект. 
//  Это позволит использовать объекты повторно и здóрово экономить ресурсы, особенно 
//  если речь идёт о больших прожорливых сервисах.


struct IService
{
	virtual void Operation() const = 0;
};

struct Service : IService
{
	void Operation() const override
	{
		cout << "Service::operation()" << endl;
	}
};

class Proxy : public IService
{
public:
	Proxy(Service *target): m_target(target){}

	void Operation() const override
	{
		if(!CheckAccess()){
			return;
		}

		m_target->Operation();

		PostOperationLogging();
	}

private:
	Service *m_target = nullptr;

	bool CheckAccess() const
	{
		cout << "PreOperation CheckAccess" << endl;
		return true;
	}

	void PostOperationLogging() const
	{
		cout << "PostOperationLogging" << endl;
	}
};

void ClientCode(const IService &service)
{
	service.Operation();
}

int main()
{
	auto subj = make_unique<Service>();
	Proxy subj_proxy(subj.get());

	std::cout << "Client: Executing the client code with a real subject:\n";
	ClientCode(*subj);

	std::cout << "\n";
  	std::cout << "Client: Executing the same client code with a proxy:\n";
	ClientCode(subj_proxy);

	return 0;
}