#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include "Purchase.h"
#ifndef BILL_H
#define BILL_H
using namespace std;

class Customer;

class Bill {
private:
    int customerId;
    string customerName;
    string customerPhone;
    string customerLocation;
    Purchase* purchases;
    int totalItems;
    double totalBill;
    double paidAmount;
    double dueAmount;
    string billDate;
public:
    Bill() {
        customerId = -1;
        customerName = "Generic Guest";
        customerPhone = "";
        customerLocation = "";
        purchases = nullptr;
        totalItems = 0;
        totalBill = 0.0;
        paidAmount = 0.0;
        dueAmount = 0.0;
        billDate = "";
    }

    Bill(const Customer* customer, Purchase* purchases, int totalItems, string billDate) {
        if (customer != nullptr) {
            customerId = customer->getId();
            customerName = customer->getName();
            customerPhone = customer->getPhoneNumber();
            customerLocation = customer->getLocation();
        } else {
            customerId = -1;
            customerName = "Generic Guest";
            customerPhone = "";
            customerLocation = "";
        }
        this->billDate = billDate;
        this->paidAmount = 0.0;
        this->dueAmount = 0.0;
        this->totalItems = totalItems;
        if (totalItems > 0 && purchases != nullptr) {
            this->purchases = new Purchase[totalItems];
            for (int i = 0; i < totalItems; i++) {
                this->purchases[i] = purchases[i];
            }
        } else {
            this->purchases = nullptr;
            this->totalItems = 0;
        }
        calculateTotalBill();
    }

    Bill(const Bill& other) {
        customerId = other.customerId;
        customerName = other.customerName;
        customerPhone = other.customerPhone;
        customerLocation = other.customerLocation;
        totalItems = other.totalItems;
        totalBill = other.totalBill;
        paidAmount = other.paidAmount;
        dueAmount = other.dueAmount;
        billDate = other.billDate;
        if (other.totalItems > 0 && other.purchases != nullptr) {
            purchases = new Purchase[other.totalItems];
            for (int i = 0; i < other.totalItems; i++) {
                purchases[i] = other.purchases[i];
            }
        } else {
            purchases = nullptr;
        }
    }

    Bill& operator=(const Bill& other) {
        if (this != &other) {
            customerId = other.customerId;
            customerName = other.customerName;
            customerPhone = other.customerPhone;
            customerLocation = other.customerLocation;
            totalItems = other.totalItems;
            totalBill = other.totalBill;
            paidAmount = other.paidAmount;
            dueAmount = other.dueAmount;
            billDate = other.billDate;
            delete[] purchases;
            if (other.totalItems > 0 && other.purchases != nullptr) {
                purchases = new Purchase[other.totalItems];
                for (int i = 0; i < other.totalItems; i++) {
                    purchases[i] = other.purchases[i];
                }
            } else {
                purchases = nullptr;
            }
        }
        return *this;
    }

    ~Bill() {
        delete[] purchases;
    }

    void calculateTotalBill() {
        totalBill = 0.0;
        for (int i = 0; i < totalItems; i++) {
            totalBill += purchases[i].getTotalPrice();
        }
        dueAmount = totalBill - paidAmount;
    }

    void addProductToBill(const Purchase& purchase) {
        Purchase* temp = new Purchase[totalItems + 1];
        for (int i = 0; i < totalItems; i++) {
            temp[i] = purchases[i];
        }
        temp[totalItems] = purchase;
        delete[] purchases;
        purchases = temp;
        totalItems++;
        calculateTotalBill();
    }

    void generateBill() {
        generateBillToStream(cout);
    }

    void generateBillToStream(ostream& out) {
        out << "Invoice Date: " << billDate << endl;
        out << "Customer ID: " << customerId << endl;
        out << "Customer Name: " << customerName << endl;
        out << "Phone Number: " << customerPhone << endl;
        out << "Location: " << customerLocation << endl;
        out << "Total Items: " << totalItems << endl;
        out << "Grand Total: " << fixed << setprecision(2) << totalBill << endl;
        out << "Paid Amount: " << fixed << setprecision(2) << paidAmount << endl;
        out << "Due Amount: " << fixed << setprecision(2) << dueAmount << endl;
        out << "------------------" << endl;
        for (int i = 0; i < totalItems; i++) {
            const Purchase& p = purchases[i];
            out << "Medicine Name: " << p.getMedicineName() << endl;
            out << "Trade Price: " << fixed << setprecision(2) << p.getTradePrice() << endl;
            out << "Quantity: " << p.getQuantity() << endl;
            out << "Total Price: " << fixed << setprecision(2) << p.getTotalPrice() << endl;
            out << "Purchase Date: " << p.getPurchaseDate() << endl;
            out << "------------------" << endl;
        }
    }

    double getTotalBill() const {
        return totalBill;
    }

    double getPaidAmount() const {
        return paidAmount;
    }

    double getDueAmount() const {
        return dueAmount;
    }

    void setPaidAmount(double amount) {
        paidAmount = amount;
        dueAmount = totalBill - paidAmount;
    }

    void setCustomerName(const string& name) {
        customerName = name;
    }

    void setCustomerPhone(const string& phone) {
        customerPhone = phone;
    }

    void setCustomerLocation(const string& location) {
        customerLocation = location;
    }

    string getBillDate() const {
        return billDate;
    }

    int getTotalItems() const {
        return totalItems;
    }

    int getCustomerId() const {
        return customerId;
    }

    string getCustomerName() const {
        return customerName;
    }

    Purchase* getPurchases() const {
        return purchases;
    }

