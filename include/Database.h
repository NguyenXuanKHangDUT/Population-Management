#pragma once

#include "Admin.hpp"

#include "DSA/Algorithms.h"

#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// singleton class
class Database
{
private:
    static Database* _Instance;

    // Containers
    MyVector<Person*> _profiles;
    HashMap<string, Person*> _IDHash; // key: PersonalID, value: Person*
    HashMap<string, int> _PersonIndex;  // key: PersonalID, value: index in profiles

    MyVector<Household*> _Families;

    // Private constructor to prevent instantiation
    Database();
    
public:
    ~Database();
    static Database* Instance();

    // I/O methods
    void readPersons();
    void readHouseholds();
    void buildPersonIndex();

    void updatePerson();
    void updateHousehold();

    // Getters
    MyVector<Person*>& getProfiles();
    HashMap<string, Person*>& getIDHash();
    HashMap<string, int>& getPersonIndex();
    MyVector<Household*>& getFamilies();
};

Database* Database::_Instance = nullptr;

// constructor: initialize data from files
Database::Database() {
    readPersons();
    readHouseholds();
    buildPersonIndex();
}

// destructor: update data to files then clear memory
Database::~Database() {
    updatePerson();
    updateHousehold();
    // for (Person* p : _profiles) delete p;
    // for (Household* h : _Families) delete h;
    this->_profiles.clear();
    this->_IDHash.clear();
    this->_PersonIndex.clear();

    this->_Families.clear();
}

Database* Database::Instance() {
    if (_Instance == nullptr) 
        _Instance = new Database();
    return _Instance;
}

void Database::readPersons() {
    // Implementation for reading person data from a file
    // ifstream file("Persons__.txt");
    ifstream file("test/Person_test.txt");
    string line;
    getline(file, line); // Skip first line
    while (getline(file, line)) {
        stringstream ss(line);
        string pID, hID, fName, bDay, addr, pnID, job, pwd;
        string Infor[10];
        int index = 0;
        while (getline(ss, Infor[index++], ',')) {
            ;// do nothing, just split by comma
        }
        
        pID = Infor[0].substr(1,12);
        hID = Infor[1].substr(1,8);
        fName = Infor[2];
        bDay = Infor[3];
        bool gender = (Infor[4] == "male") ? true : false;
        addr = Infor[5];
        pnID = Infor[6] == "'null'" ? "" : Infor[6].substr(1,12);
        job = Infor[7];
        double inc = stod(Infor[8]);
        pwd = Infor[9];
        Person* p = nullptr;
        if (job == "admin")
            p = new Admin(pID, hID, fName, bDay, gender, addr, pnID, job, inc, pwd);
        else if (pID.substr(7,5) == hID.substr(3,5)) 
            // This person is a Host
            p = new Host(pID, hID, fName, bDay, gender, addr, pnID, job, inc, pwd);
        else
            p = new Person(pID, hID, fName, bDay, gender, addr, pnID, job, inc, pwd);
        this->_profiles.push_back(p);
    }
    file.close();

    for (Person* p : this->_profiles) this->_IDHash[(*p).getPersonID()] = p;

    // set partner for each person
    for (Person* p : this->_profiles) {
        if (!(p->getPartnerID() == "null")) {
            Person* partner = nullptr;
            auto it = this->_IDHash.find(p->getPartnerID());
            if (it != this->_IDHash.end()) partner = it->second;
            
            if (partner != nullptr)
                p->setPartner(partner);
        }
    }
}

void Database::readHouseholds() {
    ifstream file("test/Household_test.txt");
    string line;
    getline(file, line); // Skip first line
    while (getline(file, line)) {
        stringstream ss(line);
        string hhID, adr, hpID, rgID;
        string Infor[4];
        int index = 0;
        while (getline(ss, Infor[index++], ',')) {
            ;// do nothing, just split by comma
        }
        hhID = Infor[0].substr(1,8);
        adr = Infor[1];
        hpID = Infor[2].substr(1,12);
        rgID = Infor[3].substr(1,2);
        if (rgID == "01") rgID = "Maria";
        else if (rgID == "10") rgID = "Rose";
        else rgID = "Sina";
        Household* newHousehold = new Household(hhID, adr, hpID, rgID);
        this->_Families.push_back(newHousehold);
    }
    file.close();
    // set host and members for each household
    for (Household* hh : this->_Families) {
        Person* host = nullptr;
        auto it = this->_IDHash.find(hh->getHost_PersonID());
        if (it != this->_IDHash.end()) host = it->second;

        if (host != nullptr) {
            // host here is of type Person*, need to cast to Host*
            Host* realHost = dynamic_cast<Host*>(host);
            realHost->setHousehold(hh);
            hh->setHost(realHost);
            realHost->setHost(realHost);
        }
        for (Person* p : this->_profiles) 
            if (p->getHouseholdID() == hh->getHouseholdID() ) {
                Host* h = dynamic_cast<Host*>(host);
                p->setHost(h);
                hh->addMember(p);
            }
    }
}

void Database::buildPersonIndex() {
    for (int i = 0; i < this->_profiles.size(); ++i)
        this->_PersonIndex[this->_profiles[i]->getPersonID()] = i;
}

void Database::updatePerson() {
    introSort(this->_profiles, +[](Person* const &pa, Person* const &pb) {
        return stod(pa->getHouseholdID().substr(2)) < stod(pb->getHouseholdID().substr(2));
    });

    ofstream file("test/Person_test.txt");
    file << "'PersonID','HouseholdID',full name,birthday,gender,address,'PartnerID',job,income,password" << endl;
    string data[10];
    for (const Person* p : this->_profiles) {
        data[0] = p->getPersonID().insert(0, "'"); data[0] += "'";
        data[1] = p->getHouseholdID().insert(0, "'"); data[1] += "'";
        data[2] = p->getFullName();
        data[3] = p->getBirthday();
        data[4] = p->getGender() == true ? "male" : "female";
        data[5] = p->getAddress();
        data[6] = p->getPartner() == nullptr ? "'null" : p->getPartner()->getPersonID().insert(0, "'"); data[6] += "'";
        data[7] = p->getJob();
        data[8] = to_string((int)p->getIncome());
        data[9] = p->getPassword();
        for (int i = 0; i < 9; i++)
            file << data[i] <<",";
        file << data[9] << "\n";
    }
}

void Database::updateHousehold() {
    ofstream file("test/Household_test.txt");
    file << "'HouseholdID',Address,'Host_PersonID','Region_ID'" << "\n";
    string data[4];
    for (const Household* h : this->_Families) {
        data[0] = h->getHouseholdID().insert(0, "'");
        data[0] += "'";
        data[1] = h->getAddress();
        data[2] = h->getHost_PersonID().insert(0, "'");
        data[2] += "'";
        if (h->getRegion() == "Sina") data[3] = "'11'";
        if (h->getRegion() == "Rose") data[3] = "'10'";
        else data[3] = "'01'";
        for (int i = 0; i < 3; i++) 
            file << data[i] << ",";
        file << data[3] << "\n";
    }
}

// Getters
MyVector<Person*>& Database::getProfiles() {
    return this->_profiles;
}
HashMap<string, Person*>& Database::getIDHash() {
    return this->_IDHash;
}
HashMap<string, int>& Database::getPersonIndex() {
    return this->_PersonIndex;
}
MyVector<Household*>& Database::getFamilies() {
    return this->_Families;
}