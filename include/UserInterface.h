#pragma once

#include <iostream>
#include <string>

#include "DSA/Algorithms.h"

#include "Person.hpp"
#include "Host.hpp"
#include "Admin.hpp"
#include "Household.hpp"

using namespace std;

void login();

class UserInterface
{
public:
    virtual bool launch() = 0;
};

class NetizenInterface : public UserInterface 
{
public:
    bool launch() override;
};

class HostInterface : public UserInterface
{
public:
    bool launch() override;
};

class AdminInterface : public UserInterface 
{
public:
    bool launch() override;
};