// menu.h
#ifndef MENU_H
#define MENU_H

#include "product.h"
#include "cartitem.h"
#include "checkout.h"
#include "auth.h"
#include <vector>
using namespace std;

class Menu {
public:
    static void showMenu(vector<Product>& products, vector<CartItem>& cart);
};

#endif