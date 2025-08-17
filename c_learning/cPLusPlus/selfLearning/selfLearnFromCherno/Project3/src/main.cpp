#include <iostream>

enum Example : unsigned char
{
	A = 0, B, C
};

class Enity
{
public:
	float X, Y;
	 
	Enity()
	{
		X = 0.0f;
		Y = 0.0f;
		std::cout << "Created Enity!" << std::endl;
	}

	/*Enity(float x, float y)
	{
		X = x;
		Y = y;
	}*/

	~Enity()
	{
		std::cout << "Destroyed Enity!" << std::endl;
	}
	/*void Init()
	{
		X = 0.0f;
		Y = 0.0f;
	}*/

	void Print()
	{
		std::cout << X << "," << Y << std::endl;
	}
};

//int Enity::X;
//int Enity::Y;

class Log
{
//private:
//	Log(){}
public:
	static void Write()
	{

	}
};

void Function()
{
	Enity e;
	e.Print();
}
int main()
{
	Log::Write();
	/*Log l;
	Example value = B;
	if (value == 1)
	{
		 
	}*/
	//Enity e;
	//e.X = 2;
	//e.Y = 3;

	//Enity e1;
	///*e1.X = 5;
	//e1.Y = 8;*/

	//Enity e2(10.3, 3.2);


	//e.Print();
	/*e1.Print();
	e2.Print();*/
	Function();

	std::cin.get();
}