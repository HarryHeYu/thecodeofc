//#include <iostream>
//#include <vector>
//#include "Vector.h"//直接在Vector.h中进行的编写
//
//int main()
//{
//#if 0
//	std::vector<int> values = { 1, 2, 3, 4, 5, 6 };
//
//	for (int i = 0; i < values.size(); i < i++)
//		std::cout << values[i] << std::endl;
//
//	for (int value : values)
//		std::cout << value << std::endl;
//
//	for (std::vector<int>::iterator it = values.begin(); it != values.end(); it++)
//		std::cout << *it << std::endl;
//#endif
//
//	//Vector<int> values;
//	//values.EmplaceBack(1);
//	//values.EmplaceBack(2);
//	//values.EmplaceBack(3);
//	//values.EmplaceBack(4);
//	//values.EmplaceBack(5);
//	Vector<std::string> values;
//	values.EmplaceBack("1");
//	values.EmplaceBack("2");
//	values.EmplaceBack("3");
//	values.EmplaceBack("4");
//	values.EmplaceBack("5");
//	
//	std::cout << "Not using iterators:\n";
//	for (int i = 0; i < values.Size(); i++)
//	{
//		std::cout << values[i] << std::endl;
//	}
//
//	std::cout << "Range_base for loop:\n";
//	for (std::string value : values)
//		std::cout << value << std::endl;
//
//	std::cout << "Iterator:\n";
//	for (Vector<std::string>::Iterator it = values.begin();
//		it != values.end(); it++)
//	{
//		std::cout << *it << std::endl;
//	}
//
//
//
//
//	std::cin.get();
//}