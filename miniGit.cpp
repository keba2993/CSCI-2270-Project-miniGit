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
    fs::remove_all(".miniGit");             // removing all files from the miniGit folder
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

    mostRecent = currentCommit;

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
    // do nothing if git is not initialized
    if (currentCommit == nullptr)
    {
        return;
    }

    cout << endl;

    // create crawl pointers for commits and files - DLL and SLL
    commitNode* comCrawl = currentCommit;
    fileNode* fileCrawl = currentCommit->head;

    // loop until empty commit is reached
    while (comCrawl != nullptr)
    {
        cout << "Commit: " << comCrawl->commitNum << " --> ";

        // loop through all files of present commit
        while (fileCrawl != nullptr)
        {
            cout << fileCrawl->fileName << " (" << fileCrawl->versionNum << ") ";

            // adding tag to inactive files
            if (fileCrawl->removed)
            {
                cout << "*RMV* ";
            }

            // checking if there is another file or not
            if (fileCrawl->next != nullptr)
            {
                cout << "--> ";
            }

            fileCrawl = fileCrawl->next;
        }

        comCrawl = comCrawl->previous;

        // only set this if there exists another commit
        if (comCrawl != nullptr)
        {
            fileCrawl = comCrawl->head;
        }

        cout << endl;
    }
}

