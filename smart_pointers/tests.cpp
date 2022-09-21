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


void weak_ptr_usage()
{
	// weak_ptr constructed from a std::shared_ptr or std::weak_ptr 
	// that will be viewed by this std::weak_ptr.
	// It may be nulltpr ( .use_count == 0 )
 
	std::weak_ptr<string> w_ptr;

	{
		auto ptr = std::make_shared<std::string>("string in heap");
		w_ptr = ptr;
		std::cout << "w_ptr.use_count() inside scope: " << w_ptr.use_count() << '\n';


		// creates a shared_ptr that manages the referenced object.
		// returns expired() ? shared_ptr<T>() : shared_ptr<T>(*this)
		// executed atomically 
		auto s_ptr = w_ptr.lock();	

		if(s_ptr){
			cout << "w_prt.lock returns: " << *s_ptr << endl;
		}

		std::cout << "w_ptr.use_count() inside scope: " << w_ptr.use_count() << '\n';
	}

	std::cout << "w_ptr.use_count() out of scope: " << w_ptr.use_count() << '\n';
	std::cout << "w_ptr.expired() out of scope: " << std::boolalpha << w_ptr.expired() << '\n';

}

void make_smart_pointers()
{
	{
		// Uses two allocations: 
		//   1. for object (double)
		//   2. for Control block 
		std::shared_ptr<double> sptr1(new double); 

		// Advantages:  object is stored independantly of
		// Control block. So when reference_counter will reach 0
		// object will be deleted.
	}
	
	{
		// Uses one allocation. Object is stored in Control block. 
		// So when reference_counter will reach 0, object will not be 
		// deleted if weak_reference_counter is set
		// (i.e. until weak_ptrs exists). 
		//
		// No support for custom_deleter 
		//
		auto sptr2 = std::make_shared<double>();

		std::weak_ptr<double> wptr(sptr2);

		sptr2.reset();
		// object's memory not released, because weak_ptr exists and uses
		// same Control block


		// some code (memory of object is not freed)


	}	// weak_reference_counter decreases -> Control block with object released



}



int main()
{
	smart_pointers_with_ctr_exceptions();

	make_smart_pointers();

	weak_ptr_usage();

	return 0;
}