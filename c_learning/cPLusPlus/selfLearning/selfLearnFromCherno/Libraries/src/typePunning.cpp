//#include <iostream>
//
//struct Entity
//{
//	int x, y;
//
//	int* GetPostions()
//	{
//		return &x;
//	}
//};
//
//int main()
//{
//	Entity e = { 5, 8 };
//
//	int* position = e.GetPostions();
//	position[1] = 2;
//
//	std::cout << e.y << std::endl;
//
//	int a = 50;
//	double& value = *(double*)&a;
//	std::cout << value << std::endl;
//
//	std::cin.get();
//}