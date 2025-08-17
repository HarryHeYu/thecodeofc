#include <iostream>
#include <print>
#include <expected>

//template<>
//struct DivideReturnResult
//{
//	union{ 
//		int Result;
//		std::string A;
//	}
//
//	bool HasValue;
//
//	operator bool() const { return A; }
//	int operator*() const { return Result; }
//};
//
//DivideReturnResult Divide(int a, int b)
//{
//	if (b == 0)
//		return{ 0, false };
//
//	return { a / b, true };
//}

std::expected<int, std::string> Divide(int a, int b)
{
	if (b == 0)
		return std::unexpected("Devided by 0 error!");

	return a / b;
}

int main()
{
	auto result = Divide(12, 3)
		.and_then([](int result) {return Divide(result, 2); })
		.or_else([](const std::string& error) 
			{ 
				std::println("Error: {}", error);
				return std::expected<int, std::string>{0};
			});

	if (result)
	{
		std::println("Result = {}", *result);
	}

}