#include <iostream>
#include <vector>
#include <limits>
#include <array>

using namespace std;

// 1 9 2 7 5 7 2 1 9 -> 5
int find_lonely_elem(const vector<int> &vec)
{
	int res = 0;

	// XOR of each array elements
	for(const auto &elem : vec){
		res ^= elem;
	}

	return res;
}

// 0x8C      -> 0x31
// 1000 1100 -> 0011 0001
uint8_t reverse(uint8_t in)
{
	uint8_t res = 0;

	for(int i = 7, j = 0; i >= 0; --i, ++j){

#if 0
		uint8_t tmp = (in & (1 << i));

		// half of num size
		if(i > 3){
			// 1 0001100 -> 1000 0000 -> 0000 0001
			//  0 001100 -> 0  
			//   0 01100 -> 0
			//    0 1100 -> 0
			tmp >>= i - j;
		}
		else{
			// 1 000 -> 0000 1000 -> 0001 0000 (<< 1) j = 4, i = 3
			//  1 00 -> 0000 0100 -> 0010 0000 (<< 3) j = 5, i = 2
			//   1 0 -> 0000 0010 -> 0100 0000 (<< 4) j = 6, i = 1
			//    1  -> 0000 0001 -> 1000 0000 (<< 7) j = 7, i = 0
			tmp <<= j - i;
		}

		// printf("tmp: 0x%02X\n", tmp);

		res |= tmp; 
#else
		// (in & (1 << i)) 				- select a bit using mask (1 << i = 0b 00i0 0000)
		// (in & (1 << i)) >> i 		- move selected bit at the position 0
		// ((in & (1 << i)) >> i) << j	- shift selected bit at the reversed pos (7->0, 3->4, 0->7, etc)
		res |= ((in & (1 << i)) >> i) << j;
#endif		
 
	}

	return res;
}

void num_test()
{
	cout << "max uint: " << std::numeric_limits<uint>::max() << endl;

	cout << "inverted 7 as int: " << ~7 << endl;
	cout << "-8 as uint: " << uint(-8) << endl;	
	// int -8 = invert_bits(8 - 1) = invert_bits(0b 0000 0000 0000 0111) =
	// = 0b 1111 1111 1111 1000 = 0xFFF8 = 0xFFFF - 0x7
	// uint(0xFFFF) = 4 294 967 295	(max uint)
	// uint(-8) = 4 294 967 295 - 7 = 4 294 967 296 - 8

	cout << "4u - 8: " <<4u - 8 << endl;  	
	// int -8 will be casted to uint, bacause 4u is uint 4
	// -8 -> uint(-8) = 4 294 967 296 - 8
	// 4u - 8 = 4 + 4 294 967 296 - 8 = 4 294 967 296 - 4
}

void arrays_test()
{
	int arr[]{1, 2, 3, 4, 5};

	// arr[i] == *(arr + i)
	// i[arr] == *(i + arr)

	//        arr[1]   +  arr[2]  == 2 + 3 
	cout << "c array: "<< 2[arr - 1] + (arr - 3)[5] << endl;

	std::array<int, 5> arr2 {1, 2, 3, 4, 5};
	// Error
	//cout << "std::array: "<< 2[arr2 - 1] + (arr2 - 3)[5] << endl;
}


int main()
{
	// cout << (1 ^ 9 ^ 9 ^ 1 ^ 5) << endl; // = 5

	arrays_test();

	num_test();

	vector<int> vec{1, 9, 2, 7, 5, 7, 2, 1, 9};

	cout << "lonely elem is: " << find_lonely_elem(vec) << endl;

	printf("reversed 0x8C: 0x%02X\n", reverse(0x8C));
	printf("reversed 0x31: 0x%02X\n", reverse(0x31));
	printf("reversed 0x01: 0x%02X\n", reverse(0x01));
	printf("reversed 0xA5: 0x%02X\n", reverse(0xA5));
	printf("reversed 0x03: 0x%02X\n", reverse(0x03));
	printf("reversed 0xFF: 0x%02X\n", reverse(0xFF));
	printf("reversed 0x0F: 0x%02X\n", reverse(0x0F));
	printf("reversed 0xF0: 0x%02X\n", reverse(0xF0));

	return 0;
}