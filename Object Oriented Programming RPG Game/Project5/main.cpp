#include "Game.h"
#include "Player.h"  
#include <iostream>

using namespace std;

int main() {
    Game game;
    game.start();
    cout << "Total players created: " << Player::getPlayerCount() << endl;
    return 0;
}