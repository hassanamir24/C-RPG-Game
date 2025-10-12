#include "Player.h"
#include <iostream>
#include <fstream>
#include "Constants.h"  
#include "Utilities.h"  
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include "BattleLog.h"




using namespace std;
int Player::playerCount = 0;// to count how many players are being created in the game.

Player::Player() : Character(), inventoryCount(0), inventoryCapacity(MAX_INVENTORY), currency(100),
classType("None"), isDefending(false), skillCooldown(0), restCooldown(0) {
    inventory = new Item[inventoryCapacity];
    battleHistory = new BattleLog(MAX_BATTLE_LOG);
    playerCount++;
}

Player::Player(const string& n, const std::string& classChoice)
    : Character(n, 100, 50, 10, 5, 10), inventoryCount(0), inventoryCapacity(MAX_INVENTORY),
    currency(100), classType(classChoice), isDefending(false), skillCooldown(0), restCooldown(0) {
    inventory = new Item[inventoryCapacity];
    battleHistory = new BattleLog(MAX_BATTLE_LOG);
    if (classChoice == "Warrior") {
        health = 120;
        attack = 15;
        defense = 8;
    }
    else if (classChoice == "Mage") {
        mana = 80;
        attack = 12;
        agility = 8;
    }
    else if (classChoice == "Rogue") {
        agility = 15;
        attack = 13;
        defense = 4;
    }
    else {
        throw invalid_argument("Invalid class type!");
    }
    playerCount++;
}

Player::Player(const Player& other)
    : Character(other), inventoryCount(other.inventoryCount), inventoryCapacity(other.inventoryCapacity),
    currency(other.currency), classType(other.classType), isDefending(other.isDefending),
    skillCooldown(other.skillCooldown), restCooldown(other.restCooldown) {
    inventory = new Item[inventoryCapacity];
    for (int i = 0; i < inventoryCount; i++) {
        inventory[i] = other.inventory[i];
    }
    battleHistory = new BattleLog(*other.battleHistory);
    playerCount++;
}

Player::~Player() {
    delete[] inventory;
    delete battleHistory;
}

void Player::displayStats() const {
    std::cout << "Player Stats:\n";
    Character::displayStats();
    std::cout << "Class: " << classType << "\nCurrency: " << currency
        << "\nDefending: " << (isDefending ? "Yes" : "No")
        << "\nRest Cooldown: " << restCooldown << std::endl;
}

int Player::calculateDamage() const {
    int baseDamage = attack;
    if (classType == "Rogue" && (rand() % 100) < agility * 2) {
        baseDamage *= 2;
        std::cout << "Critical hit!\n";
    }
    return baseDamage + (rand() % 5);
}

void Player::showInventory() const {
    cout << "Inventory (" << inventoryCount << "/" << inventoryCapacity << "):\n";
    if (inventoryCount == 0) {
        cout << "Empty\n";
        return;
    }
    for (int i = 0; i < inventoryCount; i++) {
        cout << i + 1 << ". ";
        inventory[i].display();
    }
}

// Purpose: Add item to inventory
bool Player::addItem(const Item& item) {
    if (inventoryCount >= inventoryCapacity) {
        cout << "Inventory full!\n";
        return false;
    }
    if (item.getIsUnique()) {
        for (int i = 0; i < inventoryCount; i++) {
            if (inventory[i].getName() == item.getName()) {
                cout << "Cannot add duplicate unique item!\n";
                return false;
            }
        }
    }
    else if (item.getCategory() != "Quest") {
        for (int i = 0; i < inventoryCount; i++) {
            if (inventory[i].getName() == item.getName()) {
                inventory[i] = Item(item.getName(), item.getCategory(), inventory[i].getQuantity() + item.getQuantity(),
                    item.getEffectValue(), false, item.getDescription());
                cout << "Already" << item.getName() << " in inventory.\n";
                return true;
            }
        }
    }
    inventory[inventoryCount++] = item;
    cout << "Added " << item.getName() << " to inventory.\n";
    return true;
}

// Purpose: Remove item from inventory
bool Player::removeItem(const string& itemName) {
    for (int i = 0; i < inventoryCount; i++) {
        if (inventory[i].getName() == itemName) {
            if (inventory[i].getCategory() == "Quest") {
                cout << "Cannot remove quest items!\n";
                return false;
            }
            if (inventory[i].getIsUnique()) {
                cout << "Warning: Removing unique item " << itemName << "!\n";
            }
            for (int j = i; j < inventoryCount - 1; j++) {
                inventory[j] = inventory[j + 1];
            }
            inventoryCount--;
            cout << "Removed " << itemName << " from inventory.\n";
            return true;
        }
    }
    cout << "Item " << itemName << " not found.\n";
    return false;
}

