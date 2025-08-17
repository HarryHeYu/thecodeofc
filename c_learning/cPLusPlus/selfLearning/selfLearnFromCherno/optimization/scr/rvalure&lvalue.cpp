//#include <iostream>
//
//void SetValue(const int& value)
//{
//}
//
//void PrintName(const std::string& name)
//{
//	std::cout << name << std::endl;
//}
//
//void PrintName2(std::string& name)
//{
//	std::cout << name << std::endl;
//}
//
//void PrintName1(std::string&& name)
//{
//	std::cout << name << std::endl;
//}
///*当然也可以直接写一样的函数名，会直接重载*/
//int main()
//{
//	int i = 10;
//	SetValue(i);
//	SetValue(10);
//
//	std::string firstName = "Harry";
//	std::string lastName = "He";
//
//	std::string fullName = firstName + lastName;
//
//	PrintName2(fullName);
//	PrintName2(firstName + lastName);
//	PrintName(fullName);
//	PrintName(firstName + lastName);
//	PrintName1(fullName);
//	PrintName(firstName + lastName);
//}