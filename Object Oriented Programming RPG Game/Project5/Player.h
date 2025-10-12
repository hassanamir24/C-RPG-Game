#pragma once
#include "Character.h"
#include "Item.h"
#include "BattleLog.h"
#include <string>
#include <ostream>

class Player : public Character {
public:
    Item* inventory;
    int inventoryCount;
    int inventoryCapacity;
    int currency;
    static int playerCount;
    string classType;
    bool isDefending;
    BattleLog* battleHistory;
    int skillCooldown;
    int restCooldown;

public:
    Player();
    Player(const string& n, const string& classChoice);
    Player(const Player& other);
    ~Player();
    void displayStats() const override;
    int calculateDamage() const override;
    void showInventory() const;
    bool addItem(const Item& item);
    bool removeItem(const string& itemName);
    void useSkill(Character& enemy);
    void useItem(int itemIndex, Character& target);
    void buyItem(const Item& item);
    void defensiveSkill();
    static int getPlayerCount();
    void saveGame() const;
    bool loadGame();
    void addBattleLog(const string& log);
    void viewBattleHistory() const;
    bool isOnCooldown() const;
    void resetCooldown();
    Item* getInventory() const;
    int getInventoryCount() const;
    void addCurrency(int amount);
    int getCurrency() const;
    void setDefending(bool state);
    int getRestCooldown() const;
    void setRestCooldown(int value);
    void rest();
    friend ostream& operator<<(ostream& os, const Player& player);
};