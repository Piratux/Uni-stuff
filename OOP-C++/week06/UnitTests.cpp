#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Fridge.h"

TEST_CASE("random test") {
    Kitchen::Fridge fridge(5);
    fridge.add_food_list({ "apple", "banana" });
    CHECK(fridge.get_food_list() == std::vector<std::string>{ "apple", "banana" });
}

/*TEST_CASE("random test") { 
    Kitchen::Fridge fridge(5);
    size_t ID = fridge.get_ID();
    size_t fridge_size = fridge.get_fridge_size();

    CHECK((fridge++).get_ID() == ID);
    CHECK(fridge.get_fridge_size() == fridge_size + 1);

    Kitchen::Fridge fridge2(fridge);
    CHECK(fridge2.get_ID() == ID + 1);
}*/