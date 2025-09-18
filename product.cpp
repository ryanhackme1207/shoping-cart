// product.cpp
#include "product.h"
#include <iostream>
#include <iomanip>
using namespace std;

Product::Product(int id, string name, double price, int stock) 
    : id(id), name(name), price(price), stock(stock) {}

int Product::getId() const { return id; }
string Product::getName() const { return name; }
double Product::getPrice() const { return price; }
int Product::getStock() const { return stock; }

void Product::reduceStock(int qty) {
    stock -= qty;
}

void Product::display() const {
    cout << id << " - " << name << " (RM " << price << ") [Stock: " << stock << "]" << endl;
}