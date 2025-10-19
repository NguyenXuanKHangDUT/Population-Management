#include "HostService.hpp"

#include "Person.hpp"
#include "Household.hpp"

#include <iostream>
#include <string>

#include "DSA/MyVector.h"
#include "DSA/HashMap.h"


using namespace std;


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