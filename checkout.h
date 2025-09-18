// checkout.h
#ifndef CHECKOUT_H
#define CHECKOUT_H

#include "cartitem.h"
#include <vector>
using namespace std;

class Checkout {
public:
    static void processOrder(vector<CartItem>& cart);
};

#endif