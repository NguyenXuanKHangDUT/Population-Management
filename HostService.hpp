#pragma once

#include "Person.hpp"
#include "Household.hpp"

#include <iostream>
#include <string>

#include "DSA/MyVector.h"
#include "DSA/HashMap.h"

using namespace std;


extern HashMap<string, Person*> NameHash;

class Host : public Person
{
    
};

class Hostservice 
{
protected:
    Host* Host;
    Household* thisFamily;

public:
    void addMember(const string&);
    void removeMember(const string&);
};