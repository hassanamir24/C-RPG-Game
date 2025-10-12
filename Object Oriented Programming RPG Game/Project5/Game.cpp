#include "Game.h"
#include "Enemy.h"
#include <iostream>
#include <fstream>
#include "Utilities.h"
#include"Constants.h"
#include <string>
#include <windows.h>
#include <cstdlib>
#include <ctime>
using namespace std;
Game::Game() : isRunning(false), currentArea(0) {
    srand(static_cast<unsigned>(time(nullptr)));
    player = nullptr;
    map = new MapArea[MAX_MAP_AREAS];
    loadGameData();
}

Game::~Game() {
    delete player;
    delete[] map;
}

void Game::start() {
    isRunning = true;
    cout << "Welcome to Legends of Valor!\n";
    createPlayer();
    mainMenu();
}

// ... (rest of Game member functions)
// Purpose: Create or load player
void Game::createPlayer() {
    string choice;
    cout << "1. New Game\n2. Load Game\nChoose an option: ";
    getline(cin, choice);
    try {
        if (choice == "1") {
            string name, classType;
            cout << "Enter player name: ";
            getline(cin, name);
            cout << "Choose class (Warrior/Mage/Rogue): ";
            getline(cin, classType);
            delete player;
            player = new Player(name, classType);
            player->addItem(Item("Health Potion", "Potion", 2, 30, false, "Restores 30 health"));
            player->addItem(Item("Quest Key", "Quest", 1, 0, true, "Unlocks areas"));
            system("cls");
            showAnimation("Creating player");
            cout << "Player created successfully!\n";
        }
        else if (choice == "2") {
            delete player;
            player = new Player();
            system("cls");
            showAnimation("Loading game");
            if (!player->loadGame()) {
                cout << "Starting new game instead.\n";
                createPlayer();
            }
        }
        else {
            throw invalid_argument("Invalid choice!");
        }
    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << "\nTry again.\n";
        createPlayer();
    }
}

