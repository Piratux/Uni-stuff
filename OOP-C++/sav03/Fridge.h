#pragma once

#include <vector>
#include <string>

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
        void AddFoodList(std::vector<std::string> food_list);
        void RemoveFood(const std::string& food_name, unsigned int count = 1);
        void RemoveFoodList(std::vector<std::string> food_list);
        void ClearFridge();
        std::vector<std::string> GetFoodList();
        unsigned int GetFoodCount(const std::string& food_name);
        unsigned int GetFoodListSize();
        unsigned int GetFridgeSize();
        unsigned int GetID();
        std::string ToString();
    };
}