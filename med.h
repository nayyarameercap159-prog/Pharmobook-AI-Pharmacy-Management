#include<iostream>
#include<string>
#ifndef MED_H
#define MED_H
using namespace std;

class Medicine{
    private:
    string medicineId;
    string medicineName;
    string category;
    string batchNumber;
    float tradePrice;
    int stockQuantity;
    string expiryDate;
    public:
    Medicine(){
        medicineId="";
        medicineName="";
        category="Other";
        batchNumber="";
        tradePrice=0.0;
        stockQuantity=0;
        expiryDate="";
    }
    Medicine(string id, string medicineName, string category, string batchNumber,
             float tradePrice, int stockQuantity, string expiryDate){
        this->medicineId=id;
        this->medicineName=medicineName;
        this->category=category;
        this->batchNumber=batchNumber;
        this->tradePrice=tradePrice;
        this->stockQuantity=stockQuantity;
        this->expiryDate=expiryDate;
    }
    // Legacy constructor for migration
    Medicine(string medicineName, float price, int stockQuantity, string expiryDate){
        this->medicineId="";
        this->medicineName=medicineName;
        this->category="Other";
        this->batchNumber="";
        this->tradePrice=price;
        this->stockQuantity=stockQuantity;
        this->expiryDate=expiryDate;
    }
    void displayMedicine() const {
        cout<<"Medicine ID: "<<medicineId<<endl;
        cout<<"Medicine Name: "<<medicineName<<endl;
        cout<<"Category: "<<category<<endl;
        cout<<"Batch Number: "<<batchNumber<<endl;
        cout<<"Trade Price: "<<tradePrice<<endl;
        cout<<"Stock Quantity: "<<stockQuantity<<endl;
        cout<<"Expiry Date: "<<expiryDate<<endl;
    }
    void addMedicine(string medicineName, float price, int stockQuantity, string expiryDate){
        this->medicineName=medicineName;
        this->tradePrice=price;
        this->stockQuantity=stockQuantity;
        this->expiryDate=expiryDate;
    }
    void setMedicineId(string id){ medicineId=id; }
    void setCategory(string cat){ category=cat; }
    void setBatchNumber(string batch){ batchNumber=batch; }
    void setTradePrice(float tp){ tradePrice=tp; }
    void updateStock(int quantity){
        stockQuantity+=quantity;
    }
    int getStockQuantity() const {
        return stockQuantity;
    }
    float getPrice() const {
        return tradePrice;
    }
    float getTradePrice() const {
        return tradePrice;
    }
    string getMedicineId() const {
        return medicineId;
    }
    string getMedicineName() const {
        return medicineName;
    }
    string getCategory() const {
        return category;
    }
    string getBatchNumber() const {
        return batchNumber;
    }
    string getExpiryDate() const {
        return expiryDate;
    }
    void reduceStock(int quantity){
        if(stockQuantity>=quantity){
            stockQuantity-=quantity;
        }
        else{
            cout<<"Insufficient stock for "<<medicineName<<endl;
        }
    }
};
#endif
