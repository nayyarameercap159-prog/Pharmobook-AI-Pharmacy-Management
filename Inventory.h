#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<iomanip>
#include<algorithm>
#include<cctype>
#include"med.h"
#ifndef INVENTORY_H
#define INVENTORY_H
using namespace std;

inline string toLowerStr(string s){
    for(size_t i=0;i<s.length();i++){
        s[i]=(char)tolower((unsigned char)s[i]);
    }
    return s;
}

class Inventory{
    private:
    Medicine* medicines;
    int totalMedicines;
    int nextMedId;

    string generateMedicineId(){
        stringstream ss;
        ss<<"MED-"<<setfill('0')<<setw(4)<<nextMedId;
        nextMedId++;
        return ss.str();
    }

    public:
    Inventory(){
        medicines=nullptr;
        totalMedicines=0;
        nextMedId=1;
    }

    bool isDuplicate(string name, string batch, int skipIndex=-1){
        string nLower=toLowerStr(name);
        string bLower=toLowerStr(batch);
        for(int i=0;i<totalMedicines;i++){
            if(i==skipIndex) continue;
            if(toLowerStr(medicines[i].getMedicineName())==nLower &&
               toLowerStr(medicines[i].getBatchNumber())==bLower){
                return true;
            }
        }
        return false;
    }

    bool addMedicine(Medicine med, bool& duplicateBlocked){
        duplicateBlocked=false;
        if(isDuplicate(med.getMedicineName(), med.getBatchNumber())){
            duplicateBlocked=true;
            return false;
        }
        if(med.getMedicineId().empty()){
            med.setMedicineId(generateMedicineId());
        }
        Medicine* temp=new Medicine[totalMedicines+1];
        for(int i=0;i<totalMedicines;i++){
            temp[i]=medicines[i];
        }
        temp[totalMedicines]=med;
        delete[] medicines;
        medicines=temp;
        totalMedicines++;
        return true;
    }

    void addMedicine(Medicine med){
        bool dup=false;
        addMedicine(med, dup);
    }

    void displayInventory(){
        if(totalMedicines==0){
            cout<<"Inventory is empty."<<endl;
            return;
        }
        for(int i=0;i<totalMedicines;i++){
            cout<<"No. "<<(i+1)<<endl;
            medicines[i].displayMedicine();
            cout<<"------------------"<<endl;
        }
    }

  void displayByCategory(string categoryFilter){
        if(totalMedicines==0){
            cout<<"Inventory is empty."<<endl;
            return;
        }
        bool found=false;
        string filterLower=toLowerStr(categoryFilter);
        for(int i=0;i<totalMedicines;i++){
            if(filterLower=="all" || toLowerStr(medicines[i].getCategory())==filterLower){
                medicines[i].displayMedicine();
                cout<<"------------------"<<endl;
                found=true;
            }
        }
        if(!found) cout<<"No medicines in this category."<<endl;
    }

    ~Inventory(){
        delete[] medicines;
    }

    void searchMedicineByName(string name){
        for(int i=0;i<totalMedicines;i++){
            if(medicines[i].getMedicineName()==name){
                medicines[i].displayMedicine();
                return;
            }
        }
        cout<<"Medicine not found"<<endl;
    }

    void updateStock(string name,int quantity){
        for(int i=0;i<totalMedicines;i++){
            if(medicines[i].getMedicineName()==name){
                medicines[i].updateStock(quantity);
                return;
            }
        }
        cout<<"Medicine not found"<<endl;
    }

    void reduceStock(string name,int quantity){
        for(int i=0;i<totalMedicines;i++){
            if(medicines[i].getMedicineName()==name){
                medicines[i].reduceStock(quantity);
                return;
            }
        }
        cout<<"Medicine not found"<<endl;
    }

    void deleteMedicine(string name){
        if(totalMedicines==0){
            cout<<"Inventory is empty."<<endl;
            return;
        }
        int index=-1;
        for(int i=0;i<totalMedicines;i++){
            if(medicines[i].getMedicineName()==name){
                index=i;
                break;
            }
        }
        if(index==-1){
            cout<<"Medicine not found"<<endl;
            return;
        }
        if(totalMedicines==1){
            delete[] medicines;
            medicines=nullptr;
            totalMedicines=0;
            cout<<"Medicine deleted successfully. Inventory is now empty."<<endl;
            return;
        }
        Medicine* temp=new Medicine[totalMedicines-1];
        for(int i=0,j=0;i<totalMedicines;i++){
            if(i!=index){
                temp[j++]=medicines[i];
            }
        }
        delete[] medicines;
        medicines=temp;
        totalMedicines--;
        cout<<"Medicine deleted successfully."<<endl;
    }

