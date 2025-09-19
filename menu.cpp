// menu.cpp
#include "menu.h"
#include "userpreferences.h"
#include <iostream>
#include <iomanip>
#include <algorithm> // for sort
#include <cctype>    // for ::tolower
#include <limits>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

// Helper function: convert to lowercase
string toLowerCase(const string &str)
{
    string lower = str;
    transform(lower.begin(), lower.end(), lower.begin(),
              [](unsigned char c)
              { return std::tolower(c); });
    return lower;
}

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

// Function to safely get menu choice (1-11 only)
int getMenuChoice() {
    int choice;
    while (true) {
        choice = getValidInteger("Enter choice: ");
        if (choice >= 1 && choice <= 11) {
            return choice;
        } else {
            cout << R"(
         +----------------------------------------+
         |              ERROR!                    |
         |   Please enter a number between 1-11! |
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

// Function to create/update user statistics file
void updateUserStatistics(const string& username, const vector<CartItem>& cart) {
    string filename = "user_stats_" + username + ".txt";
    map<string, int> categoryCount;
    map<int, int> productCount;
    double totalSpent = 0.0;
    int totalItems = 0;
    
    // Read existing statistics
    ifstream inFile(filename);
    if (inFile.is_open()) {
        string line;
        bool readingCategories = false;
        bool readingProducts = false;
        
        while (getline(inFile, line)) {
            if (line == "CATEGORIES:") {
                readingCategories = true;
                readingProducts = false;
                continue;
            } else if (line == "PRODUCTS:") {
                readingProducts = true;
                readingCategories = false;
                continue;
            } else if (line.find("TOTAL_SPENT:") == 0) {
                totalSpent = stod(line.substr(12));
                readingCategories = false;
                readingProducts = false;
                continue;
            } else if (line.find("TOTAL_ITEMS:") == 0) {
                totalItems = stoi(line.substr(12));
                readingCategories = false;
                readingProducts = false;
                continue;
            }
            
            if (readingCategories && !line.empty()) {
                size_t colonPos = line.find(':');
                if (colonPos != string::npos) {
                    string category = line.substr(0, colonPos);
                    int count = stoi(line.substr(colonPos + 1));
                    categoryCount[category] = count;
                }
            } else if (readingProducts && !line.empty()) {
                size_t colonPos = line.find(':');
                if (colonPos != string::npos) {
                    int productId = stoi(line.substr(0, colonPos));
                    int count = stoi(line.substr(colonPos + 1));
                    productCount[productId] = count;
                }
            }
        }
        inFile.close();
    }
    
    // Update statistics with current purchase
    for (const auto& item : cart) {
        categoryCount[item.product.getCategory()] += item.quantity;
        productCount[item.product.getId()] += item.quantity;
        totalSpent += item.product.getPrice() * item.quantity;
        totalItems += item.quantity;
    }
    
    // Write updated statistics
    ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << "CATEGORIES:" << endl;
        for (const auto& pair : categoryCount) {
            outFile << pair.first << ":" << pair.second << endl;
        }
        
        outFile << "PRODUCTS:" << endl;
        for (const auto& pair : productCount) {
            outFile << pair.first << ":" << pair.second << endl;
        }
        
        outFile << "TOTAL_SPENT:" << fixed << setprecision(2) << totalSpent << endl;
        outFile << "TOTAL_ITEMS:" << totalItems << endl;
        
        outFile.close();
    }
}

// Function to get user's favorite categories
vector<string> getUserFavoriteCategories(const string& username, int limit = 3) {
    string filename = "user_stats_" + username + ".txt";
    map<string, int> categoryCount;
    vector<string> favorites;
    
    ifstream inFile(filename);
    if (inFile.is_open()) {
        string line;
        bool readingCategories = false;
        
        while (getline(inFile, line)) {
            if (line == "CATEGORIES:") {
                readingCategories = true;
                continue;
            } else if (line == "PRODUCTS:" || line.find("TOTAL_") == 0) {
                readingCategories = false;
                break;
            }
            
            if (readingCategories && !line.empty()) {
                size_t colonPos = line.find(':');
                if (colonPos != string::npos) {
                    string category = line.substr(0, colonPos);
                    int count = stoi(line.substr(colonPos + 1));
                    categoryCount[category] = count;
                }
            }
        }
        inFile.close();
        
        // Sort categories by count and get top ones
        vector<pair<string, int>> sortedCategories(categoryCount.begin(), categoryCount.end());
        sort(sortedCategories.begin(), sortedCategories.end(),
             [](const pair<string, int>& a, const pair<string, int>& b) {
                 return a.second > b.second;
             });
        
        for (int i = 0; i < min(limit, (int)sortedCategories.size()); i++) {
            favorites.push_back(sortedCategories[i].first);
        }
    }
    
    return favorites;
}

// Function to show personalized recommendations based on user statistics
void showPersonalizedRecommendations(const string& username, const vector<Product>& products) {
    vector<string> favoriteCategories = getUserFavoriteCategories(username, 3);
    
    if (favoriteCategories.empty()) {
        cout << R"(
 ===============================================================
 |                 WELCOME TO THE STORE!                    |
 ===============================================================
 
 Since this is your first time, here are our popular items:
)" << endl;
        
        // Show first 5 products as default recommendations
        vector<Product> recommendations;
        for (int i = 0; i < min(5, (int)products.size()); i++) {
            recommendations.push_back(products[i]);
        }
        
        if (!recommendations.empty()) {
            cout << " +------+-------------------------+---------------------+---------+-----------+" << endl;
            cout << " |  ID  |      PRODUCT NAME       |      CATEGORY       |  STOCK  |   PRICE   |" << endl;
            cout << " +------+-------------------------+---------------------+---------+-----------+" << endl;
            
            for (const auto& p : recommendations) {
                cout << " | " << setw(4) << right << p.getId()
                     << " | " << setw(23) << left << p.getName()
                     << " | " << setw(19) << left << p.getCategory()
                     << " | " << setw(7) << right << p.getStock()
                     << " | RM " << setw(6) << right << fixed << setprecision(2) << p.getPrice() << " |" << endl;
            }
            cout << " +------+-------------------------+---------------------+---------+-----------+" << endl;
        }
        
        return;
    }
    
    cout << R"(
 ===============================================================
 |              PERSONALIZED RECOMMENDATIONS                 |
 ===============================================================
 
 Based on your shopping history, we recommend these items from your 
 favorite categories: )" << endl;
    
    for (size_t i = 0; i < favoriteCategories.size(); i++) {
        cout << favoriteCategories[i];
        if (i < favoriteCategories.size() - 1) cout << ", ";
    }
    cout << endl << endl;
    
    vector<Product> recommendations;
    
    // Find products from favorite categories
    for (const string& category : favoriteCategories) {
        for (const auto& product : products) {
            if (toLowerCase(product.getCategory()) == toLowerCase(category) && 
                product.getStock() > 0 && 
                recommendations.size() < 8) {
                recommendations.push_back(product);
            }
        }
    }
    
    if (!recommendations.empty()) {
        cout << " +------+-------------------------+---------------------+---------+-----------+" << endl;
        cout << " |  ID  |      PRODUCT NAME       |      CATEGORY       |  STOCK  |   PRICE   |" << endl;
        cout << " +------+-------------------------+---------------------+---------+-----------+" << endl;
        
        for (const auto& p : recommendations) {
            cout << " | " << setw(4) << right << p.getId()
                 << " | " << setw(23) << left << p.getName()
                 << " | " << setw(19) << left << p.getCategory()
                 << " | " << setw(7) << right << p.getStock()
                 << " | RM " << setw(6) << right << fixed << setprecision(2) << p.getPrice() << " |" << endl;
        }
        cout << " +------+-------------------------+---------------------+---------+-----------+" << endl;
    }
}

