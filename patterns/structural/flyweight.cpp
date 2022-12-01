#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

// Легковес — это структурный паттерн проектирования, который позволяет 
// вместить бóльшее количество объектов в отведённую оперативную память. 
// Легковес экономит память, разделяя общее состояние объектов между собой, 
// вместо хранения одинаковых данных в каждом объекте.

// Применимость:
//
// Когда не хватает оперативной памяти для поддержки всех нужных объектов.
//
// Эффективность паттерна Легковес во многом зависит от того, как и где он используется. 
// Применяйте этот паттерн, когда выполнены все перечисленные условия:
// - в приложении используется большое число объектов;
// - из-за этого высоки расходы оперативной памяти;
// - большую часть состояния объектов можно вынести за пределы их классов;
// - большие группы объектов можно заменить относительно небольшим количеством разделяемых 
//   объектов, поскольку внешнее состояние вынесено.


// Этот класс-легковес содержит часть полей, которые описывают
// деревья. Эти поля не уникальны для каждого дерева, в отличие,
// например, от координат: несколько деревьев могут иметь ту же
// текстуру.
//
// Поэтому мы переносим повторяющиеся данные в один-единственный
// объект и ссылаемся на него из множества отдельных деревьев.
class TreeType
{
public:
	TreeType(uint color, const std::string& texture): m_color(color), m_texture(texture) {}

	// Position (x, y) is the contex
	std::string Draw(int x, int y) const
	{
		stringstream ss;

		ss << m_texture << "_" << m_color << "_at(" << x << "," << y << ")";

		return ss.str();
	}

private:
	uint m_color;
	std::string m_texture;	
};

// Фабрика легковесов решает, когда нужно создать новый
// легковес, а когда можно обойтись существующим.
class TreeTypesFactory
{
public:

	shared_ptr<TreeType> CreateTreeType(uint color, const std::string& texture)
	{
		std::string key = texture + std::to_string(color);

		auto it = m_typesCache.find(key);
		if(it != m_typesCache.end()){
			return it->second;
		}

		m_typesCache[key] = std::make_shared<TreeType>(color, texture);
		return m_typesCache.at(key);
	}

private:
	std::unordered_map<std::string, shared_ptr<TreeType>> m_typesCache;
};

// Контекстный объект, из которого мы выделили легковес
// TreeType. В программе могут быть тысячи объектов Tree, так
// как накладные расходы на их хранение совсем небольшие — в
// памяти нужно держать всего три целых числа (две координаты и
// ссылка).
class Tree
{
public:
	Tree(const std::shared_ptr<TreeType>& type, int x, int y):
		m_type(type), m_x(x), m_y(y) {}

	string Draw() const
	{
		return m_type->Draw(m_x, m_y);
	}

private:
	std::shared_ptr<TreeType> m_type;	// shared state

	// unique state
	int m_x = 0;
	int m_y = 0;
};

// Классы Tree и Forest являются клиентами Легковеса. 
class Forest
{
public:

	void AddTree(uint color, const std::string& texture, int x, int y)
	{
		const auto type = m_typesFactory.CreateTreeType(color, texture);

		Tree tree(type, x, y);
		m_trees.push_back(tree);
	}

	void Draw() const
	{
		for(const auto& tree : m_trees)
		{
			cout << tree.Draw() << endl;
		}
	}

private:
	TreeTypesFactory m_typesFactory;
	vector<Tree> m_trees;
};

int main()
{
	Forest forest;

	for(size_t i = 0; i < 10; ++i){
		forest.AddTree(1, "bold", i * 2, i);
	}
	
	for(size_t i = 0; i < 10; ++i){
		forest.AddTree(255, "curve", i, i * 10);
	}

	forest.Draw();

	return 0;
}