#ifndef LIBRARY_H
#define LIBRARY_H

#include <string>
#include <vector>
#include <iostream>

#include "Author.h"
#include "Book.h"
#include "Member.h"
#include "Loan.h"

class Library {
private:
    std::vector<Book> books;
    std::vector<Member> members;
    std::vector<Loan> loans;

public:

    void addBook(const Book& b) {
        books.push_back(b);
    }

    void addMember(const Member& m) {
        members.push_back(m);
    }

    void addLoan(const Loan& loan) {
        loans.push_back(loan);
    }

    bool hasBook(const std::string& isbn) const {
        for (const auto& b : books) {
            if (b.getISBN() == isbn)
                return true;
        }
        return false;
    }

    bool isBookAvailable(const std::string& isbn) const {
        for (const auto& loan : loans) {
            if (loan.getISBN() == isbn && !loan.isReturned()) {
                return false;
            }
        }
        return true;
    }

    bool loanBook(const std::string& isbn, const std::string& memberId,
                  const std::string& start, const std::string& due)
    {
        if (!hasBook(isbn)) {
            std::cout << "Cannot loan: book with ISBN " << isbn << " not found.\n";
            return false;
        }
        if (!isBookAvailable(isbn)) {
            std::cout << "Cannot loan: book with ISBN " << isbn << " is already loaned.\n";
            return false;
        }

        loans.emplace_back(isbn, memberId, start, due);
        std::cout << "Book with ISBN " << isbn << " loaned to member " << memberId << ".\n";
        return true;
    }

    bool returnBook(const std::string& isbn, const std::string& memberId) {
        for (auto& loan : loans) {
            if (loan.getISBN() == isbn &&
                loan.getMemberID() == memberId &&
                !loan.isReturned())
            {
                loan.markReturned();
                std::cout << "Book with ISBN " << isbn << " returned by member " << memberId << ".\n";
                return true;
            }
        }
        std::cout << "No active loan matches this book and member.\n";
        return false;
    }

    std::vector<Book> findByAuthor(const std::string& authorName) const {
        std::vector<Book> result;
        for (const auto& b : books) {
            if (b.getAuthor().getName() == authorName) {
                result.push_back(b);
            }
        }
        return result;
    }

    std::string to_string() const {
        int activeLoans = 0;
        for (const auto& loan : loans) {
            if (!loan.isReturned())
                ++activeLoans;
        }

        std::string summary;
        summary += "Library summary:\n";
        summary += "Books: " + std::to_string(books.size()) + "\n";
        summary += "Members: " + std::to_string(members.size()) + "\n";
        summary += "Active loans: " + std::to_string(activeLoans) + "\n";

        return summary;
    }
};

#endif