// Function to display user statistics
void showUserStatistics(const string& username) {
    string filename = "user_stats_" + username + ".txt";
    map<string, int> categoryCount;
    double totalSpent = 0.0;
    int totalItems = 0;
    
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << R"(
 ===============================================================
 |                   USER STATISTICS                        |
 ===============================================================
 
         +----------------------------------------+
         |          NO STATISTICS YET!            |
         |                                        |
         |     Start shopping to see your         |
         |        personalized statistics         |
         +----------------------------------------+
)" << endl;
        return;
    }
    
    string line;
    bool readingCategories = false;
    
    while (getline(inFile, line)) {
        if (line == "CATEGORIES:") {
            readingCategories = true;
            continue;
        } else if (line == "PRODUCTS:") {
            readingCategories = false;
            continue;
        } else if (line.find("TOTAL_SPENT:") == 0) {
            totalSpent = stod(line.substr(12));
            continue;
        } else if (line.find("TOTAL_ITEMS:") == 0) {
            totalItems = stoi(line.substr(12));
            continue;
        }
        
        if (readingCategories && !line.empty()) {
            size_t colonPos = line.find(':');
            if (colonPos != string::npos) {
                string category = line.substr(0, colonPos);
                int count = stoi(line.substr(colonPos + 1));
                categoryCount[category] = count;
            }
        }
    }
    inFile.close();
    
    cout << R"(
 ===============================================================
 |                   USER STATISTICS                        |
 ===============================================================
)" << endl;
    
    cout << " Total Amount Spent: RM " << fixed << setprecision(2) << totalSpent << endl;
    cout << " Total Items Purchased: " << totalItems << endl << endl;
    
    if (!categoryCount.empty()) {
        cout << " Your Shopping Categories:" << endl;
        cout << " +-------------------------+---------+" << endl;
        cout << " |       CATEGORY          |  COUNT  |" << endl;
        cout << " +-------------------------+---------+" << endl;
        
        // Sort categories by count
        vector<pair<string, int>> sortedCategories(categoryCount.begin(), categoryCount.end());
        sort(sortedCategories.begin(), sortedCategories.end(),
             [](const pair<string, int>& a, const pair<string, int>& b) {
                 return a.second > b.second;
             });
        
        for (const auto& pair : sortedCategories) {
            cout << " | " << setw(23) << left << pair.first
                 << " | " << setw(7) << right << pair.second << " |" << endl;
        }
        cout << " +-------------------------+---------+" << endl;
        
        cout << "\n Your favorite category is: " << sortedCategories[0].first << "!" << endl;
    }
}