    void generateFormattedInvoice(const string& filename = "") {
        if (filename.empty()) {
            generateFormattedInvoiceToStream(cout);
        } else {
            ofstream outFile(filename);
            if (outFile.is_open()) {
                generateFormattedInvoiceToStream(outFile);
                outFile.close();
            } else {
                cerr << "Error: Could not open file " << filename << " for writing." << endl;
            }
        }
    }

    void generateFormattedInvoiceToStream(ostream& out) {
        // ===== HEADER BLOCK =====
        out << "\n";
        centerAlignText(out, "BILAL PHARMACY", 80);
        centerAlignText(out, "OPP: DHQ Hospital Bhimber AJK Ph# 03088788994", 80);
        out << "\n";

        out << "M/S: " << customerName << string(50, ' ') << "LIC#: ___________" << endl;
        out << "Inv. No.: " << billDate << string(40, ' ') << "AREA: ___________" << endl;
        out << "LIC. EXPIRY: _________________" << string(30, ' ') << "Inv. Date: " << billDate << endl;
        out << "\n";

        // ===== GRID HEADER =====
        printInvoiceGridHeader(out);

        // ===== GRID ITEMS =====
        double totalPacks = 0;
        double totalGross = 0.0;
        double totalDiscount = 0.0;

        for (int i = 0; i < totalItems; i++) {
            const Purchase& p = purchases[i];
            double qty = p.getQuantity();
            double tradePrice = p.getTradePrice();
            double grossAmount = qty * tradePrice;
            double discount = 0.0; // Default discount is 0% as per requirements
            double netAmount = grossAmount - discount;

            totalPacks += qty;
            totalGross += grossAmount;
            totalDiscount += discount;

            printInvoiceGridRow(out, 
                                static_cast<int>(qty),
                                p.getMedicineName(),
                                "1 Strip", // Packing - can be enhanced with medicine details
                                p.getMedicine().getBatchNumber(),
                                tradePrice,
                                grossAmount,
                                discount,
                                netAmount);
        }

        out << "+--------+---------------------------+--------+--------+-------+---------+---------+---------+\n";

        // ===== SUMMARY BLOCKS =====
        out << "\n";
        printInvoiceSummary(out, totalPacks, totalItems, totalGross, totalDiscount);

        // ===== FOOTER =====
        out << "\n";
        printInvoiceFooter(out);
        out << "\n";
    }

private:
    void centerAlignText(ostream& out, const string& text, int width) {
        int padding = (width - text.length()) / 2;
        out << string(padding, ' ') << text << endl;
    }

    void printInvoiceGridHeader(ostream& out) {
        out << "+--------+---------------------------+--------+--------+-------+---------+---------+---------+\n";
        out << "| QTY    | Name of Item              | Packing | Batch No.| T.P   | Gross   | Discount| Total   |\n";
        out << "+--------+---------------------------+--------+--------+-------+---------+---------+---------+\n";
    }

    void printInvoiceGridRow(ostream& out, 
                             int qty,
                             const string& itemName,
                             const string& packing,
                             const string& batchNo,
                             double tradePrice,
                             double grossAmount,
                             double discount,
                             double totalAmount) {
        out << "| ";
        out << setw(6) << qty << " | ";
        out << setw(25) << left << itemName.substr(0, 25) << " | ";
        out << setw(6) << left << packing.substr(0, 6) << " | ";
        out << setw(8) << left << batchNo.substr(0, 8) << "| ";
        out << fixed << setprecision(2);
        out << setw(5) << right << tradePrice << " | ";
        out << setw(7) << right << grossAmount << " | ";
        out << setw(7) << right << discount << " | ";
        out << setw(7) << right << totalAmount << " |\n";
    }

    void printInvoiceSummary(ostream& out, double totalPacks, int totalItemTypes, double grossTotal, double discountTotal) {
        double netTotal = grossTotal - discountTotal;

        out << setw(60) << right << "Total Packs: " << setw(10) << right << fixed << setprecision(0) << totalPacks << endl;
        out << setw(60) << right << "Total Items: " << setw(10) << right << totalItemTypes << endl;
        out << setw(60) << right << "GROSS:       " << setw(10) << right << fixed << setprecision(2) << grossTotal << endl;
        out << setw(60) << right << "DISCOUNT:    " << setw(10) << right << fixed << setprecision(2) << discountTotal << endl;
        
        out << "\n+--------- TOTAL NET VALUE --------+\n";
        out << "|  " << setw(35) << left << "NET TOTAL: " + to_string_precision(netTotal, 2);
        out << "|\n";
        out << "+-----------------------------------+\n";
    }

    void printInvoiceFooter(ostream& out) {
        out << "\n(i) FORM 2A WARRANTY:\n";
        out << "The medicines supplied are genuinely branded and in accordance with the prescriptions\n";
        out << "provided. All medicines are authentic and sourced directly from authorized distributors.\n";
        out << "The buyer is responsible for storage as per pharmaceutical guidelines.\n\n";

        out << "(ii) ALTERNATIVE MEDICINE RULES:\n";
        out << "As per pharmaceutical regulations, any alternative medicine prescribed must be approved\n";
        out << "by the relevant health authority. The customer has the right to request the original\n";
        out << "prescribed medicine if an alternative is dispensed. Returns and exchanges are subject to\n";
        out << "30 days from the date of purchase with original receipt and packaging intact.\n";
    }

    string to_string_precision(double value, int precision) {
        ostringstream out;
        out << fixed << setprecision(precision) << value;
        return out.str();
    }
};
#endif