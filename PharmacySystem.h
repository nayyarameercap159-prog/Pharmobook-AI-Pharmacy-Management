#ifndef PHARMACYSYSTEM_H
#define PHARMACYSYSTEM_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

#include "Person.h"
#include "med.h"
#include "Customer.h"
#include "Admin.h"
#include "Inventory.h"
#include "Bill.h"

using namespace std;

// Enable ANSI coloring on Windows if supported
#ifdef _WIN32
#include <windows.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
inline void enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
inline void sleepMs(int ms) {
    Sleep(ms);
}
inline void clearScreen() {
    system("cls");
}
#else
#include <unistd.h>
inline void enableANSI() {}
inline void sleepMs(int ms) {
    usleep(ms * 1000);
}
inline void clearScreen() {
    system("clear");
}
#endif

// ANSI color escape codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define BOLD    "\033[1m"

class PharmacySystem {
private:
    Inventory inventory;
    Customer* customers;
    int totalCustomers;
    Bill* bills;
    int totalBills;
    
    // Admin credentials
    string adminUsername;
    string adminPassword;

    // Database Filenames
    string medicinesFile;
    string customersFile;
    string billsFile;
    string adminFile;
    string monthStateFile;
    string currentSalesMonth;

    string getCurrentMonthKey() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        stringstream ss;
        if (ltm != nullptr) {
            ss << (1900 + ltm->tm_year) << "-";
            ss << setfill('0') << setw(2) << (1 + ltm->tm_mon);
        } else {
            ss << "2026-05";
        }
        return ss.str();
    }

    void archiveMonthlySales(string monthKey) {
        if (totalBills == 0) return;

        double totalSales = 0.0;
        int totalItems = 0;
        for (int i = 0; i < totalBills; i++) {
            totalSales += bills[i].getTotalBill();
            totalItems += bills[i].getTotalItems();
        }

#ifdef _WIN32
        system("if not exist archives mkdir archives");
#else
        system("mkdir -p archives");
#endif
        string archivePath = "archives/sales_" + monthKey + ".txt";
        ofstream fout(archivePath);
        if (!fout) {
            cout << RED << "Could not archive monthly sales to " << archivePath << RESET << endl;
            return;
        }
        fout << "PharmoBook Monthly Sales Archive" << endl;
        fout << "Month: " << monthKey << endl;
        fout << "Total Invoices: " << totalBills << endl;
        fout << "Total Items Sold: " << totalItems << endl;
        fout << "Total Sales Revenue: " << fixed << setprecision(2) << totalSales << endl;
        fout << "==========================================" << endl;
        for (int i = 0; i < totalBills; i++) {
            fout << "Transaction No. " << (i + 1) << endl;
            bills[i].generateBillToStream(fout);
            fout << "==========================================" << endl;
        }
        fout.close();
        cout << GREEN << "\n\tMonthly sales archived to " << archivePath << RESET << endl;
    }

    void checkMonthRollover() {
        string thisMonth = getCurrentMonthKey();
        if (currentSalesMonth.empty()) {
            currentSalesMonth = thisMonth;
            return;
        }
        if (currentSalesMonth != thisMonth) {
            archiveMonthlySales(currentSalesMonth);
            delete[] bills;
            bills = nullptr;
            totalBills = 0;
            currentSalesMonth = thisMonth;
            saveMonthState();
            saveBillsToFile(billsFile);
            cout << YELLOW << "\n\tNew month started. Previous sales archived and reset." << RESET << endl;
            sleepMs(2000);
        }
    }

    void saveMonthState() {
        ofstream fout(monthStateFile);
        if (!fout) return;
        fout << currentSalesMonth << endl;
        fout.close();
    }

    void loadMonthState() {
        ifstream fin(monthStateFile);
        if (!fin) {
            currentSalesMonth = getCurrentMonthKey();
            return;
        }
        getline(fin, currentSalesMonth);
        fin.close();
        if (currentSalesMonth.empty()) currentSalesMonth = getCurrentMonthKey();
    }

    double getMonthlySalesTotal() {
        double total = 0.0;
        for (int i = 0; i < totalBills; i++) {
            total += bills[i].getTotalBill();
        }
        return total;
    }

    // Helper to get current date as YYYY-MM-DD
    string getCurrentDateStr() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        stringstream ss;
        if (ltm != nullptr) {
            ss << (1900 + ltm->tm_year) << "-";
            ss << setfill('0') << setw(2) << (1 + ltm->tm_mon) << "-";
            ss << setfill('0') << setw(2) << ltm->tm_mday;
        } else {
            ss << "2026-05-24";
        }
        return ss.str();
    }