// Helper function to add item to cart
void addToCartFromSearch(vector<Product>& products, vector<CartItem>& cart, const vector<Product>& displayedProducts) {
    if (displayedProducts.empty()) {
        cout << R"(
         +----------------------------------------+
         |              ERROR!                    |
         |        No products to add!             |
         +----------------------------------------+
)" << endl;
        return;
    }

    cout << R"(
 ===============================================================
 |                    ADD TO CART                           |
 ===============================================================
)" << endl;

    int id = getPositiveInteger("Enter Product ID: ");
    int qty = getPositiveInteger("Enter Quantity: ");

    // Check if product is in the displayed list first
    bool inDisplayed = false;
    for (const auto &dp : displayedProducts) {
        if (dp.getId() == id) {
            inDisplayed = true;
            break;
        }
    }

    if (!inDisplayed) {
        cout << R"(
         +----------------------------------------+
         |              ERROR!                    |
         |    Product not in current results!     |
         |     Please select from shown items     |
         +----------------------------------------+
)" << endl;
        return;
    }

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
         |     Available: )" << setw(2) << p.getStock() << R"( | Requested: )" << setw(2) << qty << R"(        |
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
         +----------------------------------------+
)" << endl;
    }
}

// Helper function to display products in table format
void displayProductTable(const vector<Product>& products) {
    cout << " +------+-------------------------+---------------------+---------+-----------+" << endl;
    cout << " |  ID  |      PRODUCT NAME       |      CATEGORY       |  STOCK  |   PRICE   |" << endl;
    cout << " +------+-------------------------+---------------------+---------+-----------+" << endl;
    
    for (const auto &p : products) {
        cout << " | " << setw(4) << right << p.getId()
             << " | " << setw(23) << left << p.getName()
             << " | " << setw(19) << left << p.getCategory()
             << " | " << setw(7) << right << p.getStock()
             << " | RM " << setw(6) << right << fixed << setprecision(2) << p.getPrice() << " |" << endl;
    }
    cout << " +------+-------------------------+---------------------+---------+-----------+" << endl;
}

