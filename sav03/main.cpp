#include <iostream>

#include "Fridge.h"

using namespace std;

int main()
{
    using namespace Kitchen;

    try
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

        Fridge bad_fridge(0);
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