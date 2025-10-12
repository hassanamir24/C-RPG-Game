#pragma once
#include "Item.h"  // Include the existing Item class
#include <string>

// Animation function declaration
void showAnimation(const string& message, int dots = 3, int delayMs = 500);

// Random item generator declaration
Item generateRandomItem();