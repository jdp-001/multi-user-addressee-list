// Program name: MultiUserAddresseeList (vectors applied)
// Status: Code of feature/user-input branch
// Date: 09.07.2025
// Author: Jaroslaw Dolinski

#include <iostream>

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

int main() {
    // Test of readLine() function
    cout << readLine();

    // Test of readCharacter() function
    cout << readCharacter();

    return 0;
}
