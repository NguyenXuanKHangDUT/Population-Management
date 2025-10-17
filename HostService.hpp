#include <bits/stdc++.h>
using namespace std;

#include "MyVector.h"

#include "Person.hpp"
#include "Household.hpp"

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