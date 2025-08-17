#include <iostream>
#include <string>
#include <memory>

class Singleton
{
private:
public:
	static Singleton& Get() {
		static Singleton instance;
		return instance;
	}
	
	void Hello() {}
};

//Singleton* Singleton::s_Instance = nullptr;

int main()
{
	Singleton::Get().Hello();

	std::cin.get();
}