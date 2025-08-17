//#include <iostream>
//#include <vector>
//
//static int s_AllocationCount = 0;
//static int s_CopyCount = 0;
//static int s_MoveCount = 0;
//
//void* operator new(size_t size)
//{
//	std::cout << "Allocated " << size << " bytes\n";
//	s_AllocationCount++;
//	return malloc(size);
//}
//
//struct Data
//{
//	int Value = 0;
//
//	Data() = default;
//	Data(int value)
//		: Value(value) { }
//
//	Data(const Data& other)
//		: Value(other.Value)
//	{
//		s_CopyCount++;
//		std::cout << "Copied Data\n";
//	}
//
//	Data(Data&& other)
//		: Value(other.Value)
//	{
//		s_MoveCount++;
//		std::cout << "Moved Data\n";
//	}
//};
//
//static void PrintVector(const std::vector<Data>& vector)
//{
//	std::cout << "Size: " << vector.size() << std::endl;
//	if (vector.empty())
//		return;
//
//	std::cout << "Elements: {";
//	for (int i = 0; i < vector.size(); i++)
//	{
//		std::cout << vector[i].Value;
//		if (i < vector.size() - 1)
//			std::cout << ", ";
//	}
//	std::cout << " }";
//} 
//
//int main()
//{
//	// 当然在这里使用array会使得效率更高
//
//	std::vector<Data> vector;
//	/*const std::vector<Data>& vector2 = vector;*/
//	vector.reserve(5);
//	//vector.resize(5);
//
//	//reserve(5)只是预留空间，不会增加元素数量
//	for (int i = 0; i < 5; i++)
//		vector.emplace_back(i);
//		/*vector.push_back(Data(i));*/
//		/*vector.emplace_back(i);
//		可以不复制直接创建*/
//
//	/*
//	对应的是resize，因为用默认构造函数
//	构造n个元素，后面的for循环相当于是再插入5个元素
//	for (int i = 0; i < 5; i++)
//	vector.push_back(Data(i));*/
//	
//	/*
//	vector(5)和resize(5)都会让vector变成5个元素，都是用默认构造函数构造
//	for循环只是给已有的5个元素赋新值，不会改变元素数量，也不会分配新内存
//	for (int i = 0; i < 5; i++)
//		vector[i] = Data(i);
//		*/
//
//
//	PrintVector(vector);
//
//	std::cout << "\n"  << s_AllocationCount << " allocations\n";
//	std::cout << s_CopyCount << " copies\n";
//	std::cout << s_MoveCount << " moves\n";
//
//	std::cin.get();
//}