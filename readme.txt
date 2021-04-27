// CSCI 2270 - Project: miniGit
// Author: Kevin Barone & Brennan Belei
// Date: 20th April 2021


====== What the project is ======

    This project is a miniGit version control system that allows the user to add and commit changes
for the files that they are working on. The primary functions that this projec tincludes are
add, remove, commit, and checkout. The functionality of each is described in detail below. The 
program runs by first welcoming the user to the miniGit system. It then lists the 6 option choices, 
initialize, add, remove, commit, checkout, and quit. The user types in their numerical selection and 
they will be prompted with further requirements based on their choice. It is important to note that 
the user will be unable to perform any option until after they have initialized the system with their 
name and email. THis must first be completed before the miniGit system is fully ready.

desribe class struct - and filesystem**
    The class miniGit is comprised of a constructor, descructor, the functions listed below, and the
private data members currentCommit - pointer to current commit node, name - name of user, and 
email - user's email. The constructor simply creates the .minigit directory in the local directory using
the filesystem library. The descructor frees all memory associated with the DLL and SLL by visiting 
the previous nodes from the current commit and deleting each file node in their SLL, and then the next 
pointer is used to visit the DLL nodes after current commit and delete their SLL nodes. The remove_all
function of filesystem library is also used to delete all files in the .minigit directory.

    The struct used for the DLL is commitNode which has a commitNum, a head pointer to point to the SLL
associated wiht the commit, and next and previous pointers to other commit nodes. The struct for the 
SLL is fileNode which has a fileName, fileVersion - name of version file, versionNum, and a next pointer.


====== Functionality ======

    The driver.cpp file is where a miniGit object pointer is initialized, thus allowing the program to run.
A do-while loop is then used to constantly prompt the user with the 6 menu options desribed below. This loop
does not exit until the user chooses to quit the program. A switch statement is then used to carry out the 
user's choice based on the number they input. Of course an if statement is first used to check that a 
single digit is entered as the program would end with an error is a string is inputted. Choice 1 asks
the user to input their name and email to initialize the miniGit system, choice 2 prompts user for a file
name to be added to commit list, choice 3 prompts user for file name to be removed, choice 4 commits the 
user's changes to added files, choice 5 prompts user for commit number that they would like to checkout,
and choice 6 quits the program. After the loop is exited, the git is deleted and all memory is freed.

    The main functions that must be discussed are the init, add, remove, commit and checkout functions. 
The methodology behind each of these functions is described below. The helper functions -- DLLSearch, 
SLLSearch, makeVersion, isEqual, readWrite, and printGit -- are used to aid the algorithms for the 
aforementioned main functions. Their methodology is also briefly describes below.

HELPER FUNCTIONS:
    - DLLSearch - searches the doubly linked list commit nodes for the given commit number
    - SLLSearch - searches the singly linked list of files nodes at a commit node to see if a file already
                  exists in the list
    - makeVersion - returns the file name for each different version of a file
    - isEqual - returns whether or not two files' contents are exactly the same or not - used in commit
    - readWrite - copies the contents of a given file to a new file to be stored in the .minigit directory
    - printGit - prints the structure of the miniGit system (DLL and SLL) in a nice format - for debugging

MAIN FUNCTIONS:
    - init - initializes the DLL with an empty commit node with commit num zero - essentially the default
             constructor for the DLL structure
           - also sets the users name and email data members

    - addFile - takes in a file name and creates a new file node with it to be added to the current commit
              - first checks to see if the system has be initialized, then proceeds to swee if the file 
                exists in teh current directory and checks if the file has already been added using SLLSearch
              - if it is a new file, the new file node is created (makeVersion is used to create fileVersion string) 
                and then the SLL is traversed, using a while loop, till the end and the new node is tagged 
                onto the end

    - removeFile - takes in file name and removes the node of that file from the SLL of the current commit
                 - first checks that the system has been initialized and that the file has been added to the list
                 - a while loop is then used to traverse the SLL until the requested file node is found, it is then 
                   checked where the node is being removed from, head, middle, or end - each test case is dealt 
                   with properly as you would with any linked list

    - commit - it is again first checked if the system has been initialized, if it has, the current commit SLL is 
               is traversed - each file node's fileVersion file is opened
             - if the file can't be opened - this means a new file was added and needs to be committed
             - if the fileVersion and fileName files are not equal (isEqual is used) - need to commit
             - bool isChange is used to confirm that a new commit node must be introduced - if this is false then
               the function is exited before a new commit node is made
             - once a change is confirmed the current commit node is deeply copied to a new commit node using a 
               while loop to make new file nodes and copy the exact elements of the current commit's SLL
             - after the copying of the structure - another while loop is used to check for equality of each file
             - if no versionFile exists in .minigit directory then readWrite is called to create new version file
             - if the version file in .minigit exists and is equal to the actual file, then nothing is done for
               this file - otherwise the version num and version file name of the node is updated so that a new 
               version file can be made in the .minigit directory using readWrite again

    - checkout - 

Additional Comments....