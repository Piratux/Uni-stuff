// fix UML with underline and default value last_ID

#pragma once

#include <vector>
#include <string>
#include <memory>

namespace Kitchen
{
    class Fridge
    {
    private:
        class FridgeImpl;
        std::unique_ptr<FridgeImpl> impl;

        class FridgeException;

    public:
        Fridge();
        Fridge(size_t fridge_size);
        Fridge(const Fridge& other);
        Fridge& operator = (Fridge other);
        ~Fridge();

        void init(size_t fridge_size);
        void add_food(const std::string& food_name, size_t count = 1);
        void add_food_list(const std::vector<std::string>& food_list);
        void remove_food(const std::string& food_name, size_t count = 1);
        void remove_food_list(const std::vector<std::string>& food_list);
        void clear();
        std::vector<std::string> get_food_list() const;
        size_t get_food_count(const std::string& food_name) const;
        size_t get_food_list_size() const;
        size_t get_fridge_size() const;
        size_t get_ID() const;
        std::string to_string() const;

    public:
        // compares if fridge ingredients are same by sorting array
        bool operator == (const Fridge& lhs) const;
        bool operator != (const Fridge& lhs) const;
        // compares which fridge is more filled up by comparing (get_food_list_size/get_fridge_size) values
        bool operator <  (const Fridge& lhs) const;
        bool operator >  (const Fridge& lhs) const;
        bool operator <= (const Fridge& lhs) const;
        bool operator >= (const Fridge& lhs) const;
        // increment operators set a new ID
        Fridge& operator ++ (); // pre-increment
        Fridge operator  ++ (int dummy); // post-increment
        friend std::istream& operator >> (std::istream& is, Fridge& fridge); // adds single item to fridge
        friend std::ostream& operator << (std::ostream& os, const Fridge& fridge); // uses to_string()
    };
}