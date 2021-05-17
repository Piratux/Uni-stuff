#include <iostream>
#include <cassert>

#include "Fridge.h"

using namespace std;
namespace kit = Kitchen;

int main()
{
    try
    {
        cout << "--Constructor tests--\n";
        cout << "> --Alone class on stack--\n";
        kit::Fridge f(12);
        f.add_food_list({ "Fish", "Rice", "Ice-cream", "Duck", "Garlic", "Egg" });
        f.add_food("Fish");
        f.add_food("Pancake", 5);
        cout << f;
        f.remove_food("Fish", 3);
        cout << f;
        f.clear();
        cout << f;

        cout << "\n> --Many classes on stack--\n";
        kit::Fridge f2[3];
        f2[1].init(10);
        f2[1].add_food("Egg", 3);
        cout << f2[1];
        f2[1].remove_food("Egg", 2);
        cout << f2[1];

        cout << "\n> --Alone class on heap--\n";
        kit::Fridge* f3 = new kit::Fridge(5);
        f3->add_food("Milk", f3->get_fridge_size());
        cout << *f3;

        cout << "\n> --Many classes on heap--\n";
        kit::Fridge* f4 = new kit::Fridge[3];
        f4[2].init(10);
        f4[2].add_food_list(f3->get_food_list());
        cout << f4[2];
        f4[2].remove_food("Milk", f4[2].get_food_count("Milk") - 2);
        cout << f4[2];

        // comparisson operator tests
        cout << "\n--Comparisson operator tests--\n";

        kit::Fridge f5(10);
        f5.add_food_list({ "Egg", "Milk", "Egg", "Grapes" });
        kit::Fridge f6(5);
        f6.add_food_list({ "Milk", "Grapes", "Egg", "Egg" });
        assert(f5 == f6);
        assert(!(f5 != f6));
        assert(f5 < f6);
        assert(!(f5 > f6));
        assert(f5 <= f6);
        assert(!(f5 >= f6));

        // increment operator tests
        cout << "\n--Increment operator tests--\n";

        kit::Fridge f10(5);

        cout << "f10 size before: " << f10.get_fridge_size() << '\n';
        cout << "f10 size during post-increment: " << (f10++).get_fridge_size() << '\n';
        cout << "f10 size after post-increment: " << f10.get_fridge_size() << '\n';
        cout << "f10 size during pre-increment: " << (++f10).get_fridge_size() << '\n';

        // stream operator tests
        cout << "\n--Stream operator tests--\n";
        cout << "Before: " << f5;
        cout << "Enter single food product: ";
        cin >> f5;
        cout << "After: " << f5;

        // exception tests
        cout << "\n--Exception tests--\n";
        kit::Fridge f7(2);
        f7.add_food_list({ "A", "B" });
        try
        {
            cin >> f7; // can't add food to full fridge
        }
        catch (kit::Fridge::FridgeException& e)
        {
            cerr << e.what() << '\n';
        }
        try
        {
            kit::Fridge bad_fridge(0); // can't construct empty fridge
        }
        catch (kit::Fridge::FridgeException& e)
        {
            cerr << e.what() << '\n';
        }
    }
    catch (std::exception& e)
    {
        cerr << e.what() << '\n';
    }
    catch (kit::Fridge::FridgeException& e)
    {
        cerr << e.what() << '\n';
    }
    catch (...)
    {
        cerr << "Unknown exception occured\n";
    }
}