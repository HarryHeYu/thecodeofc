//#include <iostream>
//#include <fstream>
//#include <optional>
//
//std::optional<std::string>ReadFileStirng(const std::string& filepath)
//{
//	std::ifstream stream(filepath);
//	if (stream)
//	{
//		std::string result;
//		stream.close();
//		return result;
//	}
//	return {};
//}
//
//int main()
//{
//	std::optional<std::string> data = ReadFileStirng("data.text");
//	
//	std::string value = data.value_or("Not present");
//	std::cout << value << std::endl;
//
//	if(data.has_value())
//	{
//		std::cout << "Read successfully!" << std::endl;
//	}
//	else
//	{
//		std::cout << "Failed to read!" << std::endl;
//	}
//}