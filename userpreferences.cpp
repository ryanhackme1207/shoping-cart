// userpreferences.cpp
#include "userpreferences.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

// Static member initialization
vector<UserPurchaseData> UserPreferences::userPurchaseHistory;

void UserPreferences::recordPurchase(const string& username, const vector<CartItem>& cart) {
    UserPurchaseData* userData = findOrCreateUserData(username);
    
    // Record purchases by category
    for (const auto& item : cart) {
        string category = item.product.getCategory();
        userData->categoryPurchases[category] += item.quantity;
    }
}

string UserPreferences::getMostPreferredCategory(const string& username) {
    UserPurchaseData* userData = findOrCreateUserData(username);
    
    if (userData->categoryPurchases.empty()) {
        return ""; // No purchase history
    }
    
    // Find category with maximum purchases
    string mostPreferred = "";
    int maxPurchases = 0;
    
    for (const auto& pair : userData->categoryPurchases) {
        if (pair.second > maxPurchases) {
            maxPurchases = pair.second;
            mostPreferred = pair.first;
        }
    }
    
    return mostPreferred;
}

vector<Product> UserPreferences::getRecommendations(const string& username, const vector<Product>& allProducts, int maxRecommendations) {
    vector<Product> recommendations;
    string preferredCategory = getMostPreferredCategory(username);
    
    if (preferredCategory.empty()) {
        // No purchase history, recommend random products
        for (int i = 0; i < min(maxRecommendations, (int)allProducts.size()); i++) {
            if (allProducts[i].getStock() > 0) {
                recommendations.push_back(allProducts[i]);
            }
        }
        return recommendations;
    }
    
    // Find products in preferred category with stock > 0
    for (const auto& product : allProducts) {
        if (product.getCategory() == preferredCategory && product.getStock() > 0) {
            recommendations.push_back(product);
            if (recommendations.size() >= maxRecommendations) {
                break;
            }
        }
    }
    
    // If not enough products in preferred category, add from other categories
    if (recommendations.size() < maxRecommendations) {
        for (const auto& product : allProducts) {
            if (product.getCategory() != preferredCategory && product.getStock() > 0) {
                recommendations.push_back(product);
                if (recommendations.size() >= maxRecommendations) {
                    break;
                }
            }
        }
    }
    
    return recommendations;
}

void UserPreferences::showRecommendations(const string& username, const vector<Product>& allProducts) {
    vector<Product> recommendations = getRecommendations(username, allProducts, 5);
    string preferredCategory = getMostPreferredCategory(username);
    
    cout << R"(
 ===============================================================
 |                 PERSONALIZED RECOMMENDATIONS              |
 ===============================================================
)" << endl;

    if (!preferredCategory.empty()) {
        cout << " Based on your purchase history, you prefer: " << preferredCategory << " products" << endl;
        cout << " Here are some recommendations for you:" << endl << endl;
    } else {
        cout << " Welcome! Here are some popular products to get you started:" << endl << endl;
    }
    
    if (recommendations.empty()) {
        cout << R"(
         +----------------------------------------+
         |          NO RECOMMENDATIONS           |
         |         All products are out of        |
         |              stock!                    |
         +----------------------------------------+
)" << endl;
        return;
    }
    
    cout << " +------+-------------------------+---------------------+---------+-----------+" << endl;
    cout << " |  ID  |      PRODUCT NAME       |      CATEGORY       |  STOCK  |   PRICE   |" << endl;
    cout << " +------+-------------------------+---------------------+---------+-----------+" << endl;
    
    for (const auto& product : recommendations) {
        cout << " | " << setw(4) << right << product.getId()
             << " | " << setw(23) << left << product.getName()
             << " | " << setw(19) << left << product.getCategory()
             << " | " << setw(7) << right << product.getStock()
             << " | RM " << setw(6) << right << fixed << setprecision(2) << product.getPrice() << " |" << endl;
    }
    cout << " +------+-------------------------+---------------------+---------+-----------+" << endl;
    cout << "\n >> Use 'Add to Cart' option to purchase these recommended items!" << endl;
}

UserPurchaseData* UserPreferences::findOrCreateUserData(const string& username) {
    // Find existing user data
    for (auto& userData : userPurchaseHistory) {
        if (userData.username == username) {
            return &userData;
        }
    }
    
    // Create new user data if not found
    userPurchaseHistory.push_back(UserPurchaseData(username));
    return &userPurchaseHistory.back();
}

void UserPreferences::showUserStatistics(const string& username) {
    UserPurchaseData* userData = findOrCreateUserData(username);
    
    cout << R"(
 ===============================================================
 |                  YOUR PURCHASE STATISTICS                 |
 ===============================================================
)" << endl;

    if (userData->categoryPurchases.empty()) {
        cout << R"(
         +----------------------------------------+
         |           NO PURCHASE HISTORY          |
         |                                        |
         |        Start shopping to see your      |
         |           preferences here!            |
         +----------------------------------------+
)" << endl;
        return;
    }
    
    cout << " +-------------------------+-----------+" << endl;
    cout << " |        CATEGORY         | QUANTITY  |" << endl;
    cout << " +-------------------------+-----------+" << endl;
    
    // Sort categories by purchase quantity (descending)
    vector<pair<string, int>> sortedCategories(userData->categoryPurchases.begin(), userData->categoryPurchases.end());
    sort(sortedCategories.begin(), sortedCategories.end(), 
         [](const pair<string, int>& a, const pair<string, int>& b) {
             return a.second > b.second;
         });
    
    for (const auto& pair : sortedCategories) {
        cout << " | " << setw(23) << left << pair.first 
             << " | " << setw(9) << right << pair.second << " |" << endl;
    }
    cout << " +-------------------------+-----------+" << endl;
    
    if (!sortedCategories.empty()) {
        cout << "\n >> Your favorite category is: " << sortedCategories[0].first 
             << " (" << sortedCategories[0].second << " items purchased)" << endl;
    }
}