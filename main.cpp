// Program name: SignUp (vectors applied)
// Status: Beta version
// Date: 06.07.2025
// Author: Jaroslaw Dolinski

#include <iostream>
#include <windows.h>
#include <vector>
using namespace std;

struct User {
    int id;
    string username, password;
};

// Register a new user and return number of logged users
int signUp(vector<User>& users) {
    string username, password;
    int numberOfUsers = users.size();
    cout << "Enter username: ";
    cin >> username;
    int i = 0;
    while (i < numberOfUsers) {
        if (users[i].username == username) {
            cout << "Such username already exists. Enter another username" << endl;
            cin >> username;
            i = 0;
        } else {
            i++;
        }
    }
    cout << "Enter password: ";
    cin >> password;
    User newUser;
    newUser.username = username;
    newUser.password = password;
    newUser.id = numberOfUsers + 1;
    users.push_back(newUser);
    cout << "Account created" << endl;
    Sleep(1000);
    return numberOfUsers + 1;
}

// Log the user and return his id
int logIn(vector<User>& users) {
    string username, password;
    int numberOfUsers = users.size();
    cout << "Enter username: ";
    cin >> username;
    for (int i = 0; i < numberOfUsers; i++) {
        if (users[i].username == username) {
            for (int attempt = 0; attempt < 3; attempt++) {
                cout << "Enter password. " << 3 - attempt << " attempts remaining: ";
                cin >> password;
                if (password == users[i].password) {
                    cout << "You are logged in" << endl;
                    Sleep(1000);
                    return users[i].id;
                }
            }
            cout << "You entered invalid password 3 times. Wait 3 seconds before next attempt." << endl;
            Sleep(3000);
            return 0;
        }
    }
    cout << "Username doesn't exist" << endl;
    Sleep(1500);
    return 0;
}

// Change the password of the logged in user
void changePassword(vector<User>& users, int idOfLoggedUser) {
    string password;
    int numberOfUsers = users.size();
    cout << "Enter new password: ";
    cin >> password;
    for (int i = 0; i < numberOfUsers; i++) {
        if (users[i].id == idOfLoggedUser) {
            users[idOfLoggedUser - 1].password = password;
            cout << "Password changed" << endl;
            Sleep(1500);
        }
    }
}

int main() {
    vector<User> users;
    int idOfLoggedUser = 0; // > 0 means somebody is logged in, 0 means nobody is logged in and Interface menu is active
    int numberOfUsers = 0;
    char choice;
    while(true) {
        if (idOfLoggedUser == 0) {
            // Interface
            system("cls");
            cout << "1. Sign up" << endl;
            cout << "2. Log in" << endl;
            cout << "9. Quit program" << endl;
            cin >> choice;

            if (choice == '1') {
                numberOfUsers = signUp(users);
            } else if (choice == '2') {
                idOfLoggedUser = logIn(users);
            } else if (choice == '9') {
                exit(0);
            }
        } else {
            // Menu
            system("cls");
            cout << "1. Change password" << endl;
            cout << "2. Log out" << endl;
            cin >> choice;
            if (choice == '1') {
                changePassword(users, idOfLoggedUser);
                Sleep(1500);
            } else if (choice == '2') {
                idOfLoggedUser = 0;
            }
        }
    }
    return 0;
}

