#ifndef MENU_H
#define MENU_H

#include <vector>
#include "product.h"
#include "checkout.h"

class Menu {
public:
    static void showMenu(vector<Product>& products, vector<CartItem>& cart);
};

#endif
