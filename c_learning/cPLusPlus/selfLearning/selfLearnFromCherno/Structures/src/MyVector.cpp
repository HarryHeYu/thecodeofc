//#include <iostream>
//
//template<typename T>
//class Vector
//{
//public:
//	Vector()
//		: m_Data(nullptr), m_Size(0), m_Capacity(0)
//	{ }
//
//	~Vector()
//	{
//		delete[] m_Data;
//	}
//
//	size_t Size() { return m_Size; }
//
//	T& operator[](size_t index)
//	{
//		return m_Data[index];
//	}
//
//	const T& operator[](size_t index) const { return m_Data[index]; }
//
//	void push_back(T& value)
//	{
//		if (m_Size >= m _Capacity && m_Capacity != 0)
//			m_Capacity *= 2;
//		else if (m_Size >= m _Capacity && m_Capacity == 0)
//			m_Capacity = 1;
//		
//		m_Data[m_Size++] = value;
//	}
//private:
//	T* m_Data;
//	size_t m_Size;
//	size_t m_Capacity;
//};
//
//int main()
//{
//	std::cin.get();
//}