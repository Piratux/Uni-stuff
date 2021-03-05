// fix UML with underline and default value last_ID

#pragma once

#include <vector>
#include <string>
#include <compare>

namespace Kitchen
{
    class Fridge
    {
    private:
        std::vector<std::string> food_list;
        unsigned int fridge_size;
        unsigned int ID;
    public:
        static unsigned int last_ID;

    public:
        Fridge(unsigned int fridge_size = 10);
        ~Fridge();
        void AddFood(const std::string& food_name, unsigned int count = 1);
        void AddFoodList(const std::vector<std::string>& food_list);
        void RemoveFood(const std::string& food_name, unsigned int count = 1);
        void RemoveFoodList(const std::vector<std::string>& food_list);
        void ClearFridge();
        std::vector<std::string> GetFoodList();
        unsigned int GetFoodCount(const std::string& food_name) const;
        unsigned int GetFoodListSize() const;
        unsigned int GetFridgeSize() const;
        unsigned int GetID() const;

    public:
        std::string ToString() const;

        // compares if fridge ingredients are same by sorting array
        bool operator == (const Fridge& lhs) const;
        bool operator != (const Fridge& lhs) const;
        // compares which fridge is more filled up by comparing GetFoodListSize/GetFridgeSize values
        bool operator <  (const Fridge& lhs) const;
        bool operator >  (const Fridge& lhs) const;
        bool operator <= (const Fridge& lhs) const;
        bool operator >= (const Fridge& lhs) const;
        // increment operators set a new ID
        Fridge& operator++(); // pre-increment
        Fridge operator++(int dummy); // post-increment
        friend std::istream& operator>>(std::istream& is, Fridge& fridge); // adds single item to fridge
        friend std::ostream& operator<<(std::ostream& os, const Fridge& fridge); // uses toString()
    };
}