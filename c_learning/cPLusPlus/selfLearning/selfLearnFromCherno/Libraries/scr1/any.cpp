//#include <iostream>
//#include <any>
//#include <variant>
//
//int main()
//{
//	std::any data;
//	data = 2;
//	data = "Harry";
//	data = std::string("Harry");
//
//	std::string string = std::any_cast<std::string>(data);
//	std::string& stri = std::any_cast<std::string&>(data);
//
//	std::variant<int, std::string> data1;
//	data1 = 1;
//	data1 = "Harry";
//	std::string str = std::get<std::string>(data1);
//	std::cout << str << std::endl;
//
//	std::cin.get();
//}