// Add welcome with recommendations function
void showWelcomeWithRecommendations(const string& username, const vector<Product>& products) {
    cout << R"(                                  
  _____|  |__   ____ ______   ____   ____  
 /  ___/  |  \ /  _ \\____ \_/ __ \_/ __ \ 
 \___ \|   Y  (  <_> )  |_>  >  ___/\ ___/ 
/____  >___|  /\____/|   __/ \___ >  \___ >
     \/     \/       |__|  \/      \/  )" << "\n\n";

    cout << "Welcome back, " << username << "!" << endl;
    showPersonalizedRecommendations(username, products);
    
    cout << "\nPress Enter to continue to main menu...";
    cin.ignore();
    cin.get();
}

void Menu::showMenu(vector<Product>& products, vector<CartItem>& cart) {
    static bool firstTime = true; // Show recommendations only once per login
    
    int choice;
    do {
        if (firstTime) {
            showWelcomeWithRecommendations(Auth::getCurrentUser().username, products);
            firstTime = false;
        }
        
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
        cout << "6. Search / Sort Products\n";
        cout << "7. View Recommendations\n";
        cout << "8. View My Statistics\n";
        cout << "9. Checkout\n";
        cout << "10. Logout\n";
        cout << "11. Exit\n";
        
        choice = getMenuChoice();

        if (choice == 1) {
            cout << R"(
 ===============================================================
 |                    PRODUCT CATALOG                        |
 ===============================================================
)" << endl;
            
            displayProductTable(products);
            
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
            displayProductTable(products);
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
         |     Available: )" << setw(2) << p.getStock() << R"( | Requested: )" << setw(2) << qty << R"(        |
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
                    
                    cout << " | " << setw(4) << right << item.product.getId() 
                         << " | " << setw(23) << left << item.product.getName() 
                         << " | " << setw(7) << right << item.quantity 
                         << " | RM " << setw(6) << right << fixed << setprecision(2) << item.product.getPrice()
                         << " | RM " << setw(8) << right << fixed << setprecision(2) << itemTotal << " |" << endl;
                }
                
                cout << " +------+-------------------------+---------+-----------+-------------+" << endl;
                cout << "\n                         +-----------------------+" << endl;
                cout << "                         |  GRAND TOTAL: RM " << setw(6) << fixed << setprecision(2) << grandTotal << " |" << endl;
                cout << "                         +-----------------------+\n" << endl;
                
                cout << " >> Use option 4 to update quantities or option 5 to remove items" << endl;
                cout << " >> Ready to checkout? Choose option 9!" << endl;
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
                    cout << " | " << setw(4) << right << item.product.getId() 
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
                    cout << " | " << setw(4) << right << item.product.getId() 
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

        else if (choice == 6)
        {
            searchAndSortMenu(products, cart);
        }

        else if (choice == 7) {
            showPersonalizedRecommendations(Auth::getCurrentUser().username, products);
        }

        else if (choice == 8) {
            showUserStatistics(Auth::getCurrentUser().username);
        }

        else if (choice == 9) {
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
                // Record purchase statistics before checkout
                updateUserStatistics(Auth::getCurrentUser().username, cart);
                Checkout::processOrder(cart);
            }
        }

        else if (choice == 10) {
            firstTime = true; // Reset for next login
            Auth::logout();
            return; // Exit to main, which will show auth menu again
        }

        else if (choice == 11) {
            Auth::logout();
            cout << R"(
         +----------------------------------------+
         |             GOODBYE!                   |
         |     Thank you for using our system!    |
         |            See you again!              |
         +----------------------------------------+
)" << endl;
            exit(0); // Exit the entire program
        }

    } while (choice != 11);
}

