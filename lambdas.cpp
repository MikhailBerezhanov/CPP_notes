#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <memory>

using namespace std;

//
// Lambda usage warnings and features
//

void how_lambdas_work()
{
	int ival = 1;
	double dval = 10.123;

	// Lambda expression generates 
	// closure-class at the COMPILE time.
	//
	// - local_i captured by reference
	// - dval captured by value (copied)
	auto func = [&ival, dval](bool param) mutable -> bool {
		
		// change external ival
		++ival;

		// change internal dval.
		--dval;

		return dval > 10.0;
	};

	func(false);

	// dval here is 10.123 as was
	cout << "After lambda call ival: " << ival << ", dval: " << dval << endl;


	// --- Equivalent (generated by compiler )--- 

	// Generated Closure Class may look like
	// (Класс замыкания)
	class GeneratedClosureClass
	{
	public:
		GeneratedClosureClass(int *pvar, double dvar): 
			captured_by_ref_i(pvar), captured_by_val_d(dvar) {}

		// Method is const by default. Use mutable  
		// to make captured members changable.
		bool operator() (bool param) const
		{
			// pointer not changed,
			// only data it points at is changed.
			++(*captured_by_ref_i); 	

			// changing member variable
			--captured_by_val_d;

			return captured_by_val_d > 10.0;
		}

	private:
		// Captured list goes here
		/* mutable */ int *captured_by_ref_i = nullptr;
		mutable double captured_by_val_d = 0.0;
	};


	// At RUN-time closure object is created
	// (Замыкание)  
	// NOTE: it takes the address of local variable ival .
	GeneratedClosureClass closure(&ival, dval);
	auto func_ = closure;	// what happenes indeed while lambda expression

	func_(false);
}


vector<function<double(double)>> filters_vec;

// 1. Be careful or avoid default capture !
void default_capture_is_dangerous()
{
	int divisor = 10;

	// Default Capture by reference
	auto filter1 = [&](double param){
		return param / divisor;
	};

	// 1.1
	// Bad idea - divisor is captured by reference 
	// and divisor will be poped out of stack when 
	// function returns. Dangling reference (Висячая ссылка)
	filters_vec.emplace_back( filter1 );
	// Capture by value (copying) wiil fix this example, 
	// bit is not a panacea. 


	// 1.2 Careful With global variables 
	static int sdivisor = 100;
	// WARN: no capture take place, sdivisor is global variable 
	// in current scope - it is used directly in closure,
	auto filter2 = [=](double param) {
		// NOTE: no mutable needed inspite of sdivisor changing
		sdivisor += 10;	// changes not internal value, but external (static)
		return param / sdivisor;
	};


	// 1.3 Careful with class members 
	struct Widget
	{

		void add_filter()
		{
			// Default Capture by value
			auto filter = [=](double param){
				return param / m_divisor;
			};

			// What happens indeed:
			/*
			auto filter = [this](double param){
				return param / this->m_divisor;
			};
			*/

			// filter closure contains copy of pointer 
			// at current object (this). When object 
			// will go out of scope, closure will
			// contain digling pointer!
			filters_vec.emplace_back( filter );
		}

		double m_divisor = 5;
	};

}

// 2. Since C++14 there is generalized lambda capture
// (обобщенный захват лямбда-выражения или инициализирующий захват)  
void use_initializing_capture()
{
	struct Point
	{
		double x = 0.0;
		double y = 0.0;
	};

	// What if we want to capture non-copyable object 
	// or capture it by rvalue reference (move) ?
	// C++11 has no special mechanism for that purpose.
	// But C++14 has.

	std::unique_ptr<Point> upoint(new Point);

	// error non-copyable
	/* auto func = [upoint](){ cout << upoint->x << upoint->y << endl; }; */
	// ok 
	auto func2 = [&upoint](){ cout << upoint->x << upoint->y << endl; };

	// C++14:
	// better
	auto func3 = [up = std::move(upoint)](){ cout << up->x << up->y << endl; };
	// best
	auto func4 = [up = std::make_unique<Point>()](){ cout << up->x << up->y << endl; };

	vector<string> heavy_vec;
	auto func5 = [vec = std::move(heavy_vec)](){  };
}

// 3. For perfect forwarding inside lamdas use decltype().
//
// x - lvalue  => decltype(x) - lvalue
// x - rvalue  => decltype(x) - rvalue ref 
//
vector<string> str_vec;

void perfect_forwarding_with_lambdas()
{
	// universal reference approach
	auto lambda = [](auto&&... params){

		str_vec.emplace_back( std::forward< decltype(params) >(params)... );

	};

	lambda(15, 'a');
	lambda("literal");

	string s = "mik";
	lambda(s);
	lambda(std::move(s));

	lambda();	// empty string
}


int main()
{
	how_lambdas_work();

	default_capture_is_dangerous();

	use_initializing_capture();

	perfect_forwarding_with_lambdas();
	for(const auto &str : str_vec){
		cout << str << endl;
	}


	return 0;
}