public:
    PharmacySystem() {
        enableANSI();
        customers = nullptr;
        totalCustomers = 0;
        bills = nullptr;
        totalBills = 0;
        
        adminUsername = "admin";
        adminPassword = "password";

        medicinesFile = "medicines_db.txt";
        customersFile = "customers_db.txt";
        billsFile = "bills_db.txt";
        adminFile = "admin_db.txt";
        monthStateFile = "month_state.txt";
        currentSalesMonth = "";
    }

    ~PharmacySystem() {
        delete[] customers;
        delete[] bills;
    }

    void addCustomer(Customer c) {
        Customer* temp = new Customer[totalCustomers + 1];
        for (int i = 0; i < totalCustomers; i++) {
            temp[i] = customers[i];
        }
        temp[totalCustomers] = c;
        delete[] customers;
        customers = temp;
        totalCustomers++;
    }

    void addBill(Bill b) {
        Bill* temp = new Bill[totalBills + 1];
        for (int i = 0; i < totalBills; i++) {
            temp[i] = bills[i];
        }
        temp[totalBills] = b;
        delete[] bills;
        bills = temp;
        totalBills++;
    }

    Customer* getCustomerById(int id) {
        for (int i = 0; i < totalCustomers; i++) {
            if (customers[i].getId() == id) {
                return &customers[i];
            }
        }
        return nullptr;
    }

    void deleteBillByIndex(int index) {
        if (index < 0 || index >= totalBills) {
            cout << RED << "\n\tInvalid transaction number." << RESET << endl;
            return;
        }
        Purchase* purchases = bills[index].getPurchases();
        for (int j = 0; j < bills[index].getTotalItems(); j++) {
            inventory.updateStock(purchases[j].getMedicineName(), purchases[j].getQuantity());
        }
        if (totalBills == 1) {
            delete[] bills;
            bills = nullptr;
            totalBills = 0;
            cout << GREEN << "\n\tReceipt deleted. Stock restored." << RESET << endl;
            return;
        }
        Bill* temp = new Bill[totalBills - 1];
        for (int i = 0, j = 0; i < totalBills; i++) {
            if (i != index) temp[j++] = bills[i];
        }
        delete[] bills;
        bills = temp;
        totalBills--;
        cout << GREEN << "\n\tReceipt deleted. Stock restored." << RESET << endl;
    }

    // Save databases
    void saveAll() {
        inventory.saveToFile(medicinesFile);
        saveCustomersToFile(customersFile);
        saveBillsToFile(billsFile);
        saveAdminToFile(adminFile);
        saveMonthState();
    }

    // Load databases
    void loadAll() {
        inventory.loadFromFile(medicinesFile);
        loadCustomersFromFile(customersFile);
        loadBillsFromFile(billsFile);
        loadAdminFromFile(adminFile);
        loadMonthState();
        checkMonthRollover();
    }

    void saveAdminToFile(string filename) {
        ofstream fout(filename);
        if (!fout) return;
        fout << adminUsername << endl;
        fout << adminPassword << endl;
        fout.close();
    }

    void loadAdminFromFile(string filename) {
        ifstream fin(filename);
        if (!fin) return;
        getline(fin, adminUsername);
        getline(fin, adminPassword);
        fin.close();
    }

    void saveCustomersToFile(string filename) {
        ofstream fout(filename);
        if (!fout) {
            cout << RED << "Error saving customers database!" << RESET << endl;
            return;
        }
        fout << totalCustomers << endl;
        for (int i = 0; i < totalCustomers; i++) {
            fout << customers[i].getId() << endl;
            fout << customers[i].getName() << endl;
            fout << customers[i].getLocation() << endl;
            fout << customers[i].getPhoneNumber() << endl;
            fout << customers[i].getTotalPurchases() << endl;
            fout << customers[i].getLastPurchaseDate() << endl;
            
            Purchase* purchases = customers[i].getPurchaseHistory();
            for (int j = 0; j < customers[i].getTotalPurchases(); j++) {
                Medicine m = purchases[j].getMedicine();
                fout << m.getMedicineName() << endl;
                fout << m.getTradePrice() << endl;
                fout << m.getStockQuantity() << endl;
                fout << m.getExpiryDate() << endl;
                fout << purchases[j].getQuantity() << endl;
                fout << purchases[j].getPurchaseDate() << endl;
            }
        }
        fout.close();
    }

    void loadCustomersFromFile(string filename) {
        ifstream fin(filename);
        if (!fin) return;
        int count = 0;
        if (!(fin >> count)) {
            fin.close();
            return;
        }
        fin.ignore();
        
        delete[] customers;
        customers = nullptr;
        totalCustomers = 0;
        
        for (int i = 0; i < count; i++) {
            int id = 0;
            string name = "", location = "", phone = "", purchaseDate = "";
            int totalItems = 0;
            
            fin >> id;
            fin.ignore();
            getline(fin, name);
            getline(fin, location);
            getline(fin, phone);
            fin >> totalItems;
            fin.ignore();
            getline(fin, purchaseDate);
            
            Customer c(id, name, 0, location, phone, 0, purchaseDate);
            
            for (int j = 0; j < totalItems; j++) {
                string medName = "", expiry = "", pDate = "";
                double price = 0.0;
                int stock = 0, qty = 0;
                
                getline(fin, medName);
                fin >> price;
                fin >> stock;
                fin.ignore();
                getline(fin, expiry);
                fin >> qty;
                fin.ignore();
                getline(fin, pDate);
                
                Medicine m(medName, price, stock, expiry);
                Purchase p(m, qty, pDate);
                c.addPurchase(p);
            }
            addCustomer(c);
        }
        fin.close();
    }

    void saveBillsToFile(string filename) {
        ofstream fout(filename);
        if (!fout) {
            cout << RED << "Error saving transactions database!" << RESET << endl;
            return;
        }
        fout << totalBills << endl;
        for (int i = 0; i < totalBills; i++) {
            int custId = bills[i].getCustomerId();
            fout << custId << endl;
            fout << bills[i].getBillDate() << endl;
            fout << bills[i].getTotalItems() << endl;
            fout << bills[i].getPaidAmount() << endl;
            fout << bills[i].getDueAmount() << endl;
            
            Purchase* purchases = bills[i].getPurchases();
            for (int j = 0; j < bills[i].getTotalItems(); j++) {
                Medicine m = purchases[j].getMedicine();
                fout << m.getMedicineName() << endl;
                fout << m.getTradePrice() << endl;
                fout << m.getStockQuantity() << endl;
                fout << m.getExpiryDate() << endl;
                fout << purchases[j].getQuantity() << endl;
                fout << purchases[j].getPurchaseDate() << endl;
            }
        }
        fout.close();
    }
    void loadBillsFromFile(string filename) {
        ifstream fin(filename);
        if (!fin) return;
        int count = 0;
        if (!(fin >> count)) {
            fin.close();
            return;
        }
        fin.ignore();
        
        delete[] bills;
        bills = nullptr;
        totalBills = 0;
        
        for (int i = 0; i < count; i++) {
            int custId = -1;
            string billDate = "";
            int totalItems = 0;
            
            fin >> custId;
            fin.ignore();
            getline(fin, billDate);
            fin >> totalItems;
            fin.ignore();
            
            Customer* cust = nullptr;
            if (custId != -1) {
                cust = getCustomerById(custId);
            }
            
            Purchase* purchases = new Purchase[totalItems];
            
            for (int j = 0; j < totalItems; j++) {
                string medName = "", expiry = "", pDate = "";
                double price = 0.0;
                int stock = 0, qty = 0;
                
                getline(fin, medName);
                fin >> price;
                fin >> stock;
                fin.ignore();
                getline(fin, expiry);
                fin >> qty;
                fin.ignore();
                getline(fin, pDate);
                
                Medicine m(medName, price, stock, expiry);
                purchases[j] = Purchase(m, qty, pDate);
            }
            
            Bill b(cust, purchases, totalItems, billDate);
            addBill(b);
            
            delete[] purchases;
        }
        fin.close();
    }

    // Prints a uniform banner header
    void printHeader() {
        cout << CYAN << "========================================================================\n" << RESET;
        cout << GREEN << BOLD << "   ______  __                                   ______                 __  \n";
        cout << "  / __  /_/ /_   ____ _ _____ ____ ___   ____  / __  / ____   ____  __/ /__\n";
        cout << " / /_/ /_/ __ \\ / __ `// ___// __ `__ \\ / __ \\/ /_/ / / __ \\ / __ \\/ / // _/\n";
        cout << "/ ____/ / / / // /_/ // /   / / / / / // /_/ / /__  / / /_/ // /_/ / / ,<   \n";
        cout << "/_/    /_/ /_/ \\__,_//_/   /_/ /_/ /_/ \\____/_____/  \\____/ \\____/_/_/|_|  \n" << RESET;
        cout << CYAN << "========================================================================\n" << RESET;
        cout << WHITE << BOLD << "                      Pharmacy Management System\n" << RESET;
        cout << YELLOW << BOLD << "                    >>> Made by AI Engineer <<<\n" << RESET;
        cout << CYAN << "========================================================================\n" << RESET;
    }

    // Displays the requested beautiful splash screen
    void printSplashScreen() {
        clearScreen();
        printHeader();
        cout << "\n\n";
        cout << "\t" << WHITE << "Welcome to PharmoBook - Elite Database Console" << RESET << "\n";
        
        // Progress bar simulation
        cout << "\n\tInitializing core modules...\n";
        cout << "\t[";
        for (int i = 0; i < 40; i++) {
            cout << " ";
        }
        cout << "]\r\t[";
        cout.flush();
        for (int i = 0; i < 40; i++) {
            sleepMs(40);
            cout << GREEN << "#" << RESET;
            cout.flush();
        }
        cout << "] Done!\n\n";
        sleepMs(600);
        clearScreen();
    }

    // Login system
    bool loginFlow() {
        int attempts = 3;
        while (attempts > 0) {
            clearScreen();
            printHeader();
            cout << "\n\t\t\t" << BOLD << WHITE << "--- ADMIN LOGIN ---" << RESET << "\n\n";
            string user, pass;
            cout << "\tEnter Username: ";
            if (!(cin >> user)) return false;
            cout << "\tEnter Password: ";
            // simple text password for simplicity
            if (!(cin >> pass)) return false;

            if (user == adminUsername && pass == adminPassword) {
                cout << GREEN << "\n\tLogin Successful! Access Granted." << RESET << endl;
                sleepMs(1000);
                return true;
            } else {
                attempts--;
                cout << RED << "\n\tInvalid Username or Password! " << attempts << " attempts remaining." << RESET << endl;
                sleepMs(1500);
            }
        }
        cout << RED << "\n\tAccess Denied! Too many failed attempts." << RESET << endl;
        sleepMs(1500);
        return false;
    }

    void changePasswordMenu() {
        clearScreen();
        printHeader();
        cout << "\n\t\t" << BOLD << WHITE << "--- CHANGE PASSWORD ---" << RESET << "\n\n";
        string oldPass, newPass;
        cout << "\tEnter Current Password: ";
        cin >> oldPass;
        if (oldPass != adminPassword) {
            cout << RED << "\n\tIncorrect current password!" << RESET << endl;
            sleepMs(1500);
            return;
        }
        cout << "\tEnter New Password: ";
        cin >> newPass;
        adminPassword = newPass;
        saveAll();
        cout << GREEN << "\n\tPassword changed successfully!" << RESET << endl;
        sleepMs(1500);
    }

    // Main system execution loop
    void run() {
        loadAll();
        printSplashScreen();
        if (!loginFlow()) {
            return;
        }

        int choice = 0;
        while (true) {
            clearScreen();
            printHeader();
            cout << "\n\t\t\t" << BOLD << WHITE << "--- ADMIN DASHBOARD ---" << RESET << "\n\n";
            cout << "\t[1] Manage Inventory (Medicines)\n";
            cout << "\t[2] Manage Customer Base\n";
            cout << "\t[3] Sales & Billing (Transactions)\n";
            cout << "\t[4] Change Admin Password\n";
            cout << "\t[5] Save & Logout\n";
            cout << "\t[6] Exit Application\n\n";
            cout << "\t" << CYAN << "Monthly Sales (" << currentSalesMonth << "): "
                 << GREEN << "$" << fixed << setprecision(2) << getMonthlySalesTotal()
                 << RESET << " | Invoices: " << totalBills << endl << endl;
            cout << "\tChoose Option (1-6): ";
            
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }

            if (choice == 1) {
                manageInventoryMenu();
            } else if (choice == 2) {
                manageCustomersMenu();
            } else if (choice == 3) {
                billingMenu();
            } else if (choice == 4) {
                changePasswordMenu();
            } else if (choice == 5) {
                saveAll();
                cout << GREEN << "\n\tData saved. Logging out..." << RESET << endl;
                sleepMs(1200);
                if (!loginFlow()) return;
            } else if (choice == 6) {
                saveAll();
                cout << GREEN << "\n\tDatabase saved. Goodbye!" << RESET << endl;
                sleepMs(1200);
                break;
            }
        }
    }

    // Submenu: Inventory
    void manageInventoryMenu() {
        int choice = 0;
        while (true) {
            clearScreen();
            printHeader();
            cout << "\n\t\t\t" << BOLD << WHITE << "--- INVENTORY MANAGEMENT ---" << RESET << "\n\n";
            cout << "\t[1] Add New Medicine\n";
            cout << "\t[2] View All Stocks\n";
            cout << "\t[3] View by Category\n";
            cout << "\t[4] Search Medicine by Name\n";
            cout << "\t[5] Update Stock Quantity\n";
            cout << "\t[6] Delete Medicine Record\n";
            cout << "\t[7] Check Low Stock Alerts\n";
            cout << "\t[8] Check Expired Medicines Alert\n";
            cout << "\t[9] Back to Main Menu\n\n";
            cout << "\tChoose Option (1-9): ";
            
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }

            if (choice == 1) {
                cin.ignore();
                string name, expiry, batch, category;
                double tradePrice = 0.0;
                int stock = 0;
                cout << "\n\tEnter Medicine Name: ";
                getline(cin, name);
                cout << "\tSelect Category:\n";
                cout << "\t  [1] Tablets  [2] Syrups  [3] Antibiotics\n";
                cout << "\t  [4] Capsules [5] Injections [6] Ointments [7] Other\n";
                cout << "\tChoice (1-7): ";
                int catChoice = 7;
                cin >> catChoice;
                cin.ignore();
                switch (catChoice) {
                    case 1: category = "Tablets"; break;
                    case 2: category = "Syrups"; break;
                    case 3: category = "Antibiotics"; break;
                    case 4: category = "Capsules"; break;
                    case 5: category = "Injections"; break;
                    case 6: category = "Ointments"; break;
                    default: category = "Other"; break;
                }
                cout << "\tEnter Batch Number: ";
                getline(cin, batch);
                cout << "\tEnter Trade Price ($): ";
                cin >> tradePrice;
                cout << "\tEnter Stock Quantity: ";
                cin >> stock;
                cin.ignore();
                cout << "\tEnter Expiry Date (YYYY-MM-DD): ";
                getline(cin, expiry);

                Medicine m("", name, category, batch, tradePrice, stock, expiry);
                bool duplicateBlocked = false;
                if (inventory.addMedicine(m, duplicateBlocked)) {
                    saveAll();
                    cout << GREEN << "\n\tMedicine added successfully! Auto ID assigned." << RESET << endl;
                } else if (duplicateBlocked) {
                    cout << RED << "\n\tDuplicate blocked! Same name and batch already exists." << RESET << endl;
                } else {
                    cout << RED << "\n\tCould not add medicine." << RESET << endl;
                }
                sleepMs(1500);
            } else if (choice == 2) {
                clearScreen();
                printHeader();
                cout << "\n\t\t\t" << BOLD << WHITE << "--- CURRENT INVENTORY ---" << RESET << "\n\n";
                inventory.displayInventory();
                cout << "\n\tPress Enter to return...";
                cin.ignore();
                cin.get();
            } else if (choice == 3) {
                cin.ignore();
                string category;
                cout << "\n\tEnter Category (Tablets/Syrups/Antibiotics/Capsules/Injections/Ointments/Other/All): ";
                getline(cin, category);
                clearScreen();
                printHeader();
                cout << "\n\t\t\t" << BOLD << WHITE << "--- INVENTORY BY CATEGORY ---" << RESET << "\n\n";
                inventory.displayByCategory(category);
                cout << "\n\tPress Enter to return...";
                cin.get();
            } else if (choice == 4) {
                cin.ignore();
                string name;
                cout << "\n\tEnter Medicine Name to search: ";
                getline(cin, name);
                clearScreen();
                printHeader();
                cout << "\n\t\t\t" << BOLD << WHITE << "--- SEARCH RESULT ---" << RESET << "\n\n";
                inventory.searchMedicineByName(name);
                cout << "\n\tPress Enter to return...";
                cin.get();
            } else if (choice == 5) {
                cin.ignore();
                string name;
                int qty = 0;
                cout << "\n\tEnter Medicine Name to update: ";
                getline(cin, name);
                cout << "\tEnter Stock Adjustment (use +ve to add, -ve to reduce): ";
                cin >> qty;
                inventory.updateStock(name, qty);
                saveAll();
                sleepMs(1200);
            } else if (choice == 6) {
                cin.ignore();
                string name;
                cout << "\n\tEnter Medicine Name to DELETE: ";
                getline(cin, name);
                inventory.deleteMedicine(name);
                saveAll();
                sleepMs(1500);
            } else if (choice == 7) {
                clearScreen();
                printHeader();
                cout << "\n\t\t" << BOLD << WHITE << "--- LOW STOCK ALERTS ---" << RESET << "\n\n";
                inventory.lowStockAlert();
                cout << "\n\tPress Enter to return...";
                cin.ignore();
                cin.get();
            } else if (choice == 8) {
                clearScreen();
                printHeader();
                cout << "\n\t\t" << BOLD << WHITE << "--- EXPIRED MEDICINES ALERTS ---" << RESET << "\n\n";
                string curDate = getCurrentDateStr();
                inventory.expiredMedicineAlert(curDate);
                cout << "\n\tPress Enter to return...";
                cin.ignore();
                cin.get();
            } else if (choice == 9) {
                break;
            }
        }
    }

    // Submenu: Customers
    void manageCustomersMenu() {
        int choice = 0;
        while (true) {
            clearScreen();
            printHeader();
            cout << "\n\t\t\t" << BOLD << WHITE << "--- CUSTOMER MANAGEMENT ---" << RESET << "\n\n";
            cout << "\t[1] Add New Customer Record\n";
            cout << "\t[2] View All Customers\n";
            cout << "\t[3] Search Customer by ID\n";
            cout << "\t[4] View Customer Purchase History\n";
            cout << "\t[5] View Full Customer Profile\n";
            cout << "\t[6] Generate Monthly Customer Invoice\n";
            cout << "\t[7] Back to Main Menu\n\n";
            cout << "\tChoose Option (1-7): ";
            
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }

            if (choice == 1) {
                int id = 0;
                string name, loc, phone;
                cout << "\n\tEnter Customer ID: ";
                cin >> id;
                cin.ignore();
                cout << "\tEnter Name: ";
                getline(cin, name);
                cout << "\tEnter Location: ";
                getline(cin, loc);
                cout << "\tEnter Phone Number: ";
                getline(cin, phone);

                // Check if ID already exists
                if (getCustomerById(id) != nullptr) {
                    cout << RED << "\n\tCustomer ID already exists!" << RESET << endl;
                    sleepMs(1500);
                    continue;
                }

                Customer c(id, name, 0, loc, phone, 0, getCurrentDateStr());
                addCustomer(c);
                saveAll();
                cout << GREEN << "\n\tCustomer added successfully!" << RESET << endl;
                sleepMs(1200);
            } else if (choice == 2) {
                clearScreen();
                printHeader();
                cout << "\n\t\t\t" << BOLD << WHITE << "--- REGISTERED CUSTOMERS ---" << RESET << "\n\n";
                if (totalCustomers == 0) {
                    cout << "\tNo customer records found." << endl;
                } else {
                    for (int i = 0; i < totalCustomers; i++) {
                        cout << "Record No. " << (i+1) << endl;
                        customers[i].display();
                        cout << "------------------------------------------" << endl;
                    }
                }
                cout << "\n\tPress Enter to return...";
                cin.ignore();
                cin.get();
            } else if (choice == 3) {
                int id = 0;
                cout << "\n\tEnter Customer ID to search: ";
                cin >> id;
                Customer* c = getCustomerById(id);
                clearScreen();
                printHeader();
                cout << "\n\t\t\t" << BOLD << WHITE << "--- CUSTOMER RECORD ---" << RESET << "\n\n";
                if (c != nullptr) {
                    c->display();
                } else {
                    cout << RED << "\tCustomer ID not found." << RESET << endl;
                }
                cout << "\n\tPress Enter to return...";
                cin.ignore();
                cin.get();
            } else if (choice == 4) {
                int id = 0;
                cout << "\n\tEnter Customer ID: ";
                cin >> id;
                Customer* c = getCustomerById(id);
                clearScreen();
                printHeader();
                cout << "\n\t\t\t" << BOLD << WHITE << "--- PURCHASE HISTORY ---" << RESET << "\n\n";
                if (c != nullptr) {
                    c->displayPurchasedMedicines();
                } else {
                    cout << RED << "\tCustomer ID not found." << RESET << endl;
                }
                cout << "\n\tPress Enter to return...";
                cin.ignore();
                cin.get();
            } else if (choice == 5) {
                int id = 0;
                cout << "\n\tEnter Customer ID: ";
                cin >> id;
                Customer* c = getCustomerById(id);
                clearScreen();
                printHeader();
                if (c != nullptr) {
                    c->displayFullProfile();
                } else {
                    cout << RED << "\tCustomer ID not found." << RESET << endl;
                }
                cout << "\n\tPress Enter to return...";
                cin.ignore();
                cin.get();
            } else if (choice == 6) {
                generateMonthlyInvoice();
            } else if (choice == 7) {
                break;
            }
        }
    }

    void generateMonthlyInvoice() {
        int id = 0;
        cout << "\n\tEnter Customer ID: ";
        cin >> id;
        Customer* c = getCustomerById(id);
        if (c == nullptr) {
            cout << RED << "\tCustomer ID not found." << RESET << endl;
            sleepMs(1500);
            return;
        }

        string filterMonth;
        cout << "\tEnter Month and Year (YYYY-MM): ";
        cin >> filterMonth;

        Purchase* ph = c->getPurchaseHistory();
        int totalP = c->getTotalPurchases();
        
        // Filter purchases
        Purchase* filtered = new Purchase[totalP];
        int fCount = 0;
        for (int i = 0; i < totalP; i++) {
            if (ph[i].getPurchaseDate().find(filterMonth) != string::npos) {
                filtered[fCount++] = ph[i];
            }
        }

        if (fCount == 0) {
            cout << YELLOW << "\n\tNo purchases found for " << filterMonth << "." << RESET << endl;
            delete[] filtered;
            sleepMs(1500);
            return;
        }

        string outName = "Invoice_" + to_string(id) + "_" + filterMonth + ".txt";
        ofstream fout(outName);
        if (!fout) {
            cout << RED << "\n\tError creating invoice file." << RESET << endl;
            delete[] filtered;
            return;
        }

        // Generate the output to both console and file
        ostringstream oss;
        oss << "====================================================================================================\n";
        oss << "                                        BILAL PHARMACY                                        \n";
        oss << "                     OPP: DHQ Hospital Bhimber AJK Ph# 03088788994                           \n";
        oss << "====================================================================================================\n";
        oss << " M/S: " << left << setw(35) << c->getName() << " | LIC#       : " << "\n";
        oss << " AREA: " << left << setw(34) << c->getLocation() << " | LIC. EXPIRY: " << "\n";
        oss << " Inv. Date: " << left << setw(29) << getCurrentDateStr() << " | Inv. No.   : " << "INV-" << filterMonth << "-" << id << "\n";
        oss << "----------------------------------------------------------------------------------------------------\n";
        oss << " QTY  | " << left << setw(20) << "Name of Item" << " | Packing  | Batch No. | T.P   | Gross Amount | Discount | Total Amount\n";
        oss << "----------------------------------------------------------------------------------------------------\n";
        
        double grandTotal = 0;
        int totalPacks = 0;
        for (int i = 0; i < fCount; i++) {
            double tp = filtered[i].getTradePrice();
            int qty = filtered[i].getQuantity();
            double gross = tp * qty;
            double discount = 0.0;
            double total = gross - discount;
            grandTotal += total;
            totalPacks += qty;

            oss << " " << right << setw(4) << qty << " | "
                << left << setw(20) << filtered[i].getMedicineName().substr(0,20) << " | "
                << left << setw(8) << "Box" << " | "
                << left << setw(9) << filtered[i].getMedicine().getBatchNumber().substr(0,9) << " | "
                << right << setw(5) << fixed << setprecision(2) << tp << " | "
                << right << setw(12) << fixed << setprecision(2) << gross << " | "
                << right << setw(8) << fixed << setprecision(2) << discount << " | "
                << right << setw(12) << fixed << setprecision(2) << total << "\n";
        }
        
        oss << "----------------------------------------------------------------------------------------------------\n";
        oss << "                                                           Total Packs: " << totalPacks << "\n";
        oss << "                                                           Total Items: " << fCount << "\n";
        oss << "                                                           GROSS:       " << fixed << setprecision(2) << grandTotal << "\n";
        oss << "                                                           DISCOUNT:    " << "0.00" << "\n";
        oss << "====================================================================================================\n";
        oss << "                                                        [ Total Net Value: " << fixed << setprecision(2) << grandTotal << " ]\n";
        oss << "====================================================================================================\n";
        oss << " (i) FORM 2A Warranty: I/We hereby warranty that the drugs sold under this invoice do not\n";
        oss << "     contravene any provisions of the Drugs Act 1976 and the rules framed thereunder.\n";
        oss << " (ii) Alternative Medicine Rules: The products are supplied in accordance with the alternative\n";
        oss << "      medicine regulations. Non-returnable and non-refundable.\n";
        oss << "====================================================================================================\n";
        
        cout << oss.str();
        fout << oss.str();

        fout.close();
        delete[] filtered;
        
        cout << GREEN << "\n\tMonthly Invoice generated and saved to " << outName << RESET << endl;
        cout << "\n\tPress Enter to return...";
        cin.ignore();
        cin.get();
    }

    // Submenu: Billing
    void billingMenu() {
        int choice = 0;
        while (true) {
            clearScreen();
            printHeader();
            cout << "\n\t\t\t" << BOLD << WHITE << "--- SALES & TRANSACTIONS ---" << RESET << "\n\n";
            checkMonthRollover();
            cout << "\t[1] Create New Order / Invoice\n";
            cout << "\t[2] View Transaction History\n";
            cout << "\t[3] View Sales Revenue Summary\n";
            cout << "\t[4] View Monthly Sales Total\n";
            cout << "\t[5] Delete Receipt / Invoice\n";
            cout << "\t[6] Back to Main Menu\n\n";
            cout << "\t" << CYAN << "Current Month (" << currentSalesMonth << ") Sales: "
                 << GREEN << "$" << fixed << setprecision(2) << getMonthlySalesTotal() << RESET << endl << endl;
            cout << "\tChoose Option (1-6): ";
            
            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }

            if (choice == 1) {
                // billing flow
                int custId = -1;
                cout << "\n\tEnter Customer ID (-1 for Generic Guest): ";
                cin >> custId;
                
                Customer* cust = nullptr;
                if (custId != -1) {
                    cust = getCustomerById(custId);
                    if (cust == nullptr) {
                        char opt;
                        cout << RED << "\tCustomer ID not found." << RESET << " Add new customer? (y/n): ";
                        cin >> opt;
                        if (opt == 'y' || opt == 'Y') {
                            string name, loc, phone;
                            cin.ignore();
                            cout << "\tEnter Name: ";
                            getline(cin, name);
                            cout << "\tEnter Location: ";
                            getline(cin, loc);
                            cout << "\tEnter Phone Number: ";
                            getline(cin, phone);
                            Customer newC(custId, name, 0, loc, phone, 0, getCurrentDateStr());
                            addCustomer(newC);
                            saveAll();
                            cust = getCustomerById(custId);
                            cout << GREEN << "\tCustomer added and selected." << RESET << endl;
                        } else {
                            cout << YELLOW << "\tProceeding as Generic Guest..." << RESET << endl;
                        }
                    }
                }

                // Temporary arrays for new bill items
                Purchase* tempPurchases = nullptr;
                int tempCount = 0;

                while (true) {
                    clearScreen();
                    printHeader();
                    cout << "\n\t\t" << BOLD << WHITE << "--- SELECT MEDICINE ---" << RESET << "\n\n";
                    
                    inventory.displayInventory();
                    
                    cin.ignore();
                    string medName;
                    cout << "\n\tEnter Medicine Name to buy: ";
                    getline(cin, medName);
                    
                    Medicine* med = inventory.getMedicineByName(medName);
                    if (med == nullptr) {
                        cout << RED << "\tMedicine not found in inventory!" << RESET << endl;
                        sleepMs(1200);
                    } else {
                        cout << GREEN << "\tFound: " << med->getMedicineName() << " | Price: $" << med->getPrice() << " | Stock: " << med->getStockQuantity() << RESET << endl;
                        int qty = 0;
                        cout << "\tEnter Quantity to buy: ";
                        cin >> qty;
                        
                        if (qty <= 0) {
                            cout << RED << "\tInvalid quantity!" << RESET << endl;
                            sleepMs(1000);
                        } else if (qty > med->getStockQuantity()) {
                            cout << RED << "\tInsufficient stock! Only " << med->getStockQuantity() << " available." << RESET << endl;
                            sleepMs(1500);
                        } else {
                            // Deduct from inventory stock
                            med->reduceStock(qty);
                            
                            // Add item to temporary array
                            Purchase* newTempPurchases = new Purchase[tempCount + 1];
                            for (int i = 0; i < tempCount; i++) {
                                newTempPurchases[i] = tempPurchases[i];
                            }
                            newTempPurchases[tempCount] = Purchase(*med, qty, getCurrentDateStr());
                            
                            delete[] tempPurchases;
                            tempPurchases = newTempPurchases;
                            tempCount++;
                            
                            cout << GREEN << "\tAdded to invoice." << RESET << endl;
                            sleepMs(800);
                        }
                    }
                    
                    char another;
                    cout << "\n\tAdd another medicine to this invoice? (y/n): ";
                    cin >> another;
                    if (another != 'y' && another != 'Y') {
                        break;
                    }
                }

                if (tempCount > 0) {
                    string currentDate = getCurrentDateStr();
                    Bill newBill(cust, tempPurchases, tempCount, currentDate);
                    
                    // Add bill to history
                    addBill(newBill);
                    
                    // If registered customer, record it in customer's purchase history
                    if (cust != nullptr) {
                        for (int i = 0; i < tempCount; i++) {
                            cust->addPurchase(tempPurchases[i]);
                        }
                    }

                    // Save state
                    saveAll();

                    clearScreen();
                    printHeader();
                    cout << "\n\t\t\t" << BOLD << GREEN << "--- INVOICE GENERATED ---" << RESET << "\n\n";
                    newBill.generateBill();
                    
                    delete[] tempPurchases;
                } else {
                    cout << YELLOW << "\n\tInvoice cancelled. No items purchased." << RESET << endl;
                }
                
                cout << "\n\tPress Enter to continue...";
                cin.ignore();
                cin.get();
                
            } else if (choice == 2) {
                clearScreen();
                printHeader();
                cout << "\n\t\t\t" << BOLD << WHITE << "--- TRANSACTION HISTORY ---" << RESET << "\n\n";
                if (totalBills == 0) {
                    cout << "\tNo transactions recorded." << endl;
                } else {
                    for (int i = 0; i < totalBills; i++) {
                        cout << "Transaction No. " << (i+1) << endl;
                        bills[i].generateBill();
                        cout << "==========================================" << endl;
                    }
                }
                cout << "\n\tPress Enter to return...";
                cin.ignore();
                cin.get();
            } else if (choice == 3) {
                clearScreen();
                printHeader();
                cout << "\n\t\t" << BOLD << WHITE << "--- SALES REVENUE SUMMARY ---" << RESET << "\n\n";
                
                double totalSales = 0.0;
                int totalPurchasedItems = 0;
                for (int i = 0; i < totalBills; i++) {
                    totalSales += bills[i].getTotalBill();
                    totalPurchasedItems += bills[i].getTotalItems();
                }
                
                cout << "\tTotal Invoices Generated: " << totalBills << endl;
                cout << "\tTotal Items Sold:        " << totalPurchasedItems << endl;
                cout << "\tTotal Sales Revenue:     " << GREEN << "$" << fixed << setprecision(2) << totalSales << RESET << endl;
                cout << "----------------------------------------------------" << endl;
                cout << "\tTotal Registered Customers: " << totalCustomers << endl;
                cout << "\tTotal Medicines in System:  " << inventory.getTotalMedicines() << endl;
                cout << "\n\tPress Enter to return...";
                cin.ignore();
                cin.get();
            } else if (choice == 4) {
                clearScreen();
                printHeader();
                cout << "\n\t\t" << BOLD << WHITE << "--- MONTHLY SALES (" << currentSalesMonth << ") ---" << RESET << "\n\n";
                cout << "\tTotal Invoices:     " << totalBills << endl;
                cout << "\tMonthly Revenue:    " << GREEN << "$" << fixed << setprecision(2) << getMonthlySalesTotal() << RESET << endl;
                cout << "\tArchived reports in: archives/sales_YYYY-MM.txt" << endl;
                cout << "\n\tPress Enter to return...";
                cin.ignore();
                cin.get();
            } else if (choice == 5) {
                if (totalBills == 0) {
                    cout << YELLOW << "\n\tNo receipts to delete." << RESET << endl;
                    sleepMs(1200);
                } else {
                    int num = 0;
                    cout << "\n\tEnter Transaction No. to DELETE (1-" << totalBills << "): ";
                    cin >> num;
                    deleteBillByIndex(num - 1);
                    saveAll();
                    sleepMs(1500);
                }
            } else if (choice == 6) {
                break;
            }
        }
    }
};

#endif
