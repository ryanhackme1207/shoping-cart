// main.cpp
#include "menu.h"
#include "auth.h"
#include "product.h"
#include "cartitem.h"
#include <iostream>
#include <vector>
using namespace std;

int main() {
    while (true) {
        // Check if user is logged in, if not show auth menu
        if (!Auth::isUserLoggedIn()) {
            Auth::showAuthMenu();
        }
        
        // If we reach here, user is logged in
        if (Auth::isUserLoggedIn()) {
            // Initialize products
            vector<Product> products = {
                Product(1, "Laptop", 2500.0, 5),
                Product(2, "Mouse", 50.0, 10),
                Product(3, "Keyboard", 150.0, 7)
            };
            
            vector<CartItem> cart;
            
            // Show main shopping menu
            Menu::showMenu(products, cart);
        }
    }
    
    return 0;
}