#include <iostream>
#include <memory>

using namespace std;

struct Base
{
	Base(){cout << "Base()" << endl; }

	// NOTE: without virtual ~Derived wouldn't be called
	//virtual ~Base() { cout << "virtual ~Base()" << endl; }
	~Base() { cout << "~Base()" << endl; }
};

struct Derived: public Base
{
	Derived(){cout << "Derived()" << endl;}
	~Derived(){cout << "~Derived()" << endl;}

	int data;
};


int main()
{
	{
		cout << "Base *bp = new Derived; delete bp;\n";
		Base *bp = new Derived;
		delete bp;
	}
	

	// If T is a derived class of some base B, then std::unique_ptr<T> is 
	// implicitly convertible to std::unique_ptr<B>. The default deleter 
	// of the resulting std::unique_ptr<B> will use operator delete for B, 
	// leading to undefined behavior unless the destructor of B is virtual.
	{
		cout << "\nunique_ptr<Base> dptr (new Derived)\n";
		std::unique_ptr<Base> dptr (new Derived);
	} 

	// NOTE: that std::shared_ptr behaves differently: std::shared_ptr<B> will 
	// use the operator delete for the type T and the owned object will be 
	// deleted correctly even if the destructor of B is not virtual.
	{
		cout << "\nshared_ptr<Base> sptr (new Derived)\n";
		std::shared_ptr<Base> sptr (new Derived);
	}
	
	cout << endl;
	return 0;
}