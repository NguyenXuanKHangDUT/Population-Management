#include "UserInterface.h"
#include <iostream>
#include <string>

using namespace std;

extern MyVector<Person*> profiles; // Each pointer points to a Person
extern MyVector<Household*> Families;  // Each pointer points to a Household
extern HashMap<string, int> PersonIndex;  // Maps Personal_ID to their index in profiles so that we can look up and update quickly
extern HashMap<string, Person*> IDHash;   // Maps Personal_ID to Person pointers

extern Person* User;

void login() {
    cout << "-      WELCOME TO POPULATION MANAGEMENT SYSTEM     -\n";
    Login:
    cout << "Login \n";
    cout << "Personal ID: "; string id; cin >> id;
    Password:
    cout << "Password: "; string pw; cin >> pw;
    auto target = IDHash.find(id);
    if (target == IDHash.end()) {
        cout << "There were no person with that ID, login again!\n";
        goto Login;
    }
    else {
        User = target->second;
        if (pw != User->getPassword()) {
            cout << "Wrong password, type again!\n";
            goto Password;
        }
    }
    cout << "Welcome " << User->getFullName() << "!\n";

    if (User->getPassword() == "11111111") {
        cout << "First time login detected. Please change your password.\n";
        Change_Password:
        cout << "New password (3 <= length <= 8): ";
        string npw; cin >> npw;
        if (npw.length() >= 3 && npw.length() <= 8) {
            User->setPassword(npw);
            cout << "Password changed successfully.\n";
        } else if (npw == "11111111") {
            cout << "New password cannot be the default password. type again!!\n";
            goto Change_Password;
        }
        else {
            cout << "Invalid password length. type again!!\n";
            goto Change_Password;
        }
    }
}

bool NetizenInterface::launch() {
    cout << "-  Netizen Interface  -\n\n";

    First_Entrance:
    cout << "Choose an option:\n";
    cout << "1. View Personal Information\n";
    cout << "2. View my Household information\n";
    cout << "3. Update Personal Information\n";
    cout << "4. Set marriage to someone\n";
    cout << "5. Sign out\n";
    cout << "0. Exit\n";
    string choice; cin >> choice;
    if (choice == "0") return true;
    else if (choice == "5") return false;
    else if (choice == "1") {
        cout << *User;
        cout << "Type 'go_on' to back to main menu\n";
        string c; 
        while (c != "go_on") cin >> c;
        goto First_Entrance;
    }
    else if (choice == "2") {
        Household* hhTmp = User->getHost()->getHousehold();
        cout << *hhTmp;
        cout << "Type 'go_on' to back to main menu\n";
        string c; 
        while (c != "go_on") cin >> c;
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
            cout << "Type 'go_on' to back to main menu\n";
            string c; 
            while (c != "go_on") cin >> c;
            goto Change_Information;
        }
    }
    else if (choice == "4") {
        Choose_Partner:
        cout << "Type 'back' to back\n";
        cout << "Type " << (User->getGender() ? "her" : "his") << " Personal ID: ";
        string id; cin >> id;
        if (id == "back") goto First_Entrance;
        auto target = IDHash.find(id);
        if (target == IDHash.end()) {
            cout << "There were no person with that ID, type again!\n";
            goto Choose_Partner;
        }
        Person* partner = target->second;
        if (partner->getPartner() != nullptr) {
            cout << "that person was got marriage already, so sad\nTry another person!\n";
            goto Choose_Partner;
        }
        else if (User->getGender() == partner->getGender()) {
            cout << "LGBT is unaccepted in this Country\nTry another person\n";
            goto Choose_Partner;
        }
        else {
            User->setPartner(partner);
            if (User->getPartner() == partner) {
                cout << "Set marriage to " << partner->getFullName() << " successfully!\n";
                cout << "Type 'go_on' to back to main menu\n";
                string c; 
                while (c != "go_on") cin >> c;
                goto First_Entrance;
            }
        }
    }
    else {
        cout << "Invalid input. Please follow the menu options\n";
        goto First_Entrance;
    }
    return true;
}

