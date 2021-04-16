// CSCI 2270 - Project: miniGit
// Author: Kevin Barone 
// Date: 14th April 2021

/* Description:
*/

#ifndef MINIGIT_H
#define MINIGIT_H

#pragma once

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct fileNode
{
    string fileName;
    string fileVersion;
    fileNode* next;
};

struct commitNode
{
    int commitNum;
    fileNode* head;
    commitNode* previous;
    commitNode* next;
};

class miniGit
{
    public:
        miniGit();
        ~miniGit();

        // getters and setters for name and email
        void setName(string n);
        void setEmail(string e);

        string getName();
        string getEmail();

        // display the menu options
        void displayOptions();

        // add file 
        void addFile(string fileName);

        // remove file
        void removeFile(string fileName);

        // commit changes
        void commit();

        // checkout 
        void checkout();

        // get current
        commitNode* getCurrent();

        // traverse doubleLL
        void DLLSearch(int number);

        // search singleLL
        void SLLSearch(int number);

    private:
        commitNode* currentCommit;
        string name;
        string email;
};

#endif