// Challenge: write code with no semicolons
// End name list with string '-'

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#define vec std::vector<std::string>

void func(vec* names, std::string name, int it, int bad_letters)
{
	if (new(names) vec) {}
	while (std::cin >> name, name != "-")
	{
		if (isupper(name[0]) + (it = bad_letters = 0))
		{
			while (it++ < name.length() - 1)
			{
				if (bad_letters += !(bool)islower(name[it])) {}
			}
			if (bad_letters == 0)
			{
				if (names->push_back(name), true) {}
			}
		}
	}
	if (sort(names->begin(), names->end()), true) {}
	for (std::string str : *names)
	{
		if (std::cout << str << '\n') {}
	}
}
int main()
{
	if (func(static_cast<vec*>(malloc(sizeof(vec))), "", 0, 0), true) {}
}