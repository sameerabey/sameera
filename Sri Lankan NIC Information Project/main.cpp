#include <iostream>
#include "NIC.h"

int main() {
    while (true) {
        std::string fullName, nic;
        char choice;

        std::cout << "Enter NIC number (9 digits followed by an optional capital letter): ";
        std::cin >> nic;
        std::cin.ignore(); // To clear the newline character from the buffer

        // Validate the last character of NIC number
        char lastChar = nic.back();
        if (!isalpha(lastChar) || !isupper(lastChar) || (lastChar != 'V' && lastChar != 'X')) {
            std::cout << "Error: The last character of NIC number should be a capital letter 'V' or 'X'." << std::endl;
            continue; // Restart loop if validation fails
        }

        std::cout << "Enter your full name: ";
        std::getline(std::cin, fullName);

        NIC nicObj(fullName, nic);
        nicObj.displayDetails();
        try {
            nicObj.saveToCSV();
            std::cout << "Saved to NIC_Details.csv" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error saving to file: " << e.what() << std::endl;
        }

        std::cout << "Do you want to enter another NIC? (y/n): ";
        std::cin >> choice;
        std::cin.ignore(); // To clear the newline character from the buffer

        if (choice == 'n' || choice == 'N') {
            break;
        }
    }

    return 0;
}
