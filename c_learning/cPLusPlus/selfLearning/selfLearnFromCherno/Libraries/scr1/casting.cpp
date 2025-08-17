//#include <iostream>
//
//class Base
//{
//public:
//	Base() { std::cout << "Base Constructor\n"; }
//	virtual~Base() { std::cout << "Base Destructor\n"; }
//};
//
//class Derived : public Base
//{
//public:
//	Derived() { std::cout << "Derived Constructor\n"; }
//	~Derived() { std::cout << "Derived Destructor\n"; }
//};
//
//class AnotherClass : public Base
//{
//public:
//	AnotherClass() {}
//	~AnotherClass() {}
//};
//
//int main()
//{
//	double value = 5.25;
//	double a = (int)value + 5.3;
//
//	Derived* derived = new Derived();
//
//	Base* base = derived;
//
//	AnotherClass* ac = dynamic_cast<AnotherClass*>(base);
//
//	if (!ac)
//	{
//		std::cout << "Null! " << std::endl;
//	}
//
//	std::cin.get();
//}