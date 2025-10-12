#pragma once
#include "Player.h"
#include "MapArea.h"
#include <string>

class Game {
private:
    Player* player;
    MapArea* map;
    int currentArea;
    bool isRunning;

public:
    Game();
    ~Game();
    void start();
    void createPlayer();
    void mainMenu();
    void fightEnemy(bool isBoss = false);
    void exploreMap();
    void shop();
    void loadGameData();
    void finalScoreReport() const;
};