// Purpose: Display main menu and handle input
void Game::mainMenu() {
    while (isRunning) {
        player->resetCooldown();
        if (player->isDefending) {
            player->setDefending(false);
        }
        cout << "==============================================" << endl;
        cout << "|                  MAIN MENU                 |" << endl;
        cout << "==============================================" << endl;
        cout << "| 1. Show Stats                              |" << endl;
        cout << "| 2. Show Inventory                          |" << endl;
        cout << "| 3. Add Item                                |" << endl;
        cout << "| 4. Remove Item                             |" << endl;
        cout << "| 5. Explore Map                             |" << endl;
        cout << "| 6. Shop                                    |" << endl;
        cout << "| 7. View Battle History                     |" << endl;
        cout << "| 8. Save Game                               |" << endl;
        cout << "| 9. Quit                                    |" << endl;
        cout << "==============================================" << endl;
        cout << "Choose an option: ";
        string choice;
        getline(cin, choice);
        try {
            if (choice == "1") {
                system("cls");
                showAnimation("Displaying stats");
                cout << *player;
            }
            else if (choice == "2") {
                system("cls");
                showAnimation("Opening inventory");
                player->showInventory();
            }
            else if (choice == "3") {
                system("cls");
                showAnimation("Adding item");
                string name, category, desc;
                int qty, effect;
                bool unique;
                cout << "Enter item name: ";
                getline(cin, name);
                cout << "Enter category (Potion/Weapon/Armor/Quest): ";
                getline(cin, category);
                cout << "Enter quantity: ";
                cin >> qty;
                cout << "Enter effect value: ";
                cin >> effect;
                cout << "Is unique? (1 for yes, 0 for no): ";
                cin >> unique;
                cout << "Enter description: ";
                cin.ignore();
                getline(cin, desc);
                player->addItem(Item(name, category, qty, effect, unique, desc));
            }
            else if (choice == "4") {
                system("cls");
                showAnimation("Removing item");
                string name;
                cout << "Enter item name to remove: ";
                getline(cin, name);
                player->removeItem(name);
            }
            else if (choice == "5") {
                system("cls");
                showAnimation("Preparing to explore");
                exploreMap();
            }
            else if (choice == "6") {
                system("cls");
                showAnimation("Entering shop");
                shop();
            }
            else if (choice == "7") {
                system("cls");
                showAnimation("Viewing battle history");
                player->viewBattleHistory();
            }
            else if (choice == "8") {
                system("cls");
                player->saveGame();
            }
            else if (choice == "9") {
                system("cls");
                showAnimation("Preparing to quit");
                cout << "Save before quitting? (y/n): ";
                getline(cin, choice);
                if (choice == "y" || choice == "Y") {
                    player->saveGame();
                }
                finalScoreReport();
                isRunning = false;
                cout << "Thanks for playing!\n";
            }
            else {
                throw invalid_argument("Invalid choice!");
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
}

// Purpose: Simulate combat with an enemy
void Game::fightEnemy(bool isBoss) {
    string enemyTypes[] = { "Goblin", "Skeleton", "Wolf", "Troll" };
    string enemyType = isBoss ? "Dragon" : enemyTypes[rand() % 4];
    Enemy* enemy = new Enemy(enemyType, player->getLevel(), isBoss);
    system("cls");
    showAnimation("Encountering enemy");
    cout << "\nEncountered a " << enemyType << "!\n";
    if (isBoss) {



        cout << "  _____  _____   ___   ____   ____   _   _ \n";
        cout << " |  __ \\|  __ \\ / _ \\ |  _ \\ |  _ \\ | \\ | |\n";
        cout << " | |  | | |  | | | | || | | || | | ||  \\| |\n";
        cout << " | |  | | |  | | | | || | | || | | || . ` |\n";
        cout << " | |__| | |__| | |_| || |_| || |_| || |\\  |\n";
        cout << " |_____/|_____/ \\___/ |____/ |____/ |_| \\_|\n";
        cout << "           FINAL BOSS                 \n";



    }
    enemy->displayStats();
    player->addBattleLog("Encountered " + enemyType + " (Level " + to_string(enemy->getLevel()) + ").");

    bool playerTurn = player->getAgility() >= enemy->getAgility();
    while (player->getHealth() > 0 && enemy->getHealth() > 0) {
        if (playerTurn) {
            cout << "\nYour turn:\n1. Attack\n2. Use Skill\n3. Use Item\n4. Defend\nChoose an action: ";
            string choice;
            getline(cin, choice);
            try {
                if (choice == "1") {
                    int damage = player->calculateDamage();
                    enemy->takeDamage(damage);
                    player->addBattleLog("Attacked " + enemyType + " for " + to_string(damage) + " damage.");
                }
                else if (choice == "2") {
                    player->useSkill(*enemy);
                }
                else if (choice == "3") {
                    player->showInventory();
                    cout << "Enter item number to use: ";
                    int itemNum;
                    cin >> itemNum;
                    cin.ignore();
                    player->useItem(itemNum, *player);
                }
                else if (choice == "4") {
                    player->defensiveSkill();
                }
                else {
                    throw invalid_argument("Invalid action!");
                }
            }
            catch (const exception& e) {
                cout << "Error: " << e.what() << "\n";
                continue;
            }
        }
        else {
            if (enemy->getHealth() > 0) {
                int damage = enemy->calculateDamage();
                if (player->isDefending) {
                    damage /= 2; // Halve damage when defending
                }
                player->takeDamage(damage);
                player->addBattleLog(enemyType + " attacked for " + to_string(damage) + " damage.");
                if (enemy->isBossEnemy() && enemy->getHealth() < enemy->getLevel() * 50 && enemy->getPhase() < 3) {
                    enemy->nextPhase();
                    player->addBattleLog(enemyType + " entered Phase " + to_string(enemy->getPhase()) + ".");
                }
            }
        }
        playerTurn = !playerTurn;
    }

    if (player->getHealth() <= 0) {
        cout << "You have been defeated!\n";
        player->addBattleLog("Defeated by " + enemyType + ".");
        finalScoreReport();
        isRunning = false;
    }
    else {
        cout << enemyType << " defeated!\n";
        int xp = isBoss ? 200 : 50 + enemy->getLevel() * 10;
        player->gainExperience(xp);
        player->addCurrency(isBoss ? 100 : 20);
        cout << "Gained " << xp << " XP and " << (isBoss ? 100 : 20) << " gold!\n";
        player->addBattleLog("Defeated " + enemyType + ", gained " + to_string(xp) + " XP and " + to_string(isBoss ? 100 : 20) + " gold.");
        if (rand() % 100 < (isBoss ? 100 : 50)) {
            Item loot = isBoss ? Item("Legendary Sword", "Weapon", 1, 20, true, "A powerful sword")
                : Item("Health Potion", "Potion", 1, 30, false, "Restores 30 health");
            player->addItem(loot);
            player->addBattleLog("Found " + loot.getName() + ".");
        }
        if (isBoss && currentArea < MAX_MAP_AREAS - 1) {
            map[currentArea + 1].unlock();
            cout << map[currentArea + 1].getName() << " unlocked!\n";
        }
    }
    delete enemy;
}


// Purpose: Explore the map with player choices
void Game::exploreMap() {
    cout << "\nAvailable Areas:\n";
    for (int i = 0; i < MAX_MAP_AREAS; i++) {
        cout << i + 1 << ". ";
        map[i].display();
    }
    cout << "Choose an area (1-" << MAX_MAP_AREAS << "): ";
    int choice;
    cin >> choice;
    cin.ignore();
    if (choice < 1 || choice > MAX_MAP_AREAS) {
        cout << "Invalid area!\n";
        return;
    }
    currentArea = choice - 1;
    if (map[currentArea].getIsLocked()) {
        bool hasKey = false;
        for (int i = 0; i < player->getInventoryCount(); i++) {
            if (player->getInventory()[i].getName() == "Quest Key") {
                hasKey = true;
                break;
            }
        }
        if (!hasKey || player->getLevel() < map[currentArea].getRequiredLevel()) {
            cout << "Area locked! Required level: " << map[currentArea].getRequiredLevel() << " and Quest Key.\n";
            return;
        }
    }
    system("cls");
    showAnimation("Entering " + map[currentArea].getName());
    player->addBattleLog("Entered " + map[currentArea].getName() + ".");
    cout << "\nYou are in " << map[currentArea].getName() << ".\n";
    cout << "What would you like to do?\n"
        << "1. Collect Item\n2. Start Quest\n3. Rest\n4. Fight Enemies\n5. Return to Menu\n"
        << "Choose an option: ";
    string action;
    getline(cin, action);
    system("cls");
    try {
        if (action == "1") {
            showAnimation("Searching for items");
            Item randomItem = generateRandomItem();
            if (player->addItem(randomItem)) {
                player->addBattleLog("Collected " + randomItem.getName() + " in " + map[currentArea].getName() + ".");
                cout << "Found and collected: " << randomItem.getName() << "!\n";
            }
        }
        else if (action == "2") {
            showAnimation("Seeking a quest");
            Item questItem = Item("Ancient Relic", "Quest", 1, 0, true, "A relic tied to " + map[currentArea].getName());
            if (player->addItem(questItem)) {
                player->addBattleLog("Started quest with " + questItem.getName() + " in " + map[currentArea].getName() + ".");
                cout << "Accepted quest: Obtained " << questItem.getName() << "!\n";
                player->gainExperience(30);
                cout << "Gained 30 XP for starting the quest!\n";
            }
        }
        else if (action == "3") {
            showAnimation("Finding a place to rest");
            player->rest();
        }
        else if (action == "4") {
            showAnimation("Looking for enemies");
            int event = rand() % 100;
            if (event < 70) {
                fightEnemy(false); // Normal enemy
            }
            else {
                fightEnemy(true); // Boss fight
            }
        }
        else if (action == "5") {
            showAnimation("Returning to menu");
            cout << "Returning to main menu.\n";
        }
        else {
            throw invalid_argument("Invalid action!");
        }
    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << "\n";
    }
}

// Purpose: Shop interface
void Game::shop() {
    Item shopItems[] = {
        Item("Health Potion", "Potion", 1, 30, false, "Restores 30 health"),
        Item("Attack Buff", "Potion", 1, 5, false, "Increases attack by 5"),
        Item("Iron Sword", "Weapon", 1, 15, false, "A sturdy sword"),
        Item("Mythril Armor", "Armor", 1, 10, true, "Light and strong armor"),
        Item("Revive Potion", "Potion", 1, 50, true, "Revives with 50 health")
    };
    cout << "\nWelcome to the Shop!\n";
    for (int i = 0; i < 5; i++) {
        cout << i + 1 << ". " << shopItems[i].getName() << " (" << shopItems[i].getEffectValue() * 10 << " gold)\n";
        shopItems[i].display();
    }
    cout << "Your gold: " << player->getCurrency() << "\nChoose an item to buy (1-5, 0 to exit): ";
    int choice;
    cin >> choice;
    cin.ignore();
    if (choice == 0) return;
    if (choice < 1 || choice > 5) {
        cout << "Invalid choice!\n";
        return;
    }
    player->buyItem(shopItems[choice - 1]);
}

// Purpose: Load initial game data
void Game::loadGameData() {
    map[0] = MapArea("Forest", 1, false, "A lush green forest");
    map[1] = MapArea("Cave", 3, true, "A dark and dangerous cave");
    map[2] = MapArea("Town", 2, true, "A bustling town");
    map[3] = MapArea("Mountain", 4, true, "A treacherous mountain");
    map[4] = MapArea("Castle", 5, true, "A fortified castle");
}

// Purpose: Generate final score report
void Game::finalScoreReport() const {
    int areasExplored = 0;
    for (int i = 0; i < MAX_MAP_AREAS; i++) {
        if (!map[i].getIsLocked()) areasExplored++;
    }
    system("cls");
    showAnimation("Generating score report");
    cout << "\nFinal Score Report:\n";
    cout << "Player: " << player->getName() << " (Level " << player->getLevel() << ")\n";
    cout << "Total XP: " << player->getExperience() << "\n";
    cout << "Gold Collected: " << player->getCurrency() << "\n";
    cout << "Battles Fought: " << player->battleHistory->logCount << "\n";
    cout << "Items Owned: " << player->getInventoryCount() << "\n";
    cout << "Areas Explored: " << areasExplored << "/" << MAX_MAP_AREAS << "\n";
    ofstream outFile("score_report.txt");
    if (outFile) {
        outFile << "Final Score Report\n";
        outFile << "Player: " << player->getName() << " (Level " << player->getLevel() << ")\n";
        outFile << "Total XP: " << player->getExperience() << "\n";
        outFile << "Gold Collected: " << player->getCurrency() << "\n";
        outFile << "Battles Fought: " << player->battleHistory->logCount << "\n";
        outFile << "Items Owned: " << player->getInventoryCount() << "\n";
        outFile << "Areas Explored: " << areasExplored << "/" << MAX_MAP_AREAS << "\n";
        outFile.close();
        cout << "Report saved to score_report.txt\n";
    }
    else {
        cout << "Failed to save score report.\n";
    }
}