// CSCI 2270 - Project: miniGit
// Author: Kevin Barone 
// Date: 14th April 2021

/* Description:
*/

#include "miniGit.hpp"

int main()
{
    miniGit git;
    int numIn = 0;
    string input = "";
    cout << "Welcome to miniGit" << endl;

    // ask user for their name
    cout << "Enter your name: " << endl;
    getline(cin, input);

    git.setName(input);

    // ask user for their email
    cout << "Enter your email: " << endl;
    getline(cin, input);

    git.setEmail(input);

    // loop to display options while the user does not quit
    do
    {
        git.displayOptions();
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