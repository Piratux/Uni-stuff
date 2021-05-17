#include "Fridge.h"

#include <sstream>
#include <stdexcept>
#include <algorithm>

namespace Kitchen
{
    class Fridge::FridgeImpl
    {
    private:
        friend Fridge;

        std::vector<std::string> food_list;
        size_t fridge_size;
        size_t ID;
        static size_t last_ID;
        static size_t alive_object_count;

    public:
        FridgeImpl(size_t fridge_size)
            : fridge_size(fridge_size),
            ID(++last_ID)
        {
            ++Fridge::FridgeImpl::alive_object_count;
        }
        FridgeImpl(const FridgeImpl& other)
            : food_list(other.food_list),
            fridge_size(other.fridge_size),
            ID(++last_ID)
        {
            ++Fridge::FridgeImpl::alive_object_count;
        }
        FridgeImpl& operator = (FridgeImpl other)
        {
            std::swap(food_list, other.food_list);
            std::swap(fridge_size, other.fridge_size);

            return *this;
        }
        ~FridgeImpl()
        {
            --Fridge::FridgeImpl::alive_object_count;
        }
    };

    size_t Fridge::FridgeImpl::last_ID = 0;
    size_t Fridge::FridgeImpl::alive_object_count = 0;

    Fridge::FridgeException::FridgeException(const char* file, size_t line)
    {
        msg += "Error in: ";
        msg.append(file);
        msg += "\nAt line: ";
        msg.append(std::to_string(line));
    }
    Fridge::FridgeException::FridgeException(const char* error_description)
    {
        msg.append(error_description);
    }
    const char* Fridge::FridgeException::what()
    {
        return msg.c_str();
    }
     
    Fridge::Fridge()
    {
    }
    Fridge::Fridge(size_t fridge_size)
    {
        if(fridge_size == 0)
            throw std::invalid_argument("Fridge: Fridge size can't be 0");

        this->impl = std::make_unique<FridgeImpl>(fridge_size);
    }
    Fridge::Fridge(const Fridge& other)
    {
        if (other.impl == nullptr)
            throw FridgeException(__FILE__, __LINE__);

        this->impl = std::make_unique<FridgeImpl>(*other.impl);
    }
    Fridge& Fridge::operator = (Fridge other)
    {
        std::swap(impl, other.impl);

        return *this;
    }
    Fridge::~Fridge()
    {
    }

    void Fridge::init(size_t fridge_size)
    {
        if (fridge_size == 0)
            throw std::invalid_argument("Fridge: Fridge size can't be 0");

        this->impl = std::make_unique<FridgeImpl>(fridge_size);
    }
    void Fridge::add_food(const std::string& food_name, size_t count)
    {
        if (impl == nullptr)
            throw FridgeException(__FILE__, __LINE__);

        for (size_t i = 0; i < count; ++i)
        {
            if (impl->food_list.size() < impl->fridge_size)
            {
                impl->food_list.push_back(food_name);
            }
            else
            {
                throw FridgeException("Fridge: Fridge is full");
                break;
            }
        }
    }
    void Fridge::add_food_list(const std::vector<std::string>& food_list)
    {
        if (impl == nullptr)
            throw FridgeException(__FILE__, __LINE__);

        for (const std::string& food : food_list)
        {
            add_food(food);
        }
    }
    void Fridge::remove_food(const std::string& food_name, size_t count)
    {
        if (impl == nullptr)
            throw FridgeException(__FILE__, __LINE__);

        for (size_t i = 0; i < impl->food_list.size(); ++i)
        {
            if (count <= 0)
            {
                break;
            }
            if (impl->food_list[i] == food_name)
            {
                impl->food_list.erase(impl->food_list.begin() + i);
                --i;
                --count;
            }
        }
    }
    void Fridge::remove_food_list(const std::vector<std::string>& food_list)
    {
        if (impl == nullptr)
            throw FridgeException(__FILE__, __LINE__);

        for (const std::string& food : food_list)
        {
            remove_food(food);
        }
    }
    void Fridge::clear()
    {
        if (impl == nullptr)
            throw FridgeException(__FILE__, __LINE__);

        impl->food_list.clear();
    }
    std::vector<std::string> Fridge::get_food_list() const
    {
        if (impl == nullptr)
            throw FridgeException(__FILE__, __LINE__);

        return impl->food_list;
    }
    size_t Fridge::get_food_count(const std::string& food_name) const
    {
        if (impl == nullptr)
            throw FridgeException(__FILE__, __LINE__);

        size_t found = 0;
        for (const std::string& food : impl->food_list)
        {
            if (food == food_name)
            {
                ++found;
            }
        }
        return found;
    }
    size_t Fridge::get_food_list_size() const
    {
        if (impl == nullptr)
            throw FridgeException(__FILE__, __LINE__);

        return impl->food_list.size();
    }
    size_t Fridge::get_fridge_size() const
    {
        if (impl == nullptr)
            throw FridgeException(__FILE__, __LINE__);

        return impl->fridge_size;
    }
    size_t Fridge::get_ID() const
    {
        if (impl == nullptr)
            throw FridgeException(__FILE__, __LINE__);

        return impl->ID;
    }
    std::string Fridge::to_string() const
    {
        if (impl == nullptr)
            throw FridgeException(__FILE__, __LINE__);

        std::stringstream ss;
        if (get_food_list_size() == 0)
        {
            ss << "{}\n";
            return ss.str();
        }

        ss << "{ " << std::to_string(get_food_list_size()) << ": ";
        for (size_t i = 0; i < impl->food_list.size() - 1; ++i)
        {
            ss << impl->food_list[i] << ", ";
        }
        ss << impl->food_list.back() << " }\n";
        return ss.str();
    }
    size_t Fridge::get_alive_object_count()
    {
        return Fridge::FridgeImpl::alive_object_count;
    }

