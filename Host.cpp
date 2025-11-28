#include "Host.hpp"

#include "Person.hpp"
#include "Household.hpp"

#include <iostream>
#include <string>

// #include "DSA/MyVector.h"
// #include "DSA/HashMap.h"
#include "DSA/Algorithms.h"

using namespace std;

extern MyVector<Person*> profiles;
extern HashMap<string, Person*> IDHash;
extern HashMap<string, int> PersonIndex;

Host::Host(const string& pID, const string& hID, const string& fName, const string& bDay, bool gend, const string& addr, const string& pnID, const string& job, const double& inc, const string& pwd) 
        : Person(pID, hID, fName, bDay, gend, addr, pnID, job, inc, pwd) {
            this->hhPtr = nullptr;
        }
Host::~Host() {
    hhPtr = nullptr;
}
// getter
Household* Host::getHousehold() const {
    return this->hhPtr;
}

// setter
void Host::setHousehold(Household* h) {
    this->hhPtr = h;
}

// functional methods
bool Host::banishMember() {
    name:
    cout << "Name: "; string name; cin.ignore(); getline(cin, name);

    Person* member = this->hhPtr->getPersonByName(name);
    if (!member) {
        cout << "There were no member with that name, type againe\n";
        goto name;
    }
    this->hhPtr->removeMember(member);
    return true;
}

bool Host::summonMember() {
    cout << "Full Name: "; string name; cin.ignore(); getline(cin, name);
    cout << "Birthday (dd/mm/yyyy): "; string bd; cin >> bd;
    cout << "Gender (1/0 : male/female): "; bool gd; cin >> gd;
    cout << "Job: "; string j; cin.ignore(); getline(cin, j);
    cout << "Income (USD): "; double ic; cin >> ic;

    string newID = makeUniqueID(name, bd, gd, this->Region);

    Person* member = new Person(newID, this->HouseholdID, name, bd, gd, this->Address, "'null'", j, ic, "11111111");
    member->setHost(this);
    this->hhPtr->addMember(member);

    // add to global profiles and IDHash
    IDHash[member->getPersonID()] = member;
    profiles.push_back(member);
    PersonIndex[member->getPersonID()] = profiles.size()-1;
    
    cout << member->getFullName() << " has been added successfully with Personal ID: " << member->getPersonID() << " to your household.\n";
    return true;
}

ostream& operator<<(ostream& out, const Host& p) {
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
    // if (p.Host != nullptr)
    out << "Host of Household: " << p.HostPtr->getFullName() << "    ID: " << p.HostPtr->getPersonID() << endl;
    out << "Region: " << p.Region << endl;
    return out;
}