#include "product.h"
#include <iostream>
using namespace std;

Product::Product(int pid, string pname, double pprice, int pstock) {
    id = pid;
    name = pname;
    price = pprice;
    stock = pstock;
}

int Product::getId() const { return id; }
string Product::getName() const { return name; }
double Product::getPrice() const { return price; }
int Product::getStock() const { return stock; }

void Product::reduceStock(int qty) {
    if (qty <= stock) stock -= qty;
}

void Product::display() const {
    cout << id << " - " << name << " (RM " << price 
         << ") [Stock: " << stock << "]" << endl;
}
