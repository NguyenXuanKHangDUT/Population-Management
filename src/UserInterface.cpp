#include "UserInterface.h"
#include <iostream>
#include <string>

using namespace std;

extern MyVector<Person*> profiles; // Each pointer points to a Person
extern MyVector<Household*> Families;  // Each pointer points to a Household
extern HashMap<string, int> PersonIndex;  // Maps PersonalID to their index in profiles so that we can look up and update quickly
extern HashMap<string, Person*> IDHash;   // Maps PersonalID to Person pointers

extern Person* User;

void login() {
    cout << "\u001b[36m";
    cout << "------------------------------------------------\n";
    cout << "                Population Management          -\n";
    cout << "------------------------------------------------\n";
    cout << "\u001b[0m";
    Login:
    cout << "Login \n";
    cout << "Personal ID: "; string id; cin >> id;
    Password:
    cout << "Password: "; string pw; cin >> pw;
    auto target = IDHash.find(id);
    if (target == IDHash.end()) {
        // yellow
        cout << "\u001b[33m";
        cout << "There were no person with that ID, login again!\n";
        cout << "\u001b[0m";
        goto Login;
    }
    else {
        User = target->second;
        if (pw != User->getPassword()) {
            cout << "\u001b[33m";
            cout << "Wrong password, type again!\n";
            cout << "\u001b[0m";
            goto Password;
        }
    }
    // green
    cout << "\u001b[32m";
    cout << "\nWelcome " << User->getFullName() << "!\n";
    cout << "\u001b[0m";

    if (User->getPassword() == "11111111") {
        cout << "First time login detected. Please change your password.\n";
        Change_Password:
        cout << "New password (3 <= length <= 8): ";
        string npw; cin >> npw;
        if (npw == "11111111") {
            // yellow
            cout << "\u001b[33m";
            cout << "New password cannot be the default password. type again!!\n";
            cout << "\u001b[0m";
            goto Change_Password;
        }
        else if (npw.length() >= 3 && npw.length() <= 8) {
            User->setPassword(npw);
            cout << "Password changed successfully.\n";
        }
        else {
            // red
            cout << "\u001b[31m";
            cout << "Invalid password length. type again!!\n";
            cout << "\u001b[0m";
            goto Change_Password;
        }
    }
}

bool NetizenInterface::launch() {
    // baby pink
    cout << "\u001b[35m";
    cout << "----------------------------------------------------------------\n";
    cout << "-                      Netizen Interface                       -\n";
    cout << "----------------------------------------------------------------\n\n\n";
    cout << "\u001b[0m";

    First_Entrance:
    cout << "Choose an option:\n";
    cout << "-        <>        -\n";
    cout << "1. View my Personal information\n";
    cout << "2. View my Household information\n";
    cout << "3. Update my Personal Information\n\n";
    cout << "4. Set marriage to someone\n\n";
    cout << "5. Sign out\n";
    cout << "0. Exit\n";
    string choice; cin >> choice;
    if (choice == "0") return true;
    else if (choice == "5") return false;
    else if (choice == "1") {
        cout << *User;
        // yellow
        cout << "\u001b[33m";
        cout << "Type 'go' to return to main menu\n";
        cout << "\u001b[0m";
        string c; 
        while (c != "go") cin >> c;
        goto First_Entrance;
    }
    else if (choice == "2") {
        Household* hhTmp = User->getHost()->getHousehold();
        cout << *hhTmp;
        cout << "\u001b[33m";
        cout << "Type 'go' to return to main menu\n";
        cout << "\u001b[0m";
        string c; 
        while (c != "go") cin >> c;
        goto First_Entrance;
    }
    else if (choice == "3") {
        Change_Information:
        cout << "Changeable information: \n";
        cout << "-(1) Job: "; cout << User->getJob() << endl;
        cout << "-(2) Income: "; cout << User->getIncome() << endl;
        cout << "-(0) Back to main menu.\n";
        cout << "Your choice: "; string c; cin >> c;
        if (c == "0") goto First_Entrance;
        else if (c == "1") {
            Change_Job:
            cout << "Type your new job: "; string j; cin.ignore(); getline(cin, j);
            User->setJob(j);
            goto Change_Income;
        }
        else if (c == "2") {
            Change_Income:
            cout << "Type your new income: "; double ic; cin >> ic;
            User->setIncome(ic);
            cout << "\u001b[33m";
            cout << "Type 'go' to return to main menu\n";
            cout << "\u001b[0m";
            string c; 
            while (c != "go") cin >> c;
            goto Change_Information;
        }
    }
    else if (choice == "4") {
        if (User->getPartner()) {
            cout << "You have already got marriage\n";
            cout << "\u001b[33m";
            cout << "Type 'go' to return to main menu\n";
            cout << "\u001b[0m";
            string c; 
            while (c != "go") cin >> c;
            goto First_Entrance;
        }
        Choose_Partner:
        // grey
        cout << "\u001b[90m";
        cout << "Type 'back' to back\n";
        cout << "\u001b[36m";
        cout << "Type " << (User->getGender() ? "her" : "his") << " Personal ID: ";
        cout << "\u001b[0m";
        string id; cin >> id;
        if (id == "back") goto First_Entrance;
        auto target = IDHash.find(id);
        if (target == IDHash.end()) {
            // yellow
            cout << "\u001b[33m";
            cout << "There were no person with that ID, type again!\n";
            cout << "\u001b[0m";
            goto Choose_Partner;
        }
        Person* partner = target->second;
        if (partner->getPartner() != nullptr) {
            cout << "\u001b[33m";
            cout << "that person was got marriage already \nTry another person!\n";
            cout << "\u001b[0m";
            goto Choose_Partner;
        }
        else if (User->getGender() == partner->getGender()) {
            // red
            cout << "\u001b[31m";
            cout << "LGBT is unaccepted in this Country\nTry another person\n";
            cout << "\u001b[0m";
            goto Choose_Partner;
        }
        else {
            User->setPartner(partner);
            partner->setPartner(User);
            if (User->getPartner() == partner) {
                cout << "Set marriage to " << partner->getFullName() << " successfully!\n";
                cout << "Type 'go' to return to main menu\n";
                string c; 
                while (c != "go") cin >> c;
                goto First_Entrance;
            }
        }
    }
    else {
        cout << "\u001b[31m";
        cout << "Invalid input. Please follow the menu options\n";
        cout << "\u001b[0m";
        goto First_Entrance;
    }
    return true;
}

