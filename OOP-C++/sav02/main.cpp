#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

class Fridge
{
private:
	vector<string> food_list;
	unsigned int fridge_size;
public:
	Fridge(unsigned int fridge_size = 10)
	{
		this->fridge_size = (fridge_size == 0) ? 1 : fridge_size;
	}
	~Fridge()
	{}
	void AddFood(const string& food_name, unsigned int count = 1)
	{
		for (unsigned int i = 0; i < count; ++i)
		{
			if (food_list.size() < fridge_size)
			{
				food_list.push_back(food_name);
			}
			else
			{
				break;
			}
		}
	}
	void AddFoodList(vector<string> food_list)
	{
		for (const string& food : food_list)
		{
			AddFood(food);
		}
	}
	void ClearFridge()
	{
		food_list.clear();
	}
	unsigned int GetFoodCount(const string& food_name)
	{
		unsigned int found = 0;
		for (const string& food : food_list)
		{
			if (food == food_name)
			{
				++found;
			}
		}
		return found;
	}
	vector<string> GetFoodList()
	{
		return food_list;
	}
	unsigned int GetFoodListSize()
	{
		return food_list.size();
	}
	unsigned int GetFridgeSize()
	{
		return fridge_size;
	}
	void RemoveFood(const string& food_name, unsigned int count = 1)
	{
		for (unsigned int i = 0; i < food_list.size(); ++i)
		{
			if (count <= 0)
			{
				break;
			}
			if (food_list[i] == food_name)
			{
				food_list.erase(food_list.begin() + i);
				--i;
				--count;
			}
		}
	}
	void RemoveFoodList(vector<string> food_list)
	{
		for (const string& food : food_list)
		{
			RemoveFood(food);
		}
	}
	string ToString()
	{
		stringstream ss;
		if (GetFoodListSize() == 0)
		{
			ss << "{}\n";
			return ss.str();
		}

		ss << "{ ";
		for (unsigned int i = 0; i < food_list.size() - 1; ++i)
		{
			ss << food_list[i] << ", ";
		}
		ss << food_list.back() << " }\n";
		return ss.str();
	}
};
int main()
{
	cout << "--Alone class on stack--\n";
	Fridge f(12);
	f.AddFoodList({ "Fish", "Rice", "Ice-cream", "Duck", "Garlic", "Egg" });
	f.AddFood("Fish");
	f.AddFood("Pancake", 5);
	cout << f.ToString() << '\n';
	f.RemoveFood("Fish", 3);
	cout << f.ToString() << '\n';
	f.ClearFridge();
	cout << f.ToString() << '\n';

	cout << "--Many classes on stack--\n";
	Fridge f2[3];
	f2[1].AddFood("Egg", 3);
	cout << f2[1].ToString() << '\n';
	f2[1].RemoveFood("Egg", 2);
	cout << f2[1].ToString() << '\n';

	cout << "--Alone class on heap--\n";
	Fridge* f3 = new Fridge(5);
	f3->AddFood("Milk", f3->GetFridgeSize());
	cout << f3->ToString() << '\n';

	cout << "--Many classes on heap--\n";
	Fridge* f4 = new Fridge[3];
	f4[2].AddFoodList(f3->GetFoodList());
	cout << f4[2].ToString();
	f4[2].RemoveFood("Milk", f4[2].GetFoodCount("Milk") - 2);
	cout << f4[2].ToString();
}