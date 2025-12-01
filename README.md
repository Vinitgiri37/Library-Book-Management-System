# Library-Book-Management-System
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

struct Book {
    int id;
    std::string title;
    std::string author;
    std::string isbn;
    bool available;
    std::string issued_to;
};

void loadBooks(std::vector<Book>& books, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) return;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        Book b;
        std::getline(ss, token, ',');
        b.id = std::stoi(token);
        std::getline(ss, b.title, ',');
        std::getline(ss, b.author, ',');
        std::getline(ss, b.isbn, ',');
        std::getline(ss, token, ',');
        b.available = (token == "1");
        std::getline(ss, b.issued_to, ',');
        books.push_back(b);
    }
}

void saveBooks(const std::vector<Book>& books, const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& b : books) {
        file << b.id << "," << b.title << "," << b.author << "," << b.isbn << "," 
             << (b.available ? "1" : "0") << "," << b.issued_to << "\n";
    }
}

void addBook(std::vector<Book>& books) {
    Book b;
    int max_id = 0;
    for (const auto& book : books) {
        max_id = std::max(max_id, book.id);
    }
    b.id = max_id + 1;
    std::cout << "Enter title: ";
    std::getline(std::cin, b.title);
    std::cout << "Enter author: ";
    std::getline(std::cin, b.author);
    std::cout << "Enter ISBN: ";
    std::getline(std::cin, b.isbn);
    b.available = true;
    b.issued_to = "";
    books.push_back(b);
    std::cout << "Book added successfully!\n";
}

void issueBook(std::vector<Book>& books) {
    int id;
    std::cout << "Enter book ID to issue: ";
    std::cin >> id;
    std::cin.ignore(); // To handle newline after cin
    auto it = std::find_if(books.begin(), books.end(), [id](const Book& b) { return b.id == id; });
    if (it != books.end()) {
        if (it->available) {
            std::string user;
            std::cout << "Enter user name: ";
            std::getline(std::cin, user);
            it->available = false;
            it->issued_to = user;
            std::cout << "Book issued successfully!\n";
        } else {
            std::cout << "Book is already issued.\n";
        }
    } else {
        std::cout << "Book not found.\n";
    }
}

void returnBook(std::vector<Book>& books) {
    int id;
    std::cout << "Enter book ID to return: ";
    std::cin >> id;
    std::cin.ignore();
    auto it = std::find_if(books.begin(), books.end(), [id](const Book& b) { return b.id == id; });
    if (it != books.end()) {
        if (!it->available) {
            it->available = true;
            it->issued_to = "";
            std::cout << "Book returned successfully!\n";
        } else {
            std::cout << "Book is not issued.\n";
        }
    } else {
        std::cout << "Book not found.\n";
    }
}

void searchBooks(const std::vector<Book>& books) {
    std::string query;
    std::cout << "Enter title or author to search: ";
    std::getline(std::cin, query);
    bool found = false;
    for (const auto& b : books) {
        if (b.title.find(query) != std::string::npos || b.author.find(query) != std::string::npos) {
            std::cout << "ID: " << b.id << ", Title: " << b.title << ", Author: " << b.author 
                      << ", ISBN: " << b.isbn << ", Available: " << (b.available ? "Yes" : "No");
            if (!b.available) std::cout << ", Issued to: " << b.issued_to;
            std::cout << "\n";
            found = true;
        }
    }
    if (!found) {
        std::cout << "No books found.\n";
    }
}

int main() {
    std::vector<Book> books;
    const std::string filename = "library.csv";
    loadBooks(books, filename);

    int choice;
    do {
        std::cout << "\nLibrary Book Management System\n";
        std::cout << "1. Add Book\n";
        std::cout << "2. Issue Book\n";
        std::cout << "3. Return Book\n";
        std::cout << "4. Search Books\n";
        std::cout << "5. Exit\n";
        std::cout << "Enter choice: ";
        std::cin >> choice;
        std::cin.ignore(); // To handle newline after cin

        switch (choice) {
            case 1:
                addBook(books);
                saveBooks(books, filename);
                break;
            case 2:
                issueBook(books);
                saveBooks(books, filename);
                break;
            case 3:
                returnBook(books);
                saveBooks(books, filename);
                break;
            case 4:
                searchBooks(books);
                break;
            case 5:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);

    return 0;
}
