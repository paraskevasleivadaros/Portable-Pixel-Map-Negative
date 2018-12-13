#pragma once

template <typename  T>
const char * print(T x) {

	if ((std::is_same<T, bool>::value)) {

		return ((x) ? "true" : "false");
	}
	else {

		return x;
	}
}

//#define B2S(b) ((b) ? "true" : "false")
#define printVal(x) {std::cout << "[Variable '" << #x << "' is " << print(x) << "]\n\n";}
//#define printValAndAddr(x) {std::cout << "[Variable '" << #x << "' is " << print(x) << " and it's stored at " << &x << "]\n\n";}