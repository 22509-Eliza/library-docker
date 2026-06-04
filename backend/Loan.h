#ifndef LOAN_H
#define LOAN_H

#include <string>

class Loan {
private:
    std::string isbn;
    std::string memberId;
    std::string startDate;
    std::string dueDate;
    bool returned;

public:
    Loan(const std::string& isbn,
         const std::string& memberId,
         const std::string& startDate,
         const std::string& dueDate)
        : isbn(isbn), memberId(memberId),
          startDate(startDate), dueDate(dueDate),
          returned(false) {}

    const std::string& getISBN() const { return isbn; }
    const std::string& getMemberID() const { return memberId; }
    const std::string& getStartDate() const { return startDate; }
    const std::string& getDueDate() const { return dueDate; }
    bool isReturned() const { return returned; }

    bool isOverdue(const std::string& currentDate) const {
        return !returned && currentDate > dueDate;
    }

    void markReturned() {
        returned = true;
    }

    std::string to_string() const {
        return "Loan[ISBN=" + isbn +
               ", Member=" + memberId +
               ", Start=" + startDate +
               ", Due=" + dueDate +
               ", Returned=" + (returned ? "yes" : "no") + "]";
    }
};

#endif
