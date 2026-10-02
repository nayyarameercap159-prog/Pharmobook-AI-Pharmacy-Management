# Pharmobook-AI-Pharmacy-Management


# 💊 PharmoBook

**A Pharmacy Management System written in C++ using Object-Oriented Programming.**

PharmoBook helps a pharmacy manage its medicine inventory, customer records, sales and billing from a colorful terminal interface. All data is stored in plain text files, so no database setup is required. It also generates professional printable invoices for **BILAL PHARMACY** (Bhimber, AJK).

![C++](https://img.shields.io/badge/C%2B%2B-11-blue?logo=cplusplus)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)
![Storage](https://img.shields.io/badge/storage-text%20files-green)

---

## ✨ Features

### 🔐 Admin Login
- Username/password protected access
- Change admin password from inside the app
- Credentials persisted between sessions

### 📦 Inventory Management
- Add medicines with auto-generated IDs (`MED-0001`, `MED-0002`, ...)
- Categories: Tablets, Syrups, Antibiotics, Capsules, Injections, Ointments, Other
- Duplicate protection (same name + batch number is blocked, case-insensitive)
- View all stock, filter by category, search by name
- Update stock quantity or delete a medicine record
- **Low stock alerts** (stock below 5 units)
- **Expired medicine alerts** based on expiry date

### 👥 Customer Management
- Add and view customer records
- Search customer by ID
- View purchase history and full customer profile (total spent, most bought medicine, last purchase)
- Generate monthly customer invoices

### 🧾 Sales & Billing
- Create new orders/invoices (stock is reduced automatically)
- View transaction history
- Sales revenue summary and monthly sales total
- Delete a receipt (**stock is restored automatically**)
- **Automatic monthly rollover:** when a new month starts, the previous month's sales are archived to `archives/sales_YYYY-MM.txt` and the sales counter resets

### 🖨️ Formatted Invoice Generator
- Professional pharmacy invoice layout with header, customer details, item grid and footer
- Columns: `QTY | Name of Item | Packing | Batch No. | T.P | Gross | Discount | Total`
- Trade Price (T.P) based pricing: `Gross = QTY × T.P`
- Summary block: Total Packs, Total Items, Gross, Discount and boxed **Net Total**
- Footer with Form 2A warranty and alternative medicine rules
- Output to console, to a file, or to any `ostream`

---

## 🧱 OOP Concepts Demonstrated

| Concept | Where it is used |
|---|---|
| **Inheritance** | `Customer` and `Admin` inherit from `Person` |
| **Polymorphism** | Virtual `display()` and virtual destructor in `Person` |
| **Encapsulation** | Private data members with public getters/setters in every class |
| **Composition** | `Purchase` contains a `Medicine`; `Bill` contains `Purchase` objects |
| **Dynamic memory** | Manual dynamic arrays (`new[]` / `delete[]`) with no STL containers |
| **Rule of Three** | Copy constructor, copy assignment and destructor in `Bill` and `Customer` |
| **File handling** | Persistent storage using `ifstream` / `ofstream` |

---

## 📁 Project Structure

```
PharmoBook/
├── medicine.cpp        # Entry point (main)
├── PharmacySystem.h    # Core system: menus, file I/O, sales, monthly rollover
├── Person.h            # Base class (id, name, location)
├── Admin.h             # Admin login (inherits Person)
├── Customer.h          # Customer with purchase history (inherits Person)
├── med.h               # Medicine class
├── Inventory.h         # Inventory management (dynamic array of Medicine)
├── Purchase.h          # A single purchase (Medicine + quantity + date)
├── Bill.h              # Bill + formatted invoice generator
├── invoice_test.cpp    # Standalone demo/test for the invoice generator
└── INVOICE_INTEGRATION_EXAMPLE.cpp  # Invoice integration guide
```

### Class Overview

```
Person
├── Admin
└── Customer ──has──► Purchase[] ──has──► Medicine

Inventory ──manages──► Medicine[]
Bill ──has──► Purchase[]
PharmacySystem ──uses──► Inventory, Customer[], Bill[]
```

---

## 🚀 Getting Started

### Requirements
- A C++ compiler with C++11 support (g++, MinGW, TDM-GCC, Dev-C++, MSVC)

### Build and Run

**Windows / Linux / macOS (g++):**
```bash
g++ -std=c++11 medicine.cpp -o pharmobook
./pharmobook        # on Windows: pharmobook.exe
```

**Run the invoice generator demo only:**
```bash
g++ -std=c++11 invoice_test.cpp -o invoice_test
./invoice_test
```
This prints a sample invoice to the console and saves it to `invoice_output.txt`.

### Default Login

| Username | Password |
|---|---|
| `admin` | `password` |

> ⚠️ **Change the default password after your first login** (Main Menu → option 4).

---

## 🗂️ Data Files

PharmoBook creates these files automatically in the working directory:

| File | Contents |
|---|---|
| `medicines_db.txt` | Inventory records |
| `customers_db.txt` | Customers and purchase history |
| `bills_db.txt` | Current month's transactions |
| `admin_db.txt` | Admin credentials |
| `month_state.txt` | Current sales month (for rollover) |
| `archives/sales_YYYY-MM.txt` | Archived monthly sales reports |

> 💡 Add these to your `.gitignore` so real pharmacy data is never pushed to GitHub:
> ```
> *_db.txt
> month_state.txt
> archives/
> invoice_output.txt
> ```

---

## 🖥️ Menu Overview

```
MAIN MENU
 [1] Manage Inventory (Medicines)
 [2] Manage Customer Base
 [3] Sales & Billing (Transactions)
 [4] Change Admin Password
 [5] Save & Logout
 [6] Exit Application

INVENTORY                         SALES & BILLING
 [1] Add New Medicine              [1] Create New Order / Invoice
 [2] View All Stocks               [2] View Transaction History
 [3] View by Category              [3] View Sales Revenue Summary
 [4] Search Medicine by Name       [4] View Monthly Sales Total
 [5] Update Stock Quantity         [5] Delete Receipt / Invoice
 [6] Delete Medicine Record        [6] Back to Main Menu
 [7] Check Low Stock Alerts
 [8] Check Expired Medicines
 [9] Back to Main Menu
```

---

## 🧾 Sample Invoice Output

```
                    BILAL PHARMACY
      OPP: DHQ Hospital Bhimber AJK Ph# 03088788994

M/S: Ahmed Hassan                          LIC#: ___________
Inv. No.: INV-2026-0001                    AREA: ___________

+--------+---------------------------+--------+--------+-------+---------+---------+---------+
| QTY    | Name of Item              | Packing | Batch No.| T.P   | Gross   | Discount| Total   |
+--------+---------------------------+--------+--------+-------+---------+---------+---------+
|      3 | Paracetamol 500mg         | 1 Strip | BATCH2601|  15.50|   46.50 |    0.00 |   46.50 |
...
                                   Total Packs:         9
                                   GROSS:          238.25

+--------- TOTAL NET VALUE --------+
|  NET TOTAL: 238.25               |
+-----------------------------------+
```

### Using the invoice generator in your own code

```cpp
Bill bill(customerPtr, purchases, itemCount, "INV-2026-0001");

bill.generateFormattedInvoice();                  // print to console
bill.generateFormattedInvoice("invoice_001.txt"); // save to file
bill.generateFormattedInvoiceToStream(myStream);  // any ostream
```

---

## ⚠️ Known Limitations

- Admin password is stored in **plain text** (suitable for learning/demo, not production)
- Data is stored in text files, not a real database
- Customer purchase history stores medicine snapshots, not inventory references
- Invoice discount defaults to 0 and is not yet configurable from the menu

---

## 🔮 Future Improvements

- Per-item discount percentages and tax (GST/VAT) support
- Password hashing
- Search by medicine ID and partial name matching
- SQLite database backend
- Barcode / QR code support on invoices
- Role-based access (admin vs. cashier)

---

## 👨‍💻 Author

**Muhammad Nayyar Ameer**
GitHub: [@your-username](https://github.com/your-username)

---

## 📄 License

This project is licensed under the MIT License. Feel free to use, modify and learn from it.
