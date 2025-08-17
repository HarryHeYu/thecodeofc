//#include <iostream>
//
//class Entity
//{
//public:
//	Entity() = default;
//	const std::string& GetName()const { return m_Name;  }
//	void PrintType()
//	{
//		std::cout << "Entity\n";
//	}
//private:
//	Entity* m_Parent;
//	std::string m_Name;
//};
//
//struct EntityData
//{
//	EntityData* m_Parent;
//	std::string m_Name;
//};
//
//const std::string& Entity_GetName(const EntityData* self)
//{
//	//self == nullptr
//	return self->m_Name;
//}
//
//void Entity_PrintType(EntityData* self)
//{
//	//self == nullptr
//	std::cout << "Entity\n";
//}
//
//int main()
//{
//	Entity* entity = nullptr;
//	entity->PrintType();
//
//	uint64_t offset = offsetof(EntityData, m_Name);
//	
//	offset = (uint64_t)&(((EntityData*)0)->m_Name);
//	std::cout << "Offset of m_Name: " << offset << std::endl;
//
//	std::cout << entity->GetName() << std::endl;
//	std::cin.get();
//}