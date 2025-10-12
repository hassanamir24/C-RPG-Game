#include "Utilities.h"
#include <iostream>
#include <windows.h>  // For Sleep()
#include <cstdlib>    // For rand()
#include <ctime>      // For time()
#include <string>

using namespace std;
void showAnimation(const string& message, int dots, int delayMs) {
    cout << message;
    for (int i = 0; i < dots; i++) {
        cout << ".";
        cout.flush();
        Sleep(delayMs);
    }
    cout << endl;
}

Item generateRandomItem() {
    // Initialize random seed if not already done
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }

    const string names[] = { "Health Potion", "Mana Potion", "Iron Sword",
                                 "Steel Armor", "Ancient Relic", "Gold" };
    const string categories[] = { "Potion", "Potion", "Weapon",
                                     "Armor", "Quest", "Quest" };
    const string descriptions[] = {
        "Restores 30 health",
        "Restores 20 mana",
        "Increases attack by 15",
        "Increases defense by 10",
        "A mysterious quest item",
        "Shiny gold coins"
    };
    const int effects[] = { 30, 20, 15, 10, 0, 0 };
    const bool uniques[] = { false, false, false, true, true, false };

    int index = rand() % 6;
    int quantity = (categories[index] == "Quest") ? 1 : (rand() % 3 + 1);

    return Item(names[index], categories[index], quantity,
        effects[index], uniques[index], descriptions[index]);
}