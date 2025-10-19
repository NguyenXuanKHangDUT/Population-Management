#include "Household.hpp"

#include "Person.hpp"

#include <iostream>
#include <string>

#include "DSA/MyVector.h"
#include "DSA/HashMap.h"

using namespace std;


Household::Household(const string& hhID, const string& adr, const string& hpID, const string& rg) {
    this->Household_ID = hhID;
    this->Address = adr;
    this->Host_Personal_ID = hpID;
    this->Region = rg;
}

// Getters
string Household::getHousehold_ID() const { return this->Household_ID;}
string Household::getAddress() const { return this->Address;}
string Household::getHost_Personal_ID() const { return this->Host_Personal_ID;}
double Household::getIncome() const {
    double ic = 0;
    for (Person* p : this->Member)
        ic += p->getIncome();
    return ic;
}
Person* Household::getHost() const { return this->Host;}
string Household::getRegion() const { return this->Region;}
Person* Household::getPersonByID(const string& id, const MyVector<Person*>& profiles, const HashMap<string, Person*>& IDHash) const {
    auto target = IDHash.find(id);
    if (target != IDHash.end() ) {
        return target->second;
    }
    return nullptr; // Not found
}
Person* Household::getPersonByName(const string& name) const {
    for (Person* p : this->Member) 
        if (p->getFullName() == name) return p;
    return nullptr;
}

// Setters
void Household::setHost(Person* host) { this->Host = host;}
void Household::addMember(Person* member) { this->Member.push_back(member);}

void Household::removeMember(Person* member) {
    for (int i = 0; i < this->Member.size(); ++i) 
        if (this->Member[i] == member) {
            this->Member.erase(this->Member.begin()+i);
            delete this->Member[i];
            break;
        }
}

// Display family information
ostream& operator<<(ostream& out, const Household& hh) {
    out << "Household ID: " << hh.Household_ID << endl;
    out << "Address: " << hh.Address << endl;
    out << "Region: " << hh.Region << endl;
    out << "Host: ";
    out << hh.Host->getFullName() << "    ID: " << hh.Host->getPersonal_ID() << endl;
    out << "Members: " << endl;
    out << "Family's income (per month): " << hh.getIncome() << " USD\n";
    for (const Person* member : hh.Member) {
        out << " - " << member->getFullName() << " (ID: " << member->getPersonal_ID() << ", Age: " << member->getAge() << ")" << endl;
    }
    return out;
}