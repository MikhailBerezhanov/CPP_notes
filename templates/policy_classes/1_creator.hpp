#include <cstdlib>

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
// policy realization

template <class T>
struct OpNewCreator
{
    static T* Create()
    {
        return new T{};
    }
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