// CSCI 2270 - Project: miniGit
// Author: Kevin Barone & Brennan Belei
// Date: 14th April 2021

/* Description:
 * This is the driver file for the miniGit system. It contains the declaration of a miniGit object pointer
 * and other necessary variables that are used to allow the user to choose and perform the different
 * git actions. A simple do-while is used to keep the program running until the user chooses
 * to quit it and reset the miniGit system.
*/

#include "miniGit.hpp"

int main()
{
    miniGit* git = new miniGit();;
    int numIn = 0;
    string input = "";
    string input2 = "";
    cout << "Welcome to miniGit" << endl;

    // loop to display options while the user does not quit
    do
    {
        git->displayOptions();
        getline(cin, input);

        // checking that the input is a single number
        if (input.length() > 1 || !isdigit(input[0]))
        {
            cerr << endl << "ERROR: Invalid input -- enter a single digit number please" << endl;
        }
        else
        {
            cout << endl;
            numIn = stoi(input);

            // switch statement to perform proper function based on user's choice
            switch (numIn)
            {
                case 1: // initialize miniGit
                    // ask user for their name
                    cout << "Enter your name: " << endl;
                    getline(cin, input);

                    // ask user for their email
                    cout << "Enter your email: " << endl;
                    getline(cin, input2);

                    git->init(input, input2);        // initializing git system
                    break;
                case 2: // add a file
                    cout << "Enter the filename to be added: " << endl;
                    getline(cin, input);

                    git->addFile(input);
                    git->printGit();
                    break;
                case 3: // remove a file
                    cout << "Enter the filename to be removed: " << endl;
                    getline(cin, input);

                    git->removeFile(input);
                    break;
                case 4: // commit changes
                    git->commit();
                    break;
                case 5: // checkout a commit
                    git->checkout(3);
                    break;
                case 6: // quit the program
                    cout << "Thank you for using miniGit!" << endl;
                    break;
                default:
                    cerr << "ERROR: Invalid input -- unavailable option choice" << endl;
                    break;
            }
        }

    } while (numIn != 6);

    delete git;
    git = nullptr;
    
    return 0;
}