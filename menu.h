#ifndef MENU_H
#define MENU_H

#include "product.h"
#include "cartitem.h"
#include "auth.h"
#include "checkout.h"
#include <vector>
using namespace std;

class Menu {
public:
    static void showMenu(vector<Product>& products, vector<CartItem>& cart);
};

#endif
