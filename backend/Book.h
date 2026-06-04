#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <string>
#include <sstream>
#include "Author.h"

class Book {
private:
    std::string title;
    Author author;
    int year;
    double price;
    std::string isbn;

    inline static int totalBooks = 0;

public:

    Book()
        : title("Unknown"), author(), year(2000), price(0.0), isbn("000-000") {
        ++totalBooks;
    }


    Book(std::string t, Author a, int y, double p, std::string i)
        : title(std::move(t)), author(std::move(a)), isbn(std::move(i)) {
        setYear(y);
        setPrice(p);
        ++totalBooks;
    }


    Book(const Book& other) = default;  // тъй като няма сурови ресурси


    Book(Book&& other) noexcept = default;


    Book& operator=(const Book& other) = default;


    Book& operator=(Book&& other) noexcept = default;


    ~Book() { --totalBooks; }


    void setPrice(double p) {
        if (p >= 0) price = p;
        else {
            std::cerr << "[Warning] Price cannot be negative. Set to 0.\n";
            price = 0.0;
        }
    }

    void setYear(int y) {
        if (y > 0 && y <= 2025) year = y;
        else {
            std::cerr << "[Warning] Year is invalid. Defaulting to 2000.\n";
            year = 2000;
        }
    }


    const std::string& getTitle() const { return title; }
    const Author& getAuthor() const { return author; }
    int getYear() const { return year; }
    double getPrice() const { return price; }
    const std::string& getISBN() const { return isbn; }


    std::string to_string() const {
        std::stringstream ss;
        ss << "'" << title << "' by " << author.getName()
           << " (" << year << "), $" << price
           << " [ISBN: " << isbn << "]";
        return ss.str();
    }


    static int getTotalBooks() { return totalBooks; }
};

#endif