bool HostInterface::launch() {
    cout << "-  Host Interface  -\n\n";
    Host* hostUser = dynamic_cast<Host*>(User);
    
    First_Entrance:
    cout << "Choose an option:\n";
    cout << "1. View Personal Information\n";
    cout << "2. View my Household information\n";
    cout << "3. Update Personal Information\n";
    cout << "4. Set marriage to someone\n";
    cout << "5. Add 1 more member to family\n";
    cout << "6. Banish 1 member from family\n";
    cout << "7. Sign out\n";
    cout << "0. Exit\n";
    string choice; cin >> choice;
    if (choice == "0") return true;
    else if (choice == "7") return false;
    else if (choice == "1") {
        cout << *hostUser;
        cout << "Type 'go_on' to back to main menu\n";
        string c; 
        while (c != "go_on") cin >> c;
        goto First_Entrance;
    }
    else if (choice == "2") {
        Household* hhTmp = hostUser->getHousehold();
        cout << *hhTmp;
        cout << "Type 'go_on' to back to main menu\n";
        string c; 
        while (c != "go_on") cin >> c;
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
            cout << "Type 'go_on' to back to main menu\n";
            string c; 
            while (c != "go_on") cin >> c;
            goto Change_Information;
        }
    }
    else if (choice == "4") {
        Choose_Partner:
        cout << "Type 'back' to back\n";
        cout << "Type " << (hostUser->getGender() ? "her" : "his") << " Personal ID: ";
        string id; cin >> id;
        if (id == "back") goto First_Entrance;
        auto target = IDHash.find(id);
        if (target == IDHash.end()) {
            cout << "There were no person with that ID, type again!\n";
            goto Choose_Partner;
        }
        Person* partner = target->second;
        if (partner->getPartner() != nullptr) {
            cout << "that person was got marriage already, so sad\nTry another person!\n";
            goto Choose_Partner;
        }
        else if (hostUser->getGender() == partner->getGender()) {
            cout << "LGBT is unaccepted in this Country\nTry another person\n";
            goto Choose_Partner;
        }
        else {
            hostUser->setPartner(partner);
            if (hostUser->getPartner() == partner) {
                cout << "Set marriage to " << partner->getFullName() << " successfully!\n";
                cout << "Type 'go_on' to back to main menu\n";
                string c; 
                while (c != "go_on") cin >> c;
                goto First_Entrance;
            }
        }
    }
    else if (choice == "5") {
        if (hostUser->summondMember()) {
            cout << "Add successfully!\n";
            cout << "Type 'go_on' to back to main menu\n";
            string c; 
            while (c != "go_on") cin >> c;
            goto First_Entrance;
        }
        else {
            cout << "Fail to add\n";
            cout << "Type 'go_on' to back to main menu\n";
            string c; 
            while (c != "go_on") cin >> c;
            goto First_Entrance;
        }
    }
    else if (choice == "6") {
        if (hostUser->banishMember()) {
            cout << "Banish successfully!\n";
            cout << "Type 'go_on' to back to main menu\n";
            string c; 
            while (c != "go_on") cin >> c;
            goto First_Entrance;
        }
        else {
            cout << "Fail to banish\n";
            cout << "Type 'go_on' to back to main menu\n";
            string c; 
            while (c != "go_on") cin >> c;
            goto First_Entrance;
        }
    }
    else {
        cout << "Invalid input. Please follow the menu options\n";
        goto First_Entrance;
    }
    return true;
}

