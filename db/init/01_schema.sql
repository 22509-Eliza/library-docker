-- Initial schema for the Library System
-- Tables are also created by the C++ app (IF NOT EXISTS),
-- but this script pre-creates them so pgAdmin / psql queries work immediately.

CREATE TABLE IF NOT EXISTS authors (
    name       TEXT PRIMARY KEY,
    birth_year INT NOT NULL
);

CREATE TABLE IF NOT EXISTS books (
    isbn        TEXT PRIMARY KEY,
    title       TEXT NOT NULL,
    author_name TEXT REFERENCES authors(name),
    year        INT,
    price       NUMERIC(8,2)
);

CREATE TABLE IF NOT EXISTS members (
    member_id   TEXT PRIMARY KEY,
    name        TEXT NOT NULL,
    year_joined INT
);

CREATE TABLE IF NOT EXISTS loans (
    id          SERIAL PRIMARY KEY,
    isbn        TEXT REFERENCES books(isbn),
    member_id   TEXT REFERENCES members(member_id),
    start_date  TEXT,
    due_date    TEXT,
    returned    BOOLEAN DEFAULT FALSE
);
