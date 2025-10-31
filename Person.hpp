#pragma once

#include <iostream>
#include <string>

#include "DSA/MyVector.h"
#include "DSA/HashMap.h"

using namespace std;

class Host; // forward declaration
class Person
{
protected:
    // Attributes from the .txt file
    string PersonalID;
    string HouseholdID;
    string FullName;
    string Birthday;
    bool Gender; // true = male, false = female
    string Address; // Region/{1 sub-region}
    string PartnerID;
    Person* Partner;
    string Job;
    double Income;
    string password;

    // Derived/Computed attributes (attributes that are calculated)
    // Example: Age, etc.
    int Age;
    int YearOfBirth;
    Host* HostPtr; // Host of Household
    string Region;

public:
    // Constructor
    Person(const string&, const string&, const string&, const string&, bool, const string&, const string&, const string&, const double&, const string&);
    // Destructor
    virtual ~Person();

    // Getters
    string getPersonalID() const;
    string getHouseholdID() const;
    string getFullName() const;
    string getBirthday() const;
    bool getGender() const;
    string getAddress() const;
    string getPartnerID() const;
    Person* getPartner() const;
    string getJob() const;
    double getIncome() const;
    int getAge() const;
    Person* getPersonByID(const string&, const HashMap<string, Person*>&) const;
    Host* getHost() const;
    string getPassword() const;

    // Setters
    void setPersonalID(const string&);
    void setHouseholdID();
    
    void setPartner(Person*);
    void setpartnerID(const string&);
    void setHost(Host*);
    void setJob(const string&);
    void setIncome(const double&);
    void setPassword(const string&);

    // Display Person information
    friend ostream& operator<<(ostream&, const Person&);

};