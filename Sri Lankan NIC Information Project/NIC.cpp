#include "NIC.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sys/stat.h>
#include <ctime>

bool NIC::isLeapYear(int year) const {
    return (year % 400 == 0) || ((year % 4 == 0) && (year % 100 != 0));
}

int NIC::daysInMonth(int month, int year) const {
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return isLeapYear(year) ? 29 : 28;
        default:
            return 0;
    }
}

bool NIC::fileExists(const std::string& filename) const {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

NIC::NIC(const std::string& _fullName, const std::string& _nic) : fullName(_fullName), nic(_nic) {
    // Extracting components
    std::string numberPart = nic.substr(0, 9); // First 9 characters
    finalLetter = nic.length() == 10 ? nic[9] : '\0'; // Last character if present, otherwise '\0'

    // Verify if the last character is a capital letter 'V' or 'X'
    if (finalLetter != '\0' && (!isalpha(finalLetter) || (finalLetter != 'V' && finalLetter != 'X'))) {
        throw std::runtime_error("Error: The last character, if present, should be a capital letter 'V' or 'X'.");
    }

    // Extracting birth year, day of year
    if (numberPart.length() == 9) {
        // The NIC contains only 9 digits, so the birth year is in the shortened format
        birthYear = std::stoi(numberPart.substr(0, 2)) + 1900; // Assuming 20th century birth years
        dayOfYear = std::stoi(numberPart.substr(2, 3));
    } else {
        // The NIC contains the full year
        birthYear = std::stoi(numberPart.substr(0, 4));
        dayOfYear = std::stoi(numberPart.substr(4, 3));
    }
}

void NIC::displayDetails() const {
    std::string gender;
    int day = dayOfYear; // Make a copy to avoid modifying member variable

    if (day > 500) {
        gender = "Female";
        day -= 500;
    } else {
        gender = "Male";
    }

    // Calculate month
    int month = 1;
    if (day > 335) { // December
        month = 12;
        day -= 335;
    } else if (day > 305) { // November
        month = 11;
        day -= 305;
    } else if (day > 274) { // October
        month = 10;
        day -= 274;
    } else if (day > 244) { // September
        month = 9;
        day -= 244;
    } else if (day > 213) { // August
        month = 8;
        day -= 213;
    } else if (day > 182) { // July
        month = 7;
        day -= 182;
    } else if (day > 152) { // June
        month = 6;
        day -= 152;
    } else if (day > 121) { // May
        month = 5;
        day -= 121;
    } else if (day > 91) { // April
        month = 4;
        day -= 91;
    } else if (day > 60) { // March
        month = 3;
        day -= 60;
    } else if (day > 31) { // February
        month = 2;
        day -= isLeapYear(birthYear) ? 29 : 28;
    }

    // Determine the meaning of the final letter
    std::string finalLetterMeaning;
    switch (finalLetter) {
        case 'V':
            finalLetterMeaning = "Eligible to vote";
            break;
        case 'X':
            finalLetterMeaning = "Not eligible to vote";
            break;
        default:
            finalLetterMeaning = "Unknown";
    }

    // Get the current date
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int currentYear = 1900 + ltm->tm_year;
    int currentMonth = 1 + ltm->tm_mon;
    int currentDay = ltm->tm_mday;

    // Calculate the age in years, months, and days
    int ageYears = currentYear - birthYear;
    int ageMonths = currentMonth - month;
    int ageDays = currentDay - day;

    if (ageDays < 0) {
        ageMonths--;
        ageDays += daysInMonth((currentMonth == 1) ? 12 : (currentMonth - 1), (currentMonth == 1) ? (currentYear - 1) : currentYear);
    }
    if (ageMonths < 0) {
        ageYears--;
        ageMonths += 12;
    }

    // Printing details
    std::cout << "NIC Details:" << std::endl;
    std::cout << "Full Name: " << fullName << std::endl;
    std::cout << "Birth Year: " << birthYear << std::endl;
    std::cout << "Month: " << month << std::endl;
    std::cout << "Day: " << day << std::endl;
    std::cout << "Gender: " << gender << std::endl;
    std::cout << "Final Letter: " << (finalLetter != '\0' ? std::string(1, finalLetter) : "N/A") << " (" << finalLetterMeaning << ")" << std::endl;
    std::cout << "Current Age: " << ageYears << " years, " << ageMonths << " months, " << ageDays << " days" << std::endl;
}

void NIC::saveToCSV() const {
    const std::string filename = "NIC_Details.csv";
    bool fileExistsAlready = fileExists(filename);

    std::ofstream file(filename, std::ios::app); // Open file in append mode
    if (!file.is_open()) {
        throw std::runtime_error("Error: Unable to open file.");
    }

    if (!fileExistsAlready) {
        // Write the header row if the file did not exist before
        file << "Full Name,NIC,Birth Year,Month,Day,Gender,Final Letter,Final Letter Meaning,Age Years,Age Months,Age Days" << std::endl;
    }

    std::string gender = (dayOfYear > 500) ? "Female" : "Male";
    int day = dayOfYear > 500 ? dayOfYear - 500 : dayOfYear;
    int month = 1;

    if (day > 335) {
        month = 12; day -= 335;
    } else if (day > 305) {
        month = 11; day -= 305;
    } else if (day > 274) {
        month = 10; day -= 274;
    } else if (day > 244) {
        month = 9; day -= 244;
    } else if (day > 213) {
        month = 8; day -= 213;
    } else if (day > 182) {
        month = 7; day -= 182;
    } else if (day > 152) {
        month = 6; day -= 152;
    } else if (day > 121) {
        month = 5; day -= 121;
    } else if (day > 91) {
        month = 4; day -= 91;
    } else if (day > 60) {
        month = 3; day -= 60;
    } else if (day > 31) {
        month = 2; day -= isLeapYear(birthYear) ? 29 : 28;
    }

    // Determine the meaning of the final letter
    std::string finalLetterMeaning;
    switch (finalLetter) {
        case 'V':
            finalLetterMeaning = "Eligible to vote";
            break;
        case 'X':
            finalLetterMeaning = "Not eligible to vote";
            break;
        default:
            finalLetterMeaning = "Unknown";
    }

    // Get the current date
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int currentYear = 1900 + ltm->tm_year;
    int currentMonth = 1 + ltm->tm_mon;
    int currentDay = ltm->tm_mday;

    // Calculate the age in years, months, and days
    int ageYears = currentYear - birthYear;
    int ageMonths = currentMonth - month;
    int ageDays = currentDay - day;

    if (ageDays < 0) {
        ageMonths--;
        ageDays += daysInMonth((currentMonth == 1) ? 12 : (currentMonth - 1), (currentMonth == 1) ? (currentYear - 1) : currentYear);
    }
    if (ageMonths < 0) {
        ageYears--;
        ageMonths += 12;
    }

    // Write details to CSV
    file << fullName << "," << nic << "," << birthYear << "," << month << "," << day << "," << gender << "," << (finalLetter != '\0' ? std::string(1, finalLetter) : "N/A") << "," << finalLetterMeaning << "," << ageYears << "," << ageMonths << "," << ageDays << std::endl;

    file.close();
}

