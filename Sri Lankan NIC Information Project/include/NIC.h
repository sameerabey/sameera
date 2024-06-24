#ifndef NIC_H
#define NIC_H

#include <string>

class NIC {
private:
    std::string fullName;
    std::string nic;
    int birthYear;
    int dayOfYear;
    char finalLetter;

    bool isLeapYear(int year) const;
    int daysInMonth(int month, int year) const;
    bool fileExists(const std::string& filename) const;

public:
    NIC(const std::string& _fullName, const std::string& _nic);

    void displayDetails() const;
    void saveToCSV() const;
};

#endif // NIC_h
