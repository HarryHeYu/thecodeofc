//#include <iostream>
//#include <string>
//
//class Entity
//{
//public:
//	Entity()
//	{
//		std::cout << "Created entity!" << std::endl;
//	}
//
//	~Entity()
//	{
//		std::cout << "Destroyed entity!" << std::endl;
//	}
//};
//
//class ScopePtr
//{
//private:
//	Entity* m_Ptr;
//public:
//	ScopePtr(Entity* ptr)
//		: m_Ptr(ptr)
//	{
//	}
//
//	~ScopePtr()
//	{
//		delete m_Ptr;
//	}
//};
//
//int main()
//{
//	{
//		ScopePtr e = new Entity();
//	}
//
//	std::cin.get();
//}