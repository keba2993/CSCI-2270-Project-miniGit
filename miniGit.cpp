// CSCI 2270 - Project: miniGit
// Author: Kevin Barone & Brennan Belei
// Date: 14th April 2021

/* Description:
 * This is the implementation of the miniGit class functions. The addFile function adds a fileNode to the 
 * current commit's SLL, removeFile is used to remove a file form the commit list, commit performs 
 * the commit changes functionality of the git system, and checkout allows the user to view a previous
 * version of their files. Helper functions like makeVersion, readWrite, and isEqual are used to streamline
 * the implementation of more complicated functions.
*/

#include "miniGit.hpp"

using namespace std;

#include <filesystem>             
namespace fs = std::filesystem;

miniGit::miniGit()  // constructor
{
    fs::create_directory(".miniGit");       // creating directory for miniGit
}
miniGit::~miniGit() // destructor
{
    commitNode* hold = currentCommit;
    commitNode* tmpCommit = currentCommit;

    // resolving test case where there are no nodes initialized yet
    if (currentCommit != nullptr)
    {
        hold = currentCommit->next;
    }

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

// get current
commitNode* miniGit::getCurrent()
{
    return currentCommit;
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

// this will print out the full DLL and SLL data structure
void miniGit::printGit()
{
    // Kevin will implement
    commitNode* comCrawl = currentCommit;
    fileNode* fileCrawl = currentCommit->head;

    while (comCrawl != nullptr)
    {
        cout << "Commit: " << comCrawl->commitNum << " --> ";

        while (fileCrawl != nullptr)
        {
            cout << fileCrawl->fileName << " (" << fileCrawl->versioNum << ") ";

            if (fileCrawl->next != nullptr)
            {
                cout << "--> ";
            }

            fileCrawl = fileCrawl->next;
        }

        comCrawl = comCrawl->previous;
        fileCrawl = comCrawl->head;
    }
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

// add file to commit list
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

// remove file from commit list
// ************************************* GETTING ERROR: double free detected ****************************
void miniGit::removeFile(string fileName)
{
    // prompt user for the filename to remove
    // Scan the SLL of the currentCommit in order to find if the filename already exists
    // if found delete the node with standard SLL practice
    // else print error statement

    // make sure the system is initialized
    if (currentCommit == nullptr)
    {
        cerr << endl << "ERROR: miniGit system not initialized - please complete init" << endl;
        return;
    }

    // checking for empty commit list
    if (currentCommit->head == nullptr)
    {
        cerr << endl << "ERROR: Cannot remove file - no files have been added to commit list" << endl;
        return;
    }
    // string justName = "";
    fileNode * currFile = currentCommit->head;
    fileNode * nextFile = currentCommit->head;

    while (nextFile != nullptr)
    {
        // for (int i = 0; i < nextFile->fileName.length(); i++)
        // {
        //     if(nextFile->fileName[i] == '.')
        //     {
        //         justName = nextFile->fileName.substr(0, i);
        //     }
        // }

        if (nextFile->fileName == fileName)
        {
            // the file has been successfully located
            cout << endl << "File: " << fileName << " was successfully deleted." << endl;

            // check first to see if it is at the very back of the SLL
            if (nextFile->next == nullptr)
            {
                delete nextFile;
                currFile->next = nullptr;
                nextFile = nullptr;
                return;
            }
            // special case for if the head is to be deleted
            else if (nextFile == currentCommit->head)
            {
                currentCommit->head = nextFile->next;
                delete nextFile;
                nextFile = nullptr;
                return;
            }
            // now we can assume that its somewhere in the middle
            else
            {
                currFile->next = nextFile->next;
                delete nextFile;
                nextFile = nullptr;
                return;
            }
        }
        else
        {
            // we haven't found it yet so increment our pointers
            currFile = nextFile;
            nextFile = nextFile->next;
        }
        
    }

    // here we can assume that the user has entered an invalid name
    cerr << endl << "ERROR: Please enter an existing file name to remove" << endl;
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

// traverse doubleLL
commitNode* miniGit::DLLSearch(int number)
{
    // if greater or less than current commint num
    // while previous or while next != null
    // if numbers match return this node
    // if while loop exits return null

    if (number > currentCommit->commitNum)
    {
        while (currentCommit != nullptr)
        {
            if (number == currentCommit->commitNum)
            {
                return currentCommit;
            }
            else
            {
                currentCommit = currentCommit->next;
            }
        
        }

        // if we reach here and the function has not returned then we know that the number is invalid
        cerr << endl << "ERROR: Invalid commit number - please enter a valid commit number" << endl;
        return nullptr;
    }
    else if(number < currentCommit->commitNum)
    {
        while (currentCommit != nullptr)
        {
            if (number == currentCommit->commitNum)
            {
                return currentCommit;
            }
            else
            {
                currentCommit = currentCommit->next;
            }
        }

        // if we reach here and the function has not returned then we know that the number is invalid
        cerr << endl << "ERROR: Invalid commit number - please enter a valid commit number" << endl;
        return nullptr;
    }
    else
    {
        // at this point we assume that we are already on the requested commit
        cout << endl << "You are already on this commit" << endl;
        return currentCommit;
    }

    return nullptr;     // don't think ths will ever be reached
}

// search singleLL - used to see if a file already was added to commit list
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

// copying file contents to another file
void miniGit::readWrite(string readFrom, string writeTo)
{
    // this function will streamline the process of commiting and checking out files as both functions rely on 
    // reading and writing to different files just in different orders

    ifstream readFile(readFrom);
    ofstream writeFile(writeTo);

    char letterRead, letterWrite;

    // checking if readFile can be opened
    if (!readFile.is_open())
    {
        cerr << endl << "ERROR: Invalid file for input" << endl;
        return;
    }
    // checking if writeFile can be opened
    if (!writeFile.is_open())
    {
        cerr << endl << "ERROR: Invalid file for output" << endl;
        return;
    }
    else
    {
        // loop through the characters of the readFile
        while (true)
        {
            letterRead = readFile.get();

            // check if the end of the file is reached
            if (letterRead == EOF)
            {
                break;
            } 

            writeFile << letterRead;    // write each character individually to writeFile
        }
    }

    writeFile.close();
    readFile.close();    
}

// being used to compare two files to see if their contents are equal
bool miniGit::isEqual(string readFrom, string writeTo)
{
    ifstream readFile(readFrom);
    ifstream writeFile(writeTo);

    char letterRead, letterWrite;

    // checking if the files can be opened
    if (!readFile.is_open() || !writeFile.is_open())
    {
        cerr << endl << "ERROR: At least one invalid file for comparison" << endl;
        return false;
    }
    else
    {
        // loop through each character of the files
        while (true)
        {
            letterRead = readFile.get();
            letterWrite = writeFile.get();

            // return false if the current letters do not match
            if (letterRead != letterWrite) 
            {
                return false;
            }

            // return true if both files reach the end at the same time
            if (letterRead == EOF && letterWrite == EOF)
            {
                return true;
            }
            // return false if one file reaches the end before the other
            else if (letterWrite == EOF || letterRead == EOF)
            {
                return false;
            }
        }
    }
    
    writeFile.close();
    readFile.close();

    // return true otherwise
    return true;
}