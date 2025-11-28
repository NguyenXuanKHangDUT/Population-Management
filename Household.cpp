#include "Household.hpp"
#include "Host.hpp"
#include "Person.hpp"

#include <iostream>
#include <string>

#include "DSA/MyVector.h"
#include "DSA/HashMap.h"

using namespace std;

extern MyVector<Person*> profiles;
extern HashMap<string, Person*> IDHash;
extern HashMap<string, int> PersonIndex;

Household::Household(const string& hhID, const string& adr, const string& hpID, const string& rg) {
    this->HouseholdID = hhID;
    this->Address = adr;
    this->Host_PersonID = hpID;
    this->Region = rg;
}

Household::~Household() {
    for (Person* p : this->Member) {
        delete p;
    }
    this->Member.clear();
    this->nameMember.clear();
    HostPtr = nullptr;
}

// Getters
string Household::getHouseholdID() const { return this->HouseholdID;}
string Household::getAddress() const { return this->Address;}
string Household::getHost_PersonID() const { return this->Host_PersonID;}
double Household::getIncome() const {
    double ic = 0;
    for (Person* p : this->Member)
        ic += p->getIncome();
    return ic;
}
Host* Household::getHost() const { return this->HostPtr;}
string Household::getRegion() const { return this->Region;}
Person* Household::getPersonByName(const string& name) const {
    // find in hashmap
    auto target = this->nameMember.find(name);
    if (target != this->nameMember.end() ) {
        return target->second;
    }
    return nullptr; // Not found
}

// Setters
void Household::setHost(Host* host) { this->HostPtr = host;}
void Household::addMember(Person* member) { 
    this->Member.push_back(member);
    this->nameMember[member->getFullName()] = member;
}

void Household::removeMember(Person* member) {
    if (!member) return;

    // check marriage
    if (member->getPartner() != nullptr) {
        Person* partner = member->getPartner();
        // Break up
        partner->setPartner(nullptr);
        partner->setpartnerID("null");
    }

    // delete member from global profiles and IDHash
    string id = member->getPersonID();
    IDHash.erase(id);

    auto it = PersonIndex.find(id);
    if (it == PersonIndex.end()) {
        cout << "Error: Person not found in PersonIndex during removal." << endl;
        return;
    }

    int idx = it->second;
    int last = profiles.size()-1;

    if (idx != last) {
        swap(profiles[idx], profiles.back());
        PersonIndex[profiles[idx]->getPersonID()] = idx;
    }

    profiles.pop_back();

    // update PersonIndex
    PersonIndex.erase(id);

    // delete from Household
    this->nameMember.erase(member->getFullName());
    for (int i = 0; i < this->Member.size(); ++i) 
        if (this->Member[i] == member) {
            this->Member.erase(this->Member.begin()+i);
            break;
        }
}

// Display Families information
ostream& operator<<(ostream& out, const Household& hh) {
    out << "Household ID: " << hh.HouseholdID << endl;
    out << "Address: " << hh.Address << endl;
    out << "Region: " << hh.Region << endl;
    out << "Host: ";
    out << hh.HostPtr->getFullName() << "    ID: " << hh.HostPtr->getPersonID() << endl;
    out << "Members: " << endl;
    out << "Families's income (per month): " << hh.getIncome() << " USD\n";
    // for (const Person* member : hh.Member) {
    //     out << " - " << member->getFullName() << " (ID: " << member->getPersonalID() << ", Age: " << member->getAge() << ")" << endl;
    // }
    for (auto it = hh.nameMember.begin(); it != hh.nameMember.end(); ++it) {
        Person* member = it->second;
        out << " - " << member->getFullName() << " (ID: " << member->getPersonID() << ", Age: " << member->getAge() << ")" << endl;
    }
    return out;
}