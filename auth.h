// auth.h
#ifndef AUTH_H
#define AUTH_H

#include <string>
using namespace std;

struct User {
    string username;
    string password;
    string email;
    string fullName;
};

class Auth {
private:
    static User currentUser;
    static bool isLoggedIn;
    
public:
    static void showAuthMenu();
    static bool login();
    static bool registerUser();
    static void logout();
    static bool isUserLoggedIn();
    static User getCurrentUser();
    static bool validateUser(const string& username, const string& password);
    static bool saveUserToFile(const User& user);
    static string getValidString(const string& prompt);
};

#endif