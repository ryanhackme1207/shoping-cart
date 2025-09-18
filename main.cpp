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
            Product(1, "Laptop", 2500.0, 5, "Electronics"),
            Product(2, "Mouse", 50.0, 10, "Computer Accessories"),
            Product(3, "Keyboard", 150.0, 7, "Computer Accessories"),
            Product(4, "Microwave", 400.0, 3, "Kitchen"),
            Product(5, "Headphones", 200.0, 6, "Electronics"),
            Product(6, "Frying Pan", 80.0, 12, "Kitchen")
        };

            
            vector<CartItem> cart;
            
            // Show main shopping menu
            Menu::showMenu(products, cart);
        }
    }
    
    return 0;
}