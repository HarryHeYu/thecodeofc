#include <iostream>
#include <string>
using namespace std::literals::string_literals;

class Entity
{
private:
	int m_X, m_Y;
	mutable int var;
public:
	 int GetX() const
	{
		var = 2;
		return m_X;
	}

	 int GetX()
	 {
		 return m_X;
	 }

	void SetX(int x)
	{
		m_X = x;
	}
};

/* const int* a 和int const* a 是一样的，指针本身的地址可以修改，但是不可以直接修改地址中储存的内容
int* const a 是指着地址不可修改，但是指向的地址的内容可以修改 
 const int* const a 是地址和地址的内容都不可以修改 */

void PrintEntity(const Entity& e)
{
	std::cout << e.GetX() << std::endl;
}

int main()
{
	Entity e;

	const char* name = "Harry";
	
	std::string name1 = "harry"s + " hello";
	
	std::string example = R"(I want to do some things
    And it's so big)";

	const char*  ex = "I wanna do\n"
		"myself";

	const int MAX_AGE = 90;
	const int* a = new int;
	
	a = (int*)&MAX_AGE;

	std::cout << *a << std::endl;

	std::cout << ex << std::endl;

	std::cout << example << std::endl;

	std::cout << name << std::endl;

	std::cin.get();
}