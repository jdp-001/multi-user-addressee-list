// Program name: MultiUserAddresseeList (vectors applied)
// Status: Code of feature/user-input branch
// Date: 13.07.2025
// Author: Jaroslaw Dolinski

#include <iostream>
#include <windows.h>
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <conio.h>
#include <cstdio> // for remove()

#include <limits>
using namespace std;

struct User {
    int id;
    string username;
    string password;
};

struct Addressee {
    int id;
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
            cout << "This is not a single character. Enter again: ";
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

        cout << "This is not a number. Enter again: ";
    }
    return number;
}

void appendNewUserToUsersFile(const User& user, const string& filename) {
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
        cout << "Failed to open the file and save the data in it" << endl;
        Sleep(1500);
    }
}

// Register a new user
void signUp(vector<User>& users, const string& filename) {
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
    Sleep(1500);
}

// Log the user and return his id
int logIn(const vector<User>& users) {
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
                    Sleep(1500);
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

// Save users file
void saveUsersFile(vector<User>& users, const string& usersFilename) {
    fstream usersFile;
    int numberOfUsers = users.size();
    usersFile.open(usersFilename, ios::out);
    if (usersFile.good()) {
        for (int i = 0; i < numberOfUsers; i++) {
            usersFile << users[i].id << "|";
            usersFile << users[i].username << "|";
            usersFile << users[i].password << "|" << endl;
        }
        usersFile.close();
    } else {
        cout << "Failed to open the file and save the data in it" << endl;
        Sleep(1500);
    }
}

// Change the password of the logged in user
void changePassword(vector<User>& users, const int idOfLoggedUser, const string& usersFilename) {
    string newPassword;
    int numberOfUsers = users.size();
    cout << "Enter new password: ";
    cin >> newPassword;

    for (int i = 0; i < numberOfUsers; i++) {
        if (users[i].id == idOfLoggedUser) {
            users[i].password = newPassword;
            cout << "Password changed" << endl;
            Sleep(1500);
            saveUsersFile(users, usersFilename);
            break;
        }
    }
}

void loadUsers(vector<User>& users, const string& filename) {
    fstream file;
    string line, userId, username, password, dummy;
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
        }

        file.close();
    }
}

// Load Addressees of the currently logged user and return last addressee id (of all users)
int loadAddressees(vector<Addressee>& addressees, const int idOfLoggedUser, const string& filename) {
    fstream file;
    string line, id, userId, firstName, lastName, phone, email, address, dummy;
    int lastAddresseeId = 0;
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
            }
        }

        file.close();
    } else {
        cout << "Failed to open the file (no file?)" << endl;
    }
    return lastAddresseeId;
}

void waitForKeyPress() {
    cout << "Press any key to continue...";
    getch();
}

void showOneAddressee(const vector<Addressee>& addressees, const int id) {
    cout << "Id:         " << addressees[id].id << endl;
    cout << "First name: " << addressees[id].firstName << endl;
    cout << "Last name:  " << addressees[id].lastName << endl;
    cout << "Phone:      " << addressees[id].phone << endl;
    cout << "Email:      " << addressees[id].email << endl;
    cout << "Address:    " << addressees[id].address << endl;
}

void appendNewAddresseeToAddresseesFile(const Addressee& addressee, const int idOfLoggedUser, const string& filename) {
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
        cout << "Failed to open addressee file and save the data in it" << endl;
        Sleep(1500);
    }
}

