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
    // setter
    void setHousehold(Household*);
    // getter
    Household* getHousehold() const;

    
    virtual bool banishMember();
    virtual bool summondMember();

    friend ostream& operator<<(ostream&, const Host&);
};