
#include <iostream>
#include <thread>
#include <future>	// async, future, promise
#include <chrono>	// seconds
#include <mutex>

#include "exercises.h"

// Класс RAII ( получение ресурса есть иницизалиция ) (в данном случае уничтожение объекта -
// есть деинициализация )
class ThreadRAII{
public:
	enum class DtorAction { join, detach };

	ThreadRAII(std::thread&& tt, DtorAction a):
		action(a), t(std::move(tt)) {}

	// Чтобы после уничтожения объект thread не оказался в "присоединяемом" состоянии
	~ThreadRAII(){
		if(t.joinable()){
			if(action == DtorAction::join){
				t.join();
			}
			else{
				t.detach();
			}
		}
	}

	// Поддержка операций перемещения
	ThreadRAII( ThreadRAII&& ) = default;
	ThreadRAII& operator=( ThreadRAII&& ) = default;

	// Чтобы использовать методы std::thread
	std::thread& get() { return t; }

private:
	DtorAction action;
	std::thread t;		// Объявляется в списке членов-данных последним, чтобы все данные
						// уже были инициализированы ДО запуска потока
};

void real_async_calls()
{
	std::cout << "This thread: " << std::this_thread::get_id() << std::endl;

	auto func = []() { std::cout << "Start async: " << std::this_thread::get_id() << std::endl; };

	auto func2 = []() { std::cout << "Start deferred: " << std::this_thread::get_id() << std::endl; };

	// По умолчанию std::async конструктор вызывается как
	auto fut = std::async(std::launch::async | std::launch::deferred, func);
	// и сам определяет запускать ли функцию в параллельном потоке или в текущем,
	// откладывая ее вызов до fut.get() или fut.wait() блокируя текукщий поток!

	// Для гарантированного асинхронного выполнения следует использовать
	auto fut2 = std::async(std::launch::async, func);


	auto fut3 = std::async(std::launch::deferred, func2);
	std::this_thread::sleep_for(std::chrono::seconds(2));

	fut3.get();
}


std::future<int> pack_task()
{
	// Функция для параллельного выполнения
	auto func = []()->int { 
		std::cout << "async task: " << std::this_thread::get_id() << std::endl; 
		return 10;
	};

	// Подгоавливает ф-ию или иной вызываемый объект к асинхронному выполнению, "заворачивая"
	// ее таким образом, что ее результат помещается в общее пространство (разделяемое между
	// future и promise)
	std::packaged_task<int()> pt ( func );

	auto fut = pt.get_future();

	// Запуск параллельно
	std::thread t (std::move(pt));
	t.detach(); // Если в конце области видимости с t будет неподключаемым

	return fut;

	// ПРИМ: Аналогичный функционал предлагает std::promise
}

// Оповещение одного потока другим( задача обнаружения и задача реакции )
// Использование переменной условия
void tasks_notification_1()
{
	std::condition_variable cv;	// переменная условия события
	std::mutex m;	// мьютекс для использования с cv

	// Задача реакции на событие (тело потока 1)
	auto reaction = [&m, &cv](){

		// ... (Подготовка к реакции на событе)

		// Открытие критического раздела
		{
			// Блокировка мьютекса
			std::unique_lock<std::mutex> lock(m);
			// Ожидание уведомления
			cv.wait(lock, [](){ return "Произошло ли событие"; });	
				// ПРОБЛЕМА 1. Вызов wait иногда приводит к ложным пробуждениям (когда
				// переменная условия не была уведомлена). Поэтому нужно обязательно 
				// проверять ожидаемое условие. Для этого можно передать лямбда-выражение 
				// в wait. Однако поток реакции далеко не всегда в состоянии определить
				// имело ли место ожидаемое событие!

			// ... (Реакция на событие)
		}	// Разблокировка мьютекса, выход из критического раздела
	};

	// Задача обнаружения события (тело потока 2)
	auto detection = [&cv](){
		// ... (Обнаружение события)

		// Уведомлние задачи - реакции
		cv.notify_one();
	};

	// ПРОБЛЕМА 2. Если задача обнаружения уведомляет переменную условия ДО вызова
	// wait задачей реакции, задача реакции "зависнет".
}

// Использование атоманрного булевого флага 
void tasks_notification_2()
{
	std::atomic<bool> flag(false);

	// Задача реакции на событие (тело потока 1)
	auto reaction = [&flag](){

		// ... (Подготовка к реакции на событе)

		// Ожидание события 
		while( !flag.load() );
		// ПРОБЛЕМА : Не настоящая блокировка потока -> расход ресурсов (постоянный опрос)

		// Реакция на событие
	};


	// Задача обнаружения события (тело потока 2)
	auto detection = [&flag](){
		// ... (Обнаружение события)

		// Уведомлние задачи - реакции
		flag.store(true);
	};
}

// Комбинированный метод булевый флаг + перенная условия
void tasks_notification_3()
{
	// Флаг указывает произошло ли событие
	bool flag(false);
	// Доступ к флагу синхронизирован мьютексом
	std::condition_variable cv;	// переменная условия события
	std::mutex m;

	// Задача реакции на событие (тело потока 1)
	auto reaction = [&flag, &m, &cv](){

		// ... (Подготовка к реакции на событе)

		// Открытие критического раздела
		{
			// Блокировка мьютекса
			std::unique_lock<std::mutex> lock(m);
			// Ожидание уведомления
			cv.wait(lock, [&flag](){ return flag; });	
			// ... (Реакция на событие)
		}	// Разблокировка мьютекса, выход из критического раздела
	};

	// Задача обнаружения события (тело потока 2)
	auto detection = [&flag, &m, &cv](){
		// ... (Обнаружение события)

		// Уведомлние задачи - реакции
		{
			std::lock_guard<std::mutex> lock(m);
			flag = true;
		}
		cv.notify_one();
	};

	// ПРОБЛЕМА: Этот подход работает, но приходится управлять И флагом , 
	// И переменной условия. Кажется не очень чистым решением.
}

// Использование фьючерса и promise<void>
void tasks_notification_4()
{
	std::promise<void> p;	// Коммуникационный канал

	// Задача реакции на событие (тело потока 1)
	auto reaction = [&p](){

		// ... (Подготовка к реакции на событе)

		// Ожидание уведомления
		p.get_future().wait(); // блокирует поток

		// ... (Реакция на событие)
	};

	// Задача обнаружения события (тело потока 2)
	auto detection = [&p](){
		// ... (Обнаружение события)

		// Уведомлние задачи - реакции
		p.set_value();
	};


	// ПРОБЛЕМА: Работает одноразово! set_value() у promise можно вызвать лишь один раз!
}



void threads_and_tasks()
{
	using namespace std;

	real_async_calls();

	auto func = []() { cout << "Start thread: " << this_thread::get_id() << endl; }; 

	// Поток немедленно стартует свою функцию (по умолчанию он joinable !)
	// Если внутри потока происходит необрабатываемое исключение, вызывается  std::terminate
	thread t(func);

	this_thread::sleep_for(chrono::seconds(1));

	// Если поток в момент вызова деструктора std::thread объекта остается "присоединяемым", 
	// программа экстренно завершается  std::terminate. 
	// т.к. неизвестно надо ли было вызвать join или detach при уничтожении объекта. 
	t.join();

	ThreadRAII tr ( thread(func), ThreadRAII::DtorAction::join );

	// Получить дескриптор потока для низкоувронеого управления через API pthread
	auto nh = tr.get().native_handle(); 

	// При выходе tr  не останется в присоединяемом состоянии
}




