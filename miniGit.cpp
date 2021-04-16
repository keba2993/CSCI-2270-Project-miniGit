// CSCI 2270 - Project: miniGit
// Author: Kevin Barone 
// Date: 14th April 2021

/* Description:
*/

#include "miniGit.hpp"

using namespace std;

miniGit::miniGit()  // constructor
{
    currentCommit = new commitNode();
    currentCommit->commitNum = 0;
    currentCommit->head = nullptr;
    currentCommit->next = nullptr;
    currentCommit->previous = nullptr;

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
        << "4. checkout version" << endl
        << "5. quit" << endl;
}

// helper function to form version fileName when adding a file
string makeVersion(string fileName, int vNum)
{
    int  dotIndex = 0;

    for (int i = 0; i < fileName.length(); ++i)
    {
        if (fileName[i] == '.')
        {
            dotIndex = i;
            break;
        }
    }

    return fileName.substr(0, dotIndex) + "_0" + to_string(vNum) + fileName.substr(dotIndex);
}

// add file
void miniGit::addFile(string fileName)
{
    ifstream addedFile(fileName);

    // check if file was already added to this commit
    if (SLLSearch(fileName))
    {
        cerr << "ERROR: file already added --  cannot add to miniGit" << endl;
    }
    // checking that the file can be opened
    else if (!addedFile.is_open())
    {
        cerr << "ERROR: Invalid file name -- cannot add to miniGit" << endl;
        return;
    }
    else
    {
        fileNode* addedFile = new fileNode();
        addedFile->fileName = fileName;
        addedFile->versioNum = 0;
        addedFile->next = nullptr;

        addedFile->fileVersion = makeVersion(fileName, addedFile->versioNum);

        if (currentCommit->head == nullptr)
        {
            currentCommit->head = addedFile;
        }
        else
        {
            fileNode* tmp = currentCommit->head;

            while (tmp->next != nullptr)
            {
                tmp = tmp->next;
            }

            tmp->next = addedFile;
        }
    }
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
commitNode* miniGit::DLLSearch(int number)
{

}

// search singleLL
bool miniGit::SLLSearch(string file)
{
    // search for file and check that version num is updated or not
}