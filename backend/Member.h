#ifndef MEMBER_H
#define MEMBER_H

#include <string>
#include <iostream>

class Member {
private:
    std::string name;
    std::string memberID;
    int yearJoined;

public:
    Member()
        : name("Unknown"), memberID("0000"), yearJoined(2000) {}

    explicit Member(std::string name, std::string id, int year)
        : name(std::move(name)), memberID(std::move(id)) {
        setYearJoined(year);
        setMemberID(memberID);
    }

    std::string to_string() const {
        return "Member: " + name +
               ", Member ID: " + memberID +
               ", Year Joined: " + std::to_string(yearJoined);
    }

    std::string getName() const { return name; }
    std::string getMemberID() const { return memberID; }
    int getYearJoined() const { return yearJoined; }

    void setName(std::string newName) {
        name = std::move(newName);
    }

    void setMemberID(std::string id) {
        if (!id.empty()) {
            memberID = std::move(id);
        } else {
            std::cerr << "[Warning] Member ID cannot be empty. Keeping previous value.\n";
        }
    }

    void setYearJoined(int year) {
        if (year >= 1900 && year <= 2025) {
            yearJoined = year;
        } else {
            std::cerr << "[Warning] Invalid yearJoined. Must be between 1900 and 2025.\n";
        }
    }
};

#endif

