#include <iostream>

using namespace std;

#define SHOW(var)	( cout << #var << ": " << var << endl )

int main()
{
	char ca = -129;
	SHOW(static_cast<int>(ca));
	
	short sa = 1000;
	// Promotion
	int ib = sa - 90;	// int ib = static_cast<int>(sa) - 90 ;
	SHOW(ib);

	int ia = 1000;
	char cb = ia - 90;	// char cb = static_cast<char>(ia - 90)
	SHOW(static_cast<int>(cb));	// char(910) 910 % 256    
	// 256 * 3 = 768, 910 - 768 = 142 > 127. 
	// char(128) == -128
	// -128 + (142 - 128) = -114

	// smaller (char) cast to larger (int)
	char cb2 = ia - ca; // char cb2 = static_cast<char>(ia - static_cast<int>(a))



	float fu = 2.74356F;	// 2.74356f -  float 
							// 2.74356  -  double

	// Narrowing conversion because the fractional portion of the 
	// floating point value is discarded and lost 
	ia = fu;
	SHOW(ia);


	// signed interegers implicitly converts to unsigned if met in the same expresssion
	unsigned char uca = ca;	// uca = static_cast<unsigned_char>(ca);
	SHOW(uca);

	unsigned char ucb = ca + cb; 
	// ucb = static_cast<unsigned char>( static_cast<int>(ca) + static_cast<int>(cb) )
	// BUT
	unsigned char ucc = 4U + ca + cb;
	// ucc = static_cast<unsigned char>( 4U + static_cast<uint>(ca) + static_cast<uint>(cb) )

	auto d = ucb + ucc;
	// chars, unsigned chars, shorts, unsigned short casts to int 
	// when no unsigned int operand met
	// int d = static_cast<int>(ucb) + static_cast<int>(ucc);

	// Integers and floating point values in the same expression:
	int ival = 10;
	unsigned long ulval = 4;
	double dval = ival * ulval;
	// dval = static_cast<double>( static_cast<unsigned long>(ival) * ulval )

	float fval = 4.3f;
	dval = ulval + fval;
	// dval = static_cast<double>( static_cast<float>(ulval) + fval);

	return 0;
}