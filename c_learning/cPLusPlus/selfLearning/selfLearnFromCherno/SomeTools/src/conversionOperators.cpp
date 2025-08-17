//#include <iostream>
//#include <memory>
//
//#include <chrono>
//
//struct Timer
//{
//	using Clock = std::chrono::high_resolution_clock;
//	using TimePoint = std::chrono::time_point<Clock>;
//
//	void Start() { m_Start = Clock::now(); }
//	void Stop() { m_Stop = Clock::now(); }
//
//	double GetSeconds() const
//	{
//		using namespace std::chrono;
//		return duration_cast<microseconds>(m_Stop - m_Start).count() * 0.001f * 0.001f;
//	}
//
//	double GetMilliseconds() const
//	{
//		using namespace std::chrono;
//		return duration_cast<microseconds>(m_Stop - m_Start).count() * 0.001f;
//	}
//
//	operator double() const { return GetSeconds(); }
//private:
//	TimePoint m_Start, m_Stop;
//};
//
//template<typename T>
//class ScopedPtr
//{
//public:
//	ScopedPtr() = default;
//	ScopedPtr(T* ptr) : m_Ptr(ptr) {}
//	~ScopedPtr() { delete m_Ptr; }
//
//	bool IsValid() const { return m_Ptr != nullptr; }
//
//	T* Get() { return m_Ptr; }
//	const T* Get() const { return m_Ptr; }
//
//	operator bool() const { return IsValid(); }
//private:
//	T* m_Ptr = nullptr;
//};
//
//struct Entity
//{
//	float X = 0.0f, Y = 0.0f;
//};
//
//void ProcessEntity(const ScopedPtr<Entity>& e)
//{
//	if (e) // 在加了operator()的重载之后就可以了
//	{
//		// ...
//	}
//}
//
//int main()
//{
//	ScopedPtr<Entity> e = new Entity;
//
//	Timer timer;
//	timer.Start();
//	timer.Stop();
//	timer.GetSeconds();
//
//	Timer timer2;
//	timer2.Start();
//	timer2.Stop();
//
//	// double time = timer.GetSeconds();->
//	double time = timer.GetSeconds() + timer2.GetSeconds();
//	//虽然多了个GetSeconds()看起来更麻烦，但是可读性提高了
//
//	
//	std::cin.get();
//}