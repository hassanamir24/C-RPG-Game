#pragma once
#include "Character.h"
#include <string>

class Enemy : public Character { // inherited from the character class
private:
    string enemyType;
    bool isBoss;
    int phase;                  // to decide the difficulty level  of enemy

public:
    Enemy();// default constructor

    Enemy(const string& type, int level, bool boss = false); // parameterised constructor
    ~Enemy();// destructor

    void displayStats() const override;
    int calculateDamage() const override;
    bool isBossEnemy() const; // to check if its a pro level enemy amongst all enemies
    void nextPhase();
    int getPhase() const;
};