#pragma once

template<typename T, size_t S>
class Array
{
public:
	//Array(int size)
	//{
	//	/*m_Data = (int*)alloca(size * sizeof(int));*/

	//}

	constexpr size_t Size() const { return S; }

	T& operator[](size_t index)
	{
		/*if (!(index < S))
		{
			__debugbreak;
		}*/
		return m_Data[index];
	}
	const T& operator[](size_t index) const { return m_Data[index]; }

	T* Data() { return m_Data; }
	const T* Data() const { return m_Data; }
private:
	//int* m_Data;
	T m_Data[S];
};