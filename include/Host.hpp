#pragma once

#include "Person.hpp"
#include "Household.hpp"

#include <iostream>
#include <string>

#include "DSA/MyVector.h"
#include "DSA/HashMap.h"

using namespace std;

class Household;
class Host : public Person
{
protected:
    Household* hhPtr;
public:
    Host(const string&, const string&, const string&, const string&, bool, const string&, const string&, const string&, const double&, const string&);
    ~Host() override;

    // getter
    Household* getHousehold() const;
    
    // setter
    void setHousehold(Household*);

    // functional methods
    virtual bool banishMember();
    virtual bool summonMember();

    friend ostream& operator<<(ostream&, const Host&);
};