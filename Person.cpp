#include "Person.hpp"
#include "Host.hpp"

#include <iostream>
#include <string>

#include "DSA/MyVector.h"
#include "DSA/HashMap.h"

using namespace std;

Person::Person(const string& pID, const string& hID, const string& fName, const string& bDay, bool gend, const string& addr, const string& pnID, const string& job, const double& inc, const string& pwd) {
    this->PersonID = pID;
    this->HouseholdID = hID;
    this->FullName = fName;

    this->Birthday = date(bDay);
    this->Gender = gend;
    this->Address = addr;
    this->PartnerID = pnID;
    this->Partner = nullptr; // Initially no partner, can be set later using setPartner()
    this->HostPtr = nullptr; // Initially no host, can be set later using setHost()
    this->Job = job;
    this->Income = inc;
    this->password = pwd;

    // Calculate Age from Birthday 
    // Birthday's format: "DD/MM/YYYY"
    // this->YearOfBirth = stoi(Birthday.substr(Birthday.find_last_of('/')+1));
    this->YearOfBirth = this->Birthday.getYear();
    this->Age = 2025 - this->YearOfBirth; // Current year is 2025

    this->Region = this->Address.substr(0, this->Address.find('/'));
}

Person::~Person() {
    Partner = nullptr;
    HostPtr = nullptr;
}

// Getters
string Person::getPersonID() const { return this->PersonID;}
string Person::getHouseholdID() const { return this->HouseholdID;}
string Person::getFullName() const { return FullName;}
string Person::getBirthday() const { return this->Birthday.getDate_String();}
string Person::getAddress() const { return Address;}
string Person::getPartnerID() const { return PartnerID;}
Person* Person::getPartner() const { return Partner;}
string Person::getJob() const { return Job;}
Host* Person::getHost() const {
    return this->HostPtr;
}
string Person::getPassword() const { return password;}

// Setters
void Person::setPartner(Person* partner) { this->Partner = partner;}
void Person::setpartnerID(const string& pnID) { this->PartnerID = pnID;}
void Person::setHost(Host* host) {
    // this->HostPtr = dynamic_cast<Host*>(host);
    this->HostPtr = host;
}
void Person::setJob(const string& job) { this->Job = job;}
void Person::setIncome(const double& ic) { this->Income = ic;}
void Person::setPassword(const string& pwd) { this->password = pwd;}

// Display Person information
ostream& operator<<(ostream& out, const Person& p) {
    out << "----------------------------------" << endl;
    out << "Person Information:" << endl;
    out << "Person ID: " << p.PersonID << endl;
    out << "Full Name: " << p.FullName << endl;
    out << "Birthday: " << p.Birthday << " (Age: " << p.Age << ")" << endl;
    out << "Gender: " << (p.Gender?"Male":"Female") << endl;
    out << "Address: " << p.Address << endl;
    out << "Job: " << p.Job << " (Income: (per month) " << p.Income << " USD)" << endl;
    out << "Marriage: ";
    if (p.Partner != nullptr)
        out << p.Partner->getFullName() << "    ID: " << p.Partner->getPersonID() << (p.Gender?" (Wife)":" (Husband)") << endl;
    else out << "Single" << endl;
    out << "Household ID: " << p.HouseholdID << endl;
    // if (p.HostPtr != nullptr)
    out << "Host of Household: " << p.HostPtr->getFullName() << "    ID: " << p.HostPtr->getPersonID() << endl;
    out << "Region: " << p.Region << endl;
    return out;
}