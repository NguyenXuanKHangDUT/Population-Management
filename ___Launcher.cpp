#include "Database.h"
#include "UserInterface.h"

#include <iostream>
#include <string>

using namespace std;

MyVector<Person*> profiles;
HashMap<string, Person*> IDHash;
HashMap<string, int> PersonIndex;

MyVector<Household*> Families;

Person* User = nullptr;

/*
g++ ___Launcher.cpp Household.cpp Host.cpp Admin.cpp Person.cpp UserInterface.cpp -o a
*/
// 111654714251
// 111026170669
int main() {
    // Init database
    Database* db = Database::Instance();

    profiles = db->getProfiles();
    Families = db->getFamilies();
    PersonIndex = db->getPersonIndex();
    IDHash = db->getIDHash();

    // Example usage (dùng tên Families cũ):
    // int co = 0;
    // for (const Household* h : Families) { 
    //     cout << *h << endl;
    //     co++;
    // }
    // int c = 0;
    // for (const Person* p : profiles) { 
    //     cout << *p << endl;
    //     c++;
    // }
    // cout << co << " " << c << endl;

    UserInterface* ui = nullptr;

    while (1) {
        // Assume login() uses the global containers (IDHash)
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
    // reassign global containers before deleting
    db->getProfiles() = profiles;
    db->getFamilies() = Families;
    db->getPersonIndex() = PersonIndex;
    db->getIDHash() = IDHash;
    
    cout << "Updating data ... \n";
    delete db;
    cout << "Data updated!";
    return 0;
}