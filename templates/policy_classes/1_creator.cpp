#include <cstdlib>
#include <iostream>

using namespace std;

// Creator policy class interface:
template <class T>
struct ICreationPolicy
{
    // NOTE: not neccessary be static, or return 
    // new onject. Can be member function also 
    // Can return nullptr, or throw.
    // Interface is not strict - it depends on usage.
    static T* Create()
    {
        // ...
        return nullptr;
    }
};

// The implementation can be chosen while
// policy realization (policy should have no data members)

template <class T>
struct OpNewCreator
{
    static T* Create()
    {
        return new T{};
    }
protected:
    // Prevents desctuction with delete() through base class pointer.
    // Destructor can be called only by derived classes.
    ~OpNewCreator() = default;
};

template <class T>
struct MallocCreator
{
    static T* Create()
    {
        void* buf = std::malloc(sizeof(T));
        if (!buf)
        {
            return nullptr;
        }
        // placement new
        return new(buf) T{};
    }  
};

template <class T>
struct PrototypeCreator
{
    PrototypeCreator(T* obj = nullptr)
        : m_prototype(obj)
    {

    }

    T* Create()
    {
        return m_prototype ? m_prototype->Clone() : nullptr;
    }

    T* GetPrototype() { return m_prototype; }
    void SetPrototype(T* obj) { m_prototype = obj; }

private:
    T* m_prototype;
};


// Client code (host class):
template <class CreationPolicy>
class WidgetManager : public CreationPolicy
{
    //... use CreationPolicy::Create() method
};

typedef WidgetManager < OpNewCreator<double> > MyWidgetMgr;

// OR using template template parameters:
// (`CreatedObject` can be omitted, cuz WidgetManager2 class has no access to it)
template <template <class CreatedObject> class CreationPolicy>
class WidgetManager2 : public CreationPolicy<double>
{
    // NOTE: can use creation policity for any type inside
    void DoSomething()
    {
        char* pc = CreationPolicy<char>{}.Create(); // construct inplace
    }
};

typedef WidgetManager2 <OpNewCreator> MyWidgetMgr2;

// template template param can be aswell defaulted:
template <template <class> class CreationPolicy = OpNewCreator>
class WidgetManager3 : public CreationPolicy<double>
{

};



int main()
{   
    auto pmem1 = MallocCreator<double>::Create();
    cout << "MallocCreator<double>::Create(): " << (pmem1 ? std::to_string(*pmem1) : "nullptr") << endl;
    free(pmem1);

    auto pmem2 = OpNewCreator<char>::Create();
    cout << "OpNewCreator<char>{}.Create(): " << (pmem2 ? std::to_string(*pmem2) : "nullptr") << endl;
    delete pmem2;

    WidgetManager3<OpNewCreator>* wm3 = new WidgetManager3;  // use default policy 

    // NOTE: care about desctuction safety!
    //      OpNewCreator is a base class , but has no virtual destructor by design.
    //      As it is public inheritance, deleting through base class pointer is allowed.
    //      That leeds to UB.
    OpNewCreator<double>* pCreator = wm3;
    delete pCreator;    // ERROR - destructor is not public
    //
    // Solution is to use protected non-virtual desctructors in policies.
    //
    delete wm3;         // OK - desctruction through derived class
    

    return 0;
}