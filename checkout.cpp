#include "checkout.h"
#include <iostream>
#include <fstream>
using namespace std;

void Checkout::processOrder(const vector<CartItem>& cart) {
    if (cart.empty()) {
        cout << "Your cart is empty! Cannot checkout.\n";
        return;
    }

    string name, email, address;
    cout << "\n--- Checkout ---\n";
    cout << "Enter your name: ";
    getline(cin, name);
    cout << "Enter your email: ";
    getline(cin, email);
    cout << "Enter your address: ";
    getline(cin, address);

    double total = 0;
    cout << "\n--- Order Summary ---\n";
    for (auto &item : cart) {
        double cost = item.product.getPrice() * item.quantity;
        cout << item.product.getName() << " x " << item.quantity 
             << " = RM " << cost << endl;
        total += cost;
    }
    cout << "Total: RM " << total << endl;

    // Save to file
    ofstream fout("orders.txt", ios::app);
    fout << "Order by: " << name << " (" << email << ")\n";
    fout << "Address: " << address << "\n";
    fout << "Items:\n";
    for (auto &item : cart) {
        fout << " - " << item.product.getName() << " x " 
             << item.quantity << "\n";
    }
    fout << "Total: RM " << total << "\n\n";
    fout.close();

    cout << "\nOrder placed successfully!\n";
}