// Purpose: Use class-specific skill
void Player::useSkill(Character& enemy) {
    if (isOnCooldown()) {
        cout << "Skill on cooldown!\n";
        return;
    }
    if (mana < 20) {
        cout << "Not enough mana!\n";
        return;
    }
    useMana(20);
    skillCooldown = 2;
    int damage = 0;
    string skillName;
    if (classType == "Warrior") {
        damage = attack * 2;
        skillName = "Power Strike";
    }
    else if (classType == "Mage") {
        damage = attack * 1.5;
        skillName = "Fireball";
    }
    else if (classType == "Rogue") {
        damage = attack * 1.8;
        skillName = "Backstab";
    }
    cout << "Used " << skillName << "!\n";
    enemy.takeDamage(damage);
    addBattleLog(name + " used " + skillName + " on " + enemy.getName() + " for " + to_string(damage) + " damage.");
}

// Purpose: Use an item from inventory
void Player::useItem(int itemIndex, Character& target) {
    if (itemIndex < 1 || itemIndex > inventoryCount) {
        throw invalid_argument("Invalid item index!");
    }
    Item& item = inventory[itemIndex - 1];
    if (item.getCategory() == "Potion") {
        if (item.getName().find("Revive") != string::npos && target.getHealth() == 0) {
            target.heal(50);
            cout << "Revived " << target.getName() << "!\n";
        }
        else if (item.getName().find("Health") != string::npos) {
            target.heal(item.getEffectValue());
        }
        else if (item.getName().find("Buff") != string::npos) {
            attack += item.getEffectValue();
            cout << "Applied buff: +" << item.getEffectValue() << " attack!\n";
            addBattleLog(name + " used " + item.getName() + ", gained +" + to_string(item.getEffectValue()) + " attack.");
        }
        removeItem(item.getName());
        addBattleLog(name + " used " + item.getName() + " on " + target.getName() + ".");
    }
    else if (item.getCategory() == "Weapon" || item.getCategory() == "Armor") {
        attack += item.getEffectValue();
        cout << "Equipped " << item.getName() << ", attack increased by " << item.getEffectValue() << "!\n";
        removeItem(item.getName());
        addBattleLog(name + " equipped " + item.getName() + ".");
    }
    else {
        cout << "Cannot use this item!\n";
    }
}

// Purpose: Purchase an item
void Player::buyItem(const Item& item) {
    int cost = item.getEffectValue() * 10;
    if (currency < cost) {
        cout << "Not enough currency!\n";
        return;
    }
    if (addItem(item)) {
        currency -= cost;
        cout << "Purchased " << item.getName() << " for " << cost << " gold.\n";
        addBattleLog(name + " bought " + item.getName() + " for " + to_string(cost) + " gold.");
    }
}

// Purpose: Activate defensive skill
void Player::defensiveSkill() {
    if (mana < 15) {
        cout << "Not enough mana!\n";
        return;
    }
    useMana(15);
    isDefending = true;
    defense += 10;
    cout << "Activated defensive skill! Defense increased.\n";
    addBattleLog(name + " used defensive skill.");
}

// Purpose: Get total players created
int Player::getPlayerCount() {
    return playerCount;
}

// Purpose: Save game state
void Player::saveGame() const {
    ofstream outFile("savegame.txt");
    if (!outFile) {
        throw runtime_error("Failed to open save file!");
    }
    outFile << name << endl;
    outFile << classType << endl;
    outFile << health << " " << mana << " " << experience << " " << level << " "
        << attack << " " << defense << " " << agility << " " << currency << " "
        << restCooldown << endl;
    outFile << inventoryCount << endl;
    for (int i = 0; i < inventoryCount; i++) {
        outFile << inventory[i].getName() << "|" << inventory[i].getCategory() << "|"
            << inventory[i].getQuantity() << "|" << inventory[i].getEffectValue() << "|"
            << inventory[i].getIsUnique() << "|" << inventory[i].getDescription() << endl;
    }
    outFile << battleHistory->logCount << endl;
    for (int i = 0; i < battleHistory->logCount; i++) {
        outFile << battleHistory->logs[i] << endl;
    }
    outFile.close();
    showAnimation("Saving game");
    cout << "Game saved successfully.\n";
}