// helper function to form version fileName when adding a file
string makeVersion(string fileName, int vNum)
{
    int  dotIndex = 0;

    // loop trhough filename until dot is found
    for (int i = 0; i < fileName.length(); ++i)
    {
        if (fileName[i] == '.')
        {
            dotIndex = i;
            break;
        }
    }

    // returning version name in format <filename>__<version number>.<file type>
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
    else if (SLLSearch(fileName) != nullptr)
    {
        // if file is added and active - error
        if (!SLLSearch(fileName)->removed)
        {
            cerr << endl << "ERROR: File already added -- cannot add to miniGit" << endl;
        }
        else
        {   
            // if inactive make it active
            SLLSearch(fileName)->removed = false;
        }
        
        return;
    }
    // check that user is on most recent commit
    else if (currentCommit->next != nullptr)
    {
        cerr << endl << "ERROR: Cannot add file - uncommitted changes from checkout" << endl;
        return;
    }
    else
    {
        // creating new file node
        fileNode* addedFile = new fileNode();
        addedFile->fileName = fileName;
        addedFile->versionNum = 0;
        addedFile->next = nullptr;
        addedFile->removed = false;

        // making new version filename
        addedFile->fileVersion = makeVersion(fileName, addedFile->versionNum);

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

    // check that user is on most recent commit
    if (currentCommit->next != nullptr)
    {
        cerr << endl << "ERROR: Cannot remove file - uncommitted changes from checkout" << endl;
        return;
    }

    // checking for empty commit list
    if (currentCommit->head == nullptr)
    {
        cerr << endl << "ERROR: Cannot remove file - no files have been added to commit list" << endl;
        return;
    }

    fileNode * currFile = nullptr;
    fileNode * nextFile = currentCommit->head;

    while (nextFile != nullptr)
    {
        if (nextFile->fileName == fileName)
        {
            // the file has been successfully located

            nextFile->removed = true;       // setting as inactive

            cout << endl << "File: " << fileName << " was successfully deleted." << endl;
            return;
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
    // scan the SLL of the current commitNode 
    // string called newFileName has no version number attached
    // we need to first check if some version of the newFileName already exists in the LL
    // use the SLLSearch function

    // make sure the system is initialized
    if (currentCommit == nullptr)
    {
        cerr << endl << "ERROR: miniGit system not initialized - please complete init" << endl;
        return;
    }

    // check that user is on most recent commit
    if (currentCommit->next != nullptr)
    {
        checkCommit();
        return;
    }

    // only look for changes if most recent commit is not 0
    if (mostRecent->commitNum != 0)
    {
        // loop to see if any files changed or if new files were added
        fileNode* findChanges = currentCommit->head;
        fileNode* prevCrawl = currentCommit->previous->head;
        bool isChange = false;

        // looping through all files of current commit
        while (findChanges != nullptr)
        {
            ifstream checkFor(".minigit/" + findChanges->fileVersion);

            // can not open fileVersion file - means there is a new file
            if (!checkFor.is_open())
            {
                isChange = true;
                break; // exit loop to save on complexity cost
            }

            checkFor.close();

            // file and fileVersion aren't equal so there is a change to commit
            if (!isEqual(findChanges->fileName, findChanges->fileVersion, false) && !findChanges->removed)
            {
                isChange = true;
                break;
            }

            // file was recently removed
            if (findChanges->removed != prevCrawl->removed)
            {
                isChange = true;
                break;
            }

            findChanges = findChanges->next;
            prevCrawl = prevCrawl->next;
        } 

        // if no change is found then do not create new commit node
        if (!isChange)
        {
            cout << "There has been no changes to the added files. Commit was not executed." << endl;
            return;
        }
    }

    // deep copying of previous commit
    commitNode* newCommit = new commitNode();
    newCommit->commitNum = currentCommit->commitNum + 1;
    newCommit->previous = currentCommit;
    newCommit->next = nullptr;
    newCommit->head = nullptr;

    currentCommit->next = newCommit;

    fileNode* copyPrev = currentCommit->head;
    fileNode* prev = nullptr;

    while (copyPrev != nullptr)
    {
        fileNode* copyNew = new fileNode();
        copyNew->versionNum = copyPrev->versionNum;
        copyNew->fileName = copyPrev->fileName;
        copyNew->fileVersion = copyPrev->fileVersion;
        copyNew->removed = copyPrev->removed;
        copyNew->next = nullptr;

        if (prev != nullptr)
        {
            prev->next = copyNew;
        }

        prev = copyNew;

        if (copyPrev == currentCommit->head)
        {
            newCommit->head = copyNew;
        }

        copyPrev = copyPrev->next;
    }

    currentCommit = newCommit;
    mostRecent = currentCommit;

    // for each file in the directory we need to loop thru the SLL and find its matching filenode
    // then we need to compare it to see it has been changed using isEqual
    fileNode* crawler = currentCommit->head;

    while (crawler != nullptr)
    {
        ifstream versionFile(".minigit/" + crawler->fileVersion);

        // checking if the file has been commited before
        if (!versionFile.is_open())
        {
            readWrite(crawler->fileName, crawler->fileVersion, false);

            versionFile.close();
        }
        // there is a version of the file in the miniGit directory
        else 
        {
            versionFile.close();

            if (!crawler->removed)
            {
                //we have found the corresponding file  and now need to compare it
                if (isEqual(crawler->fileName, crawler->fileVersion, false))
                {
                    // do nothing
                    // the files are equal and there is no need to readWrite
                }
                else
                {
                    // making new version name for this file - since edits were made
                    crawler->versionNum++;
                    crawler->fileVersion = makeVersion(crawler->fileName, crawler->versionNum);

                    // copying main file contents to new version file
                    readWrite(crawler->fileName, crawler->fileVersion, false);
                }
            }
        }

        crawler = crawler->next;
    }

    cout << "Commit Status: Success" << endl;
}

// committing after a checkout
void miniGit::checkCommit()
{
    // loop to see if any files changed or if new files were added
    fileNode* findChanges = currentCommit->head;
    fileNode* mostRecentCrawler = mostRecent->head;
    bool isChange = false;

    // looping through all files of current commit
    while (mostRecentCrawler != nullptr)
    {   
        if (findChanges == nullptr)
        {
            isChange = true;
            break;
        }

        // file and fileVersion aren't equal so there is a change to commit
        if (!isEqual(findChanges->fileVersion, mostRecentCrawler->fileVersion, true))
        {
            isChange = true;
            break;
        }

        // file was recently removed
        if (findChanges->removed != mostRecentCrawler->removed)
        {
            isChange = true;
            break;
        }

        findChanges = findChanges->next;
        mostRecentCrawler = mostRecentCrawler->next;
    } 

    // if no change is found then do not create new commit node
    if (!isChange)
    {
        cout << "There has been no changes to the added files. Commit was not executed." << endl
            << "Reverting to last commit." << endl;

        currentCommit = mostRecent;
        return;
    }

    // deep copying of previous commit
    commitNode* newCommit = new commitNode();
    newCommit->commitNum = mostRecent->commitNum + 1;
    newCommit->previous = mostRecent;
    newCommit->next = nullptr;
    newCommit->head = nullptr;

    mostRecent->next = newCommit;

    fileNode* copyPrev = mostRecent->head;          // crawler for most recent commit
    fileNode* currentCrawl = currentCommit->head;   // crawler for current commit
    fileNode* prev = nullptr;

    while (copyPrev != nullptr)
    {
        fileNode* copyNew = new fileNode();
        copyNew->versionNum = copyPrev->versionNum;
        copyNew->fileName = copyPrev->fileName;
        copyNew->fileVersion = copyPrev->fileVersion;
        copyNew->removed = copyPrev->removed;
        copyNew->next = nullptr;

        // setting previous new node next pointer
        if (prev != nullptr)
        {
            prev->next = copyNew;
        }

        prev = copyNew;

        // setting head pointer
        if (copyPrev == mostRecent->head)
        {
            newCommit->head = copyNew;
        }

        // if this file is not in current - then it needs to be removed
        if (currentCrawl == nullptr)
        {
            copyNew->removed = true;
        }
        else
        {   
            // setting removed property of new file node
            copyNew->removed = currentCrawl->removed;
            currentCrawl = currentCrawl->next;
        }
        copyPrev = copyPrev->next;
    }

    fileNode* newCrawl = newCommit->head;   // new commit crawler

    while (currentCrawl != nullptr)
    {
        // checking the version numbers and updating them for the current commit
        if (currentCrawl->versionNum != newCrawl->versionNum)
        {
            newCrawl->versionNum = currentCrawl->versionNum;
            newCrawl->fileVersion = currentCrawl->fileVersion;
        }

        currentCrawl = currentCrawl->next;
        newCrawl = newCrawl->next;
    }

    currentCommit = newCommit;  // moving current commit

    // for each file in the directory we need to loop thru the SLL and find its matching filenode
    // then we need to compare it to see it has been changed using isEqual
    fileNode* crawler = currentCommit->head;
    fileNode* prevCrawl = currentCommit->previous->head;

    while (crawler != nullptr)
    {

        // making new version name for this file - since edits were made - if not removed
        if (!crawler->removed)
        {
            crawler->versionNum = prevCrawl->versionNum + 1;
            crawler->fileVersion = makeVersion(crawler->fileName, crawler->versionNum);

            // copying main file contents to new version file
            readWrite(crawler->fileName, crawler->fileVersion, false);
        }

        crawler = crawler->next;
        prevCrawl = prevCrawl->next;
    }

    mostRecent = currentCommit;
    cout << "Commit Status: Successful after checkout" << endl;
}

// checkout
void miniGit::checkout(int commitNum)
{
    // check if the system is initialized
    if (currentCommit == nullptr)
    {
        cerr << endl << "ERROR: miniGit system not initialized - please complete init" << endl;
        return;
    }

    // loop to see if any files changed or if new files were added
    fileNode *findChanges = currentCommit->head;
    fileNode *prevCrawl = currentCommit->previous->head;
    bool isChange = false;

    // looping through all files of current commit
    while (findChanges != nullptr)
    {
        ifstream checkFor(".minigit/" + findChanges->fileVersion);

        // can not open fileVersion file - means there is a new file
        if (!checkFor.is_open())
        {
            isChange = true;
            break; // exit loop to save on complexity cost
        }

        checkFor.close();

        // file and fileVersion aren't equal so there is a change to commit
        if (!isEqual(findChanges->fileName, findChanges->fileVersion, false) && !findChanges->removed)
        {
            isChange = true;
            break;
        }

        // file was recently removed
        if (findChanges->removed != prevCrawl->removed)
        {
            isChange = true;
            break;
        }

        findChanges = findChanges->next;
        prevCrawl = prevCrawl->next;
    }

    // if change is found then do not allow user to checkout commit
    if (isChange)
    {
        cout << endl
             << "There has been changes to the added files. Checkout was not executed."
             << "Please perform a commit to save most recent file versions." << endl;
        return;
    }

    //take the wanted commit and make the newest commit the desired commit
    commitNode * targetCommit = DLLSearch(commitNum);

    if (targetCommit == nullptr)
    {
        // valid commitNum has not been entered
        return;
    }
    else
    {
        fileNode * fileCrawlerForCheck = targetCommit->head;
        // a valid commit number has been entered
        while (fileCrawlerForCheck != nullptr)
        {
            // read in adn update files based on checkout versions
            readWrite (fileCrawlerForCheck->fileVersion, fileCrawlerForCheck->fileName, true);

            fileCrawlerForCheck = fileCrawlerForCheck->next;
        }

        currentCommit = targetCommit;

        cout << endl << "Checkout on commit number: " << commitNum << " successful." << endl;
        return;
    }
}

// traverse doubleLL
commitNode* miniGit::DLLSearch(int number)
{
    // if greater or less than current commint num
    // while previous or while next != null
    // if numbers match return this node
    // if while loop exits return null

    commitNode* crawler = currentCommit;
    // number is greater than current commit
    if (number > currentCommit->commitNum)
    {
        while (crawler != nullptr)
        {
            // cout << crawler->commitNum << endl;
            if (number == crawler->commitNum)
            {
                return crawler;
            }
            
            crawler = crawler->next;
        
        }

        // if we reach here and the function has not returned then we know that the number is invalid
        cerr << endl << "ERROR: Invalid commit number - please enter a valid commit number" << endl;
        return nullptr;
    }
    // number is less than current commit
    else if (number < currentCommit->commitNum)
    {
        while (crawler != nullptr)
        {
            if (number == crawler->commitNum)
            {
                return crawler;
            }

            crawler = crawler->previous;
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
fileNode* miniGit::SLLSearch(string file)
{
    // search for file
    fileNode* tmp = currentCommit->head;

    while (tmp != nullptr)
    {
        if (tmp->fileName == file)
        {
            return tmp;        // file was found
        }

        tmp = tmp->next;
    }

    return nullptr;
}

// copying file contents to another file
void miniGit::readWrite(string readFrom, string writeTo, bool isCheck)
{
    // this function will streamline the process of commiting and checking out files as both functions rely on 
    // reading and writing to different files just in different orders

    ifstream readFile;
    ofstream writeFile;

    // different set ups for whether this fucntion was called by commit or checkout
    if (!isCheck)
    {
        readFile.open(readFrom);
        writeFile.open("./.minigit/" + writeTo);
    }
    else
    {
        readFile.open(".minigit/" + readFrom);
        writeFile.open(writeTo);
    }

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
bool miniGit::isEqual(string readFrom, string writeTo, bool afterCO)
{
    ifstream readFile;
    ifstream writeFile;

    // different set ups if this was called in normal commit or after a checkout
    if (afterCO)
    {
        readFile.open("./.minigit/" + readFrom);
        writeFile.open("./.minigit/" + writeTo);
    }
    else
    {
        readFile.open(readFrom);
        writeFile.open("./.minigit/" + writeTo);
    }

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