// Search & Sort submenu with add to cart functionality
void Menu::searchAndSortMenu(vector<Product> &products, vector<CartItem>& cart)
{
    int choice;
    vector<Product> displayedProducts; // Store currently displayed products
    
    do
    {
        cout << R"(
 ===============================================================
 |                    SEARCH & SORT MENU                       |
 ===============================================================
 1. Search by Name
 2. Search by Category
 3. Filter by Price Range
 4. Sort by Price (Low → High)
 5. Sort by Price (High → Low)
 6. Sort by Name (A → Z)
 7. Add to Cart from Results
 8. Back to Main Menu
)" << endl;

        choice = getValidInteger("Enter choice: ");

        if (choice == 1)
        {
            string keyword;
            cout << "Enter product name keyword: ";
            cin.ignore();
            getline(cin, keyword);

            displayedProducts.clear();
            cout << "\n--- Search Results ---\n";
            
            bool found = false;
            for (auto &p : products)
            {
                if (p.getName().find(keyword) != string::npos)
                {
                    displayedProducts.push_back(p);
                    found = true;
                }
            }
            
            if (found) {
                displayProductTable(displayedProducts);
                cout << "\n >> Use option 7 to add items to cart from these results!" << endl;
            } else {
                cout << "No products found with that name.\n";
            }
        }
        else if (choice == 2)
        {
            string category;
            cout << "Enter category: ";
            cin.ignore();
            getline(cin, category);

            string searchCat = toLowerCase(category);
            displayedProducts.clear();

            cout << "\n--- Category Results ---\n";
            bool found = false;
            for (auto &p : products)
            {
                if (toLowerCase(p.getCategory()).find(searchCat) != string::npos)
                {
                    displayedProducts.push_back(p);
                    found = true;
                }
            }
            
            if (found) {
                displayProductTable(displayedProducts);
                cout << "\n >> Use option 7 to add items to cart from these results!" << endl;
            } else {
                cout << "No products found in this category.\n";
            }
        }

        else if (choice == 3)
        {
            double minPrice, maxPrice;
            cout << "Enter minimum price: ";
            cin >> minPrice;
            cout << "Enter maximum price: ";
            cin >> maxPrice;

            displayedProducts.clear();
            cout << "\n--- Price Range Results ---\n";
            
            bool found = false;
            for (auto &p : products)
            {
                if (p.getPrice() >= minPrice && p.getPrice() <= maxPrice)
                {
                    displayedProducts.push_back(p);
                    found = true;
                }
            }
            
            if (found) {
                displayProductTable(displayedProducts);
                cout << "\n >> Use option 7 to add items to cart from these results!" << endl;
            } else {
                cout << "No products found in this price range.\n";
            }
        }
        else if (choice == 4)
        {
            sort(products.begin(), products.end(),
                 [](Product &a, Product &b)
                 { return a.getPrice() < b.getPrice(); });

            displayedProducts = products; // All products are displayed
            cout << "\n--- Sorted by Price (Low → High) ---\n";
            displayProductTable(displayedProducts);
            cout << "\n >> Use option 7 to add items to cart from these results!" << endl;
        }
        else if (choice == 5)
        {
            sort(products.begin(), products.end(),
                 [](Product &a, Product &b)
                 { return a.getPrice() > b.getPrice(); });

            displayedProducts = products; // All products are displayed
            cout << "\n--- Sorted by Price (High → Low) ---\n";
            displayProductTable(displayedProducts);
            cout << "\n >> Use option 7 to add items to cart from these results!" << endl;
        }
        else if (choice == 6)
        {
            sort(products.begin(), products.end(),
                 [](Product &a, Product &b)
                 { return a.getName() < b.getName(); });

            displayedProducts = products; // All products are displayed
            cout << "\n--- Sorted by Name (A → Z) ---\n";
            displayProductTable(displayedProducts);
            cout << "\n >> Use option 7 to add items to cart from these results!" << endl;
        }
        else if (choice == 7)
        {
            addToCartFromSearch(products, cart, displayedProducts);
        }

    } while (choice != 8);
}