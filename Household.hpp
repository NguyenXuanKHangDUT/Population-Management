#pragma once 

#include "Person.hpp"

#include <iostream>
#include <string>

#include "DSA/MyVector.h"
#include "DSA/HashMap.h"

using namespace std;



class Household 
{
private:
    string Household_ID;
    string Address;
    string Host_Personal_ID;
    Person* Host;
    string Region;
    
    MyVector<Person*> Member;
    
public:
    friend class Host;
    Household(const string&, const string&, const string&, const string&);

    // Getters
    string getHousehold_ID() const;
    string getAddress() const;
    string getHost_Personal_ID() const;
    Person* getHost() const;
    string getRegion() const;
    Person* getPersonByID(const string&, const MyVector<Person*>&, const HashMap<string, Person*>&) const;
    
    Person* getPersonByName(const string&) const;
    double getIncome() const;

    // Setters
    void setHost(Person*);
    void addMember(Person*);
    void removeMember(Person*);

    // Display family information
    friend ostream& operator<<(ostream&, const Household&);
};