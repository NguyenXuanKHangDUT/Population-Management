#pragma once

#include "Person.hpp"
#include "Household.hpp"

#include <iostream>
#include <string>

#include "DSA/MyVector.h"
#include "DSA/HashMap.h"

using namespace std;

class Person; // forward declaration
class Household;
class Host : public Person
{
private:
    Household* hhPtr;
public:
    Host(const string&, const string&, const string&, const string&, bool, const string&, const string&, const string&, const double&, const string&);
    ~Host() override;
    // setter
    void setHousehold(Household*);
    // getter
    Household* getHousehold() const;
    bool banishMember();
    bool summondMember();

    friend ostream& operator<<(ostream&, const Host&);
};