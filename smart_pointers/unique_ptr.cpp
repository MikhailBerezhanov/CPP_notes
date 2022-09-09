#include <iostream>


// in STL it also has specialization for new[]
// 	template <typename T, typename Deleter = std::default_delete<T>>
//	class unique_ptr;
//
//	template <typename T, typename Deleter>
//	class unique_ptr<T[], Deleter>
//
// It supports custom deleter throu template parameter Deleter
//

template <typename T>
class unique_ptr
{
public:
	unique_ptr() noexcept : pmem(nullptr) {}
	unique_ptr(T *ptr) noexcept : pmem(ptr) {}
	~unique_ptr(){ deallocate(); }

	// Copy contrustor
	unique_ptr(const unique_ptr &rhs) = delete;
	// Copy assignment
	unique_ptr& operator= (const unique_ptr &rhs) = delete;

	// Move constructor
	unique_ptr(unique_ptr &&rhs) noexcept {
		pmem = rhs.pmem;
		rhs.pmem = nullptr;
	}

	// Move assignment
	unique_ptr& operator= (unique_ptr &&rhs);

	/*T*/T& operator* () const noexcept { return *pmem; } 	// *get()
	T* operator-> () const noexcept { return pmem; }	// get()

	operator bool() const noexcept { return pmem != nullptr; }

	T* get() const noexcept { return pmem; }

	T* release() noexcept {
		T *tmp = pmem;
		deallocate();
		return tmp;
	}

	void reset(T *ptr) noexcept {
		deallocate();
		pmem = ptr;
	}

private:
	T *pmem = nullptr;

	void deallocate() noexcept {
		if(pmem){
			delete pmem;
			pmem = nullptr;
		}
	}

};

// Example of definition outside of the class
template <typename T>
unique_ptr<T>& unique_ptr<T>::operator= (unique_ptr<T> &&rhs)
{
	if(this != &rhs){

		deallocate();

		pmem = rhs.pmem;
		rhs.pmem = nullptr;
	}

	return *this;
}






int main()
{
	struct Point
	{
		double x = -10.10;
		double y = 20.12;
		double z = 43.43;
	};

	unique_ptr<Point> up{ new Point };

	unique_ptr<Point> up2;

	up2 = std::move(up);

	std::cout << "x: " << up2.get()->x << ", y: " << up2->y << ", z: " << (*up2).z << std::endl;



	unique_ptr<int> up3{new int{10}};

	// operator*() must return l-value to make this work
	(*up3) += 5; 
	(*up3)++;
	--*up3;

	std::cout << "*up3: " << *up3 << std::endl;

	return 0;
}