#include <iostream>
#include <memory>

using namespace std;

// Мост — это структурный паттерн проектирования, 
// который разделяет один или несколько классов на 
// две отдельные иерархии — абстракцию и реализацию, 
// позволяя изменять их независимо друг от друга.
//
// Паттерн Мост предлагает заменить наследование 
// агрегацией или композицией. Для этого нужно выделить 
// одну из «плоскостей» в отдельную иерархию и ссылаться 
// на объект этой иерархии, вместо хранения его состояния 
// и поведения внутри одного класса.

// Признаки применения паттерна: Если в программе чётко выделены 
// классы «управления» и несколько видов классов «платформ», причём 
// управляющие объекты делегируют выполнение платформам, то можно сказать, 
// что у вас используется Мост.

// Применимость:

// 1. Когда вы хотите разделить монолитный класс, который содержит 
// несколько различных реализаций какой-то функциональности (например, 
// если класс может работать с разными системами баз данных).
// 
// Чем больше класс, тем тяжелее разобраться в его коде, и тем больше это 
// затягивает разработку. Кроме того, изменения, вносимые в одну из реализаций,
//  приводят к редактированию всего класса, что может привести к внесению случайных 
// ошибок в код.
// 
// Мост позволяет разделить монолитный класс на несколько отдельных иерархий. 
// После этого вы можете менять их код независимо друг от друга. Это упрощает 
// работу над кодом и уменьшает вероятность внесения ошибок.

// 2. Когда класс нужно расширять в двух независимых плоскостях.
// 
// Мост предлагает выделить одну из таких плоскостей в отдельную иерархию классов, 
// храня ссылку на один из её объектов в первоначальном классе.

// 3. Когда вы хотите, чтобы реализацию можно было бы изменять во время выполнения 
// программы.
//
// Мост позволяет заменять реализацию даже во время выполнения программы, так как 
// конкретная реализация не «вшита» в класс абстракции.


/**
 * Реализация устанавливает интерфейс для всех классов реализации. Он не должен
 * соответствовать интерфейсу Абстракции. На практике оба интерфейса могут быть
 * совершенно разными. Как правило, интерфейс Реализации предоставляет только
 * примитивные операции, в то время как Абстракция определяет операции более
 * высокого уровня, основанные на этих примитивах.
 */
// Interface of a device  ( Implementation )
struct IDevice
{
	virtual ~IDevice() = default;

	virtual void switch_on() = 0;
	virtual void switch_off() = 0;

	virtual void set_volume(int percent) = 0;
	virtual void set_channel(int ch) = 0;
};

class TV : public IDevice
{
public:
	virtual void switch_on() override { cout << "TV::switch_on" << endl; }
	virtual void switch_off() override { cout << "TV::switch_off" << endl; }
	virtual void set_volume(int percent) override { cout << "TV::set_volume" << endl; }
	virtual void set_channel(int ch) override { cout << "TV::set_channel" << endl; }

private:
	bool state = false; // off
};

class Radio : public IDevice
{
public:
	virtual void switch_on() override { cout << "Radio::switch_on" << endl; }
	virtual void switch_off() override { cout << "Radio::switch_off" << endl; }
	virtual void set_volume(int percent) override { cout << "Radio::set_volume" << endl; }
	virtual void set_channel(int ch) override { cout << "Radio::set_channel" << endl; }
};


/**
 * Абстракция устанавливает интерфейс для «управляющей» части двух иерархий
 * классов. Она содержит ссылку на объект из иерархии Реализации и делегирует
 * ему всю настоящую работу.
 */
// Control of devices ( Abstraction ).
// Could be extended independantly of device interface.
class RemoteControl
{
public:
	RemoteControl(const std::shared_ptr<IDevice>& device):
		m_device(device) {}

	void toggle_power(bool on_off)
	{
		if(on_off){
			m_device->switch_on();
		}
		else{
			m_device->switch_off();
		}
	}

	void volume_change(bool up_down)
	{
		if(up_down){
			m_device->set_volume(10);
		}
		else{
			m_device->set_volume(-10);
		}
	}

	void channel_change(bool up_down)
	{
		if(up_down){
			m_device->set_channel(1);
		}
		else{
			m_device->set_channel(-1);
		}
	}

protected:
	const std::shared_ptr<IDevice> m_device; // Bridge to realization 
};

/**
 * За исключением этапа инициализации, когда объект Абстракции связывается с
 * определённым объектом Реализации, клиентский код должен зависеть только от
 * класса Абстракции. Таким образом, клиентский код может поддерживать любую
 * комбинацию абстракции и реализации.
 */
void ClientCode(RemoteControl &control)
{
	control.toggle_power(true);
	control.volume_change(false);
	control.channel_change(true);
}

int main()
{
	auto device1 = std::make_shared<Radio>();
	auto device2 = std::make_shared<TV>();

	auto controller = std::make_unique<RemoteControl>(device1);
	ClientCode(*controller);

	controller = std::make_unique<RemoteControl>(device2);
	ClientCode(*controller);

	return 0;
}
