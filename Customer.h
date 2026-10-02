#include<iostream>
#include<string>
#include<iomanip>
#ifndef CUSTOMER_H
#define CUSTOMER_H
#include "Person.h"
#include "Purchase.h"
using namespace std;

class Customer : public Person {
private:
    string phoneNumber;
    Purchase* purchaseHistory;
    int totalPurchases;
public:
    Customer() : Person() {
        phoneNumber = "";
        purchaseHistory = nullptr;
        totalPurchases = 0;
    }

    Customer(int id, string name, int /*age*/, string location, string phoneNumber, int totalPurchases, string /*lastPurchaseDate*/)
        : Person(id, name, location) {
        this->phoneNumber = phoneNumber;
        this->purchaseHistory = nullptr;
        this->totalPurchases = 0;
    }

    Customer(const Customer& other) : Person(other.id, other.name, other.location) {
        this->phoneNumber = other.phoneNumber;
        this->totalPurchases = other.totalPurchases;
        if (other.totalPurchases > 0 && other.purchaseHistory != nullptr) {
            this->purchaseHistory = new Purchase[other.totalPurchases];
            for (int i = 0; i < other.totalPurchases; i++) {
                this->purchaseHistory[i] = other.purchaseHistory[i];
            }
        } else {
            this->purchaseHistory = nullptr;
        }
    }

    Customer& operator=(const Customer& other) {
        if (this != &other) {
            this->id = other.id;
            this->name = other.name;
            this->location = other.location;
            this->phoneNumber = other.phoneNumber;
            delete[] this->purchaseHistory;
            this->totalPurchases = other.totalPurchases;
            if (other.totalPurchases > 0 && other.purchaseHistory != nullptr) {
                this->purchaseHistory = new Purchase[other.totalPurchases];
                for (int i = 0; i < other.totalPurchases; i++) {
                    this->purchaseHistory[i] = other.purchaseHistory[i];
                }
            } else {
                this->purchaseHistory = nullptr;
            }
        }
        return *this;
    }

    ~Customer() {
        delete[] purchaseHistory;
    }

    void display() {
        Person::display();
        cout << "Phone Number: " << phoneNumber << endl;
        cout << "Total Purchases: " << totalPurchases << endl;
        if (totalPurchases > 0) {
            cout << "Last Purchase Date: " << getLastPurchaseDate() << endl;
        }
    }

    void addPurchase(const Purchase& purchase) {
        Purchase* temp = new Purchase[totalPurchases + 1];
        for (int i = 0; i < totalPurchases; i++) {
            temp[i] = purchaseHistory[i];
        }
        temp[totalPurchases] = purchase;
        delete[] purchaseHistory;
        purchaseHistory = temp;
        totalPurchases++;
    }

    double calculateGrandTotal() const {
        double total = 0.0;
        for (int i = 0; i < totalPurchases; i++) {
            total += purchaseHistory[i].getTotalPrice();
        }
        return total;
    }

    int getTotalPurchases() const {
        return totalPurchases;
    }

    string getPhoneNumber() const {
        return phoneNumber;
    }

    Purchase* getPurchaseHistory() const {
        return purchaseHistory;
    }

    string getLastPurchaseDate() const {
        if (totalPurchases == 0) {
            return "N/A";
        }
        return purchaseHistory[totalPurchases - 1].getPurchaseDate();
    }

    string getMostBoughtMedicine() const {
        if (totalPurchases == 0) return "None";
        int maxCount = 0;
        string mostBought = "";
        for (int i = 0; i < totalPurchases; i++) {
            string currentMed = purchaseHistory[i].getMedicineName();
            int currentCount = 0;
            for (int j = 0; j < totalPurchases; j++) {
                if (purchaseHistory[j].getMedicineName() == currentMed) {
                    currentCount += purchaseHistory[j].getQuantity();
                }
            }
            if (currentCount > maxCount) {
                maxCount = currentCount;
                mostBought = currentMed;
            }
        }
        return mostBought;
    }

    void displayFullProfile() const {
        cout << "==========================================" << endl;
        cout << "           CUSTOMER PROFILE               " << endl;
        cout << "==========================================" << endl;
        cout << "ID:               " << id << endl;
        cout << "Name:             " << name << endl;
        cout << "Phone Number:     " << phoneNumber << endl;
        cout << "Location:         " << location << endl;
        cout << "Total Purchases:  " << totalPurchases << " transactions" << endl;
        cout << "Grand Total Spent: $" << fixed << setprecision(2) << calculateGrandTotal() << endl;
        cout << "Most Bought Med:  " << getMostBoughtMedicine() << endl;
        cout << "Last Purchase:    " << getLastPurchaseDate() << endl;
        cout << "------------------------------------------" << endl;
        cout << "PURCHASE HISTORY:" << endl;
        if (totalPurchases == 0) {
            cout << "No purchases yet." << endl;
        } else {
            for (int i = 0; i < totalPurchases; i++) {
                const Purchase& p = purchaseHistory[i];
                cout << " - " << p.getPurchaseDate() << " | " 
                     << p.getMedicineName() << " | Qty: " << p.getQuantity() 
                     << " | Total: $" << fixed << setprecision(2) << p.getTotalPrice() << endl;
            }
        }
        cout << "==========================================" << endl;
    }

    void displayPurchasedMedicines() const {
        cout << "Customer Name: " << name << endl;
        cout << "Customer ID: " << id << endl;
        cout << "Location: " << location << endl;
        cout << "Phone Number: " << phoneNumber << endl;
        cout << "Purchase History: " << endl;
        for (int i = 0; i < totalPurchases; i++) {
            const Purchase& p = purchaseHistory[i];
            cout << "Medicine Name: " << p.getMedicineName() << endl;
            cout << "Trade Price: " << fixed << setprecision(2) << p.getTradePrice() << endl;
            cout << "Quantity: " << p.getQuantity() << endl;
            cout << "Purchase Date: " << p.getPurchaseDate() << endl;
            cout << "Total Price: " << fixed << setprecision(2) << p.getTotalPrice() << endl;
            cout << "------------------" << endl;
        }
        cout << "Grand Total Spent: " << fixed << setprecision(2) << calculateGrandTotal() << endl;
    }
};
#endif