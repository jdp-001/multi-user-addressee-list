// Program name: MultiUserAddresseeList (vectors applied)
// Status: Code of feature/user-input branch
// Date: 09.07.2025
// Author: Jaroslaw Dolinski

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

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

int main() {
    // Test of readLine() function
    cout << "Enter multiple characters: " << readLine() << endl;

    // Test of readCharacter() function
    cout << "Enter one character: " << readCharacter() << endl;

    // Test of readIntNumber() function
    cout << "Enter number: " << readIntNumber() << endl;

    return 0;
}
