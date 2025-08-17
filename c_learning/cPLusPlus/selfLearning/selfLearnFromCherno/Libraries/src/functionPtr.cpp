//#include <iostream>
//#include <vector>
//#include <functional>
//#include <algorithm>
//
//void HelloWorld(int a)
//{
//	std::cout << "Hello World! Value:" << a << std::endl;
//}
//
//void ForEach(const std::vector<int>& values, const std::function<void(int)>& func)
//{
//	for (int value : values)
//		func(value);
//}
//int main()
//{
//	typedef void(*function)(int);
//
//	function fu = HelloWorld;
//
//	fu(3);
//
//	std::vector<int> values = { 1,2,3,4,5 };
//
//	auto it = std::find_if(values.begin(), values.end(), [](int value){ return value > 3; });
//	std::cout << *it << std::endl;
//
//	ForEach(values, [](int value) {std::cout << "Value:" << value << std::endl; });
//
//	int a = 3;
//	auto lamda = [=](int value)
//		{
//			std::cout << "Value:" << a << std::endl;
//		};
//	ForEach(values, lamda);
//
//
//	std::cin.get();
//}