#include <iostream>
#include <string>

#include <fstream>
#include <sstream>

#include "UserInterface.h"

using namespace std;

MyVector<Person*> profiles; // Each pointer points to a Person
MyVector<Household*> Families;  // Each pointer points to a Household

HashMap<string, int> PersonIndex;  // Maps PersonalID to their index in profiles so that we can look up and update quickly
HashMap<string, Person*> IDHash;   // Maps PersonalID to Person pointers

void readPersons();
void readHouseholds();
void buildPersonIndex();

Person* User = nullptr;
/*
g++ ___Launcher.cpp Household.cpp Host.cpp Admin.cpp Person.cpp UserInterface.cpp -o a
*/
void updatePerson();
void updateHousehold();

int main() {
    // Read input data
    readPersons();
    readHouseholds();
    buildPersonIndex();

    // Example usage: print all persons
    int co = 0;
    for (const Household* h : Families) {
        cout << *h << endl;
        co++;
    }
    int c = 0;
    for (const Person* p : profiles) {
        cout << *p << endl;
        c++;
    }
    cout << co << " " << c << endl;

    UserInterface* ui = nullptr;

    while (1) {
        login();
        UserInterface* UI = nullptr;
        if (User->getJob() == "admin")
            UI = new AdminInterface();
        else if (User->getPersonalID().substr(7,5) == User->getHouseholdID().substr(3,5))
            UI = new HostInterface();
        else
            UI = new NetizenInterface();
        
        bool isExit = UI->launch();
        delete UI;

        if (isExit) break;
        else User = nullptr;
        
    }
    
    // Update data
    cout << "Updating data..." << endl;
    updatePerson();
    updateHousehold();
    cout << "Update completed." << endl;
    return 0;
}

void readPersons() {
    // Implementation for reading person data from a file
    // ifstream file("Persons__.txt");
    ifstream file("Person_test.txt");
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
        pnID = Infor[6] == "null" ? "" : Infor[6].substr(1,12);
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
        profiles.push_back(p);
    }
    file.close();

    for (Person* p : profiles) IDHash[(*p).getPersonalID()] = p;

    // set partner for each person
    for (Person* p : profiles) {
        if (!(p->getPartnerID() == "null")) {
            Person* partner = p->getPersonByID(p->getPartnerID(), IDHash);
            if (partner != nullptr) 
                p->setPartner(partner);
        }
    }

}
void readHouseholds() {
    ifstream file("Household_test.txt");
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
        Families.push_back(newHousehold);
    }
    file.close();
    // set host and members for each household
    for (Household* hh : Families) {
        Person* host = hh->getPersonByID(hh->getHost_PersonalID(), IDHash);
        if (host != nullptr) {
            // host here is of type Person*, need to cast to Host*
            Host* realHost = dynamic_cast<Host*>(host);
            realHost->setHousehold(hh);
            hh->setHost(realHost);
            realHost->setHost(realHost);
        }
        for (Person* p : profiles) 
            if (p->getHouseholdID() == hh->getHouseholdID() ) {
                Host* h = dynamic_cast<Host*>(host);
                p->setHost(h);
                hh->addMember(p);
            }
    }
}

void buildPersonIndex() {
    for (int i = 0; i < profiles.size(); ++i)
        PersonIndex[profiles[i]->getPersonalID()] = i;
}

void updatePerson() {
    introSort(profiles, +[](Person* const &pa, Person* const &pb) {
        return stod(pa->getHouseholdID().substr(2)) < stod(pb->getHouseholdID().substr(2));
    });

    ofstream file("Person_test.txt");
    file << "PersonalID,HouseholdID,full name,birthday,gender,address,PartnerID,job,income,password" << endl;
    string data[10];
    for (const Person* p : profiles) {
        data[0] = p->getPersonalID().insert(0, "'");
        data[1] = p->getHouseholdID().insert(0, "'");
        data[2] = p->getFullName();
        data[3] = p->getBirthday();
        data[4] = p->getGender() == true ? "male" : "female";
        data[5] = p->getAddress();
        data[6] = p->getPartner() == nullptr ? "null" : p->getPartner()->getPersonalID().insert(0, "'");
        data[7] = p->getJob();
        data[8] = to_string((int)p->getIncome());
        data[9] = p->getPassword();
        for (int i = 0; i < 9; i++) 
            file << data[i] <<",";
        file << data[9] << "\n";
    }
}
void updateHousehold() {
    ofstream file("Household_test.txt");
    file << "HouseholdID,Address,Host_PersonalID,Region_ID" << "\n";
    string data[4];
    for (const Household* h : Families) {
        data[0] = h->getHouseholdID().insert(0, "'");
        data[1] = h->getAddress();
        data[2] = h->getHost_PersonalID().insert(0, "'");
        if (h->getRegion() == "Maria") data[3] = "'01";
        if (h->getRegion() == "Rose") data[3] = "'10";
        if (h->getRegion() == "Sina") data[3] = "'11";
        for (int i = 0; i < 3; i++) 
            file << data[i] << ",";
        file << data[3] << "\n";
    }
}