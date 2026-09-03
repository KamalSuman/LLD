#include <bits/stdc++.h>

#include <chrono>
using namespace std;
using namespace std::chrono;

enum class BookStatus { AVAILABLE, LOANED, RESERVED };
class PenaltyStrategy {
   public:
    virtual double penaltyAmount(double dayOverdues) = 0;
};
class StandardPenaltyStrategy : public PenaltyStrategy {
    double dayRate = 5.0;

   public:
    double penaltyAmount(double dayOverdues) override { return dayRate * dayOverdues; }
};
class PremiumPenaltyStrategy : public PenaltyStrategy {
    double dayRate = 5.0;

   public:
    double penaltyAmount(double dayOverdues) override {
        return 5 * dayRate + max((double)0, dayOverdues - 5) * dayRate * 2.0;
    }
};

class Book {
    string author;
    string title;
    int isbn;
    BookStatus bookStatus = BookStatus ::AVAILABLE;
    queue<int> bookingQueue;

   public:
    Book(string author, string title, int isbn) {
        this->author = author;
        this->title = title;
        this->isbn = isbn;
    }
    string getAuthor() { return author; }
    string getTitle() { return title; }
    int getIsbn() { return isbn; }
    queue<int>& getBookingQueue() { return bookingQueue; }
    BookStatus getBookStatus() { return bookStatus; }
    void assignBook() { this->bookStatus = BookStatus::LOANED; }
    void reservBook(int memId) { bookingQueue.push(memId); }
    void makeAvailable() { bookStatus = BookStatus::AVAILABLE; }
};
class Loan {
    int isbn;
    chrono::system_clock::time_point issueTime;
    chrono::system_clock::time_point returnTime;
    int memberId;

   public:
    Loan(int isbn, int memberId) {
        this->isbn = isbn;
        this->issueTime = chrono::system_clock::now();
        this->memberId = memberId;
    }
    void exitLoan() { this->returnTime = chrono::system_clock::now(); }
    int daysHold() {
        return static_cast<int>(
            chrono::duration_cast<chrono::days>(returnTime - issueTime).count());
    }
    int getIsbn() { return isbn; }
};
class Member {
    int memId;
    string name;
    vector<Loan*> loans;
    double unpaidFines;
    int unpaidamount = 0;

   public:
    Member(int memId, string name) {
        this->memId = memId;
        this->name = name;
    }
    void addLoan(Loan* loan) { loans.emplace_back(loan); }
    void removeLoan(Loan* loan) {
        auto it = find(loans.begin(), loans.end(), loan);
        if (it != loans.end()) loans.erase(it);
    }
    void getNotified(int isbn) {
        cout << "This book is available! You can visit the library " << isbn << endl;
    }
    Loan* getLoan(int isbn) {
        for (auto it : loans) {
            if (it->getIsbn() == isbn) return it;
        }
        return nullptr;
    }
    void addUnpaidFine(int fine) { unpaidamount += fine; }
    int getUnpaidAmoun() { return unpaidamount; }
};
class LibraryManager {
    unordered_map<int, Book*> bookByIsbn;
    unordered_map<int, Member*> members;

   public:
    LibraryManager() {};
    void addBook(int isbn, string author, string title) {
        bookByIsbn.emplace(isbn, new Book(author, title, isbn));
    }
    void removeBook(int isbn) {
        auto it = bookByIsbn.find(isbn);
        if (it != bookByIsbn.end()) bookByIsbn.erase(it);
    }
    void registerMember(int memId, string name) { members.emplace(memId, new Member(memId, name)); }
    void removeMember(int memId) {
        auto it = members.find(memId);
        if (it != members.end()) {
            members.erase(it);
        }
    }
    bool checkOut(int memId, int isbn, bool reservIt) {
        auto it = bookByIsbn.find(isbn);
        auto it2 = members.find(memId);
        if (it2 == members.end()) {
            cout << "Member is not registered!";
            return false;
        }
        if (it2->second->getUnpaidAmoun() >= 50) {
            cout << "Member has reached the limit for the fine of 50$";
            return false;
        }
        if (it == bookByIsbn.end()) {
            cout << "This book is not present in library!";
            return false;
        } else {
            if (it->second->getBookStatus() == BookStatus::AVAILABLE) {
                it->second->assignBook();
                it2->second->addLoan(new Loan(isbn, memId));
                return true;
            } else {
                if (reservIt) {
                    it->second->getBookingQueue().push(memId);
                    cout << "You are added in the queue" << endl;
                }
            }
        }
        return true;
    }
    bool returnBook(int isbn, int memId, PenaltyStrategy* strategy) {
        auto it = bookByIsbn.find(isbn);
        auto it2 = members.find(memId);
        if (it2 == members.end()) {
            cout << "Member is not registered!";
            return false;
        }
        if (it == bookByIsbn.end()) {
            cout << "This book is not present in library!";
            return false;
        }
        Loan* loan = it2->second->getLoan(isbn);
        loan->exitLoan();
        int dueDays = loan->daysHold() - 2;
        int fine = strategy->penaltyAmount(dueDays);
        it2->second->addUnpaidFine(fine);
        it2->second->removeLoan(loan);
        it->second->makeAvailable();
        queue<int>& q = it->second->getBookingQueue();
        if (!q.empty()) {
            int waitingMemId = q.front();
            q.pop();
            auto it3 = members.find(waitingMemId);
            if (it3 != members.end()) {
                it3->second->getNotified(isbn);
            }
        }
        return true;
    }
};

int main() {
    LibraryManager libraryManager;

    libraryManager.registerMember(1, "Aman");
    libraryManager.registerMember(2, "Riya");
    libraryManager.registerMember(3, "Karan");

    libraryManager.addBook(101, "George Orwell", "1984");

    libraryManager.addBook(102, "J.K. Rowling", "Harry Potter");

    libraryManager.addBook(103, "Paulo Coelho", "The Alchemist");

    libraryManager.addBook(104, "James Clear", "Atomic Habits");

    libraryManager.addBook(105, "Robert C. Martin", "Clean Code");

    StandardPenaltyStrategy penalty;

    cout << "\n--- TEST 1 ---\n";

    // Aman gets 1984
    libraryManager.checkOut(1, 101, false);

    cout << "\n--- TEST 2 ---\n";

    // Riya tries same book.
    // It is unavailable, so reserve it.
    libraryManager.checkOut(2, 101, true);

    cout << "\n--- TEST 3 ---\n";

    // Aman returns 1984.
    // Riya should get notification.
    libraryManager.returnBook(101, 1, &penalty);

    cout << "\n--- TEST 4 ---\n";

    // Riya can now check it out.
    libraryManager.checkOut(2, 101, false);

    return 0;
}