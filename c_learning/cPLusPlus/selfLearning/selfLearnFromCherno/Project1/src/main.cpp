#include <iostream>
#include "Log.h"

void Pre(int& val) {
	val++;
}

void pre(int* val) {
	*val++;
}

int main() {
	int i = 0;
	for (; i < 5; ) {
		Log("Hello, World!");
		i++;
	}
	int var = 7;
	int* ptr = &var;
	*ptr = 123;
	int val = 72;
	Pre(val);
	pre(&val);
	int a, b;
	int* ref = &a;
	*ref = 15;
	ref = &b;
	*ref = 85;
	log(a);
	log(b);
}