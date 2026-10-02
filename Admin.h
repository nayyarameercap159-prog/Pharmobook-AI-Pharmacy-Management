#include<iostream>
#include<string>
#ifndef ADMIN_H
#define ADMIN_H
#include "Person.h"
using namespace std;
class Admin:public Person{
    private:
    string username;
    string password;
    public:
    Admin():Person(){
        username="";
        password="";
    }
    Admin(int id,string name,string location,string username,string password):Person(id,name,location){
        this->username=username;
        this->password=password;
    }
    void display(){
        Person::display();
        cout<<"Username: "<<username<<endl;
        cout<<"Password: "<<password<<endl;
    }
    ~Admin(){}
    bool login(string username,string password){
        if(this->username==username && this->password==password){
            return true;
        }
        else{
            return false;
        }
    }
};
#endif
