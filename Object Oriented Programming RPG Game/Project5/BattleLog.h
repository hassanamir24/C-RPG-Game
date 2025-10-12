#pragma once
#include <string>

using namespace std;

class BattleLog {
public:
    string* logs;
    int logCount;
    int logCapacity;

public:
    BattleLog(int capacity);
    BattleLog(const BattleLog& other);
    ~BattleLog();
    void addLog(const string& log);
    void display() const;
};