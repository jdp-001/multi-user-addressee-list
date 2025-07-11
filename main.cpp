// Program name: MultiUserAddresseeList (vectors applied)
// Status: Code of feature/user-input branch
// Date: 09.07.2025
// Author: Jaroslaw Dolinski

#include <iostream>
#include <windows.h>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <conio.h>

#include <limits>
using namespace std;

struct User {
    int id;
    string username;
    string password;
};

struct Addressee {
    int id;
    //int userId;
    string firstName;
    string lastName;
    string phone;
    string email;
    string address;
};

string readLine() {
    string input = "";
    getline(cin, input);
    return input;
}

char readCharacter() {
    string input = "";

    while (true) {
        getline(cin, input);
        if (input.length() == 1)
            return input[0];
        else
            cout << "This is not a single character. Enter again. ";
    }
}

int readIntNumber() {
    string input = "";
    int number = 0;

    while (true) {
        getline(cin, input);
        stringstream myStream(input);

        if (myStream >> number)
            break;

        cout << "This is not a number. Enter again." << endl;
    }
    return number;
}

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

// Load Addressees of the currently logged user
int loadAddressees(vector<Addressee>& addressees, const int& idOfLoggedUser, string filename) {
    fstream file;
    string line, id, userId, firstName, lastName, phone, email, address, dummy;
    //int addresseeCounter = 0;
    int lastAddresseeId = 0; //!!!! Verify !!!!
    Addressee addressee;

    addressees.clear();

    file.open(filename, ios::in);
    if (file.good()) {
        while (getline(file, line)) { // Loading database
            istringstream iss(line);

            getline(iss, id, '|');
            getline(iss, userId, '|');
            getline(iss, firstName, '|');
            getline(iss, lastName, '|');
            getline(iss, phone, '|');
            getline(iss, email, '|');
            getline(iss, address, '|');
            getline(iss, dummy, '|'); // ignoring end '|'

            lastAddresseeId = stoi(id);

            if (stoi(userId) == idOfLoggedUser) {
                addressee.id = stoi(id);
                addressee.firstName = firstName;
                addressee.lastName = lastName;
                addressee.phone = phone;
                addressee.email = email;
                addressee.address = address;

                addressees.push_back(addressee);
                //addresseeCounter++; // ???

            }
        }
        file.close();
        //return addresseeCounter; // also sets numberOfAddressees!
        return lastAddresseeId;
    } else {
        //cout << "FILE IS NOT GOOD" << endl;
    }
}

void waitForKeyPress() {
    cout << "Press any key to continue...";
    getch();
}

void showOneAddressee(const vector <Addressee>& addressees, int id) {
    //cout << endl;
    cout << "Id:         " << addressees[id].id << endl;
    cout << "First name: " << addressees[id].firstName << endl;
    cout << "Last name:  " << addressees[id].lastName << endl;
    cout << "Phone:      " << addressees[id].phone << endl;
    cout << "Email:      " << addressees[id].email << endl;
    cout << "Address:    " << addressees[id].address << endl;
    //cout << endl;
}

void appendNewAddresseeToAddresseesFile(Addressee addressee, int idOfLoggedUser, string filename) {
    fstream file;
    file.open(filename, ios::out | ios::app);

    if (file.good()) {
        file << addressee.id << "|";
        file << idOfLoggedUser << "|";
        file << addressee.firstName << "|";
        file << addressee.lastName << "|";
        file << addressee.phone << "|";
        file << addressee.email << "|";
        file << addressee.address << "|" << endl;
        file.close();

        cout << "File updated" << endl;
        Sleep(1500);
    } else {
        cout << "failed to open addressee file and save the data in it" << endl;
        Sleep(1500);
    }
}

void addAddressee(vector <Addressee>& addressees, int idOfLoggedUser, string filename, int lastAddresseeId) {

    Addressee addressee;

    if (addressees.empty())
        addressee.id = 1;
    else
        addressee.id = lastAddresseeId + 1;

    cout << "Id: " << addressee.id << endl;

    cout << "Enter first name: ";
    addressee.firstName = readLine();

    cout << "Enter last name: ";
    addressee.lastName = readLine();

    cout << "Enter phone number: ";
    addressee.phone = readLine();

    cout << "Enter email: ";
    addressee.email = readLine();

    cout << "Enter address: ";
    addressee.address = readLine();

    addressees.push_back(addressee);
    appendNewAddresseeToAddresseesFile(addressee, idOfLoggedUser, filename);
}

void searchAddresseeByFirstName(const vector <Addressee>& addressees) {
}

void searchAddresseeByLastName(const vector <Addressee>& addressees) {
}

void showAllAddressees(const vector <Addressee>& addressees) {
    int numberOfAddressees = addressees.size();
    //cout << "Number of addressees = " << numberOfAddressees << endl;
    if (numberOfAddressees == 0) {
        cout << "Empty/corrupted database. Enter your first record." << endl;
        //waitForKeyPress();
    } else {
        system("cls");
        cout << "    >>>> ADDRESSEES <<<<   " << endl;
        cout << "---------------------------" << endl;

        for (int i = 0; i < numberOfAddressees; i++) {
            showOneAddressee(addressees, i);
            cout << "---------------------------" << endl;
        }
    }
    waitForKeyPress();
}

void removeAddressee(vector <Addressee>& addressees, string filename) {
}

void editAddressee(vector <Addressee>& addressees, string filename) {
}

int main() {
    vector<User> users;
    vector<Addressee> addressees;
    int idOfLoggedUser = 0; // > 0 means somebody is logged in, 0 means nobody is logged in and Interface menu is active
    int numberOfUsers = 0;
    int numberOfAddressees = 0;
    char choice;
    string usersFilename = "Users.txt";
    string addresseesFilename = "Addressees.txt";
    int lastAddresseeId = 0; // !!!! Verify !!!!

    loadUsers(users, numberOfUsers, usersFilename); // numberOfUsers necessary?

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
                numberOfUsers = signUp(users, usersFilename);
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
            cout << "1. Add addressee" << endl;
            cout << "2. Search by first name" << endl;
            cout << "3. Search by last name" << endl;
            cout << "4. View addressees" << endl;
            cout << "5. Remove addressee" << endl;
            cout << "6. Edit addressee" << endl;
            cout << "---------------------------" << endl;
            cout << "7. Change password" << endl;
            cout << "8. Log out" << endl;
            cout << "---------------------------" << endl;
            cout << "Your choice: ";
            cin >> choice;

            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Cleaning buffer !

            lastAddresseeId = loadAddressees(addressees, idOfLoggedUser, "Addressees.txt");

            switch (choice) {
            case '1':
                addAddressee(addressees, idOfLoggedUser, addresseesFilename, lastAddresseeId);
                break;
            case '2':
                searchAddresseeByFirstName(addressees);
                break;
            case '3':
                searchAddresseeByLastName(addressees);
                break;
            case '4':
                showAllAddressees(addressees);
                break;
            case '5':
                removeAddressee(addressees, addresseesFilename);
                break;
            case '6':
                editAddressee(addressees, addresseesFilename);
                break;
            case '7':
                changePassword(users, idOfLoggedUser);
                Sleep(1500);
                break;
            case '8':
                idOfLoggedUser = 0;
                break;
            default:
                cout << "Such an option does not exist." << endl;
                Sleep(1500);
                break;
            }
        }
    }
    return 0;
}
