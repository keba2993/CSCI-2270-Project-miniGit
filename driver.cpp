// CSCI 2270 - Project: miniGit
// Author: Kevin Barone & Brennan Belei
// Date: 14th April 2021

/* Description:
*/

#include "miniGit.hpp"

int main()
{
    miniGit* git;
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
        if (input[0] > 39 || input[0] < 30 || input.length() > 1)
        {
            cerr << "ERROR: Invalid input -- enter a single digit number please" << endl;
        }
        else
        {
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
                case 2: // add a files
                    cout << "Enter the filename: " << endl;
                    getline(cin, input);

                    git->addFile(input);
                    break;
                case 3: // remove a file
                    git->removeFile("");
                    break;
                case 4: // commit changes
                    git->commit();
                    break;
                case 5: // checkout a commit
                    git->checkout();
                    break;
                case 6: // quit the program
                    cout << "Thank you for using miniGit!" << endl;
                    break;
                default:
                    cerr << "ERROR: Invalid input -- unavailable option choice" << endl;
                    break;
            }
        }

    } while (numIn != 4);
    
    return 0;
}