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
    //string called newFileName has no version number attached
    //we need to first check if some version of the newFileName already exists in the LL
    //use the SLLSearch function
    bool result = SLLSearch(newFileName)
    if(result == false) we know that there are no previous versions of the file
    {
        add(newFileName);
        //since there are no previous version of the file we need to add it to the SLL
        makeVersion(newFileName, 0);
    }
    else
    {
        //for each file in the directory we need to loop thru the SLL and find its matching filenode
        //then we need to compare it to see it has been changed using isEqual
        fileNode * crawler = currentCommit->head ;
        //found this from stack overflow, seems like a way to iterate through the files in a directory
        //I think that this will be useful because we should make a list of the 
        //files in the directory so that we can search for each of them in the SLL
        string path = "/";

        string fileList[99] = {}; //could make this a vector
        int i = 0;
        for(const auto & file : directory_iterator(path))
        {
            fileList[i] = file.path();
            i++;
        } 
        for(int j = 0; j < 99; j++) //not entirely sure how to access the files from the current directory
        {
            string fileToCommit = fileList[j];

            while(crawler != nullptr)
            {
                if(fileToCommit == crawler->fileName)
                {
                    //we have found the corresponding file  and now need to compare it
                    if(isEqual(fileToCommit, crawler->fileName))
                    {
                        //the files are equal and there is no need to readWrite
                    }
                    else
                    {
                        
                        readWrite(fileToCommit, crawler->fileName)
                        makeVersion(fileToCommit, crawler->versionNum)
                    }
                }
                else
                {
                    crawler = crawler->next;
                }
            }
        }
    }
    

    */
}

// checkout
void miniGit::checkout(int commitNum)
{
    /* 
        //Get the list of files in the currentDirectory ready so we know what we are working with
        string listOfFiles[99] = {};
        int i = 0;
        for(const auto & file : directory_iterator(path))
        {
            listOfFiles[i] = file.path();
            i++;
        } 
        //do a DLL search in order to find the commitNum that we want
        commitNode * targetCommit = DLLSearch(commitNum);
        //for each of the files in the SLL of the targetCommit readWrite it over to the corresponding file in the directory
        crawler * fileNode = targetCommit->head;
        while(crawler != nullptr)
        {
            for(int k = 0; k < listOfFiles.size(); k++)
            {
                if(listOfFiles[k] == crawler->fileName)
                {
                    readWrite(crawler->fileName, listOfFiles[k]);
                }
            }
            crawler = crawler->next;
        }

    */
    
}

// get current
commitNode* miniGit::getCurrent()
{
    return currentCommit;
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

void miniGit::readWrite(string readFrom, string writeTo)
{
    //this function will streamline the process of commiting and checking out files as both functions rely on 
    //reading and writing to different files just in different orders
    
}



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