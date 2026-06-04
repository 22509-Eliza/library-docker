#include <iostream>
#include <string>
#include <libpq-fe.h>
#include "Library.h"

static void exec(PGconn* conn, const char* sql) {
    PGresult* res = PQexec(conn, sql);
    if (PQresultStatus(res) != PGRES_COMMAND_OK &&
        PQresultStatus(res) != PGRES_TUPLES_OK)
        std::cerr << "[SQL Error] " << PQerrorMessage(conn) << "\n";
    PQclear(res);
}

int main() {
    // ---- In-memory OOP demo (original logic) ----
    Library lib;
    Author a1{"Ivan Vazov", 1850};
    Author a2{"Hristo Botev", 1848};
    Book book1{"Pod igoto",     a1, 1894, 25.50, "ISBN-001"};
    Book book2{"Nema zemya",    a1, 1900, 18.90, "ISBN-002"};
    Book book3{"Hadji Dimitar", a2, 1863, 15.00, "ISBN-003"};
    lib.addBook(book1); lib.addBook(book2); lib.addBook(book3);
    Member m1{"Petar Petrov",  "M001", 2023};
    Member m2{"Maria Ivanova", "M002", 2020};
    lib.addMember(m1); lib.addMember(m2);

    std::cout << "=== Initial library snapshot ===\n" << lib.to_string() << "\n";
    std::cout << "Total books (static): " << Book::getTotalBooks() << "\n\n";

    std::cout << "=== Loan object tests ===\n";
    Loan loan1("ISBN-001","M001","2025-11-03","2025-11-17");
    std::cout << loan1.to_string() << "\n";
    std::cout << "Is loan1 overdue on 2025-11-18? " << std::boolalpha
              << loan1.isOverdue("2025-11-18") << "\n";
    loan1.markReturned();
    std::cout << "After marking returned: " << loan1.to_string() << "\n\n";

    std::cout << "=== Library loan flow ===\n";
    lib.loanBook("ISBN-001","M001","2025-11-03","2025-11-17");
    std::cout << "Available now? " << lib.isBookAvailable("ISBN-001") << "\n";
    lib.loanBook("NO-ISBN","M001","2025-11-03","2025-11-17");
    lib.returnBook("ISBN-001","M001");
    std::cout << "Available now? " << lib.isBookAvailable("ISBN-001") << "\n";
    std::cout << "\nBooks by 'Ivan Vazov':\n";
    for (const auto& bk : lib.findByAuthor("Ivan Vazov"))
        std::cout << " - " << bk.to_string() << "\n";
    std::cout << "\n=== End of in-memory tests ===\n\n";

    // ---- PostgreSQL persistence ----
    const char* connStr = std::getenv("DATABASE_URL");
    if (!connStr) { std::cerr << "DATABASE_URL not set – skipping DB demo.\n"; return 0; }

    PGconn* conn = PQconnectdb(connStr);
    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "DB connection failed: " << PQerrorMessage(conn) << "\n";
        PQfinish(conn); return 1;
    }
    std::cout << "=== Connected to PostgreSQL ===\n";

    exec(conn,
        "CREATE TABLE IF NOT EXISTS authors (name TEXT PRIMARY KEY, birth_year INT);"
        "CREATE TABLE IF NOT EXISTS books (isbn TEXT PRIMARY KEY, title TEXT, author_name TEXT, year INT, price NUMERIC);"
        "CREATE TABLE IF NOT EXISTS members (member_id TEXT PRIMARY KEY, name TEXT, year_joined INT);"
        "CREATE TABLE IF NOT EXISTS loans (isbn TEXT, member_id TEXT, start_date TEXT, due_date TEXT, returned BOOLEAN DEFAULT FALSE);"
    );

    exec(conn, "INSERT INTO authors VALUES ('Ivan Vazov',1850) ON CONFLICT DO NOTHING;");
    exec(conn, "INSERT INTO authors VALUES ('Hristo Botev',1848) ON CONFLICT DO NOTHING;");
    exec(conn, "INSERT INTO books VALUES ('ISBN-001','Pod igoto','Ivan Vazov',1894,25.50) ON CONFLICT DO NOTHING;");
    exec(conn, "INSERT INTO books VALUES ('ISBN-002','Nema zemya','Ivan Vazov',1900,18.90) ON CONFLICT DO NOTHING;");
    exec(conn, "INSERT INTO books VALUES ('ISBN-003','Hadji Dimitar','Hristo Botev',1863,15.00) ON CONFLICT DO NOTHING;");
    exec(conn, "INSERT INTO members VALUES ('M001','Petar Petrov',2023) ON CONFLICT DO NOTHING;");
    exec(conn, "INSERT INTO members VALUES ('M002','Maria Ivanova',2020) ON CONFLICT DO NOTHING;");
    exec(conn, "INSERT INTO loans(isbn,member_id,start_date,due_date) VALUES ('ISBN-001','M001','2025-11-03','2025-11-17') ON CONFLICT DO NOTHING;");

    PGresult* res = PQexec(conn, "SELECT isbn,title,author_name,year,price FROM books;");
    std::cout << "\nBooks in DB:\n";
    for (int i = 0; i < PQntuples(res); ++i)
        std::cout << "  [" << PQgetvalue(res,i,0) << "] " << PQgetvalue(res,i,1)
                  << " - " << PQgetvalue(res,i,2) << " (" << PQgetvalue(res,i,3)
                  << ") $" << PQgetvalue(res,i,4) << "\n";
    PQclear(res);

    res = PQexec(conn, "SELECT isbn,member_id,due_date,returned FROM loans;");
    std::cout << "\nLoans in DB:\n";
    for (int i = 0; i < PQntuples(res); ++i)
        std::cout << "  ISBN=" << PQgetvalue(res,i,0) << " Member=" << PQgetvalue(res,i,1)
                  << " Due=" << PQgetvalue(res,i,2) << " Returned=" << PQgetvalue(res,i,3) << "\n";
    PQclear(res);

    PQfinish(conn);
    std::cout << "\n=== DB demo complete ===\n";
    return 0;
}