    bool Fridge::operator==(const Fridge& rhs) const
    {
        if (impl == nullptr || rhs.impl == nullptr)
            throw FridgeException(__FILE__, __LINE__);

        Fridge temp1 = *this;
        Fridge temp2 = rhs;
        sort(temp1.impl->food_list.begin(), temp1.impl->food_list.end());
        sort(temp2.impl->food_list.begin(), temp2.impl->food_list.end());
        return temp1.impl->food_list == temp2.impl->food_list;
    }
    bool Fridge::operator!=(const Fridge& rhs) const
    {
        return !(*this == rhs);
    }
    bool Fridge::operator<(const Fridge& rhs) const
    {
        if (impl == nullptr || rhs.impl == nullptr)
            throw FridgeException(__FILE__, __LINE__);

        return ((float)this->get_food_list_size() / (float)this->get_fridge_size()) < ((float)rhs.get_food_list_size() / (float)rhs.get_fridge_size());
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
        if (impl == nullptr)
            throw FridgeException(__FILE__, __LINE__);

        if (impl->fridge_size != (size_t)-1)
            ++impl->fridge_size;

        return *this;
    }
    Fridge Fridge::operator++(int dummy) // post-increment
    {
        if (impl == nullptr)
            throw FridgeException(__FILE__, __LINE__);

        Fridge temp = *this;

        if (impl->fridge_size != (size_t)-1)
            ++impl->fridge_size;

        // Need to decrease it, because last_ID increases from copy constructors
        Fridge::FridgeImpl::last_ID -= 2;
        return temp;
    }

    std::istream& operator>>(std::istream& is, Fridge& fridge)
    {
        if (fridge.impl == nullptr)
            throw Fridge::FridgeException(__FILE__, __LINE__);

        // ignore '{'
        is.ignore(1);
        
        size_t food_list_size;
        is >> food_list_size;

        // check if the incoming food will fit in fridge
        if(fridge.get_fridge_size() - fridge.get_food_list_size() < food_list_size)
            throw Fridge::FridgeException("Fridge: not enough space in fridge");

        // ignore ':'
        is.ignore(1);
        std::string food;
        Fridge temp(food_list_size);

        while (true)
        {
            is >> food;
            if (food.back() == ',')
            {
                food.pop_back();
                temp.add_food(food);
            }
            else
            {
                temp.add_food(food);
                break;
            }
        }
        fridge.add_food_list(temp.get_food_list());
        
        // ignore '}' at the end
        is.ignore(10, '}');

        return is;
    }
    std::ostream& operator<<(std::ostream& os, const Fridge& fridge)
    {
        if (fridge.impl == nullptr)
            throw Fridge::FridgeException(__FILE__, __LINE__);

        os << fridge.to_string();
        return os;
    }
}