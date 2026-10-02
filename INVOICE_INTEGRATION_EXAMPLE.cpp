// =====================================================
// INTEGRATION EXAMPLE: Invoice Generator Usage
// =====================================================
// This file demonstrates how to integrate the Invoice
// Generator with PharmacySystem for producing
// professional pharmaceutical invoices.
// =====================================================

/*

BASIC USAGE EXAMPLE:
====================

// In PharmacySystem::processPurchase() or similar function:

void PharmacySystem::generateInvoiceForPurchase(
    Customer* customer, 
    Purchase* purchases, 
    int itemCount,
    string invoiceNumber)
{
    // Create a bill with customer and purchase information
    Bill invoice(customer, purchases, itemCount, invoiceNumber);
    
    // Option 1: Display on console
    cout << "Generating Invoice..." << endl;
    invoice.generateFormattedInvoice();
    
    // Option 2: Save to file
    string filename = "invoices/" + invoiceNumber + ".txt";
    invoice.generateFormattedInvoice(filename);
    cout << "Invoice saved to: " << filename << endl;
}


USAGE IN A SALES TRANSACTION:
=============================

// Example workflow in PharmacySystem
void PharmacySystem::completeSale() {
    // Get customer
    Customer* customer = findCustomerById(customerId);
    
    // Collect purchases (medicines and quantities)
    vector<Purchase> purchaseList;
    // ... add medicines to purchaseList ...
    
    // Create array from vector
    Purchase* purchases = &purchaseList[0];
    
    // Generate invoice
    Bill saleBill(customer, purchases, purchaseList.size(), generateInvoiceNumber());
    
    // Save to file for records
    saleBill.generateFormattedInvoice("records/invoice_" + getTimestamp() + ".txt");
    
    // Display to customer
    saleBill.generateFormattedInvoice();
    
    // Store bill in system
    addBillToSystem(saleBill);
}


CUSTOMIZING INVOICE DETAILS:
============================

Bill invoice(customer, purchases, count, invNumber);

// If customer is null or needs updates:
invoice.setCustomerName("Guest Purchase");
invoice.setCustomerPhone("03088788994");
invoice.setCustomerLocation("Bhimber");

// Generate with custom formatting
invoice.generateFormattedInvoice("sales_" + getCurrentDate() + ".txt");


FILE STRUCTURE RECOMMENDATION:
==============================

PharmoBook/
├── invoices/           # Directory for saved invoices
│   ├── INV-2026-0001.txt
│   ├── INV-2026-0002.txt
│   └── ...
├── Bill.h             # Contains Invoice Generator (UPDATED)
├── PharmacySystem.h   # Main system (integrates Bill)
└── medicine.cpp       # Main entry point


INVOICE OUTPUT FEATURES:
=======================

✓ Professional header with pharmacy branding
✓ Customer information fields
✓ Clean grid-based medicine table
✓ Trade price calculations (T.P only)
✓ Discount support (defaults to 0%)
✓ Summary totals with boxing
✓ Warranty and legal disclaimers
✓ Ready for printing/filing


EXAMPLE MODIFIED FILE USAGE:
============================

// In PharmacySystem::run() main loop
case 'I':  // Invoice option
    // Get customer and purchases
    // Create bill
    // Generate invoice
    {
        int custId;
        cout << "Enter Customer ID: ";
        cin >> custId;
        
        Customer* cust = findCustomer(custId);
        if (cust && cust->getTotalPurchases() > 0) {
            Purchase* hist = cust->getPurchaseHistory();
            int count = cust->getTotalPurchases();
            
            Bill invoice(cust, hist, count, getLatestInvoiceNumber());
            
            cout << "\n=== INVOICE ===" << endl;
            invoice.generateFormattedInvoice();
            
            // Save for records
            invoice.generateFormattedInvoice("invoices/" + getLatestInvoiceNumber() + ".txt");
        }
    }
    break;


TESTING THE INVOICE GENERATOR:
==============================

Compile and run invoice_test.cpp:
    g++ -std=c++11 invoice_test.cpp -o invoice_test.exe
    ./invoice_test.exe

This will:
1. Create sample medicines with batch numbers
2. Create purchases with quantities
3. Generate invoice to console
4. Save invoice to "invoice_output.txt"


KEY METHODS IN BILL CLASS:
==========================

PUBLIC:
------
generateFormattedInvoice(filename="")
  - filename empty: outputs to console
  - filename provided: saves to file

generateFormattedInvoiceToStream(ostream& out)
  - Core method, works with any ostream
  
setCustomerName(name)
setCustomerPhone(phone)
setCustomerLocation(location)
  - Setter methods for flexibility


DESIGN ADVANTAGES:
==================

✓ Modular: Each part is a separate private method
✓ Flexible: Works with any ostream (console, file, string)
✓ Maintainable: Well-organized and clearly commented
✓ Efficient: Streaming-based, minimal memory overhead
✓ Professional: Production-ready formatting
✓ Compliant: Includes required pharmaceutical disclaimers
✓ Extensible: Easy to add features like custom discounts

*/
