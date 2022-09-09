#include <iostream>
#include <memory>
#include <vector>

using namespace std;

#if 0
void auto_ptr_test()
{
	std::auto_ptr<int> a(new int{111});
	std::auto_ptr<int> b;
	b = a;	// distructing copying (a is released inside)

	if(a.get() != nullptr){
		cout << "*a = " << *a << endl;
	}

	if(b.get() != nullptr){
		cout << "*b = " << *b << endl;
	}

	// Do not works with STL container because of destroying copying
//	vector<auto_ptr<int>> apvec;
//	apvec.push_back(new int{123});

	vector<unique_ptr<int>> upvec;
	upvec.push_back(std::move(unique_ptr<int>(new int{123})));
}
#endif

class TestClass
{
public:
	TestClass() { cout << "TestClass::ctr" << endl; }
	~TestClass(){ cout << "TestClass::dtr" << endl; }
};

class TestClassThrowing
{
public:
	TestClassThrowing() 
	{ 
		cout << "TestClassThrowing::ctr" << endl;
		throw std::runtime_error("TestClassThrowing ctr fails"); 
	}

	// NOTE: desctructor will not be called, if constructor throws
	// (Object was not created fully - nothing to destruct)
	~TestClassThrowing(){ cout << "TestClassThrowing::dtr" << endl; }
};

class A
{
public:
	A() 
	{
		cout << "A::ctr" << endl;

		uptr1 = std::unique_ptr<TestClass>(new TestClass);
		uptr2 = std::unique_ptr<TestClassThrowing>(new TestClassThrowing);

		// TestClass::dtr will free memory of uptr1 correctly

		ptr1 = new TestClass; // OK
		ptr2 = new TestClassThrowing; // cause memory leak of ptr1
	}

	~A()
	{
		// Will not be called
		cout << "A::dtr" << endl;
		delete ptr1;
		delete ptr2;
	}

private:
	std::unique_ptr<TestClass> uptr1;
	std::unique_ptr<TestClassThrowing> uptr2;

	TestClass *ptr1 = nullptr;
	TestClassThrowing *ptr2 = nullptr;
};

void smart_pointers_with_ctr_exceptions()
{
	try{
		A a;	
	}
	catch(const std::exception &e){
		cerr << "A creation exception: " << e.what() << endl;
	}
}



int main()
{
	smart_pointers_with_ctr_exceptions();


	return 0;
}