#pragma once
#include <string>
using namespace std;


class Item {
private:
    string itemName;
    string category;
    int quantity;
    int effectValue;
    bool isUnique;
    std::string description;

public:
    Item();
    Item(const string& name, const string& cat, int qty, int effect, bool unique, const string& desc);
    Item(const Item& other);
    ~Item();
    void display() const;
    const string& getName() const;
    const string& getCategory() const;
    int getQuantity() const;
    int getEffectValue() const;
    bool getIsUnique() const;
    const string& getDescription() const;
};