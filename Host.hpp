#pragma once

#include "Person.hpp"
#include "Household.hpp"

#include <iostream>
#include <string>

#include "DSA/MyVector.h"
#include "DSA/HashMap.h"

using namespace std;

class Person; // forward declaration
class Host : public Person
{
private:
public:
    Host(const string&, const string&, const string&, const string&, bool, const string&, const string&, const string&, const double&, const string&);
    friend ostream& operator<<(ostream&, const Host&);
};