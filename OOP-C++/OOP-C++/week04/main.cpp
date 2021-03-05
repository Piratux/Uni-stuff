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
        f.AddFoodList({ "Fish", "Rice", "Ice-cream", "Duck", "Garlic", "Egg" });
        f.AddFood("Fish");
        f.AddFood("Pancake", 5);
        cout << f;
        f.RemoveFood("Fish", 3);
        cout << f;
        f.ClearFridge();
        cout << f;

        cout << "\n> --Many classes on stack--\n";
        kit::Fridge f2[3];
        f2[1].AddFood("Egg", 3);
        cout << f2[1];
        f2[1].RemoveFood("Egg", 2);
        cout << f2[1];

        cout << "\n> --Alone class on heap--\n";
        kit::Fridge* f3 = new kit::Fridge(5);
        f3->AddFood("Milk", f3->GetFridgeSize());
        cout << *f3;

        cout << "\n> --Many classes on heap--\n";
        kit::Fridge* f4 = new kit::Fridge[3];
        f4[2].AddFoodList(f3->GetFoodList());
        cout << f4[2];
        f4[2].RemoveFood("Milk", f4[2].GetFoodCount("Milk") - 2);
        cout << f4[2];

        // comparisson operator tests
        cout << "\n--Comparisson operator tests--\n";

        kit::Fridge f5(10);
        f5.AddFoodList({ "Egg", "Milk", "Egg", "Grapes" });
        kit::Fridge f6(5);
        f6.AddFoodList({ "Milk", "Grapes", "Egg", "Egg" });
        assert(f5 == f6);
        assert(!(f5 != f6));
        assert(f5 < f6);
        assert(!(f5 > f6));
        assert(f5 <= f6);
        assert(!(f5 >= f6));

        // increment operator tests
        cout << "\n--Increment operator tests--\n";

        cout << "f5 ID before: " << f5.GetID() << '\n';
        cout << "f5 ID during post-increment: " << f5++.GetID() << '\n';
        cout << "f5 ID after post-increment: " << f5++.GetID() << '\n';
        cout << "f5 ID during pre-increment: " << (++f5).GetID() << '\n';

        // stream operator tests
        cout << "\n--Stream operator tests--\n";
        cout << "Before: " << f5;
        cout << "Enter single food product: ";
        cin >> f5;
        cout << "After: " << f5;

        // exception tests
        cout << "\n--Exception tests--\n";
        kit::Fridge f7(2);
        f7.AddFoodList({ "A", "B" });
        try
        {
            cin >> f7; // can't add food to full fridge
        }
        catch (std::exception& e)
        {
            cerr << e.what() << '\n';
        }
        try
        {
            kit::Fridge bad_fridge(0); // can't construct empty fridge
        }
        catch (std::exception& e)
        {
            cerr << e.what() << '\n';
        }
    }
    catch (std::exception& e)
    {
        cerr << e.what() << '\n';
    }
    catch (...)
    {
        cerr << "Unknown exception occured\n";
    }
}