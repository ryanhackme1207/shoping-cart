// menu.cpp
#include "menu.h"
#include <iostream>
#include <iomanip>
#include <limits>
using namespace std;

// Function to safely get integer input
int getValidInteger(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        } else {
            cout << R"(
         +----------------------------------------+
         |              ERROR!                    |
         |      Please enter a valid number!     |
         +----------------------------------------+
)" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Function to safely get menu choice (1-8 only)
int getMenuChoice() {
    int choice;
    while (true) {
        choice = getValidInteger("Enter choice: ");
        if (choice >= 1 && choice <= 8) {
            return choice;
        } else {
            cout << R"(
         +----------------------------------------+
         |              ERROR!                    |
         |    Please enter a number between 1-8! |
         +----------------------------------------+
)" << endl;
        }
    }
}

// Function to safely get positive integer
int getPositiveInteger(const string& prompt) {
    int value;
    while (true) {
        value = getValidInteger(prompt);
        if (value > 0) {
            return value;
        } else {
            cout << R"(
         +----------------------------------------+
         |              ERROR!                    |
         |   Please enter a positive number > 0! |
         +----------------------------------------+
)" << endl;
        }
    }
}

void Menu::showMenu(vector<Product>& products, vector<CartItem>& cart) {
    int choice;
    do {
        cout << R"(                                  
  _____|  |__   ____ ______   ____   ____  
 /  ___/  |  \ /  _ \\____ \_/ __ \_/ __ \ 
 \___ \|   Y  (  <_> )  |_>  >  ___/\ ___/ 
/____  >___|  /\____/|   __/ \___ >  \___ >
     \/     \/       |__|  \/      \/  )" << "\n\n";

        cout << "\n--- Shopping Cart Menu ---\n";
        cout << "Welcome, " << Auth::getCurrentUser().fullName << "!\n";
        cout << "1. Show Products\n";
        cout << "2. Add to Cart\n";
        cout << "3. View Cart\n";
        cout << "4. Update Cart Item\n";
        cout << "5. Remove Item from Cart\n";
        cout << "6. Checkout\n";
        cout << "7. Logout\n";
        cout << "8. Exit\n";
        
        choice = getMenuChoice();

        if (choice == 1) {
            cout << R"(
 ===============================================================
 |                    PRODUCT CATALOG                        |
 ===============================================================
)" << endl;
            
            cout << " +------+-------------------------+---------+-----------+-------------+" << endl;
            cout << " |  ID  |      PRODUCT NAME       |  STOCK  |   PRICE   | DESCRIPTION |" << endl;
            cout << " +------+-------------------------+---------+-----------+-------------+" << endl;
            
            for (auto &p : products) {
                cout << " | " << setw(4) << p.getId()
                     << " | " << setw(23) << left << p.getName()
                     << " | " << setw(7) << right << p.getStock()
                     << " | RM " << setw(6) << fixed << setprecision(2) << p.getPrice()
                     << " | " << setw(11) << left << "Available" << " |" << endl;
                cout << " +------+-------------------------+---------+-----------+-------------+" << endl;
            }
            
            cout << "\n >> Enter product ID and quantity to add items to your cart!" << endl;
        }
        else if (choice == 2)
        {
            cout << R"(
 ===============================================================
 |                    ADD TO CART                           |
 ===============================================================
)" << endl;
            
            // Show available products first
            cout << " +------+-------------------------+---------+-----------+" << endl;
            cout << " |  ID  |      PRODUCT NAME       |  STOCK  |   PRICE   |" << endl;
            cout << " +------+-------------------------+---------+-----------+" << endl;
            
            for (auto &p : products) {
                cout << " | " << setw(4) << p.getId()
                     << " | " << setw(23) << left << p.getName()
                     << " | " << setw(7) << right << p.getStock()
                     << " | RM " << setw(6) << fixed << setprecision(2) << p.getPrice() << " |" << endl;
            }
            cout << " +------+-------------------------+---------+-----------+" << endl;
            cout << endl;
            
            int id = getPositiveInteger("Enter Product ID: ");
            int qty = getPositiveInteger("Enter Quantity: ");

            bool found = false;
            for (auto &p : products)
            {
                if (p.getId() == id)
                {
                    if (qty <= p.getStock())
                    {
                        bool inCart = false;
                        for (auto &item : cart)
                        {
                            if (item.product.getId() == id)
                            {
                                item.quantity += qty; // update existing
                                inCart = true;
                                break;
                            }
                        }
                        if (!inCart)
                        {
                            cart.push_back(CartItem(p, qty)); // add new
                        }
                        p.reduceStock(qty);
                        cout << R"(
         +----------------------------------------+
         |             SUCCESS!                   |
         |        Item added to cart!             |
         +----------------------------------------+
)" << endl;
                    }
                    else
                    {
                        cout << R"(
         +----------------------------------------+
         |              ERROR!                    |
         |         Not enough stock!              |
         |     Available: )" << p.getStock() << R"( | Requested: )" << qty << R"(        |
         +----------------------------------------+
)" << endl;
                    }
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << R"(
         +----------------------------------------+
         |              ERROR!                    |
         |        Invalid Product ID!             |
         |     Please check product catalog       |
         +----------------------------------------+
)" << endl;
            }
        }

        else if (choice == 3) {
            cout << R"(
 ===============================================================
 |                    YOUR SHOPPING CART                     |
 ===============================================================
)" << endl;

            if (cart.empty()) {
                cout << R"(
         +----------------------------------------+
         |             CART IS EMPTY              |
         |                                        |
         |         No items in your cart          |
         |          Add some products!            |
         +----------------------------------------+
)" << endl;
            }
            else {
                cout << " +------+-------------------------+---------+-----------+-------------+" << endl;
                cout << " |  ID  |       PRODUCT NAME      |   QTY   |   PRICE   |    TOTAL    |" << endl;
                cout << " +------+-------------------------+---------+-----------+-------------+" << endl;
                
                double grandTotal = 0.0;
                
                for (auto &item : cart) {
                    double itemTotal = item.product.getPrice() * item.quantity;
                    grandTotal += itemTotal;
                    
                    cout << " | " << setw(4) << item.product.getId() 
                         << " | " << setw(23) << left << item.product.getName() 
                         << " | " << setw(7) << right << item.quantity 
                         << " | RM " << setw(6) << fixed << setprecision(2) << item.product.getPrice()
                         << " | RM " << setw(8) << fixed << setprecision(2) << itemTotal << " |" << endl;
                }
                
                cout << " +------+-------------------------+---------+-----------+-------------+" << endl;
                cout << "\n                         +-----------------------+" << endl;
                cout << "                         |  GRAND TOTAL: RM " << setw(6) << fixed << setprecision(2) << grandTotal << " |" << endl;
                cout << "                         +-----------------------+\n" << endl;
                
                cout << " >> Use option 4 to update quantities or option 5 to remove items" << endl;
                cout << " >> Ready to checkout? Choose option 6!" << endl;
            }
        }

        else if (choice == 4)
        {
            cout << R"(
 ===============================================================
 |                   UPDATE CART ITEM                       |
 ===============================================================
)" << endl;
            
            if (cart.empty()) {
                cout << R"(
         +----------------------------------------+
         |              ERROR!                    |
         |            Cart is empty!              |
         |         Add items first!               |
         +----------------------------------------+
)" << endl;
            } else {
                // Show current cart items
                cout << " Current Cart Items:" << endl;
                cout << " +------+-------------------------+---------+" << endl;
                cout << " |  ID  |       PRODUCT NAME      |   QTY   |" << endl;
                cout << " +------+-------------------------+---------+" << endl;
                
                for (auto &item : cart) {
                    cout << " | " << setw(4) << item.product.getId() 
                         << " | " << setw(23) << left << item.product.getName() 
                         << " | " << setw(7) << right << item.quantity << " |" << endl;
                }
                cout << " +------+-------------------------+---------+" << endl;
                cout << endl;
                
                int id = getPositiveInteger("Enter Product ID to update: ");
                int newQty = getPositiveInteger("Enter new quantity: ");

                bool updated = false;
                for (auto &item : cart)
                {
                    if (item.product.getId() == id)
                    {
                        int diff = newQty - item.quantity; // difference in quantity
                        
                        // Find the original product in products vector
                        for (auto &p : products)
                        {
                            if (p.getId() == id)
                            {
                                if (diff > 0) // Need more items
                                {
                                    if (diff <= p.getStock())
                                    {
                                        p.reduceStock(diff); // reduce stock from original product
                                        item.quantity = newQty;
                                        updated = true;
                                    }
                                    else
                                    {
                                        cout << R"(
         +----------------------------------------+
         |              ERROR!                    |
         |         Not enough stock!              |
         +----------------------------------------+
)" << endl;
                                    }
                                }
                                else // Reducing quantity, restore stock
                                {
                                    p.reduceStock(diff); // diff is negative, so this adds stock back
                                    item.quantity = newQty;
                                    updated = true;
                                }
                                break;
                            }
                        }
                        break;
                    }
                }
                if (updated) {
                    cout << R"(
         +----------------------------------------+
         |             SUCCESS!                   |
         |          Cart updated!                 |
         +----------------------------------------+
)" << endl;
                } else {
                    cout << R"(
         +----------------------------------------+
         |              ERROR!                    |
         |       Item not found in cart!         |
         +----------------------------------------+
)" << endl;
                }
            }
        }

        else if (choice == 5)
        {
            cout << R"(
 ===============================================================
 |                 REMOVE ITEM FROM CART                    |
 ===============================================================
)" << endl;
            
            if (cart.empty()) {
                cout << R"(
         +----------------------------------------+
         |              ERROR!                    |
         |            Cart is empty!              |
         |         Nothing to remove!             |
         +----------------------------------------+
)" << endl;
            } else {
                // Show current cart items
                cout << " Current Cart Items:" << endl;
                cout << " +------+-------------------------+---------+" << endl;
                cout << " |  ID  |       PRODUCT NAME      |   QTY   |" << endl;
                cout << " +------+-------------------------+---------+" << endl;
                
                for (auto &item : cart) {
                    cout << " | " << setw(4) << item.product.getId() 
                         << " | " << setw(23) << left << item.product.getName() 
                         << " | " << setw(7) << right << item.quantity << " |" << endl;
                }
                cout << " +------+-------------------------+---------+" << endl;
                cout << endl;
                
                int id = getPositiveInteger("Enter Product ID to remove: ");

                bool removed = false;
                for (auto it = cart.begin(); it != cart.end(); ++it)
                {
                    if (it->product.getId() == id)
                    {
                        // restore stock
                        for (auto &p : products)
                        {
                            if (p.getId() == id)
                            {
                                p.reduceStock(-it->quantity); // negative restores stock
                                break;
                            }
                        }
                        cart.erase(it);
                        removed = true;
                        break;
                    }
                }
                if (removed) {
                    cout << R"(
         +----------------------------------------+
         |             SUCCESS!                   |
         |      Item removed from cart!           |
         +----------------------------------------+
)" << endl;
                } else {
                    cout << R"(
         +----------------------------------------+
         |              ERROR!                    |
         |       Item not found in cart!         |
         +----------------------------------------+
)" << endl;
                }
            }
        }

        else if (choice == 6) {
            cout << R"(
 ===============================================================
 |                      CHECKOUT                            |
 ===============================================================
)" << endl;
            
            if (cart.empty()) {
                cout << R"(
         +----------------------------------------+
         |              ERROR!                    |
         |            Cart is empty!              |
         |      Add items before checkout!        |
         +----------------------------------------+
)" << endl;
            } else {
                Checkout::processOrder(cart);
            }
        }

        else if (choice == 7) {
            Auth::logout();
            return; // Exit to main, which will show auth menu again
        }

    } while (choice != 8);
}
