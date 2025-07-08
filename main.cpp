// Program name: SignUp (vectors applied)
// Status: Beta version
// Date: 06.07.2025
// Author: Jaroslaw Dolinski

#include <iostream>
#include <windows.h>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
using namespace std;

struct User {
    int id;
    string username;
    string password;
};

void appendNewUserToUsersFile(User user, string filename) {
    fstream file;
    file.open(filename, ios::out | ios::app);

    if (file.good()) {
        file << user.id << "|";
        file << user.username << "|";
        file << user.password << "|" << endl;
        file.close();

        cout << "File updated" << endl;
        Sleep(1500);
    } else {
        cout << "failed to open the file and save the data in it" << endl;
        Sleep(1500);
    }
}

// Register a new user and return number of logged users
int signUp(vector<User>& users, string filename) {
    string username, password;
    int numberOfUsers = users.size();
    cout << "Enter username: ";
    cin >> username;
    int i = 0;
    while (i < numberOfUsers) {
        if (users[i].username == username) {
            cout << "Such username already exists. Enter another username: ";
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
    appendNewUserToUsersFile(newUser, filename);
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
                cout << "Enter password. " << 3 - attempt << " attempt(s) remaining: ";
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

void loadUsers(vector<User>& users, int& numberOfUsers, string filename) {
    fstream file;
    string line, userId, username, password, dummy;
    int userCounter = 0;
    User user;

    file.open(filename, ios::in);
    if (file.good()) {
        while (getline(file, line)) { // Loading database
            istringstream iss(line);

            getline(iss, userId, '|');
            getline(iss, username, '|');
            getline(iss, password, '|');
            getline(iss, dummy, '|'); // ignoring end '|'

            user.id = stoi(userId);
            user.username = username;
            user.password = password;

            users.push_back(user);
            userCounter++;
        }
        numberOfUsers = userCounter; // also sets numberOfAddressees!
        file.close();
    }
}

int main() {
    vector<User> users;
    int idOfLoggedUser = 0; // > 0 means somebody is logged in, 0 means nobody is logged in and Interface menu is active
    int numberOfUsers = 0;
    char choice;
    string filename = "Users.txt";

    loadUsers(users, numberOfUsers, filename); // numberOfUsers necessary?

    while(true) {
        if (idOfLoggedUser == 0) { // If nobody is logged in
            // Interface
            system("cls");
            cout << "    >>>> MAIN MENU <<<<    " << endl;
            cout << "---------------------------" << endl;
            cout << "1. Sign up" << endl;
            cout << "2. Log in" << endl;
            cout << "9. Quit program" << endl;
            cout << "---------------------------" << endl;
            cout << "Your choice: ";

            cin >> choice;

            if (choice == '1') {
                numberOfUsers = signUp(users, filename);
            } else if (choice == '2') {
                idOfLoggedUser = logIn(users);
            } else if (choice == '9') {
                exit(0);
            }
        } else { // If somebody is logged in
            // Menu
            system("cls");
            cout << "    >>>> USER MENU <<<<    " << endl;
            cout << "---------------------------" << endl;
            cout << "1. Add adressee" << endl;
            cout << "2. Search by first name" << endl;
            cout << "3. Search by last name" << endl;
            cout << "4. View adressees" << endl;
            cout << "5. Remove adressee" << endl;
            cout << "6. Edit addressee" << endl;
            cout << "---------------------------" << endl;
            cout << "7. Change password" << endl;
            cout << "8. Log out" << endl;
            cout << "---------------------------" << endl;
            cout << "Your choice: ";
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

