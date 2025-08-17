#include <iostream>
#include <vector>

#include <map>
#include <unordered_map>

struct CityRecord
{
	std::string Name;
	uint64_t Population;
	double Latitude, Longitude;

	// 重载小于运算符用于有序图(map)的的排序使用
	bool operator<(const CityRecord& other)
	{
		return Population < other.Population;
	}
};

//一个固定模式：将自己创建的结构体加入到
//std命名空间中的哈希函数中去
namespace std {
	template<>
	struct hash<CityRecord>
	{
		size_t operator()(const CityRecord& key)
		{
			return hash<std::string>()(key.Name);
		}
	};
}

int main()
{
	std::vector<CityRecord> cities;
	std::unordered_map<std::string, CityRecord> cityMap;
	std::map<CityRecord, uint32_t> foundedMap;

	foundedMap[{ "Chengdu", 5000000, 2.4, 9.4 }] = 1850;

	cities.emplace_back("Chengdu", 5000000, 2.4, 9.4);
	cities.emplace_back("Peking", 5000000, 2.4, 9.4);
	cities.emplace_back("Berlin", 5000000, 2.4, 9.4);
	cities.emplace_back("Paris", 5000000, 2.4, 9.4);
	cities.emplace_back("London", 5000000, 2.4, 9.4);

	cityMap["Chengdu"] = CityRecord{ "Chengdu", 5000000, 2.4, 9.4 };
	cityMap["Peking"] = CityRecord{"Peking", 5000000, 2.4, 9.4};
	/*cityMap["Berlin"] = CityRecord{"Berlin", 5000000, 2.4, 9.4};*/
	cityMap["Paris"] = CityRecord{"Paris", 5000000, 2.4, 9.4};
	cityMap["London"] = CityRecord{"London", 5000000, 2.4, 9.4};
	
	//相当于是可以借助这个方法直接创建一个新的对象
	CityRecord& berlinData = cityMap["Berlin"];
	berlinData.Population = 5;
	// ...

	cityMap.erase("London");

	// at也是查找键对应的值，只不过不会像[]一样，
	//at找不到会返回异常值，而[]找不到会直接
	//插入一个新的键
	const auto& Cities = cityMap;
	if (Cities.find("Berlin") != Cities.end())
	{
		const CityRecord& BerlinData = Cities.at("Berlin");
	}

	for (auto& [name, city] : cityMap)
	{
		std::cout << name << "\n Population:" << city.Population << std::endl;
	}

	std::cin.get();
}