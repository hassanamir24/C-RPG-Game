#include "Character.h"
#include <iostream>
#include "Constants.h"  
using namespace std;

// initial initialisation using default constructor
Character::Character() : name("Unknown"), health(100), mana(50), experience(0), level(1), attack(10), defense(5), agility(10) {}

Character::Character(const string& n, int hp, int mp, int atk, int def, int agi)
    : name(n), health(hp), mana(mp), experience(0), level(1), attack(atk), defense(def), agility(agi) {}
void Character::displayStats() const {
    std::cout << "Name: " << name << "\nHealth: " << health << "\nMana: " << mana
        << "\nLevel: " << level << "\nExperience: " << experience
        << "\nAttack: " << attack << "\nDefense: " << defense << "\nAgility: " << agility << endl;
}

//getters
const string& Character::getName() const {
    return name; 
}
int Character::getHealth() const { 
    return health;
}
int Character::getMana() const {
    return mana;
}
int Character::getExperience() const {
    return experience; 
}
int Character::getLevel() const {
    return level;
}
int Character::getAgility() const { // defines the movement of the player as one of its feature
    return agility;
}

void Character::takeDamage(int damage) {
    int actualDamage = damage - defense;                                        // yahan par jitni defense hogi like warrior ki 
                                                                                  // phir damage - defence se actual damage cal
    if (actualDamage < 0) 
        actualDamage = 0;
    health -= actualDamage;
    if (health < 0) health = 0;
    cout << name << " takes " << actualDamage << " damage! Health: " << health << endl;
}

void Character::gainExperience(int xp) {
    experience += xp;
    if (experience >= level * 100) {        // ie if level = 1 then player would need 100 exp to move to next level and will be upgraded.
        level++;
        health += 20;
        mana += 10;
        attack += 5;
        defense += 3;
        agility += 2;
        std::cout << name << " leveled up to Level " << level << "!\n";
        if (level == 5 )//&& name.find("Player") != string::npos)
        {
            cout << "Unlocked advanced class evolution!\n";
        }
    }
}

void Character::heal(int amount) {
    health += amount;                                               // to revive the player's health
    int maxHealth = 100 + level * 20;
    if (health > maxHealth) health = maxHealth;
    cout << name << " healed for " << amount << ". Health: " << health << endl;
}

void Character::useMana(int amount) {
    mana -= amount;                                         // if a character uses its magical power it will be reset accrodingly but will not be negative
    if (mana < 0) mana = 0;
}