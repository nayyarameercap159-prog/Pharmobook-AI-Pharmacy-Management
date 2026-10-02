#include "Bill.h"
#include "Customer.h"
#include "Purchase.h"
#include "med.h"
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    // Create sample medicines
    Medicine med1("MED001", "Paracetamol 500mg", "Pain Relief", "BATCH2601", 15.50, 100, "2026-06-01");
    Medicine med2("MED002", "Amoxicillin 250mg", "Antibiotics", "BATCH2502", 45.00, 50, "2026-05-15");
    Medicine med3("MED003", "Ibuprofen 400mg", "Pain Relief", "BATCH2604", 25.75, 75, "2026-07-10");

    // Create purchases
    Purchase p1(med1, 3, "2026-05-25");
    Purchase p2(med2, 2, "2026-05-25");
    Purchase p3(med3, 4, "2026-05-25");

    Purchase purchases[] = {p1, p2, p3};

    // Create a bill
    Bill bill(nullptr, purchases, 3, "INV-2026-0001");
    bill.setCustomerName("Ahmed Hassan");
    bill.setCustomerPhone("03001234567");
    bill.setCustomerLocation("Bhimber, AJK");

    // Generate invoice to console
    cout << "\n========== FORMATTED INVOICE (Console Output) ==========\n" << endl;
    bill.generateFormattedInvoice();

    // Generate invoice to file
    cout << "\n========== Generating Invoice to File ==========\n" << endl;
    bill.generateFormattedInvoice("invoice_output.txt");
    cout << "Invoice saved to: invoice_output.txt" << endl;

    return 0;
}
