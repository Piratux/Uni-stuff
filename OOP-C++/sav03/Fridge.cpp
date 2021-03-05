#include "Fridge.h"

#include <sstream>
#include <stdexcept>

namespace Kitchen
{
    unsigned int Fridge::last_ID = 0;

    Fridge::Fridge(unsigned int fridge_size)
    {
        if(fridge_size == 0)
            throw std::invalid_argument("Fridge size can't be 0");
        
        this->fridge_size = fridge_size;

        this->ID = ++last_ID;
    }
    Fridge::~Fridge()
    {}
    void Fridge::AddFood(const std::string& food_name, unsigned int count)
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
    void Fridge::AddFoodList(std::vector<std::string> food_list)
    {
        for (const std::string& food : food_list)
        {
            AddFood(food);
        }
    }
    void Fridge::ClearFridge()
    {
        food_list.clear();
    }
    unsigned int Fridge::GetFoodCount(const std::string& food_name)
    {
        unsigned int found = 0;
        for (const std::string& food : food_list)
        {
            if (food == food_name)
            {
                ++found;
            }
        }
        return found;
    }
    std::vector<std::string> Fridge::GetFoodList()
    {
        return food_list;
    }
    unsigned int Fridge::GetFoodListSize()
    {
        return food_list.size();
    }
    unsigned int Fridge::GetFridgeSize()
    {
        return fridge_size;
    }
    void Fridge::RemoveFood(const std::string& food_name, unsigned int count)
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
    void Fridge::RemoveFoodList(std::vector<std::string> food_list)
    {
        for (const std::string& food : food_list)
        {
            RemoveFood(food);
        }
    }
    unsigned int Fridge::GetID()
    {
        return ID;
    }
    std::string Fridge::ToString()
    {
        std::stringstream ss;
        if (GetFoodListSize() == 0)
        {
            ss << "{}\n";
            return ss.str();
        }

        ss << "{ ";
        ss << GetID() << ": ";
        for (unsigned int i = 0; i < food_list.size() - 1; ++i)
        {
            ss << food_list[i] << ", ";
        }
        ss << food_list.back() << " }\n";
        return ss.str();
    }
}