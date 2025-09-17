#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
using namespace std;

class Product {
private:
    int id;
    string name;
    double price;
    int stock;

public:
    Product(int pid, string pname, double pprice, int pstock);

    int getId() const;
    string getName() const;
    double getPrice() const;
    int getStock() const;

    void reduceStock(int qty);
    void display() const;
};

#endif
