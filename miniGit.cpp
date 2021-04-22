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
    //prompt user for the filename to remove
    //Scan the SLL of the currentCommit in order to find if the filename already exists
    //if found delete the node with standard SLL practice
    //else print error statement
    string justName = "";
    fileNode * currFile = currentCommit->head;
    fileNode * nextFile = currentCommit->head;
    while(nextFile != nullptr)
    {
        for(int i = 0; i < nextFile->fileName.length(); i++)
        {
            if(nextFile->fileName[i] == '.')
            {
                justName = nextFile->fileName.substr(0, i);
            }
        }
        if(justName == fileName)
        {
            //the file has been successfully located
            //check first to see if it is at the very back of the SLL
            if(nextFile->next == nullptr)
            {
                delete nextFile;
                currFile->next = nullptr;
                return;
            }
            //special case for it the head is to be deleted
            else if(nextFile == currentCommit->head)
            {
                currentCommit->head = nextFile->next;
                delete nextFile;
                return;
            }
            //now we can assume that its somewhere in the middle
            else
            {
                currFile->next = nextFile->next;
                delete nextFile;
                return;
            }
        }
        else
        {
            //we havent found it yet so increment our pointers
            currFile = currFile->next;
            nextFile = nextFile->next;
        }
        
    }
    //here we can assume that the user has entered an invalid name
    cout << "please enter an existing file to delete" << endl;
    return;

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

}

// traverse doubleLL
commitNode* miniGit::DLLSearch(int number)
{
    // if greater or less than current commint num
    // while previous or while next != null
    // if numbers match return this node
    // if while loop exits return null
    if(number > currentCommit->commitNum)
    {
        while(currentCommit != nullptr)
        {
            if(number == currentCommit->commitNum)
            {
                return currentCommit;
            }
            else
            {
                currentCommit = currentCommit->next;
            }
        
        }
        //if we reach here and the function has not returned then we know that the number is invalid
        cout << "please enter a valid commit number" << endl;
        return nullptr;
    }
    else if(number < currentCommit->commitNum)
    {
        while(currentCommit != nullptr)
        {
            if(number == currentCommit->commitNum)
            {
                return currentCommit;
            }
            else
            {
                currentCommit = currentCommit->next;
            }
        }
        cout << "please enter a valid commit number" << endl;
    }
    else
    {
        //at this point we assume that we are already on the requested commit
        cout << "you are already on this commit" << endl;
        return currentCommit;
    }
    
}

// search singleLL
bool miniGit::SLLSearch(string file)
{
    // search for file and check that version num is updated or not
}

void miniGit::readWrite(string readFrom, string writeTo)
{
    //this function will streamline the process of commiting and checking out files as both functions rely on 
    //reading and writing to different files just in different orders
    
}