bool AdminInterface::launch() {
    cout << "-  Admin Interface  -\n";
    Admin* adminUser = dynamic_cast<Admin*>(User);
    
    First_Entrance:
    cout << "Choose an option:\n";
    cout << "1. View Personal Information\n";
    cout << "2. View my Household information\n";
    cout << "3. Update Personal Information\n";
    cout << "4. Set marriage to someone\n\n";
    cout << "5. Add 1 more Host household to System\n";
    cout << "6. Banish 1 person from System\n\n";
    cout << "7. Search and view information of a person by name\n";
    cout << "8. Search and view information of a person by personal ID\n\n";
    cout << "--Statistic:\n";
    cout << "9. View populaton Pyramid\n";
    cout << "10. View marriage rate\n";
    cout << "11. View average income in each age group\n\n";
    cout << "12. Check whether person A and person B is linked or not by rule: 'Six Degrees Of Separation':\n";
    cout << "13. Sign out\n";
    cout << "0. Exit\n";
    string choice; cin >> choice;
    if (choice == "0") return true;
    else if (choice == "13") return false;
    else if (choice == "1") {
        cout << *adminUser;
        cout << "Type 'go_on' to back to main menu\n";
        string c; 
        while (c != "go_on") cin >> c;
        goto First_Entrance;
    }
    else if (choice == "2") {
        Household* hhTmp = adminUser->getHousehold();
        cout << *hhTmp;
        cout << "Type 'go_on' to back to main menu\n";
        string c; 
        while (c != "go_on") cin >> c;
        goto First_Entrance;
    }
    else if (choice == "3") {
        Change_Information:
        cout << "Changeable information: \n";
        cout << "-(1) Job: "; cout << adminUser->getJob() << endl;
        cout << "-(2) Income: "; cout << adminUser->getIncome() << endl;
        cout << "-(0) Back to main menu.\n";
        cout << "Your choice: "; string c; cin >> c;
        if (c == "0") goto First_Entrance;
        else if (c == "2") {
            Change_Income:
            cout << "Type your new income: "; double ic; cin >> ic;
            adminUser->setIncome(ic);
            goto Change_Information;
        }
    }
    else if (choice == "4") {
        Choose_Partner:
        cout << "Type 'back' to back\n";
        cout << "Type " << (adminUser->getGender() ? "her" : "his") << " Personal ID: ";
        string id; cin >> id;
        if (id == "back") goto First_Entrance;
        auto target = IDHash.find(id);
        if (target == IDHash.end()) {
            cout << "There were no person with that ID, type again!\n";
            goto Choose_Partner;
        }
        Person* partner = target->second;
        if (partner->getPartner() != nullptr) {
            cout << "that person was got marriage already, so sad\nTry another person!\n";
            goto Choose_Partner;
        }
        else if (adminUser->getGender() == partner->getGender()) {
            cout << "LGBT is unaccepted in this Country\nTry another person\n";
            goto Choose_Partner;
        }
        else {
            adminUser->setPartner(partner);
            if (adminUser->getPartner() == partner) {
                cout << "Set marriage to " << partner->getFullName() << " successfully!\n";
                cout << "Type 'go_on' to back to main menu\n";
                string c; 
                while (c != "go_on") cin >> c;
                goto First_Entrance;
            }
        }
    }
    else if (choice == "5") {
        if (adminUser->summondMember()) {
            cout << "Added successfully!\n";
            cout << "Type 'go_on' to back to main menu\n";
            string c; 
            while (c != "go_on") cin >> c;
            goto First_Entrance;
        }
        else {
            cout << "Fail to add\n";
            cout << "Type 'go_on' to back to main menu\n";
            string c; 
            while (c != "go_on") cin >> c;
            goto First_Entrance;
        }
    }
    else if (choice == "6") {
        if (adminUser->banishMember()) {
            cout << "Banish successfully!\n";
            cout << "Type 'go_on' to back to main menu\n";
            string c; 
            while (c != "go_on") cin >> c;
            goto First_Entrance;
        }
        else {
            cout << "Fail to banish\n";
            cout << "Type 'go_on' to back to main menu\n";
            string c; 
            while (c != "go_on") cin >> c;
            goto First_Entrance;
        }
    }
    else if (choice == "7") {
        adminUser->searchName();
        cout << "Type 'go_on' to back to main menu\n";
        string c; 
        while (c != "go_on") cin >> c;
        goto First_Entrance;
    }
    else if (choice == "8") {
        adminUser->searchID();
        cout << "Type 'go_on' to back to main menu\n";
        string c; 
        while (c != "go_on") cin >> c;
        goto First_Entrance;
    }
    else if (choice == "9") {
        adminUser->populationPyramid();
        cout << "Type 'go_on' to back to main menu\n";
        string c; 
        while (c != "go_on") cin >> c;
        goto First_Entrance;
    }
    else if (choice == "10") {
        adminUser->marriageRate();
        cout << "Type 'go_on' to back to main menu\n";
        string c; 
        while (c != "go_on") cin >> c;
        goto First_Entrance;
    }
    else if (choice == "11") {
        adminUser->averageIncome();
        cout << "Type 'go_on' to back to main menu\n";
        string c; 
        while (c != "go_on") cin >> c;
        goto First_Entrance;
    }
    else if (choice == "12") {
        adminUser->sixDegreesOfSeparation();
        cout << "Type 'go_on' to back to main menu\n";
        string c; 
        while (c != "go_on") cin >> c;
        goto First_Entrance;
    }
    else {
        cout << "Invalid input. Please follow the menu options\n";
        goto First_Entrance;
    }
    return true;
}