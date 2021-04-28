// CSCI 2270 - Project: miniGit
// Author: Kevin Barone & Brennan Belei
// Date: 14th April 2021

/* Description:
 * This is the header file for the miniGit system. The fileNode sturct is used for 
 * the SLL nodes which hold and keep track of the files on the commit list. The
 * commitNode struct is used to keep track of all the commits the user performs and
 * makes up the DLL. The main class contains a data member that points to the current 
 * commit the user is viewing, and contains all necessary functions (add, remove, 
 * commit, checkout, etc.) to mimic a git version control system.
*/

#ifndef MINIGIT_H
#define MINIGIT_H

#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

// struct for file nodes in SLL
struct fileNode
{
    string fileName;
    string fileVersion;
    int versionNum;
    fileNode* next;

    bool removed;
};

// struct for commit nodes in DLL
struct commitNode
{
    int commitNum;
    // do we want to add a message variable
    fileNode* head;
    commitNode* previous;
    commitNode* next;
};

class miniGit
{
    public:
        miniGit();  // constructor
        ~miniGit(); // destructor

        void init(string n, string e);    // initialize DLL

        // getters and setters for name and email
        void setName(string n);
        void setEmail(string e);

        string getName();
        string getEmail();

        // get current
        commitNode* getCurrent();

        // display the menu options
        void displayOptions();

        // print out the sturcture DLL w/ SLL
        void printGit();

        // add file 
        void addFile(string fileName);

        // remove file
        void removeFile(string fileName);

        // commit changes
        void commit();
        void checkCommit();

        // checkout 
        void checkout(int commitNum);

        // traverse doubleLL
        commitNode* DLLSearch(int number);

        // search singleLL
        fileNode* SLLSearch(string file);

        // copy file
        void readWrite(string readFrom, string writeTo, bool isCheck);

        // check equality
        bool isEqual(string read, string write, bool afterCO);

    private:
        commitNode* currentCommit;
        commitNode* mostRecent;
        string name;
        string email;
};

#endif