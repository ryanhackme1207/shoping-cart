// auth.cpp
#include "auth.h"
#include <iostream>
#include <fstream>
#include <limits>
using namespace std;

User Auth::currentUser = {"", "", "", ""};
bool Auth::isLoggedIn = false;

// Function to safely get string input
string Auth::getValidString(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (!input.empty()) {
            return input;
        } else {
            cout << R"(
         +----------------------------------------+
         |              ERROR!                    |
         |      Please enter a valid input!      |
         +----------------------------------------+
)" << endl;
        }
    }
}

// Function to safely get integer input for menu
int getAuthMenuChoice() {
    int choice;
    while (true) {
        cout << "Enter choice: ";
        if (cin >> choice && choice >= 1 && choice <= 3) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        } else {
            cout << R"(
         +----------------------------------------+
         |              ERROR!                    |
         |    Please enter a number between 1-3! |
         +----------------------------------------+
)" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void Auth::showAuthMenu() {
    int choice;
    do {
        cout << R"(
 ===============================================================
 |                    AUTHENTICATION                         |
 ===============================================================

        Welcome to Shopping Cart System!
        Please login or register to continue.

)" << endl;
        
        cout << "\n--- Authentication Menu ---\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Exit\n";
        
        choice = getAuthMenuChoice();
        
        if (choice == 1) {
            if (login()) {
                cout << R"(
         +----------------------------------------+
         |             SUCCESS!                   |
         |         Login successful!              |
         |       Welcome back!                    |
         +----------------------------------------+
)" << endl;
                return; // Exit auth menu and go to main menu
            }
        }
        else if (choice == 2) {
            if (registerUser()) {
                cout << R"(
         +----------------------------------------+
         |             SUCCESS!                   |
         |      Registration successful!          |
         |         Please login now.              |
         +----------------------------------------+
)" << endl;
            }
        }
        else if (choice == 3) {
            cout << "Thank you for visiting!\n";
            exit(0);
        }
        
    } while (choice != 3);
}

bool Auth::login() {
    cout << R"(
 ===============================================================
 |                        LOGIN                              |
 ===============================================================
)" << endl;
    
    string username = getValidString("Enter username: ");
    string password = getValidString("Enter password: ");
    
    if (validateUser(username, password)) {
        isLoggedIn = true;
        return true;
    } else {
        cout << R"(
         +----------------------------------------+
         |              ERROR!                    |
         |     Invalid username or password!      |
         |         Please try again.              |
         +----------------------------------------+
)" << endl;
        return false;
    }
}

bool Auth::registerUser() {
    cout << R"(
 ===============================================================
 |                      REGISTER                             |
 ===============================================================
)" << endl;
    
    User newUser;
    newUser.username = getValidString("Enter username: ");
    
    // Check if username already exists
    ifstream fin("users.txt");
    string line, existingUser, existingPass, existingEmail, existingName;
    while (getline(fin, line) && getline(fin, existingPass) && 
           getline(fin, existingEmail) && getline(fin, existingName)) {
        if (line == newUser.username) {
            cout << R"(
         +----------------------------------------+
         |              ERROR!                    |
         |       Username already exists!         |
         |        Please choose another.          |
         +----------------------------------------+
)" << endl;
            fin.close();
            return false;
        }
    }
    fin.close();
    
    newUser.password = getValidString("Enter password: ");
    newUser.email = getValidString("Enter email: ");
    newUser.fullName = getValidString("Enter full name: ");
    
    if (saveUserToFile(newUser)) {
        return true;
    } else {
        cout << R"(
         +----------------------------------------+
         |              ERROR!                    |
         |    Failed to save user information!    |
         |         Please try again.              |
         +----------------------------------------+
)" << endl;
        return false;
    }
}

bool Auth::validateUser(const string& username, const string& password) {
    ifstream fin("users.txt");
    string storedUser, storedPass, storedEmail, storedName;
    
    while (getline(fin, storedUser) && getline(fin, storedPass) && 
           getline(fin, storedEmail) && getline(fin, storedName)) {
        if (storedUser == username && storedPass == password) {
            currentUser.username = storedUser;
            currentUser.password = storedPass;
            currentUser.email = storedEmail;
            currentUser.fullName = storedName;
            fin.close();
            return true;
        }
    }
    fin.close();
    return false;
}

bool Auth::saveUserToFile(const User& user) {
    ofstream fout("users.txt", ios::app);
    if (fout.is_open()) {
        fout << user.username << "\n";
        fout << user.password << "\n";
        fout << user.email << "\n";
        fout << user.fullName << "\n";
        fout.close();
        return true;
    }
    return false;
}

void Auth::logout() {
    currentUser = {"", "", "", ""};
    isLoggedIn = false;
    cout << R"(
         +----------------------------------------+
         |             SUCCESS!                   |
         |        Logged out successfully!        |
         +----------------------------------------+
)" << endl;
}

bool Auth::isUserLoggedIn() {
    return isLoggedIn;
}

User Auth::getCurrentUser() {
    return currentUser;
}