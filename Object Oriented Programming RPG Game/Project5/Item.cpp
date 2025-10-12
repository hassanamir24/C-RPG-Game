#include "Item.h"
#include <iostream>
using namespace std;

#include "Constants.h"  // Add this line

Item::Item() : itemName("Empty"), category("None"), quantity(0), effectValue(0), isUnique(false), description("None") {}

Item::Item(const std::string& name, const string& cat, int qty, int effect, bool unique, const string& desc)
    : itemName(name), category(cat), quantity(qty), effectValue(effect), isUnique(unique), description(desc) {}

Item::Item(const Item& other)
    : itemName(other.itemName), category(other.category), quantity(other.quantity),
    effectValue(other.effectValue), isUnique(other.isUnique), description(other.description) {}

Item::~Item() {}

void Item::display() const {
    cout << itemName << " [" << category << "] (Qty: " << quantity << ", Effect: " << effectValue << ")";
    if (isUnique) cout << " [Unique]";
    cout << "\nDesc: " << description << endl;
}

const string& Item::getName() const { return itemName; }
const string& Item::getCategory() const { return category; }
int Item::getQuantity() const { return quantity; }
int Item::getEffectValue() const { return effectValue; }
bool Item::getIsUnique() const { return isUnique; }
const string& Item::getDescription() const { return description; }