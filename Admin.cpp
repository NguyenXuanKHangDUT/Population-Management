#include "Admin.hpp"
#include "Person.hpp"
#include "Household.hpp"
#include "Host.hpp"

#include <iostream>
#include <string>

// #include "DSA/MyVector.h"
// #include "DSA/HashMap.h"
#include "DSA/Algorithms.h"

using namespace std;

extern MyVector<Person*> profiles;
extern MyVector<Household*> Families;
extern HashMap<string, Person*> IDHash;
extern HashMap<string, int> PersonIndex;

Admin::Admin(const string& pID, const string& hID, const string& fName, const string& bDay, bool gend, const string& addr, const string& pnID, const string& job, const double& inc, const string& pwd) 
        : Host(pID, hID, fName, bDay, gend, addr, pnID, job, inc, pwd) {}

Admin::~Admin() {
    delete this;
}

bool Admin::banishMember() {
    // Admin can banish any citizen from profiles
    name:
    cout << "ID: "; string id; cin >> id;
    auto target = IDHash.find(id);
    if (target == IDHash.end()) {
        cout << "There were no member with that ID, type againe\n";
        goto name;
    }
    Person* member = target->second;
    if (member->getHost() == member) {
        cout << "this person is a host, if delete the host, all members in that household will be deleted too\n Are you sure? (1/0 : yes/no): ";
        int ch; cin >> ch;
        if (ch != 1) {
            cout << "command cancelled\n";
            goto name;
        }
        // delete all members in that household
        Household* hh = member->getHost()->getHousehold();
        while (!hh->Member.empty()) {
            Person* m = hh->Member[0];
            hh->removeMember(m);
        }
        for (int i = 0; i < Families.size(); ++i) {
            if (Families[i]->getHouseholdID() == hh->getHouseholdID()) {
                Families.erase(Families.begin()+i);
                break;
            }
        }
    }
    else {
        // remove from his/her household and profiles
        Household* hh = member->getHost()->getHousehold();
        hh->removeMember(member);
    }

    return true;
}

bool Admin::summonMember() {
    cout << "the member will be added to profiles directly, and that member will be a new household host\n";
    
    cout << "Full Name: "; string name; cin.ignore(); getline(cin, name);
    cout << "Birthday (dd/mm/yyyy): "; string bd; cin >> bd;
    cout << "Gender (1/0 : male/female): "; bool gd; cin >> gd;
    cout << "Address (Region/{1 sub-region} .. Region must be Maria/Rose/Sina):\n   "; string addr; cin.ignore(); getline(cin, addr);
    string partnerID = "null";
    cout << "Job: "; string j; cin.ignore(); getline(cin, j);
    cout << "Income (USD): "; double ic; cin >> ic;
    string rg = addr.substr(0, addr.find('/'));

    string newID = makeUniqueID(name, bd, gd, rg);
    string hhID = newID.substr(0,2)+newID.substr(6,6);
    Host* p = new Host(newID, hhID, name, bd, gd, addr, partnerID, j, ic, "11111111");
    Household* hh = new Household(hhID, addr, newID, newID.substr(0,2));
    p->setHousehold(hh);
    p->setHost(p);
    hh->addMember(p);
    hh->setHost(p);

    // add to global profiles and Families
    profiles.push_back(p);
    Families.push_back(hh);
    IDHash[newID] = p;
    PersonIndex[p->getPersonalID()] = profiles.size()-1;

    cout << p->getFullName() << " has been added successfully with Personal ID: " << p->getPersonalID() << " and Household ID: " << p->getHouseholdID() << "\n";
    cout << *hh;
    return true;
}

void Admin::searchName() {
    cout << "search name has not been done yet\n";
}

void Admin::searchID() {
    cout << "search ID has not been done yet\n";
}

void Admin::populationPyramid() {
    cout << "population Pyramid has not been done yet\n";
}

void Admin::marriageRate() {
    cout << "marriage Rate has not been done yet\n";
}

void Admin::averageIncome() {
    cout << "average Income has not been done yet\n";
}

void Admin::sixDegreesOfSeparation() {
    cout << "'Six Degrees Of Separation' has not been done yet\n";
}

ostream& operator<<(ostream& out, const Admin& p) {
    out << "----------------------------------" << endl;
    out << "Person Information:" << endl;
    out << "Personal ID: " << p.PersonalID << endl;
    out << "Full Name: " << p.FullName << endl;
    out << "Birthday: " << p.Birthday << " (Age: " << p.Age << ")" << endl;
    out << "Gender: " << (p.Gender?"Male":"Female") << endl;
    out << "Address: " << p.Address << endl;
    out << "Job: " << p.Job << " (Income: " << p.Income << " USD)" << endl;
    out << "Marriage: ";
    if (p.Partner != nullptr)
        out << p.Partner->getFullName() << "    ID: " << p.Partner->getPersonalID() << (p.Gender?" (Wife)":" (Husband)") << endl;
    else out << "Single" << endl;
    out << "Household ID: " << p.HouseholdID << endl;
    // if (p.Host != nullptr)
    out << "Host of Household: " << p.HostPtr->getFullName() << "    ID: " << p.HostPtr->getPersonalID() << endl;
    out << "Region: " << p.Region << endl;
    return out;
}   