void addAddressee(vector<Addressee>& addressees, const int idOfLoggedUser, const string& filename, const int lastAddresseeId) {

    Addressee addressee;

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

void searchAddresseeByFirstName(const vector<Addressee>& addressees) {
    string firstName;

    cout << "Enter first name of the person to be displayed. ";
    firstName = readLine();
    bool found = false;

    for (size_t i = 0; i < addressees.size(); i++) {
        if (addressees[i].firstName == firstName) {
            showOneAddressee(addressees, i);
            found = true;
        }
    }

    if (!found) cout << "First name " << firstName << " not found" << endl;
    waitForKeyPress();
}

void searchAddresseeByLastName(const vector<Addressee>& addressees) {
    string lastName;

    cout << "Enter last name of the person to be displayed: ";
    lastName = readLine();
    bool found = false;

    for (size_t i = 0; i < addressees.size(); i++) {
        if (addressees[i].lastName == lastName) {
            showOneAddressee(addressees, i);
            found = true;
        }
    }

    if (!found) cout << "Last name " << lastName << " not found" << endl;
    waitForKeyPress();
}

void showAllAddressees(const vector<Addressee>& addressees) {
    int numberOfAddressees = addressees.size();
    if (numberOfAddressees == 0) {
        cout << "Empty/corrupted database. Enter your first record." << endl;
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

void saveDataAfterRemovingOrEditingAddressee(const int idOfLoggedUser, const int removedOrEditedAddresseeId, const string& addresseesFilename, const string& editedLine = "" ) {
    fstream addresseesFile, temporaryFile;
    string line, id, userId, firstName, lastName, phone, email, address, dummy;

    addresseesFile.open(addresseesFilename, ios::in);
    temporaryFile.open("Temporary.txt", ios::out);

    if (addresseesFile.good()) {
        while (getline(addresseesFile, line)) { // Loading database
            istringstream iss(line);

            getline(iss, id, '|');
            getline(iss, userId, '|');

            if (!((stoi(userId) == idOfLoggedUser) && (stoi(id) == removedOrEditedAddresseeId))) {
                temporaryFile << line << endl;
            } else if (editedLine != "") {
                temporaryFile<< editedLine << endl;
            }
        }
    } else {
        cout << "File is not good" << endl;
    }
    addresseesFile.close();
    temporaryFile.close();

    if (remove(addresseesFilename.c_str())) cout << "Error while deleting file" << endl;

    int result = rename("Temporary.txt", addresseesFilename.c_str());
    if (result) cout << "Error while renaming file" << endl;
}

int findAddresseeIndex(const vector<Addressee>& addressees, const int numberOfAdressees, const int id) {
    int result = -1;

    for (int i = 0; i < numberOfAdressees; i++) {
        if (addressees[i].id == id) {
            result = i;
            break;
        }
    }
    return result;
}

void removeAddressee(vector<Addressee>& addressees, const string& addresseesFilename, const int idOfLoggedUser) {
    int numberOfAddressees = addressees.size();
    int id;
    int index;
    char character;

    cout << "Enter the id of addressee to remove: ";
    id = readIntNumber();
    index = findAddresseeIndex(addressees, numberOfAddressees, id);

    if (index > -1) {
        showOneAddressee(addressees, index);
        cout << "\nAre you sure?";
        cout << "\nY/y + Enter to delete.";
        cout << "\nN/n + Enter to not delete. ";

        do {
            character = readCharacter();

            if (character == 'Y' || character == 'y') {
                addressees.erase(addressees.begin() + index);
                numberOfAddressees--;
                cout << "Addressee deleted" << endl;
                saveDataAfterRemovingOrEditingAddressee(idOfLoggedUser, id, addresseesFilename);
                break;
            } else if (character == 'N' || character == 'n')
                break;
            else
                cout << "Invalid key. Y/y to delete, N/n to not." << endl;
        } while (character != 'Y' && character != 'y' && character != 'N'
                 && character != 'n');
    } else {
        cout << "Incorrect id" << endl;
        Sleep(1500);
    }
}

void showEditOptions() {
    cout << "    >>>> EDIT MENU <<<<    " << endl;
    cout << "---------------------------" << endl;
    cout << "1. First name" << endl;
    cout << "2. Last name" << endl;
    cout << "3. Phone number" << endl;
    cout << "4. Email" << endl;
    cout << "5. Address" << endl;
    cout << "---------------------------" << endl;
    cout << "6. Return to Menu" << endl;
    cout << "---------------------------" << endl;
    cout << "Your choice: ";
}

string convertAddresseeToLine(const Addressee& addressee, const int idOfLoggedUser) {
    return to_string(addressee.id) + "|" +
           to_string(idOfLoggedUser) + "|" +
           addressee.firstName + "|" +
           addressee.lastName + "|" +
           addressee.phone + "|" +
           addressee.email + "|" +
           addressee.address + "|";
}

void editAddressee(vector<Addressee>& addressees, const string& addresseesFilename, const int idOfLoggedUser) {
    int id;
    int index;
    char choiceFromEditOptions = '0';
    Addressee addressee;
    string firstName, lastName, phone, email, address;

    cout << "Enter the ID of the addressee to edit: ";
    id = readIntNumber();
    index = findAddresseeIndex(addressees, addressees.size(), id);

    if (index > -1) {
        cout << endl;
        system("cls");
        cout << "Data of the person to edit" << endl;
        cout << "---------------------------" << endl;
        showOneAddressee(addressees, index);
        cout << endl;
        showEditOptions();

        while ((choiceFromEditOptions < '1') || (choiceFromEditOptions > '6')) {
            choiceFromEditOptions = readCharacter();
            switch (choiceFromEditOptions) {
            case '1':
                cout << "Enter first name: ";
                firstName = readLine();
                addressees[index].firstName = firstName;
                saveDataAfterRemovingOrEditingAddressee(idOfLoggedUser, id, addresseesFilename, convertAddresseeToLine(addressees[index], idOfLoggedUser));
                cout << "First name changed" << endl;
                Sleep(1500);
                break;
            case '2':
                cout << "Enter last name: ";
                lastName = readLine();
                addressees[index].lastName = lastName;
                saveDataAfterRemovingOrEditingAddressee(idOfLoggedUser, id, addresseesFilename, convertAddresseeToLine(addressees[index], idOfLoggedUser));
                cout << "Last name changed" << endl;
                Sleep(1500);
                break;
            case '3':
                cout << "Enter phone number: ";
                phone = readLine();
                addressees[index].phone = phone;
                saveDataAfterRemovingOrEditingAddressee(idOfLoggedUser, id, addresseesFilename, convertAddresseeToLine(addressees[index], idOfLoggedUser));
                cout << "Phone number changed" << endl;
                Sleep(1500);
                break;
            case '4':
                cout << "Enter email: ";
                email = readLine();
                addressees[index].email = email;
                saveDataAfterRemovingOrEditingAddressee(idOfLoggedUser, id, addresseesFilename, convertAddresseeToLine(addressees[index], idOfLoggedUser));
                cout << "Email changed" << endl;
                Sleep(1500);
                break;
            case '5':
                cout << "Enter address: ";
                address = readLine();
                addressees[index].address = address;
                saveDataAfterRemovingOrEditingAddressee(idOfLoggedUser, id, addresseesFilename, convertAddresseeToLine(addressees[index], idOfLoggedUser));
                cout << "Address changed" << endl;
                Sleep(1500);
                break;
            case '6':
                break;
            default:
                cout << "Invalid option. Please try again: " << endl;
                break;
            }
        }
    } else {
        cout << "Incorrect id" << endl;
        waitForKeyPress();
    }
}

int main() {
    vector<User> users;
    vector<Addressee> addressees;
    int idOfLoggedUser = 0; // > 0 means somebody is logged in, 0 means nobody is logged in and Interface menu is active
    char choice;
    string usersFilename = "Users.txt";
    string addresseesFilename = "Addressees.txt";
    int lastAddresseeId = 0;

    loadUsers(users, usersFilename);

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
                signUp(users, usersFilename);
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

            lastAddresseeId = loadAddressees(addressees, idOfLoggedUser, addresseesFilename);

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
                removeAddressee(addressees, addresseesFilename, idOfLoggedUser);
                break;
            case '6':
                editAddressee(addressees, addresseesFilename, idOfLoggedUser);
                break;
            case '7':
                changePassword(users, idOfLoggedUser, usersFilename);
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
