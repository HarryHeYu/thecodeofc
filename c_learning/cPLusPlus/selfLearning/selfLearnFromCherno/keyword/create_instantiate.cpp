//#include <iostream>
//#include <string>
//
//using String = std::string;
//
//class Entity
//{
//private:
//	String m_Name;
//public:
//	Entity() : m_Name("Unknown"){}
//	Entity(const String& name) : m_Name(name) {}
//
//	const String& GetName() const { return m_Name; }
//};
//
//int main()
//{
//	/*Entity entity("Harry");
//	std::cout << entity.GetName() << std::endl;
//	Entity e = Entity("harry");
//	std::cout << e.GetName() << std::endl;*/
//
//	Entity* e;
//	{
//		Entity* entity = new Entity("Harry");
//		e = entity;
//		std::cout << entity->GetName() << std::endl;
//		delete entity;
//	}
//
//	std::cin.get();
//}