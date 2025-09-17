#ifndef CHECKOUT_H
#define CHECKOUT_H

#include <string>
#include <vector>
#include "product.h"
using namespace std;

struct CartItem {
    Product product;
    int quantity;
    CartItem(Product p, int q) : product(p), quantity(q) {}
};

class Checkout {
public:
    static void processOrder(const vector<CartItem>& cart);
};

#endif
