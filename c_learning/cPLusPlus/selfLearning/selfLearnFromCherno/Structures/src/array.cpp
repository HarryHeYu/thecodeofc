//#include <iostream>
//#include <malloc.h>
//
//#include <array>
//
//template<typename T, size_t S>
//class Array
//{
//public:
//	//Array(int size)
//	//{
//	//	/*m_Data = (int*)alloca(size * sizeof(int));*/
//
//	//}
//	
//	constexpr size_t Size() const { return S;  }
//
//	T& operator[](size_t index) 
//	{ 
//		/*if (!(index < S))
//		{
//			__debugbreak;
//		}*/
//		return m_Data[index];
//	}
//	const T& operator[](size_t index) const { return m_Data[index];  }
//	
//	T* Data() { return m_Data; }
//	const T* Data() const { return m_Data; }
//private:
//	//int* m_Data;
//	T m_Data[S];
//};
//int main()
//{
//	int size = 5;
//	
//	Array<int, 5> data;
//
//	memset(data.Data(), 0, data.Size() * sizeof(int));
//
//
//	for (size_t i = 0; i < data.Size(); i++)
//	{
//		std::cout << data[i] << std::endl;
//	}
//
//	static_assert(data.Size() < 10, "Size is too large!\n");
//
//	Array<std::string, data.Size()> newArray;
//
//	const auto& arrayReference = data;
//	for (size_t i = 0; i < data.Size(); i++)
//	{
//		data[i] = 2;
//		std::cout << arrayReference[i] << std::endl;
//	}
//
//	std::cin.get();
//}