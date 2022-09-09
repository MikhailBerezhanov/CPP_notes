#include <iostream>
#include <cassert>

// Default Deleter is delete
// Custom Deleter is supported throu the construction.
//

template <typename T>
class shared_ptr
{
public:
	using element_type = T;

	shared_ptr(): pmem(nullptr), use_count(nullptr) {}	// empty shared_ptr - no managed object
	shared_ptr(T *ptr): pmem(ptr), use_count(new size_t{1}) {}
	shared_ptr(const shared_ptr &another) noexcept;

	~shared_ptr();

	shared_ptr& operator= (const shared_ptr &another) noexcept;

	T& operator* () const noexcept { return *pmem; };
	T* operator-> () const noexcept { return pmem; };

	operator bool() const noexcept { return pmem != nullptr; }
	bool is_unique() const noexcept { return *use_count == 1; }

	T* get() const noexcept { return pmem; }
	size_t get_use_count() const noexcept { return *use_count; };

	void reset(T *ptr = nullptr);

private:
	T *pmem = nullptr;
	size_t *use_count = nullptr;
};

template <typename T>
shared_ptr<T>::shared_ptr(const shared_ptr &another) noexcept
{
	pmem = another.pmem;
	use_count = another.use_count;
	if(use_count){
		++(*use_count);
	}
}

template <typename T>
shared_ptr<T>::~shared_ptr()
{
	if( !use_count ){
		return;
	}

	--(*use_count);

	if((*use_count) == 0){

		// if(pmem){
			delete pmem;
			pmem = nullptr;
		// }

		delete use_count;
		use_count = nullptr;
	}
}

template <typename T>
shared_ptr<T>& shared_ptr<T>::operator= (const shared_ptr &another) noexcept
{
	if(this != &another){

		if(use_count){
			--(*use_count);

			if(((*use_count) == 0) /*&& pmem*/){
				delete pmem;
				pmem = nullptr;
			}
		}
		
		pmem = another.pmem;
		use_count = another.use_count; // change pointer to another counter
		if(use_count){
			++(*use_count);
		}
	}

	return *this;
}

template <typename T>
void shared_ptr<T>::reset(T *ptr)
{
	if(use_count){
		--(*use_count);

		if((*use_count) == 0){

			// if(pmem){
				delete pmem;
				pmem = nullptr;
			// }

			delete use_count;
			use_count = nullptr;
		}
	}
	
	// new managed object provided
	pmem = ptr;

	if(pmem){
		use_count = new size_t{1};
	}
}



int main()
{
	struct Point
	{
		Point() = default;
		Point(double x_, double y_, double z_): x(x_), y(y_), z(z_) {}

		double x = -10.10;
		double y = 20.12;
		double z = 43.43;
	};

	// Is allowed, no free will be done
	Point *pp = nullptr;
	delete pp;
	// assert(pp == nullptr);

	shared_ptr<Point> sp{ new Point };
	Point *tmp = new Point{1, 2, 3};
	sp.reset(tmp);

	shared_ptr<Point> sp2{sp};

	shared_ptr<Point> sp3;
	sp3 = sp2;

	std::cout << "x: " << sp3.get()->x << ", y: " << sp3->y << ", z: " << (*sp3).z << std::endl;

	return 0;
}