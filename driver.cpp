// CSCI 2270 - Project: miniGit
// Author: Kevin Barone 
// Date: 14th April 2021

/* Description:
*/

#include "miniGit.hpp"

int main()
{
    cout << "Welcome to miniGit" << endl;

    cout << "Enter your name: " << endl;
    

    miniGit git;
    string input = "";
    int numIn = 0;

    do
    {
        git.displayOptions();
        getline(cin, input);

        if (input[0] > 39 || input[0] < 30 || input.length() > 1)
        {
            cerr << "ERROR: Invalid input -- enter a single digit number please" << endl;
        }
        else
        {
            numIn = stoi(input);

            switch (numIn)
            {
                case 1: // add a file
                    git.addFile("");
                    break;
                case 2: // remove a file
                    git.removeFile("");
                    break;
                case 3: // commit changes
                    git.commit();
                    break;
                case 4: // checkout a commit
                    git.checkout();
                    break;
                case 5: // quit the program
                    cout << "Thank you for using miniGit!" << endl;
                    break;
                default: cerr << "ERROR: Invalid input -- unavailable option choice" << endl;
                    break;
            }
        }

    } while (numIn != 4);
    
    return 0;
}