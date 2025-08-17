#include <iostream>
#include <string>

static uint32_t s_Alloction = 0;

void* operator new(size_t size)
{
	s_Alloction++;
	std::cout << "Allocating " << size << " bytes\n";
	return malloc(size);
} 

//void PrintName(const std::string& name)
//{
//	std::cout << name << std::endl;
//}
void PrintName(std::string_view name)
{
	std::cout << name << std::endl;
}

int main()
{
//	std::string name = "Harry He";
//	
//#if 0
//	std::string firstName = name.substr(0, 5);
//	std::string lastName = name.substr(6, 8);
//#else
//	std::string_view firstName(name.c_str(), 5);
//	std::string_view lastName(name.c_str() + 6, 7);
//#endif

	const char* name = "Harry He";

#if 0
	std::string firstName = name.substr(0, 5);
	std::string lastName = name.substr(6, 8);
#else
	std::string_view firstName(name, 5);
	std::string_view lastName(name + 6, 7);
#endif
	PrintName(firstName);
	PrintName(lastName);

	std::cout << s_Alloction << "Allocations" << std::endl;
	std::cin.get();
}