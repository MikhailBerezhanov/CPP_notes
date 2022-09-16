#include <iostream>
#include <memory>

using namespace std;

// "Виртуальный" вызов без virtual

template<typename Derived>
class Base
{
public:
	void virtual_func1()
	{
		static_cast<Derived*>(this)->virtual_func1();
	}

};

class D : public Base< D >
{
public:
	void virtual_func1()
	{
		cout << "D::virtual_func1()" << endl;
	}

};

int main()
{
	unique_ptr<Base<D>> ubp(new D);

	ubp->virtual_func1();

	// NOTE: size of objects of class with virtual func increases by vptr 
	// (vtable pointer) and such call is slower becasu of pointer dereference 
	// and offset to specific function address in vtable

	// Whenever a program has a virtual function declared, a v - table is 
	// constructed for the class. The v-table consists of addresses to the virtual 
	// functions for classes that contain one or more virtual functions. The object 
	// of the class containing the virtual function contains a virtual pointer that 
	// points to the base address of the virtual table in memory. Whenever there is 
	// a virtual function call, the v-table is used to resolve to the function address. 
	// An object of the class that contains one or more virtual functions contains a 
	// virtual pointer called the vptr at the very beginning of the object in the memory. 
	// Hence the size of the object in this case increases by the size of the pointer. 
	// This vptr contains the base address of the virtual table in memory. Note that 
	// virtual tables are class specific, i.e., there is only one virtual table for 
	// a class irrespective of the number of virtual functions it contains. 
	// This virtual table in turn contains the base addresses of one or more virtual 
	// functions of the class. At the time when a virtual function is called on an object, 
	// the vptr of that object provides the base address of the virtual table for that class 
	// in memory. This table is used to resolve the function call as it contains the addresses 
	// of all the virtual functions of that class. This is how dynamic binding is resolved 
	// during a virtual function call.

	// + No vptr ,
	// + No vptr dereference while calling 
	cout << "Base size: " << sizeof(Base<D>) << ", D size: " << sizeof(D) << endl;

	return 0;
}