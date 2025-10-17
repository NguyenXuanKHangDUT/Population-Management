#include <bits/stdc++.h>
#include "MyVector.h"
using namespace std;

#include "HostService.hpp"

void Hostservice::addMember(const string& name) {
    auto target = NameHash.find(name);
    if (target != NameHash.end() ) {
        Person* newMember = target->second;
        thisFamily->addMember(newMember);
        cout << "Member " << name << " added to the household." << endl;
    } else {
        cout << "Member " << name << " not found in the source file." << endl;
    }
}

void Hostservice::removeMember(const string& name) {
    Person* member = thisFamily->getPersonByName(name);
    if (member != nullptr) {
        thisFamily->removeMember(member);
        cout << "Member " << name << " removed from the household." << endl;
    } else {
        cout << "Member " << name << " not found in the household." << endl;
    }
}