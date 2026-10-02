#ifndef PURCHASE_H
#define PURCHASE_H

#include <iostream>
#include <string>
#include "med.h"
using namespace std;

class Purchase {
private:
    Medicine medicine;
    int quantity;
    string purchaseDate;
public:
    Purchase() {
        medicine = Medicine();
        quantity = 0;
        purchaseDate = "";
    }

    Purchase(const Medicine& medicine, int quantity, const string& purchaseDate) {
        this->medicine = medicine;
        this->quantity = quantity;
        this->purchaseDate = purchaseDate;
    }

    Purchase(const Purchase& other) {
        this->medicine = other.medicine;
        this->quantity = other.quantity;
        this->purchaseDate = other.purchaseDate;
    }

    Purchase& operator=(const Purchase& other) {
        if (this != &other) {
            this->medicine = other.medicine;
            this->quantity = other.quantity;
            this->purchaseDate = other.purchaseDate;
        }
        return *this;
    }

    Medicine getMedicine() const {
        return medicine;
    }

    string getMedicineName() const {
        return medicine.getMedicineName();
    }

    int getQuantity() const {
        return quantity;
    }

    double getTradePrice() const {
        return medicine.getTradePrice();
    }

    double getTotalPrice() const {
        return medicine.getPrice() * quantity;
    }

    string getPurchaseDate() const {
        return purchaseDate;
    }
};

#endif