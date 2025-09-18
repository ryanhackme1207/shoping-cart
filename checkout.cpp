// checkout.cpp
#include "checkout.h"
#include "auth.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
using namespace std;

string getCurrentDateTime() {
    time_t now = time(0);
    char* dt = ctime(&now);
    string dateTime(dt);
    dateTime.pop_back(); // Remove newline character
    return dateTime;
}

string generateOrderID() {
    time_t now = time(0);
    return "ORD" + to_string(now);
}

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
    string orderID = generateOrderID();
    string dateTime = getCurrentDateTime();

    cout << R"(
 ===============================================================
 |                    ORDER SUMMARY                          |
 ===============================================================
)" << endl;
    
    cout << " Order ID: " << orderID << endl;
    cout << " Customer: " << currentUser.fullName << endl;
    cout << " Email: " << currentUser.email << endl;
    cout << " Address: " << address << endl;
    cout << " Date: " << dateTime << endl;
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
    }
    cout << " +------+-------------------------+---------+-----------+-------------+" << endl;
    cout << "\n                         +-----------------------+" << endl;
    cout << "                         |  GRAND TOTAL: RM " << setw(6) << fixed << setprecision(2) << total << " |" << endl;
    cout << "                         +-----------------------+\n" << endl;

    // Create customer receipt file
    string receiptFileName = "receipt_" + currentUser.username + "_" + orderID + ".txt";
    ofstream receipt(receiptFileName);
    receipt << "===============================================================\n";
    receipt << "|                    SHOPEE MALL                            |\n";
    receipt << "|                 Official Online Store                     |\n";
    receipt << "===============================================================\n\n";
    receipt << "DIGITAL RECEIPT\n";
    receipt << "Order ID: " << orderID << "\n";
    receipt << "Date: " << dateTime << "\n";
    receipt << "Platform: Shopee Online Shopping\n";
    receipt << "Store: Shopee Mall Official Store\n\n";
    receipt << "CUSTOMER INFORMATION:\n";
    receipt << "Name: " << currentUser.fullName << "\n";
    receipt << "Email: " << currentUser.email << "\n";
    receipt << "Username: " << currentUser.username << "\n";
    receipt << "Delivery Address: " << address << "\n";
    receipt << "Payment Method: Shopee Pay\n";
    receipt << "Shipping Method: Standard Delivery (3-5 working days)\n\n";
    receipt << "ITEMS PURCHASED:\n";
    receipt << "+------+-------------------------+---------+-----------+-------------+\n";
    receipt << "|  ID  |       PRODUCT NAME      |   QTY   |   PRICE   |    TOTAL    |\n";
    receipt << "+------+-------------------------+---------+-----------+-------------+\n";
    
    double subtotal = 0;
    for (auto &item : cart) {
        double cost = item.product.getPrice() * item.quantity;
        subtotal += cost;
        receipt << "| " << setw(4) << item.product.getId() 
                << " | " << setw(23) << left << item.product.getName() 
                << " | " << setw(7) << right << item.quantity 
                << " | RM " << setw(6) << fixed << setprecision(2) << item.product.getPrice()
                << " | RM " << setw(8) << fixed << setprecision(2) << cost << " |\n";
    }
    receipt << "+------+-------------------------+---------+-----------+-------------+\n\n";
    
    double shippingFee = 0.00;  // Free shipping
    double discount = 0.00;     // No discount applied
    double finalTotal = subtotal + shippingFee - discount;
    
    receipt << "PAYMENT BREAKDOWN:\n";
    receipt << "Subtotal:        RM " << fixed << setprecision(2) << subtotal << "\n";
    receipt << "Shipping Fee:    RM " << fixed << setprecision(2) << shippingFee << " (FREE)\n";
    receipt << "Discount:        RM " << fixed << setprecision(2) << discount << "\n";
    receipt << "-----------------------------------------------\n";
    receipt << "GRAND TOTAL:     RM " << fixed << setprecision(2) << finalTotal << "\n\n";
    
    receipt << "SHOPEE INFORMATION:\n";
    receipt << "Website: www.shopee.com.my\n";
    receipt << "Customer Service: 1800-188-1188\n";
    receipt << "Operating Hours: 9:00 AM - 6:00 PM (Mon-Sun)\n";
    receipt << "Email Support: help@shopee.com.my\n\n";
    
    receipt << "IMPORTANT NOTES:\n";
    receipt << "- Please keep this receipt for warranty claims\n";
    receipt << "- Return/Exchange period: 7 days from delivery date\n";
    receipt << "- Track your order: Shopee App > My Purchases\n";
    receipt << "- Rate your shopping experience on Shopee!\n\n";
    
    receipt << "Thank you for shopping with Shopee!\n";
    receipt << "Follow us: @ShopeeMY | Download Shopee App for exclusive deals\n";
    receipt << "For inquiries, contact us with Order ID: " << orderID << "\n";
    receipt.close();

    // Store in customer purchase history
    ofstream history("customer_history.txt", ios::app);
    history << "=== ORDER RECORD ===\n";
    history << "Order ID: " << orderID << "\n";
    history << "Date: " << dateTime << "\n";
    history << "Customer: " << currentUser.fullName << " (" << currentUser.email << ")\n";
    history << "Username: " << currentUser.username << "\n";
    history << "Address: " << address << "\n";
    history << "Items:\n";
    for (auto &item : cart) {
        history << " - " << item.product.getName() << " x " 
                << item.quantity << " (RM " << item.product.getPrice() * item.quantity << ")\n";
    }
    history << "Total: RM " << fixed << setprecision(2) << total << "\n";
    history << "Status: Completed\n";
    history << "==========================================\n\n";
    history.close();

    // Clear cart after successful checkout
    cart.clear();

    cout << R"(
         +----------------------------------------+
         |             SUCCESS!                   |
         |       Order placed successfully!       |
         |    Thank you for your purchase!        |
         +----------------------------------------+
)" << endl;
    
    cout << " Order ID: " << orderID << endl;
    cout << " Receipt saved as: " << receiptFileName << endl;
    cout << " Purchase history updated in: customer_history.txt" << endl;
}
