// CSCI 2270 - Project: miniGit
// Author: Kevin Barone & Brennan Belei
// Date: 14th April 2021

/* Description:
*/

#include "miniGit.hpp"

using namespace std;

#include <filesystem>              // this is not working and IDK why?
namespace fs = std::filesystem;

miniGit::miniGit()  // constructor
{
    fs::create_directory(".miniGit");       // creating directory for miniGit
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

    fs::remove_all(".miniGit");     // removing all files from the miniGit folder
}

// initializing the DLL aka the miniGit system
void miniGit::init(string n, string e)
{
    currentCommit = new commitNode();
    currentCommit->commitNum = 0;
    currentCommit->head = nullptr;
    currentCommit->next = nullptr;
    currentCommit->previous = nullptr;

    // user variables
    setName(n);
    setEmail(e);

    cout << endl << name << ", you have successfully initialized your miniGit system with the email: " << email << endl;
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
void miniGit::displayOptions()
{
    cout << endl << "Choose an option (number): " << endl
        << "1. initialize system" << endl
        << "2. add file" << endl
        << "3. remove file" << endl
        << "4. commit changes" << endl
        << "5. checkout version" << endl
        << "6. quit" << endl
        << ">> ";
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

    return fileName.substr(0, dotIndex) + "__" + to_string(vNum) + fileName.substr(dotIndex);
}

// add file
void miniGit::addFile(string fileName)
{
    // make sure the system is initialized
    if (currentCommit == nullptr)
    {
        cerr << endl << "ERROR: miniGit system not initialized - please complete init" << endl;
        return;
    }

    // open file
    ifstream addedFile(fileName);

    // checking that the file can be opened
    if (!addedFile.is_open())
    {
        cerr << endl << "ERROR: Invalid file name -- cannot add to miniGit" << endl;
        return;
    }
    // check if file was already added to this commit
    else if (SLLSearch(fileName))
    {
        cerr << endl << "ERROR: File already added --  cannot add to miniGit" << endl;
        return;
    }
    else
    {
        // creating new file node
        fileNode* addedFile = new fileNode();
        addedFile->fileName = fileName;
        addedFile->versioNum = 0;
        addedFile->next = nullptr;

        // making new version filename
        addedFile->fileVersion = makeVersion(fileName, addedFile->versioNum);

        // if SLL is empty add at head
        if (currentCommit->head == nullptr)
        {
            currentCommit->head = addedFile;
        }
        // other wise traverse till end of SLL is reached to add
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

    cout << endl << "You successfully added file: " << fileName << " to the commit list." << endl;

    addedFile.close();
}

// remove file
void miniGit::removeFile(string fileName)
{
    // brennan
    //prompt user for the filename to remove
    //Scan the SLL of the currentCommit in order to find if the filename already exists
    //if found delete the node with standard SLL practice
    //else print error statement

}

// commit changes
void miniGit::commit()
{
    //scan the SLL of the current commitNode
    /*
            //the sllSearch will do this for us anyways
            currFileNode = currComNode->head;
    
            while(currFileNode != nullptr)
            {
                if(currFileNode->fileVersion != myFileVersion)
                {
                    currFileNode = currFileNode->next;
                }
            }


        if(SLLSearch == false)
        {
            //the file was not found
            //call addFile because there is no previous version of this file in the currentCommit
            addFile(myFileVersion);
        }
        else
        {
            //the file was found in the current Commit we need to line by line compare to see if there were changes made to the matching file in the commit
            fstream fileInCommit("nameOfFile"), fileToCommit("myFile");
            //getline loop to check for difference
            bool same = true;
            while(same)
            {
                while(getline(fileInCommit, lineInCommit))
                {
                    while(getline(fileToCommit, lineToCommit))
                    {
                        if(lineInCommit == lineToCommit)
                        {
                            //the lines are matching so far
                            //change both lineInCommit and lineToCommit to be the respective next lines in files
                            
                        }
                        else
                        {
                            //the files are different and we can commit
                            same = true;
                            //do the stuff to add it to the commit 
                            return;
                        }
                        break;
                    }
                    break;

                }
            }
        }

    */
}

// checkout
void miniGit::checkout()
{

}

// get current
commitNode* miniGit::getCurrent()
{
    return currentCommit;
}

// traverse doubleLL
commitNode* miniGit::DLLSearch(int number)
{
    // brennan
    return nullptr;
}

// search singleLL
bool miniGit::SLLSearch(string file)
{
    // search for file
    fileNode* tmp = currentCommit->head;

    while (tmp != nullptr)
    {
        if (tmp->fileName == file)
        {
            return true;        // file was found
        }
    }

    return false;
}