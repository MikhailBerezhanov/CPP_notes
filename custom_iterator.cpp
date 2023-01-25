#include <iostream>
#include <iterator> // For std::forward_iterator_tag
#include <cstddef>  // For std::ptrdiff_t
#include <algorithm>

using namespace std;

class Iterator
{
public:
	// 1. Assign the iterator properties
	// Forward Iter - Can scan the container forward multiple times, can read and write the value it points to;
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = int;
	using pointer = value_type*;
	using reference = value_type&;

	// 2. Define the iterator constructors
	// All iterators must be constructible, copy-constructible, copy-assignable, destructible and swappable
	Iterator(pointer ptr): m_ptr(ptr) {}
	// Using default copy-move Ctrs. and operations.

	// 3. Implement operators
	// Dereferenceable:
	reference operator* ()
	{
		return *m_ptr;
	}

	pointer operator-> ()
	{
		return m_ptr;
	}

	// Incrementable:
	// prefix
	Iterator& operator++ ()
	{
		++m_ptr;
		return *this;
	}

	// postfix
	Iterator operator++ (int)
	{
		Iterator ret = *this;
		++(*this);
		return ret;
	}

	// Comparable:
	// friend declaration for the two comparison operators: this is handy way to define the 
	// operators as non-member functions, yet being able to access private parts of the Iterator class
	friend bool operator== (const Iterator& lhs, const Iterator& rhs)
	{
		return lhs.m_ptr == rhs.m_ptr;
	}

	friend bool operator!= (const Iterator& lhs, const Iterator& rhs)
	{
		return !(lhs == rhs);
	}

private:
	pointer m_ptr;
};

class ConstIterator
{
public:
	using iterator_category = std::forward_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = int;
	using pointer = const value_type*;
	using reference = const value_type&;

	ConstIterator(pointer ptr): m_ptr(ptr) {}

	reference operator* ()
	{
		return *m_ptr;
	}

	pointer operator-> ()
	{
		return m_ptr;
	}

	ConstIterator& operator++ ()
	{
		++m_ptr;
		return *this;
	}

	ConstIterator operator++ (int)
	{
		ConstIterator ret = *this;
		++(*this);
		return ret;
	}

	friend bool operator== (const ConstIterator& lhs, const ConstIterator& rhs)
	{	
		return lhs.m_ptr == rhs.m_ptr;
	}

	friend bool operator!= (const ConstIterator& lhs, const ConstIterator& rhs)
	{
		return !(lhs == rhs);
	}

private:
	pointer m_ptr;
};

class Container
{
public:
	using iterator = Iterator;
	using const_iterator = ConstIterator;

	iterator begin()
	{
		return Iterator(&m_data[0]);
	}

	iterator end()
	{
		// refers to an invalid memory address - out of size range
		return Iterator(&m_data[200]);
	}

	const_iterator cbegin()
	{
		return ConstIterator(&m_data[0]);
	}

	const_iterator cend()
	{
		return ConstIterator(&m_data[200]);
	}

private:
	int m_data[200] = {};
};


int main()
{
	Container integers;

	int value = 1;
	cout << "----- begin-end for -----" << endl;
	for (auto it = integers.begin(); it != integers.end(); ++it)
	{
		*it = value++;
	}

	cout << "----- range for: -----" << endl;
	for (auto i : integers)
	{
		cout << i << endl;
	}

	cout << "----- filled: -----" << endl;
	std::fill(integers.begin(), integers.end(), 0);
	for (const auto& i : integers)
	{
		cout << i;
	}
	cout << endl;


	auto it = integers.cbegin();
	*it = 123;	// Error

	return 0;
}