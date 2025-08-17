//#include <iostream>
//#include <print>
//
//#include <format>
//
//struct Point
//{
//	int x, y;
//};
//
//template<>
//struct std::formatter<Point> : std::formatter<std::string>
//{
//	auto format(const Point& p, format_context& ctx) const
//	{
//		return std::formatter<std::string>::format(std::format("{}, {}", p.x, p.y), ctx);
//	}
//};
//
//int main()
//{
//	/*std::string string = std::format("Speed is {}, mass is {}, name is {}",
//		5.5f, 40, "Harry");
//
//	std::cout << string << std::endl;*/
//	// ---->
//
//	std::println("Speed is {}, mass is {}, name is {}",
//		5.5f, 40, "Harry");
//
//	std::println("Please");
//	std::println("enter");
//	std::print("pass");
//	std::print("word");
//}