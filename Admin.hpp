#pragma once

#include "Host.hpp"

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
    ~Admin();

    // Add/Remove methods
    bool banishMember() override;
    bool summonMember() override;

    void searchName();
    void searchID();

    // filter and statistics
    void populationPyramid();
    void marriageRate();
    void averageIncome();

    
    void sixDegreesOfSeparation();

    friend ostream& operator<<(ostream&, const Admin&);
};