    void lowStockAlert(){
        int threshold=5;
        cout<<"Medicines with stock below "<<threshold<<":"<<endl;
        bool found=false;
        for(int i=0;i<totalMedicines;i++){
            if(medicines[i].getStockQuantity()<threshold){
                medicines[i].displayMedicine();
                cout<<"------------------"<<endl;
                found=true;
            }
        }
        if(!found){
            cout<<"None found."<<endl;
        }
    }

    void expiredMedicineAlert(string currentDate){
        cout<<"Expired Medicines as of "<<currentDate<<":"<<endl;
        bool found=false;
        for(int i=0;i<totalMedicines;i++){
            if(medicines[i].getExpiryDate()<currentDate){
                medicines[i].displayMedicine();
                cout<<"------------------"<<endl;
                found=true;
            }
        }
        if(!found){
            cout<<"None found."<<endl;
        }
    }

    int getTotalMedicines(){
        return totalMedicines;
    }

    int getNextMedId(){
        return nextMedId;
    }

    void setNextMedId(int id){
        nextMedId=id;
    }

    Medicine* getMedicines(){
        return medicines;
    }

    Medicine* getMedicineByIndex(int index){
        if(index>=0 && index<totalMedicines){
            return &medicines[index];
        }
        return nullptr;
    }

    Medicine* getMedicineByName(string name){
        for(int i=0;i<totalMedicines;i++){
            if(medicines[i].getMedicineName()==name){
                return &medicines[i];
            }
        }
        return nullptr;
    }

    void saveToFile(string filename){
        ofstream fout(filename);
        if(!fout){
            cout<<"Error opening file for saving inventory."<<endl;
            return;
        }
        fout<<2<<endl; // format version
        fout<<nextMedId<<endl;
        fout<<totalMedicines<<endl;
        for(int i=0;i<totalMedicines;i++){
            fout<<medicines[i].getMedicineId()<<endl;
            fout<<medicines[i].getMedicineName()<<endl;
            fout<<medicines[i].getCategory()<<endl;
            fout<<medicines[i].getBatchNumber()<<endl;
            fout<<medicines[i].getTradePrice()<<endl;
            fout<<medicines[i].getStockQuantity()<<endl;
            fout<<medicines[i].getExpiryDate()<<endl;
        }
        fout.close();
    }

    void loadFromFile(string filename){
        ifstream fin(filename);
        if(!fin){
            return;
        }
        int version=1;
        int count=0;
        string line;
        if(getline(fin, line)){
            stringstream ss(line);
            ss>>version;
        }
        if(version>=2){
            fin>>nextMedId;
            fin.ignore();
            fin>>count;
            fin.ignore();
        } else {
            count=version;
            fin.ignore();
            nextMedId=1;
        }
        delete[] medicines;
        medicines=nullptr;
        totalMedicines=0;
        for(int i=0;i<count;i++){
            if(version>=2){
                string id, name, category, batch, expiry;
                double tradePrice=0.0;
                int stock=0;
                getline(fin, id);
                getline(fin, name);
                getline(fin, category);
                getline(fin, batch);
                fin>>tradePrice;
                fin>>stock;
                fin.ignore();
                getline(fin, expiry);
                Medicine m(id, name, category, batch, tradePrice, stock, expiry);
                addMedicine(m);
            } else {
                string name, expiry;
                double price=0.0;
                int stock=0;
                getline(fin, name);
                fin>>price;
                fin>>stock;
                fin.ignore();
                getline(fin, expiry);
                Medicine m(name, price, stock, expiry);
                m.setMedicineId(generateMedicineId());
                m.setCategory("Other");
                m.setBatchNumber("BATCH-LEGACY-" + to_string(i+1));
                addMedicine(m);
            }
        }
        fin.close();
    }
};
#endif
