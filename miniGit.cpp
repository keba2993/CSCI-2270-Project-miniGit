// CSCI 2270 - Project: miniGit
// Author: Kevin Barone 
// Date: 14th April 2021

/* Description:
*/

#include "miniGit.hpp"

using namespace std;

miniGit::miniGit()  // constructor
{
    currentCommit = nullptr;
    name = "";
    email = "";
}
miniGit::~miniGit() // destructor
{
    commitNode* hold = currentCommit->next;
    commitNode* tmpCommit = currentCommit;

    // deleting all nodes that come before the currentCommit node
    while (currentCommit != nullptr)
    {
        fileNode* tmpFile = currentCommit->head;

        while (currentCommit->head != nullptr)
        {
            currentCommit->head = currentCommit->head->next;
            delete tmpFile;
            tmpFile = currentCommit->head;
        } 

        currentCommit = currentCommit->previous;        
        delete tmpCommit;
        tmpCommit = currentCommit;
    }

    // moving currentCommit back to its next node
    // this is down taht the nodes after current can be deleted if program is ended
    // with currentCommit being on some node in the middle of the DLL
    currentCommit = hold;

    // deleting all nodes that come after currentCommit
    while (currentCommit != nullptr)
    {
        fileNode* tmpFile = currentCommit->head;

        while (currentCommit->head != nullptr)
        {
            currentCommit->head = currentCommit->head->next;
            delete tmpFile;
            tmpFile = currentCommit->head;
        } 

        currentCommit = currentCommit->next;       
        delete tmpCommit;
        tmpCommit = currentCommit;
    }
}

// setters for user name and email
void miniGit::setName(string n)
{
    name = n;
}
void miniGit::setEmail(string e)
{
    email = e;
}

// getters for user name and email
string miniGit::getName()
{
    return name;
}
string miniGit::getEmail()
{
    return email;
}

// display the menu options
void displayOptions()
{
    cout << "Choose an option (number): " << endl
        << "1. add file" << endl
        << "2. remove file" << endl
        << "3. commit changes" << endl
        << "4. checkout commit" << endl
        << "5. quit" << endl;
}

// add file
void miniGit::addFile(string fileName)
{

}

// remove file
void miniGit::removeFile(string fileName)
{

}

// commit changes
void miniGit::commit()
{

}

// checkout
void miniGit::checkout()
{

}

// get current
commitNode* miniGit::getCurrent()
{

}

// traverse doubleLL
void miniGit::DLLSearch(int number)
{

}

// search singleLL
void miniGit::SLLSearch(int number)
{

}