bool HostInterface::launch() {
    // baby pink
    cout << "\u001b[35m";
    cout << "----------------------------------------------------------------\n";
    cout << "-                      Host Interface                          -\n";
    cout << "----------------------------------------------------------------\n\n\n";
    cout << "\u001b[0m";
    Host* hostUser = dynamic_cast<Host*>(User);
    
    First_Entrance:
    cout << "Choose an option:\n";
    cout << "-        <>        -\n";
    cout << "1. View my Personal information\n";
    cout << "2. View my Household information\n";
    cout << "3. Update Personal information\n\n";
    cout << "4. Set marriage to someone\n\n";
    cout << "5. Add 1 more member to family\n";
    cout << "6. Banish 1 member from family\n\n";
    cout << "7. Sign out\n";
    cout << "0. Exit\n";
    string choice; cin >> choice;
    if (choice == "0") return true;
    else if (choice == "7") return false;
    else if (choice == "1") {
        cout << *hostUser;
        cout << "\u001b[33m";
        cout << "Type 'go' to return to main menu\n";
        cout << "\u001b[0m";
        string c; 
        while (c != "go") cin >> c;
        goto First_Entrance;
    }
    else if (choice == "2") {
        Household* hhTmp = hostUser->getHousehold();
        cout << *hhTmp;
        cout << "\u001b[33m";
        cout << "Type 'go' to return to main menu\n";
        cout << "\u001b[0m";
        string c; 
        while (c != "go") cin >> c;
        goto First_Entrance;
    }
    else if (choice == "3") {
        Change_Information:
        cout << "Changeable information: \n";
        cout << "-(1) Job: "; cout << hostUser->getJob() << endl;
        cout << "-(2) Income: "; cout << hostUser->getIncome() << endl;
        cout << "-(0) Back to main menu.\n";
        cout << "Your choice: "; string c; cin >> c;
        if (c == "0") goto First_Entrance;
        else if (c == "1") {
            Change_Job:
            cout << "Type your new job: "; string j; cin.ignore(); getline(cin, j);
            hostUser->setJob(j);
            goto Change_Income;
        }
        else if (c == "2") {
            Change_Income:
            cout << "Type your new income: "; double ic; cin >> ic;
            hostUser->setIncome(ic);
            cout << "\u001b[33m";
            cout << "Type 'go' to return to main menu\n";
            cout << "\u001b[0m";
            string c; 
            while (c != "go") cin >> c;
            goto Change_Information;
        }
    }
    else if (choice == "4") {
        if (User->getPartner()) {
            cout << "You have already got marriage!\n";
            cout << "\u001b[33m";
            cout << "Type 'go' to return to main menu\n";
            cout << "\u001b[0m";
            string c; 
            while (c != "go") cin >> c;
            goto First_Entrance;
        }
        Choose_Partner:
        // grey
        cout << "\u001b[90m";
        cout << "Type 'back' to back\n";
        cout << "\u001b[36m";
        cout << "Type " << (hostUser->getGender() ? "her" : "his") << " Personal ID: ";
        cout << "\u001b[0m";
        string id; cin >> id;
        if (id == "back") goto First_Entrance;
        auto target = IDHash.find(id);
        if (target == IDHash.end()) {
            // yellow
            cout << "\u001b[33m";
            cout << "There were no person with that ID, type again!\n";
            cout << "\u001b[0m";
            goto Choose_Partner;
        }
        Person* partner = target->second;
        if (partner->getPartner() != nullptr) {
            cout << "\u001b[33m";
            cout << "that person was got marriage already \nTry another person!\n";
            cout << "\u001b[0m";
            goto Choose_Partner;
        }
        else if (hostUser->getGender() == partner->getGender()) {
            // red
            cout << "\u001b[31m";
            cout << "LGBT is unaccepted in this Country\nTry another person\n";
            cout << "\u001b[0m";
            goto Choose_Partner;
        }
        else {
            hostUser->setPartner(partner);
            partner->setPartner(hostUser);
            if (hostUser->getPartner() == partner) {
                cout << "Set marriage to " << partner->getFullName() << " successfully!\n";
                cout << "\u001b[33m";
                cout << "Type 'go' to return to main menu\n";
                cout << "\u001b[0m";
                string c; 
                while (c != "go") cin >> c;
                goto First_Entrance;
            }
        }
    }
    else if (choice == "5") {
        if (hostUser->summonMember()) {
            cout << "Add successfully!\n";
            cout << "\u001b[33m";
            cout << "Type 'go' to return to main menu\n";
            cout << "\u001b[0m";
            string c;
            while (c != "go") cin >> c;
            goto First_Entrance;
        }
        else {
            cout << "Fail to add\n";
            cout << "\u001b[33m";
            cout << "Type 'go' to return to main menu\n";
            cout << "\u001b[0m";
            string c; 
            while (c != "go") cin >> c;
            goto First_Entrance;
        }
    }
    else if (choice == "6") {
        if (hostUser->banishMember()) {
            cout << "Banish successfully!\n";
            cout << "\u001b[33m";
            cout << "Type 'go' to return to main menu\n";
            cout << "\u001b[0m";
            string c; 
            while (c != "go") cin >> c;
            goto First_Entrance;
        }
        else {
            // red
            cout << "\u001b[31m";
            cout << "Fail to banish\n";
            cout << "\u001b[33m";
            cout << "Type 'go' to return to main menu\n";
            cout << "\u001b[0m";
            string c; 
            while (c != "go") cin >> c;
            goto First_Entrance;
        }
    }
    else {
        cout << "\u001b[31m";
        cout << "Invalid input. Please follow the menu options\n";
        cout << "\u001b[0m";
        goto First_Entrance;
    }
    return true;
}

