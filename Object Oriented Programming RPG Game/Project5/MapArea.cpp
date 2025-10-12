#include "MapArea.h"
#include <iostream>
#include <string>
using namespace std;


//Default Constructor
MapArea::MapArea() : areaName("Unknown"), requiredLevel(1), isLocked(true), description("None") {}


//Parameteried Constructor
MapArea::MapArea(const string& name, int level, bool locked, const string& desc)
    : areaName(name), requiredLevel(level), isLocked(locked), description(desc) {}


//Display all the location details
void MapArea::display() const {
    cout << areaName << " (Req Level: " << requiredLevel << ")";
    if (isLocked) cout << " [Locked]";
    cout << "\nDesc: " << description << endl;
}


// Name of area
const string& MapArea::getName() const {
    return areaName;
}
// Level of the areas
int MapArea::getRequiredLevel() const { 
    return requiredLevel;
}

//tells if it is locked or unlocked
bool MapArea::getIsLocked() const {
    return isLocked; 
}
void MapArea::unlock() {
    isLocked = false; 
}