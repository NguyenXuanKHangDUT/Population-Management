#include "Host.hpp"

#include "Person.hpp"
#include "Household.hpp"

#include <iostream>
#include <string>

#include "DSA/MyVector.h"
#include "DSA/HashMap.h"

using namespace std;

extern MyVector<Person*> profiles;
extern HashMap<string, Person*> IDHash;
extern HashMap<string, int> PersonIndex;

Host::Host(const string& pID, const string& hID, const string& fName, const string& bDay, bool gend, const string& addr, const string& pnID, const string& job, const double& inc, const string& pwd) 
        : Person(pID, hID, fName, bDay, gend, addr, pnID, job, inc, pwd) {
            this->hhPtr = nullptr;
        }
Host::~Host() {
    delete hhPtr;
}

void Host::setHousehold(Household* h) {
    this->hhPtr = h;
}
Household* Host::getHousehold() const {
    return this->hhPtr;
}

bool Host::banishMember() {
    name:
    cout << "Name: "; string name; getline(cin, name);

    Person* member = this->hhPtr->getPersonByName(name);
    if (!member) {
        cout << "There were no member with that name, type againe\n";
        goto name;
    }
    this->hhPtr->removeMember(member);
    if (!member) return true;
    return false;
}

bool Host::summondMember() {
    cout << "Full Name: "; string name; cin.ignore(); getline(cin, name);
    cout << "Birthday (dd/mm/yyyy): "; string bd; cin >> bd;
    cout << "Gender (1/0 : male/female): "; bool gd; cin >> gd;
    cout << "Job: "; string j; cin.ignore(); getline(cin, j);
    cout << "Income (USD): "; double ic; cin >> ic;

    string newID = this->Personal_ID;
    newID[2] = gd ? '1' : '0';
    newID[6] = '1';  // new member
    for (int i = 7; i < 12; ++i) // i want to generate a new random ID from index 8 to 12
        newID[i] = '0' + rand() % 10;

    Person* member = new Person(newID, this->Household_ID, name, bd, gd, this->Address, "null", j, ic, "11111111");
    member->setHost(this);
    this->hhPtr->addMember(member);

    // add to global profiles and IDHash
    IDHash[member->getPersonal_ID()] = member;
    profiles.push_back(member);
    PersonIndex[member->getPersonal_ID()] = profiles.size()-1;
    
    return true;
}

ostream& operator<<(ostream& out, const Host& p) {
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
    // if (p.Host != nullptr)
    out << "Host of Household: " << p.HostPtr->getFullName() << "    ID: " << p.HostPtr->getPersonal_ID() << endl;
    out << "Region: " << p.Region << endl;
    return out;
}