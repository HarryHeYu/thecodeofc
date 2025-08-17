#include <iostream>
#include <memory>

struct B;

struct A
{
	std::shared_ptr<B> ptrB;
	~A() { std::cout << "A destroyed\n"; }
};

struct B
{
	/*std::shared_ptr<A> ptrA;*/
	std::weak_ptr<A> ptrA; // 改成weak_ptr
	// 改过来后就可以打破循环使得不会内存泄漏
	~B() { std::cout << "B destroyed\n"; }
};

int main()
{
	std::shared_ptr<A> a = std::make_shared<A>();
	std::shared_ptr<B> b = std::make_shared<B>();

	a->ptrB = b;
	b->ptrA = a;
}