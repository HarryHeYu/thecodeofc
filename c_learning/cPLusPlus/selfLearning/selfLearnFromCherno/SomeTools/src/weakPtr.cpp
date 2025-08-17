#include <iostream>
#include <memory>

struct Object
{
	~Object() { std::cout << "Deleted object\n"; }
};

struct Manager
{
	std::weak_ptr<Object>Obj;

	void Func()
	{
		if (std::shared_ptr<Object> obj = Obj.lock())
		{
			// lock()是将弱指针进行短暂的升级为shared_ptr,次数也会+1
		}
		if (Obj.expired())// 可以检查std::weak_ptr指向的资源是否已经被释放
		{

		}

		std::cout << Obj.use_count() << std::endl;
	}
};

Manager manager;

int main()
{
	{
		std::shared_ptr<Object> obj = std::make_shared<Object>();
		manager.Obj = obj;
		manager.Func();
	}
	manager.Func();

	/*std::cout << manager.Obj << std::endl;*/

	/*Manager manager;
	manager.Obj = obj.get();*/

}