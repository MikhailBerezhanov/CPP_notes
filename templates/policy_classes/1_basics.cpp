#include "1_creator.hpp"

#include <iostream>

using namespace std;

template <typename T>
struct SmartPtr
{
    explicit SmartPtr(T )
    {
        cout << "SmartPtr<T>" << endl;
    }

    T MemFun()
    {
        return T{};
    }
};

// Full specialization (explicit)
template<>
struct SmartPtr<char>
{
    explicit SmartPtr(char )
    {
        cout << "SmartPtr<char>" << endl;
    }

    char MemFun()
    {
        return 'A';
    }
};

// With explicit specialization Member function can also be specialized:
template<>
double SmartPtr<double>::MemFun()
{
    return -11.003;
}


template <typename T, typename U>
struct DualSmartPtr
{
    DualSmartPtr(T , U )
    {
        cout << "DualSmartPtr<T, U>" << endl;
    }

    void MemFun(T , U)
    {
        //...
    }
};

// Partial specialization
template <typename U>
struct DualSmartPtr<char, U>
{
    DualSmartPtr(char , U )
    {
        cout << "DualSmartPtr<char, U>" << endl;
    }
};

template <typename T>
struct DualSmartPtr<T, char>
{
    DualSmartPtr(T , char )
    {
        cout << "DualSmartPtr<T, char>" << endl;
    }
};

// NOTE: With partial specialization Member function cannot be specialized
// template<typename U>
// void DualSmartPtr<double, U>::MemFun(double, U)
// {
//     // ERROR
// }

int main()
{
    SmartPtr iptr(10); // SmartPtr<int>
    cout << "iptr.MemFun(): " << iptr.MemFun() << endl;

    SmartPtr dptr(-0.12); // SmartPtr<double>
    cout << "dptr.MemFun(): " << dptr.MemFun() << endl;

    SmartPtr cptr('c');
    cout << "cptr.MemFun(): " << cptr.MemFun() << endl;


    DualSmartPtr iiptr(10, 10);

    DualSmartPtr ciptr('a', 10);
    DualSmartPtr icptr(-10, 'b');


    auto pmem = MallocCreator<double>::Create();
    cout << "MallocCreator<double>::Create(): " << (pmem ? std::to_string(*pmem) : "nullptr") << endl;
    delete pmem;

    return 0;
}