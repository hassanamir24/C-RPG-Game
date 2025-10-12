#include "Enemy.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;


Enemy::Enemy() : Character(), enemyType("Goblin"), isBoss(false), phase(1) {}// first initialised to Goblin as default and phase = 1

Enemy::Enemy(const string& type, int playerLevel, bool boss)
    : Character(type, 80 + playerLevel * 10, 30, 8 + playerLevel * 2, 3 + playerLevel, 8),      //Passes type as the name (e.g., "Dragon").
                                                                                             //Sets health = 80 + playerLevel * 10 (e.g. for playerLevel = 2, health = 100).
                                                                                                //Sets mana = 30 (fixed, as enemies don’t use mana in this game).
                                                                                                //Sets attack = 8 + playerLevel * 2 (e.g. 12 for playerLevel = 2).
                                                                                                      //Sets defense = 3 + playerLevel(e.g., 5 for playerLevel = 2).
                                                                                                     //Sets agility = 8 (fixed, determines turn order in combat).
                                                                                                            //Other Character attributes(e.g. experience = 0, level = 1) are set by Character.
    enemyType(type), isBoss(boss), phase(1) {
    if (isBoss) {
        health *= 2;
        attack *= 1.5;
        defense *= 1.5;
    }
}

Enemy::~Enemy() {}

void Enemy::displayStats() const {
    cout << "Enemy Stats:\n";
    Character::displayStats();
    cout << "Type: " << enemyType;
    if (isBoss) cout << " [Boss, Phase " << phase << "]";
    cout << endl;
}

int Enemy::calculateDamage() const {
    int damage = attack + (rand() % 5);     // damage ko unpredictable bhanaya hai taake har damage different hou like 10,11,12
    if (isBoss) {
        if (phase == 2) damage *= 1.2;      // yahan par phase increment mein damage bhi increase hoga
        else if (phase == 3) damage *= 1.5;
    }
    return damage;
}

bool Enemy::isBossEnemy() const {
    return isBoss;
}

void Enemy::nextPhase() {
    if (isBoss && phase < 3) {
        phase++;
        health += 50;
        attack += 5;
        defense += 3;
        cout << enemyType << " enters Phase " << phase << "!\n";
    }
}

int Enemy::getPhase() const {
    return phase;
}