// checkout.cpp
#include "checkout.h"
#include "auth.h"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

void Checkout::processOrder(vector<CartItem>& cart) {
    if (cart.empty()) {
        cout << R"(
         +----------------------------------------+
         |              ERROR!                    |
         |            Cart is empty!              |
         |         Cannot checkout.               |
         +----------------------------------------+
)" << endl;
        return;
    }

    User currentUser = Auth::getCurrentUser();
    string address = Auth::getValidString("Enter your delivery address: ");

    cout << R"(
 ===============================================================
 |                    ORDER SUMMARY                          |
 ===============================================================
)" << endl;
    
    cout << " Customer: " << currentUser.fullName << endl;
    cout << " Email: " << currentUser.email << endl;
    cout << " Address: " << address << endl;
    cout << "\n +------+-------------------------+---------+-----------+-------------+" << endl;
    cout << " |  ID  |       PRODUCT NAME      |   QTY   |   PRICE   |    TOTAL    |" << endl;
    cout << " +------+-------------------------+---------+-----------+-------------+" << endl;
    
    double total = 0;
    for (auto &item : cart) {
        double cost = item.product.getPrice() * item.quantity;
        cout << " | " << setw(4) << item.product.getId() 
             << " | " << setw(23) << left << item.product.getName() 
             << " | " << setw(7) << right << item.quantity 
             << " | RM " << setw(6) << fixed << setprecision(2) << item.product.getPrice()
             << " | RM " << setw(8) << fixed << setprecision(2) << cost << " |" << endl;
        total += cost;
    }
    cout << " +------+-------------------------+---------+-----------+-------------+" << endl;
    cout << "\n                         +-----------------------+" << endl;
    cout << "                         |  GRAND TOTAL: RM " << setw(6) << fixed << setprecision(2) << total << " |" << endl;
    cout << "                         +-----------------------+\n" << endl;

    // Save to file
    ofstream fout("orders.txt", ios::app);
    fout << "Order by: " << currentUser.fullName << " (" << currentUser.email << ")\n";
    fout << "Username: " << currentUser.username << "\n";
    fout << "Address: " << address << "\n";
    fout << "Items:\n";
    for (auto &item : cart) {
        fout << " - " << item.product.getName() << " x " 
             << item.quantity << " (RM " << item.product.getPrice() * item.quantity << ")\n";
    }
    fout << "Total: RM " << total << "\n\n";
    fout.close();

    // Clear cart after successful checkout
    cart.clear();

    cout << R"(
         +----------------------------------------+
         |             SUCCESS!                   |
         |       Order placed successfully!       |
         |    Thank you for your purchase!        |
         +----------------------------------------+
)" << endl;
}