// Purpose: Load game state
bool Player::loadGame() {
    ifstream inFile("savegame.txt");
    if (!inFile) {
        cout << "No save file found.\n";
        return false;
    }
    string tempName, tempClass, line;
    int tempHealth, tempMana, tempExp, tempLevel, tempAttack, tempDefense, tempAgility, tempCurrency, tempCount, tempLogCount, tempRestCooldown;

    getline(inFile, tempName);
    getline(inFile, tempClass);
    inFile >> tempHealth >> tempMana >> tempExp >> tempLevel >> tempAttack >> tempDefense >> tempAgility >> tempCurrency >> tempRestCooldown;
    inFile >> tempCount;
    inFile.ignore();

    if (tempName.length() > MAX_NAME_LENGTH || tempCount > inventoryCapacity) {
        inFile.close();
        throw runtime_error("Invalid save file data!");
    }

    name = tempName;
    classType = tempClass;
    health = tempHealth;
    mana = tempMana;
    experience = tempExp;
    level = tempLevel;
    attack = tempAttack;
    defense = tempDefense;
    agility = tempAgility;
    currency = tempCurrency;
    restCooldown = tempRestCooldown;
    inventoryCount = 0;
    isDefending = false;
    skillCooldown = 0;

    delete[] inventory;
    inventory = new Item[inventoryCapacity];
    delete battleHistory;
    battleHistory = new BattleLog(MAX_BATTLE_LOG);

    for (int i = 0; i < tempCount; i++) {
        string itemName, category, desc;
        int qty, effect;
        bool unique;
        getline(inFile, line);                                              //line = "Health Potion|Potion|2|30|0|Restores 30 health".
                                                                                                    //Hero
                                                                                                     //Warrior
                                                                                         // 120 50 150 2 15 8 10 100 0
                                                                                                //2
                                                                         //Health Potion | Potion | 2 | 30 | 0 | Restores 30 health
                                                                             //Quest Key | Quest | 1 | 0 | 1 | Unlocks areas
        size_t pos = 0;
        itemName = line.substr(0, pos = line.find("|"));
        line.erase(0, pos + 1);
        category = line.substr(0, pos = line.find("|"));
        line.erase(0, pos + 1);
        qty = stoi(line.substr(0, pos = line.find("|")));
        line.erase(0, pos + 1);
        effect = stoi(line.substr(0, pos = line.find("|")));
        line.erase(0, pos + 1);
        unique = stoi(line.substr(0, pos = line.find("|")));
        line.erase(0, pos + 1);
        desc = line;
        try {
            addItem(Item(itemName, category, qty, effect, unique, desc));
        }
        catch (const invalid_argument& e) {
            inFile.close();
            throw runtime_error("Invalid item data in save file!");
        }
    }

    inFile >> tempLogCount;
    inFile.ignore();
    for (int i = 0; i < tempLogCount; i++) {
        getline(inFile, line);
        battleHistory->addLog(line);
    }
    inFile.close();
    showAnimation("Loading game");
    cout << "Game loaded successfully.\n";
    return true;
}

// Purpose: Add battle log entry
void Player::addBattleLog(const string& log) {
    battleHistory->addLog(log);
}

// Purpose: View battle history
void Player::viewBattleHistory() const {
    battleHistory->display();
}

// Purpose: Check if skill is on cooldown
bool Player::isOnCooldown() const {
    return skillCooldown > 0;
}

// Purpose: Decrement skill cooldown
void Player::resetCooldown() {
    if (skillCooldown > 0) skillCooldown--;
    if (restCooldown > 0) restCooldown--;
}

// Purpose: Get inventory array
Item* Player::getInventory() const {
    return inventory;
}

// Purpose: Get inventory item count
int Player::getInventoryCount() const {
    return inventoryCount;
}

// Purpose: Add currency
void Player::addCurrency(int amount) {
    currency += amount;
}

// Purpose: Get currency
int Player::getCurrency() const {
    return currency;
}

// Purpose: Set defending state
void Player::setDefending(bool state) {
    isDefending = state;
    if (!isDefending) {
        defense -= 10; // Reset defense boost
        if (defense < 0) defense = 0;
    }
}

// Purpose: Get rest cooldown
int Player::getRestCooldown() const {
    return restCooldown;
}

// Purpose: Set rest cooldown
void Player::setRestCooldown(int value) {
    restCooldown = value;
}

// Purpose: Rest to restore health and mana
void Player::rest() {
    if (restCooldown > 0) {
        cout << "Rest is on cooldown! (" << restCooldown << " turns remaining)\n";
        return;
    }
    heal(50);
    mana += 20;
    int maxMana = 50 + level * 10;
    if (mana > maxMana) mana = maxMana;
    cout << name << " restored 20 mana. Mana: " << mana << endl;
    restCooldown = 3;
    addBattleLog(name + " rested, restored 50 health and 20 mana.");
}

// Purpose: Operator overloading for player output
ostream& operator<<(ostream& os, const Player& player) {
    os << "Player: " << player.name << " (" << player.classType << ")\n";
    player.displayStats();
    return os;
}
