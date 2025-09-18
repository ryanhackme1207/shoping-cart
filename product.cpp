#include "product.h"
#include <iostream>
#include <iomanip>
using namespace std;

Product::Product(int id, string name, double price, int stock, string category) 
    : id(id), name(name), price(price), stock(stock), category(category) {}

int Product::getId() const { return id; }
string Product::getName() const { return name; }
double Product::getPrice() const { return price; }
int Product::getStock() const { return stock; }
string Product::getCategory() const { return category; }

void Product::reduceStock(int qty) {
    stock -= qty;
}

void Product::display() const {
    cout << " | " << setw(4) << id
         << " | " << setw(23) << left << name
         << " | " << setw(12) << left << category
         << " | " << setw(7) << right << stock
         << " | RM " << setw(6) << fixed << setprecision(2) << price << " |" << endl;
}
