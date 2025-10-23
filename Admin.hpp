#pragma once

#include "Person.hpp"
#include "Household.hpp"

#include <iostream>
#include <string>

#include "DSA/MyVector.h"
#include "DSA/HashMap.h"

using namespace std;

class Host; // forward declaration
class Admin : public Host
{
private:
public:
    Admin(const string&, const string&, const string&, const string&, bool, const string&, const string&, const string&, const double&, const string&);
    friend ostream& operator<<(ostream&, const Admin&);
};