bool AdminInterface::launch() {
    // baby pink
    cout << "\u001b[35m";
    cout << "----------------------------------------------------------------\n";
    cout << "-                     Admin Interface                          -\n";
    cout << "----------------------------------------------------------------\n\n\n";
    cout << "\u001b[0m";
    Admin* adminUser = dynamic_cast<Admin*>(User);
    
    First_Entrance:
    cout << "Choose an option:\n";
    cout << "1. View my Personal information\n";
    cout << "2. View my Household information\n\n";
    cout << "3. Set marriage to someone\n\n";
    cout << "4. Add 1 more Host household to System\n";
    cout << "5. Banish 1 person from System\n\n";
    cout << "6. Search and view information of a person by name\n";
    cout << "7. Search and view information of a person by person ID\n\n";
    // cyan
    cout << "\u001b[36m";
    cout << "-       Statistic:\n";
    cout << "\u001b[0m";
    cout << "8. View populaton Pyramid\n";
    cout << "9. View marriage rate\n";
    cout << "10. View average income in each age group\n\n";
    cout << "11. Check whether person A and person B is linked or not by rule: 'Six Degrees Of Separation':\n\n";
    cout << "12. Sign out\n";
    cout << "0. Exit\n";
    string choice; cin >> choice;
    if (choice == "0") return true;
    else if (choice == "12") return false;
    else if (choice == "1") {
        cout << *adminUser;
        cout << "\u001b[33m";
        cout << "Type 'go' to return to main menu\n";
        cout << "\u001b[0m";
        string c; 
        while (c != "go") cin >> c;
        goto First_Entrance;
    }
    else if (choice == "2") {
        Household* hhTmp = adminUser->getHousehold();
        cout << *hhTmp;
        cout << "\u001b[33m";
        cout << "Type 'go' to return to main menu\n";
        cout << "\u001b[0m";
        string c; 
        while (c != "go") cin >> c;
        goto First_Entrance;
    }
    else if (choice == "3") {
        if (User->getPartner()) {
            cout << "You have already got marriage!\n";
            cout << "\u001b[33m";
            cout << "Type 'go' to return to main menu\n";
            cout << "\u001b[0m";
                string c; 
                while (c != "go") cin >> c;
                goto First_Entrance;
        }
        Choose_Partner:
        // grey
        cout << "\u001b[90m";
        cout << "Type 'back' to back\n";
        cout << "\u001b[36m";
        cout << "Type " << (adminUser->getGender() ? "her" : "his") << " Personal ID: ";
        cout << "\u001b[0m";
        string id; cin >> id;
        if (id == "back") goto First_Entrance;
        auto target = IDHash.find(id);
        if (target == IDHash.end()) {
            cout << "\u001b[33m";
            cout << "There were no person with that ID, type again!\n";
            cout << "\u001b[0m";
            goto Choose_Partner;
        }
        Person* partner = target->second;
        if (partner->getPartner() != nullptr) {
            cout << "\u001b[33m";
            cout << "that person was got marriage already \nTry another person!\n";
            cout << "\u001b[0m";
            goto Choose_Partner;
        }
        else if (adminUser->getGender() == partner->getGender()) {
            cout << "\u001b[31m";
            cout << "LGBT is unaccepted in this Country\nTry another person\n";
            cout << "\u001b[0m";
            goto Choose_Partner;
        }
        else {
            adminUser->setPartner(partner);
            if (adminUser->getPartner() == partner) {
                cout << "Set marriage to " << partner->getFullName() << " successfully!\n";
                cout << "\u001b[33m";
                cout << "Type 'go' to return to main menu\n";
                cout << "\u001b[0m";
                string c; 
                while (c != "go") cin >> c;
                goto First_Entrance;
            }
        }
    }
    else if (choice == "4") {
        if (adminUser->summonMember()) {
            // yellow
            cout << "\u001b[33m";
            cout << "Type 'go' to return to main menu\n";
            cout << "\u001b[0m";
            string c; 
            while (c != "go") cin >> c;
            goto First_Entrance;
        }
        else {
            // red
            cout << "\u001b[31m";
            cout << "Failed to add\n";
            cout << "\u001b[33m";
            cout << "Type 'go' to return to main menu\n";
            cout << "\u001b[0m";
            string c; 
            while (c != "go") cin >> c;
            goto First_Entrance;
        }
    }
    else if (choice == "5") {
        if (adminUser->banishMember()) {
            cout << "Banish successfully!\n";
            cout << "\u001b[33m";
            cout << "Type 'go' to return to main menu\n";
            cout << "\u001b[0m";
            string c; 
            while (c != "go") cin >> c;
            goto First_Entrance;
        }
        else {
            cout << "\u001b[31m";
            cout << "Fail to banish\n";
            cout << "\u001b[33m";
            cout << "Type 'go' to return to main menu\n";
            cout << "\u001b[0m";
            string c; 
            while (c != "go") cin >> c;
            goto First_Entrance;
        }
    }
    else if (choice == "6") {
        adminUser->searchName();
        cout << "\u001b[33m";
        cout << "Type 'go' to return to main menu\n";
        cout << "\u001b[0m";
        string c; 
        while (c != "go") cin >> c;
        goto First_Entrance;
    }
    else if (choice == "7") {
        adminUser->searchID();
        cout << "\u001b[33m";
        cout << "Type 'go' to return to main menu\n";
        cout << "\u001b[0m";
        string c; 
        while (c != "go") cin >> c;
        goto First_Entrance;
    }
    else if (choice == "8") {
        adminUser->populationPyramid();
        cout << "\u001b[33m";
        cout << "Type 'go' to return to main menu\n";
        cout << "\u001b[0m";
        string c; 
        while (c != "go") cin >> c;
        goto First_Entrance;
    }
    else if (choice == "9") {
        adminUser->marriageRate();
        cout << "\u001b[33m";
        cout << "Type 'go' to return to main menu\n";
        cout << "\u001b[0m";
        string c; 
        while (c != "go") cin >> c;
        goto First_Entrance;
    }
    else if (choice == "10") {
        adminUser->averageIncome();
        cout << "\u001b[33m";
        cout << "Type 'go' to return to main menu\n";
        cout << "\u001b[0m";
        string c; 
        while (c != "go") cin >> c;
        goto First_Entrance;
    }
    else if (choice == "11") {
        adminUser->sixDegreesOfSeparation();
        cout << "\u001b[33m";
        cout << "Type 'go' to return to main menu\n";
        cout << "\u001b[0m";
        string c; 
        while (c != "go") cin >> c;
        goto First_Entrance;
    }
    else {
        cout << "\u001b[31m";
        cout << "Invalid input. Please follow the menu options\n";
        cout << "\u001b[0m";
        goto First_Entrance;
    }
    return true;
}