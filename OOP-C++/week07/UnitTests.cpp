#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <sstream>
#include "Fridge.h"

using namespace Kitchen;
using namespace std;

TEST_CASE("#SET")
{
    Fridge fridge(10);
    CHECK(fridge.get_fridge_size() == 10);

    fridge.add_food("Cheese");
    fridge.add_food("Egg", 3);

    CHECK(fridge.get_food_count("Cheese") == 1);
    CHECK(fridge.get_food_count("Egg") == 3);
    CHECK(fridge.get_food_count("Desra") == 0);
    CHECK(fridge.get_food_list_size() == 4);
    CHECK(fridge.get_food_list() == vector<string>{"Cheese", "Egg", "Egg", "Egg"});
    
    fridge.remove_food("Egg");
    
    CHECK(fridge.get_fridge_size() == 10);
    CHECK(fridge.get_food_count("Cheese") == 1);
    CHECK(fridge.get_food_count("Egg") == 2);
    CHECK(fridge.get_food_list_size() == 3);
    CHECK(fridge.get_food_list() == vector<string>{"Cheese", "Egg", "Egg"});

    fridge.remove_food_list(vector<string>{"Egg", "Cheese"});

    CHECK(fridge.get_fridge_size() == 10);
    CHECK(fridge.get_food_count("Cheese") == 0);
    CHECK(fridge.get_food_count("Egg") == 1);
    CHECK(fridge.get_food_list_size() == 1);
    CHECK(fridge.get_food_list() == vector<string>{"Egg"});
}

TEST_CASE("#AUTO_ID")
{
    Fridge fridge(5);
    Fridge fridge2(5);
    CHECK(fridge.get_ID() + 1 == fridge2.get_ID());
    Fridge fridge3(fridge);
    CHECK(fridge.get_ID() + 2 == fridge3.get_ID());
}

TEST_CASE("#CMP")
{
    // "==", "!="
    vector<string> food_list = { "A", "B", "C", "D", "E" };
    vector<string> food_list2 = { "C", "D", "E", "F", "A", "B" };
    Fridge fridge(5);
    Fridge fridge2(6);
    fridge.add_food_list(food_list);
    fridge2.add_food_list(food_list2);

    CHECK(fridge != fridge2);
    CHECK_FALSE(fridge == fridge2);

    fridge2.remove_food("F");

    CHECK(fridge == fridge2);
    CHECK_FALSE(fridge != fridge2);

    // "<", "<=", ">", ">=
    Fridge fridge3(4);
    Fridge fridge4(8);

    CHECK(fridge3 <= fridge4);
    CHECK(fridge3 >= fridge4);

    fridge3.add_food("A", 2);
    fridge4.add_food("B", 4);

    CHECK(fridge3 <= fridge4);
    CHECK(fridge3 >= fridge4);

    fridge3.add_food("A");

    CHECK(fridge3 > fridge4);
    CHECK(fridge3 >= fridge4);

    fridge4.add_food("B", 3);

    CHECK(fridge3 < fridge4);
    CHECK(fridge3 <= fridge4);
}

TEST_CASE("#STREAM")
{
    Fridge fridge(10);
    Fridge fridge2(10);

    fridge.add_food_list(vector<string>{"A", "A", "B", "Pica"});
    stringstream ss;
    ss << fridge;
    ss >> fridge2;

    CHECK(fridge == fridge2);
}

TEST_CASE("#PRE_POST")
{
    Fridge fridge(10);
    size_t fridge_size = fridge.get_fridge_size();

    CHECK((fridge++).get_fridge_size() == fridge_size);
    CHECK(fridge.get_fridge_size() == fridge_size + 1);
    CHECK((++fridge).get_fridge_size() == fridge_size + 2);

    size_t id = fridge.get_ID();

    CHECK((fridge++).get_ID() == id);
    CHECK((++fridge).get_ID() == id);
}

TEST_CASE("#THROW")
{
    // can't construct empty fridge
    CHECK_THROWS_AS(Fridge fridge(0), std::invalid_argument);

    // can't add food to full fridge
    Fridge fridge(2);
    Fridge fridge_copy(fridge);
    fridge.add_food("A", 2);
    CHECK_THROWS_AS(fridge.add_food("B"), Fridge::FridgeException);

    // check if object remained the same
    CHECK(fridge != fridge_copy);

    // can't use methods without "init()" after default(lazy) initialisation
    Fridge fridge2;
    CHECK_THROWS_AS(fridge2.add_food("A"), Fridge::FridgeException);

    // can't init empty fridge
    CHECK_THROWS_AS(fridge2.init(0), std::invalid_argument);

    // fridge run out of space
    Fridge fridge3(3);
    fridge3.add_food("A", 3);
    Fridge fridge4(2);
    Fridge fridge4_copy(fridge4);
    stringstream ss;
    ss << fridge3;
    CHECK_THROWS_AS(ss >> fridge4, Fridge::FridgeException);

    // check if object remained the same
    CHECK(fridge4 == fridge4_copy);
}

TEST_CASE("#COPY")
{
    // check copy constructor
    Fridge fridge(5);
    fridge.add_food("A", 3);
    Fridge fridge_copy(fridge);
    fridge_copy.add_food("B");
    CHECK(fridge != fridge_copy);

    // check assignment operator
    Fridge fridge2(5);
    fridge2.add_food("A", 3);
    Fridge fridge_copy2;
    fridge_copy2 = fridge2;
    fridge_copy2.add_food("B");
    CHECK(fridge2 != fridge_copy2);
}

TEST_CASE("#LEAK")
{
    CHECK(Fridge::get_alive_object_count() == 0);

    {
        Fridge* fridge = new Fridge();
        Fridge* fridge2 = new Fridge(10);
        Fridge* fridge3 = new Fridge(*fridge2);
        *fridge = *fridge2;
        CHECK(Fridge::get_alive_object_count() == 3);

        delete fridge;
        delete fridge2;
        delete fridge3;
    }

    CHECK(Fridge::get_alive_object_count() == 0);
}

TEST_CASE("#CUSTOM")
{
    // check if food removal works correctly
    Fridge fridge(10);
    fridge.add_food_list(vector<string>{"A", "B", "B", "C", "C"});

    fridge.remove_food("Zebra", 100);
    CHECK(fridge.get_food_list() == vector<string>{ "A", "B", "B", "C", "C" });

    fridge.remove_food("A", 100);
    CHECK(fridge.get_food_list() == vector<string>{ "B", "B", "C", "C" });

    fridge.remove_food("C", 100);
    CHECK(fridge.get_food_list() == vector<string>{ "B", "B" });

    fridge.remove_food_list(vector<string>{"A", "B", "C"});
    CHECK(fridge.get_food_list() == vector<string>{ "B" });
}