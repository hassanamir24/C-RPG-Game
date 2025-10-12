#pragma once
#include <string>



using namespace std;
class MapArea {
private:
    string areaName;
    int requiredLevel;
    bool isLocked;
    string description;

public:

    MapArea();// default constructor
    MapArea(const string& name, int level, bool locked, const string& desc); // parameterised constructor
    void display() const;
    void unlock();// unlocks the area if conditions fulfilled in game()
    
    
    // getters
    const string& getName() const;
    int getRequiredLevel() const;
    bool getIsLocked() const;
    
};