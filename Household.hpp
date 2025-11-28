#pragma once 

#include "Person.hpp"
#include "Host.hpp"

#include <iostream>
#include <string>

#include "DSA/MyVector.h"
#include "DSA/HashMap.h"

using namespace std;

class Person; // forward declaration
class Host; // forward declaration
class Household 
{
private:
    string HouseholdID;
    string Address;
    string Host_PersonID;
    string Region;
    
    Host* HostPtr;
    MyVector<Person*> Member;
    HashMap<string, Person*> nameMember;
    
public:
    friend class Host;
    friend class Admin;
    Household(const string&, const string&, const string&, const string&);
    ~Household();

    // Getters
    string getHouseholdID() const;
    string getAddress() const;
    string getHost_PersonID() const;
    Host* getHost() const;
    string getRegion() const;
    Person* getPersonByName(const string&) const;
    double getIncome() const;

    // Setters
    void setHost(Host*);

    // functional methods
    void addMember(Person*);
    void removeMember(Person*);

    // Display Families information
    friend ostream& operator<<(ostream&, const Household&);
};