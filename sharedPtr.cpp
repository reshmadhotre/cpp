#include <iostream>
#include <memory>
#include <atomic>

template<typename T>
void f(const T& param)
{
    std::cout<<"T = "<<typeid(T).name()<<std::endl;
    std::cout<<"param = "<<typeid(param).name()<<std::endl;
}

namespace ReferenceCount
{   
    using atomic_t = std::atomic<std::size_t>;

    void IncRef(atomic_t *const pRef)
    {
        pRef->fetch_add(std::size_t(1), std::memory_order_seq_cst);
        std::cout << "Incr ref \n"<<std::endl;
    }

    template<typename T>
    void DecRef(atomic_t *const pRef, T *const  pdata)
    {
        if ( (pRef != nullptr) &&
             (std::size_t(1) == pRef->fetch_sub(std::size_t(1), std::memory_order_seq_cst)) )
        {
            std::cout << "decr ref, deleted\n"<<std::endl;
            delete pdata;
            delete pRef;
        }
    }
}

template<typename T>
class sharedPtr
{
    T *pData{nullptr};
    ReferenceCount::atomic_t *pRef{nullptr};

public:
    sharedPtr() noexcept{}

    explicit sharedPtr(T* pD): pData(pD) 
    { 
        reset(pD);
        std::cout<<"Sharedptr Ctor"<<std::endl;
    }

    ~sharedPtr()
    {
        ReferenceCount::DecRef(pRef, pData);
        std::cout<<"Sharedptr Dtor"<<std::endl;
    }

    sharedPtr(const sharedPtr& other)
    {
        std::cout<<"copy Ctor"<<std::endl;
        pData = other.pData;
        pRef = other.pRef;
        ReferenceCount::IncRef(pRef);
    }

    sharedPtr& operator=(const sharedPtr& other)
    {
        std::cout<<"copy assignment"<<std::endl;
        if (this != &other)
        {
            ReferenceCount::DecRef(pRef, pData);
            pData = other.pData;
            pRef = other.pRef;
            ReferenceCount::IncRef(pRef);
        }
        return *this;
    }

    sharedPtr(sharedPtr&& other) noexcept
    {
        std::cout<<"move op"<<std::endl;
        this = std::move(other);
    }

    sharedPtr& operator=(sharedPtr&& other) noexcept
    {
        std::cout<<"move assignment"<<std::endl;
        if (this != &other)
        {
            pData = other.pData;
            pRef = other.pRef;
            pData = nullptr;
            pRef = nullptr;
        }
        return *this;
    }

    void reset(T *const pt ) noexcept
    {
        std::cout<<"reset pt"<<std::endl;
        ReferenceCount::DecRef(pRef, pData);
        pRef = new ReferenceCount::atomic_t(size_t(1));

        pData = pt;
    }

    void reset() noexcept
    {
        ReferenceCount::DecRef(pRef, pData);
        pData = nullptr;
        pRef = nullptr;
    }

    size_t useCount() const noexcept
    {
        return pRef ? pRef->load(std::memory_order_seq_cst) : std::size_t{};
    }

    bool unique() const noexcept
    {
        return std::size_t(1) == useCount();
    }

    T* operator-> () noexcept { return pData; }
    T& operator* () noexcept { return *pData; }
    T* get () noexcept { return pData; }
};

class Resource
{
    int m_value{0};
public:
	std::weak_ptr<Resource> m_ptr {}; // initially created empty

	Resource(int val): m_value(val) { std::cout << "Resource acquired\n"<<std::endl; }
	~Resource() { std::cout << "Resource destroyed\n"<<std::endl; }

    int getValue() const noexcept { return m_value;}
    void setValue(int new_value) noexcept { m_value= new_value;}
};

int main()
{
    {
        sharedPtr<Resource> sptr{new Resource(10)};
        sharedPtr<Resource> sptr1 = sptr;

        sharedPtr<Resource> sptr2;

        sptr2=sptr;
        std::cout<<"sptr2 Pointer value " <<sptr2->getValue()<<std::endl;
        sptr.get()->setValue(20);
        std::cout<<"sptr1 Pointer value " <<sptr1->getValue()<<std::endl;

        std::cout<<"sptr count " <<sptr1.useCount()<<std::endl;
        std::cout<<"sptr unique " <<sptr1.unique()<<std::endl;
    }
	//auto ptr1 { std::make_shared<Resource>() };
	//ptr1->m_ptr = ptr1;

    //int x= 42;
    //const int &i = x;
    //f(i);

	return 0;
}
