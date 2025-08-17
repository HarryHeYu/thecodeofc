//#include <iostream>
//#include <string>
//#include <chrono>
//#include <cmath>
//
//class Timer
//{
//public:
//	Timer(const char* name)
//		: m_Name(name), m_Stopped(false)
//	{
//		m_StartTimePointer = std::chrono::high_resolution_clock::now();
//	};
//
//	void Stop()
//	{
//		auto endTimePoint = std::chrono::high_resolution_clock::now();
//
//		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimePointer).time_since_epoch().count();
//		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();
//
//		std::cout << m_Name << ": " << (end - start) << "ms\n";
//
//		m_Stopped = true;
//	}
//
//	~Timer()
//	{
//		if(!m_Stopped)
//			Stop();
//	}
//
//private:
//	const char* m_Name;
//	std::chrono::time_point< std::chrono::high_resolution_clock> m_StartTimePointer;
//	bool m_Stopped;
//};
//
//void Function1()
//{
//	Timer timer("Functional1");
//
//	for (int i = 0; i < 1000; i++)
//		std::cout << "Hello World #" << i << std::endl;
//}
//
//void Function2()
//{
//	Timer timer("Functional2");
//
//	for (int i = 0; i < 1000; i++)
//		std::cout << "Hello World #" << sqrt(i) << std::endl;
//}
//
//int main()
//{
//	std::cin.get();
//}