#include<iostream>
#include<string>
#ifndef PERSON_H
#define PERSON_H
using namespace std;
class Person{
    protected:
    int id;
    string name;
    string location;
    public:
    Person(){
        id=0;
        name="";
        location="";
    }
    Person(int id,string name,string location){
        this->id=id;
        this->name=name;
        this->location=location;
    }
    virtual void display(){
        cout<<"ID: "<<id<<endl;
        cout<<"Name: "<<name<<endl;
        cout<<"Location: "<<location<<endl;
    }
    virtual ~Person(){}
    int getId() const {
        return id;
    }
    string getName() const {
        return name;
    }
    string getLocation() const {
        return location;
    }
};
#endif