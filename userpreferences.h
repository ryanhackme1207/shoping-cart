// userpreferences.h
#ifndef USERPREFERENCES_H
#define USERPREFERENCES_H

#include <string>
#include <map>
#include <vector>
#include "product.h"
#include "cartitem.h"

struct UserPurchaseData {
    std::string username;
    std::map<std::string, int> categoryPurchases; // category -> quantity purchased
    
    UserPurchaseData() {}
    UserPurchaseData(std::string user) : username(user) {}
};

class UserPreferences {
private:
    static std::vector<UserPurchaseData> userPurchaseHistory;
    
public:
    // Record user's purchase by category
    static void recordPurchase(const std::string& username, const std::vector<CartItem>& cart);
    
    // Get user's most preferred category
    static std::string getMostPreferredCategory(const std::string& username);
    
    // Get recommendations for user
    static std::vector<Product> getRecommendations(const std::string& username, const std::vector<Product>& allProducts, int maxRecommendations = 5);
    
    // Show recommendations to user
    static void showRecommendations(const std::string& username, const std::vector<Product>& allProducts);
    
    // Find or create user purchase data
    static UserPurchaseData* findOrCreateUserData(const std::string& username);
    
    // Display user's purchase statistics
    static void showUserStatistics(const std::string& username);
};

#endif