#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>

using namespace std;


// Прототип — это порождающий паттерн, который позволяет копировать 
// объекты любой сложности без привязки к их конкретным классам.

// Все классы—Прототипы имеют общий интерфейс. Поэтому вы можете 
// копировать объекты, не обращая внимания на их конкретные типы и 
// всегда быть уверены, что получите точную копию. Клонирование совершается 
// самим объектом-прототипом, что позволяет ему скопировать значения всех полей, 
// даже приватных.

// Создание дубликата объекта без необходимости
// понимания его внутреннего устройства. Возможность
// массового использования однотипных объектов с разными
// настройками вместо определение множества подклассов

// Базовое представление формы (общий интерфейс прототипа)
class ShapePrototype
{
public:
	ShapePrototype(float x_, float y_): x(x_), y(y_){}
	virtual ~ShapePrototype() = default;

	// Note: using default copy ctr, copy assign

	virtual std::unique_ptr<ShapePrototype> clone() const = 0;

	virtual std::ostream& show_fields(std::ostream &os) const
	{
		os << "x: " << x << ", y: " << y;
		return os;
	}

protected:
	float x;
	float y;
};

// Конкретные представители форм
class Rectangle : public ShapePrototype
{
public:
	Rectangle(float x_, float y_, float w_, float h_):
		ShapePrototype(x_, y_), width(w_), height(h_) {}

	/**
   * Notice that Clone method return a Pointer to a new ConcretePrototype1
   * replica. so, the client (who call the clone method) has the responsability
   * to free that memory. I you have smart pointer knowledge you may prefer to
   * use unique_pointer here.
   */
	std::unique_ptr<ShapePrototype> clone() const override
	{
		return std::unique_ptr<ShapePrototype>(new Rectangle(*this));
	}

	std::ostream& show_fields(std::ostream &os) const override
	{
		ShapePrototype::show_fields(os);
		os << " , width: " << width << ", height: " << height;
		return os;
	}

private:
	float width;
	float height;
};

class Circle : public ShapePrototype
{
public:
	Circle(float x_, float y_, float r_): 
		ShapePrototype(x_, y_), radius(r_){}

	std::unique_ptr<ShapePrototype> clone() const override
	{
		return std::unique_ptr<ShapePrototype>(new Circle(*this));;
	}

	std::ostream& show_fields(std::ostream &os) const override
	{
		ShapePrototype::show_fields(os);
		os << " , radius: " << radius;
		return os;
	}

private:
	float radius;
};

// Может хранить различные пресеты 
class ShapesFactory
{
public:

	enum class Type : int
	{
		rectangle,
		circle,
		square,
	};

	ShapesFactory()
	{
		prototypes_[Type::rectangle] = std::unique_ptr<ShapePrototype>(new Rectangle{1.0, 1.0, 100.0, 10.0});
		prototypes_[Type::circle] = std::unique_ptr<ShapePrototype>(new Circle{0.0, -11.0, 50.0});
		prototypes_[Type::square] = std::unique_ptr<ShapePrototype>(new Rectangle(0.0, 1.0, 10.0, 10.0));
	}


	std::unique_ptr<ShapePrototype> create_prototype(Type type)
	{
		return prototypes_.at(type)->clone();
	}


private:
	std::unordered_map<Type,  std::unique_ptr<ShapePrototype>> prototypes_;
};



void client_code()
{
	ShapesFactory shapes_factory;

	auto rectangle1 = shapes_factory.create_prototype(ShapesFactory::Type::rectangle);
	auto circle1 = shapes_factory.create_prototype(ShapesFactory::Type::circle);

	rectangle1->show_fields(cout) << endl;
	circle1->show_fields(cout) << endl;
}


int main()
{
	
	client_code();

	return 0;
}