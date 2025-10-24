#include "Person.hpp"
#include "Host.hpp"

#include <iostream>
#include <string>

#include "DSA/MyVector.h"
#include "DSA/HashMap.h"

using namespace std;


Person::Person(const string& pID, const string& hID, const string& fName, const string& bDay, bool gend, const string& addr, const string& pnID, const string& job, const double& inc, const string& pwd) {
    this->Personal_ID = pID;
    this->Household_ID = hID;
    this->FullName = fName;
    this->Birthday = bDay;
    this->Gender = gend;
    this->Address = addr;
    this->Partner_ID = pnID;
    this->Partner = nullptr; // Initially no partner, can be set later using setPartner()
    this->HostPtr = nullptr; // Initially no host, can be set later using setHost()
    this->Job = job;
    this->Income = inc;
    this->password = pwd;

    // Calculate Age from Birthday 
    // Birthday's format: "DD/MM/YYYY"
    this->YearOfBirth = stoi(Birthday.substr(6, 4));
    this->Age = 2025 - this->YearOfBirth; // Current year is 2025

    this->Region = this->Address.substr(0, this->Address.find('/'));
}

Person::~Person() {
    delete Partner; Partner = nullptr;
    delete HostPtr; HostPtr = nullptr;
}

// Getters
string Person::getPersonal_ID() const { return this->Personal_ID;}
string Person::getHousehold_ID() const { return this->Household_ID;}
string Person::getFullName() const { return FullName;}
string Person::getBirthday() const { return Birthday;}
bool Person::getGender() const { return Gender;}
string Person::getAddress() const { return Address;}
string Person::getPartner_ID() const { return Partner_ID;}
Person* Person::getPartner() const { return Partner;}
string Person::getJob() const { return Job;}
double Person::getIncome() const { return Income;}
int Person::getAge() const { return Age;}
Person* Person::getPersonByID(const string& id, const MyVector<Person*>& profiles, const HashMap<string, Person*>& IDHash) const {
    auto target = IDHash.find(id);
    if (target != IDHash.end() ) {
        return target->second;
    }
    return nullptr; // Not found
}
Host* Person::getHost(const HashMap<string, Person*>& IDHash) const {
    return this->HostPtr;
}
string Person::getPassword() const { return password;}

// Setters
void Person::setPartner(Person* partner) { this->Partner = partner;}
void Person::setHost(Host* host) {
    // this->HostPtr = dynamic_cast<Host*>(host);
    this->HostPtr = host;
}
void Person::setJob(const string& job) { this->Job = job;}
void Person::setPassword(const string& pwd) { this->password = pwd;}

// Display Person information
ostream& operator<<(ostream& out, const Person& p) {
    out << "----------------------------------" << endl;
    out << "Person Information:" << endl;
    out << "Personal ID: " << p.Personal_ID << endl;
    out << "Full Name: " << p.FullName << endl;
    out << "Birthday: " << p.Birthday << " (Age: " << p.Age << ")" << endl;
    out << "Gender: " << (p.Gender?"Male":"Female") << endl;
    out << "Address: " << p.Address << endl;
    out << "Job: " << p.Job << " (Income: " << p.Income << " USD)" << endl;
    out << "Marriage: ";
    if (p.Partner != nullptr)
        out << p.Partner->getFullName() << "    ID: " << p.Partner->getPersonal_ID() << (p.Gender?" (Wife)":" (Husband)") << endl;
    else out << "Single" << endl;
    out << "Household ID: " << p.Household_ID << endl;
    // if (p.HostPtr != nullptr)
    out << "Host of Household: " << p.HostPtr->getFullName() << "    ID: " << p.HostPtr->getPersonal_ID() << endl;
    out << "Region: " << p.Region << endl;
    return out;
}