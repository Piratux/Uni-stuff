#include "Fridge.h"

#include <sstream>
#include <stdexcept>
#include <algorithm>

namespace Kitchen
{
    unsigned int Fridge::last_ID = 0;

    Fridge::Fridge(unsigned int fridge_size)
    {
        if(fridge_size == 0)
            throw std::invalid_argument("Fridge: Fridge size can't be 0");
        
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
    void Fridge::AddFoodList(const std::vector<std::string>& food_list)
    {
        for (const std::string& food : food_list)
        {
            AddFood(food);
        }
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
    void Fridge::RemoveFoodList(const std::vector<std::string>& food_list)
    {
        for (const std::string& food : food_list)
        {
            RemoveFood(food);
        }
    }
    void Fridge::ClearFridge()
    {
        food_list.clear();
    }
    std::vector<std::string> Fridge::GetFoodList()
    {
        return food_list;
    }
    unsigned int Fridge::GetFoodCount(const std::string& food_name) const
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
    unsigned int Fridge::GetFoodListSize() const
    {
        return food_list.size();
    }
    unsigned int Fridge::GetFridgeSize() const
    {
        return fridge_size;
    }
    unsigned int Fridge::GetID() const
    {
        return ID;
    }
    std::string Fridge::ToString() const
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

    bool Fridge::operator==(const Fridge& rhs) const
    {
        Fridge temp1 = *this;
        Fridge temp2 = rhs;
        sort(temp1.food_list.begin(), temp1.food_list.end());
        sort(temp2.food_list.begin(), temp2.food_list.end());
        return temp1.food_list == temp2.food_list;
    }
    bool Fridge::operator!=(const Fridge& rhs) const
    {
        return !(*this == rhs);
    }
    bool Fridge::operator<(const Fridge& rhs) const
    {
        return ((float)this->GetFoodListSize() / (float)this->GetFridgeSize()) < ((float)rhs.GetFoodListSize() / (float)rhs.GetFridgeSize());
    }
    bool Fridge::operator>(const Fridge& rhs) const
    {
        return (rhs < *this);
    }
    bool Fridge::operator<=(const Fridge& rhs) const
    {
        return !(*this > rhs);
    }
    bool Fridge::operator>=(const Fridge& rhs) const
    {
        return !(*this < rhs);
    }

    Fridge& Fridge::operator++() // pre-increment
    {
        ID = ++last_ID;
        return *this;
    }
    Fridge Fridge::operator++(int dummy) // post-increment
    {
        Fridge temp = *this;
        operator++();
        return temp;
    }

    std::istream& operator>>(std::istream& is, Fridge& fridge)
    {
        if (fridge.GetFridgeSize() == fridge.GetFoodListSize())
            throw std::exception("Fridge: Fridge is full");

        std::string food;
        if (is >> food)
        {
            fridge.AddFood(food);
        }
        else
        {
            is.setstate(std::ios::failbit);
        }
        
        return is;
    }
    std::ostream& operator<<(std::ostream& os, const Fridge& fridge)
    {
        os << fridge.ToString();
        return os;
    }
}