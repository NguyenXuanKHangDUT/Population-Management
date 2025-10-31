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
    string Host_PersonalID;
    Host* HostPtr;
    string Region;
    
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
    string getHost_PersonalID() const;
    Host* getHost() const;
    string getRegion() const;
    Person* getPersonByID(const string&, const HashMap<string, Person*>&) const;
    Person* getPersonByName(const string&) const;
    double getIncome() const;

    // Setters
    void setHost(Host*);
    void addMember(Person*);
    void removeMember(Person*);

    void setHouseholdID(const string&);
    void setHost_PersonalID(const string&);

    // Display Families information
    friend ostream& operator<<(ostream&, const Household&);
};