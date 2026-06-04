#ifndef AUTHOR_H
#define AUTHOR_H

#include <iostream>
#include <string>
using namespace std;

class Author {
private:
    string name;
    int birthYear;
public:
    Author() : name("Unknown"), birthYear(1900) {}

    explicit Author(string name, int birthYear) : name(name), birthYear(birthYear) {}

    string to_string() const {
        return "Author: " + name + ", Birth Year: " + std::to_string(birthYear);
    }

    string getName() const {
        return name;
    }

    int getBirthYear() const {
        return birthYear;
    }


    void setName(string name) {
        this->name = name;
    }

    void setBirthYear(int birthYear) {
        if (birthYear >= 1850 && birthYear <= 2025) {
            this->birthYear = birthYear;
        } else {
            cout << "Invalid birth year. It should be between 1850 and 2025." << endl;
        }
    }
};

#endif
