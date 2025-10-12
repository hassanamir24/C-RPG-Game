#pragma once
#include <string>
using namespace std;


class Character {
protected:
    string name;
    int health;
    int mana;
    int experience;
    int level;
    int attack;
    int defense;
    int agility;

public:
    Character();// default constructor
    Character(const string& n, int hp, int mp, int atk, int def, int agi);// parameterised constructor
    virtual ~Character() {}// virtual destructor (polymorphism)


    virtual void displayStats() const; // polymorphism
    virtual int calculateDamage() const = 0;// data abstraction 

    const string& getName() const;
    int getHealth() const;
    int getMana() const;
    int getExperience() const;
    int getLevel() const;
    int getAgility() const;


    void takeDamage(int damage);
    void gainExperience(int xp);
    void heal(int amount);
    void useMana(int amount);
};