#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm> // For std::find

using namespace std;

struct Book {
    int id;
    string title;
    string author;
    string genre;
    bool available;
    string borrower;
};

vector<Book> books;
string filename = "books.csv";

void loadBooks() {
    books.clear();
    ifstream file(filename);
    if (!file.is_open()) {
        // If file doesn't exist, create it with header
        ofstream newFile(filename);
        newFile << "ID,Title,Author,Genre,Available,Borrower\n";
        newFile.close();
        return;
    }
    string line;
    getline(file, line); // Skip header
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Book b;
        getline(ss, token, ',');
        b.id = stoi(token);
        getline(ss, b.title, ',');
        getline(ss, b.author, ',');
        getline(ss, b.genre, ',');
        getline(ss, token, ',');
        b.available = (token == "1");
        getline(ss, b.borrower, ',');
        books.push_back(b);
    }
    file.close();
}

void saveBooks() {
    ofstream file(filename);
    file << "ID,Title,Author,Genre,Available,Borrower\n";
    for (const auto& b : books) {
        file << b.id << "," << b.title << "," << b.author << "," << b.genre << "," 
             << (b.available ? "1" : "0") << "," << b.borrower << "\n";
    }
    file.close();
}

void addBook() {
    Book b;
    b.id = books.empty() ? 1 : books.back().id + 1;
    cout << "Enter title: ";
    getline(cin, b.title);
    cout << "Enter author: ";
    getline(cin, b.author);
    cout << "Enter genre: ";
    getline(cin, b.genre);
    b.available = true;
    b.borrower = "";
    books.push_back(b);
    saveBooks();
    cout << "Book added successfully.\n";
}

void searchBooks() {
    string query;
    cout << "Enter search term (title or author): ";
    getline(cin, query);
    bool found = false;
    for (const auto& b : books) {
        if (b.title.find(query) != string::npos || b.author.find(query) != string::npos) {
            cout << "ID: " << b.id << ", Title: " << b.title << ", Author: " << b.author 
                 << ", Genre: " << b.genre << ", Available: " << (b.available ? "Yes" : "No");
            if (!b.available) cout << ", Borrower: " << b.borrower;
            cout << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "No books found matching the query.\n";
    }
}

void issueBook() {
    int id;
    cout << "Enter book ID to issue: ";
    cin >> id;
    cin.ignore(); // Handle newline after cin
    for (auto& b : books) {
        if (b.id == id) {
            if (b.available) {
                cout << "Enter borrower name: ";
                getline(cin, b.borrower);
                b.available = false;
                saveBooks();
                cout << "Book issued successfully.\n";
            } else {
                cout << "Book is already issued.\n";
            }
            return;
        }
    }
    cout << "Book not found.\n";
}

void returnBook() {
    int id;
    cout << "Enter book ID to return: ";
    cin >> id;
    cin.ignore();
    for (auto& b : books) {
        if (b.id == id) {
            if (!b.available) {
                b.available = true;
                b.borrower = "";
                saveBooks();
                cout << "Book returned successfully.\n";
            } else {
                cout << "Book is not issued.\n";
            }
            return;
        }
    }
    cout << "Book not found.\n";
}

int main() {
    loadBooks();
    int choice;
    do {
        cout << "\nLibrary Book Management System\n";
        cout << "1. Add Book\n";
        cout << "2. Search Books\n";
        cout << "3. Issue Book\n";
        cout << "4. Return Book\n";
        cout << "5. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;
        cin.ignore(); // Handle newline after cin
        switch (choice) {
            case 1:
                addBook();
                break;
            case 2:
                searchBooks();
                break;
            case 3:
                issueBook();
                break;
            case 4:
                returnBook();
                break;
            case 5:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);
    return 0;
}
