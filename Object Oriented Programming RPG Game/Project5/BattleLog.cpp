#include "BattleLog.h"
#include <iostream>
using namespace std;



BattleLog::BattleLog(int capacity) : logCount(0), logCapacity(capacity) {
    logs = new std::string[logCapacity];
}

BattleLog::BattleLog(const BattleLog& other) : logCount(other.logCount), logCapacity(other.logCapacity) {
    logs = new string[logCapacity];
    for (int i = 0; i < logCount; i++) {
        logs[i] = other.logs[i];
    }
}

BattleLog::~BattleLog() {
    delete[] logs;
}

void BattleLog::addLog(const string& log) {
    if (logCount < logCapacity) {
        logs[logCount++] = log;
    }
    else {
        for (int i = 1; i < logCapacity; i++) {
            logs[i - 1] = logs[i];
        }
        logs[logCapacity - 1] = log;
    }
}

void BattleLog::display() const {
    std::cout << "Battle History:\n";
    if (logCount == 0) {
        std::cout << "No battles recorded.\n";
        return;
    }
    for (int i = 0; i < logCount; i++) {
        std::cout << i + 1 << ". " << logs[i] << std::endl;
    }
}