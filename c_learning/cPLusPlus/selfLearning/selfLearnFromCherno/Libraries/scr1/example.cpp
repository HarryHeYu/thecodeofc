//#include <iostream>
//
//class Base
//{
//public:
//	virtual void show() { std::cout << "Base" << std::endl; }
//};
//
//class Derived : public Base
//{
//public:
//	void show() override { std::cout << "Derived" << std::endl; }
//};
//
//int main()
//{
//	double d = 3.14;
//	int i = (int)d;
//	
//	int j = static_cast<int>(d);
//	std::cout << j << std::endl;
//
//	int* p = reinterpret_cast<int*>(&d);
//	std::cout << *p << std::endl;
//
//	const int ci = 42;
//	int* pci = const_cast<int*>(&ci);
//	*pci = 99;
//	std::cout << ci << " " << *pci << std::endl;
//
//	Base* b = new Derived();
//	Derived* dptr = dynamic_cast<Derived*>(b);
//	if (dptr) dptr->show();
//
//	Base* b2 = new Base();
//	Derived* dptr2 = dynamic_cast<Derived*>(b2);
//	std::cout << (dptr2 == nullptr) << std::endl;
//
//}