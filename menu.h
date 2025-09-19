// menu.h
#ifndef MENU_H
#define MENU_H

#include "product.h"
#include "cartitem.h"
#include "auth.h"
#include "checkout.h"
#include "userpreferences.h"
#include <vector>
#include <string>

class Menu {
public:
    static void showMenu(std::vector<Product>& products, std::vector<CartItem>& cart);
    static void searchAndSortMenu(std::vector<Product>& products, std::vector<CartItem>& cart);
};

// Helper function declarations
std::string toLowerCase(const std::string &str);
int getValidInteger(const std::string& prompt);
int getMenuChoice();
int getPositiveInteger(const std::string& prompt);
void addToCartFromSearch(std::vector<Product>& products, std::vector<CartItem>& cart, const std::vector<Product>& displayedProducts);
void displayProductTable(const std::vector<Product>& products);
void showWelcomeWithRecommendations(const std::string& username, const std::vector<Product>& products);

#endif