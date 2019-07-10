/*
 * Soubor:      tail.cc
 * Projekt:     IJC:DU2
 * Autor:       David Valecký, VUT-FIT, login: xvalec01
 * Přeloženo:   gcc 5.4.0 ubuntu
 * Datum:       24. 4. 2018
 * Popis:       Vypisování zadaných řádků ze souboru/standartního vstupu v c++
*/

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>

#define MAXLENGTH 1024
#define PRINT_ROWS 10
#define OK 0
#define FILE_ERROR 1
#define INVALID_INPUT 2
#define UNKNOWN_ERROR -1

int main(int argc, char const *argv[])
{
    using namespace std;
    ios::sync_with_stdio(false);
    string argument ("-n");

    if (argc == 1)
    {
        string line;
        queue<string>  rows;
        long int i = 0;
        while (getline(cin, line))
        {   
            i++;
            if(i > PRINT_ROWS)
            {
                rows.pop();
            }
            rows.push(line);
        }
        for(long int k = 0; k < PRINT_ROWS; k++)
        {
            cout << rows.front() << "\n";
            rows.pop();
        }
    }
    else if (argc == 2)
    {
        ifstream file (argv[1],ifstream::in);

        if (!file.is_open()) 
        {
            cerr << "Invalid file opening" << endl;
            exit(FILE_ERROR);
        }

        string line;
        queue<string>  rows;

        int i = 0;
        while (getline(file, line))
        {   
            i++;
            if(i > PRINT_ROWS)
            {
                rows.pop();
            }
            rows.push(line);
        }
        for(long int k = 0; k < PRINT_ROWS; k++)
        {
            cout << rows.front() << "\n";
            rows.pop();
        }
        file.close();
    }
    else if ((argc == 3) && (argument.compare(argv[1]) == 0 ))
    {
        istringstream iss(argv[2]);

        long int print = 0;

        if ((iss >> print).fail() || (print < 0)) 
        {
            cerr << "\'" << argv[2] << "\'" << " is not valid argument" << endl;
            exit(INVALID_INPUT);
        }
        else 
        {
            string line;
            queue<string> rows;
            long int i = 0;
            while (getline(cin, line))
            {   
                i++;
                if(i > print)
                {
                    rows.pop();
                }
                rows.push(line);
            }
            if (i < print)
            {
                for(long int k = 0; k < i; k++)
                {
                    cout << rows.front() << "\n";
                    rows.pop();
                }
            }
            else
            {
                for(long int k = 0; k < print; k++)
                {
                    cout << rows.front() << "\n";
                    rows.pop();
                }
            }
        }
    }
    else if ((argc == 4) && (argument.compare(argv[1]) == 0 ))
    {
        ifstream file (argv[3],ifstream::in);

        if (!file.is_open()) 
        {
            cerr << "Invalid file opening" << endl;
            exit(FILE_ERROR);
        }
        istringstream iss(argv[2]);

        long int print = 0;

        if ((iss >> print).fail() || (print < 0)) 
        {
            cerr << "\'" << argv[2] << "\'" << " is not valid argument" << endl;
            exit(INVALID_INPUT);
        }
        else 
        {
            string line;
            queue<string>  rows;

            long int i = 0;
            while (getline(file, line))
            {   
                i++;
                if(i > print)
                {
                    rows.pop();
                }
                rows.push(line);
            }
            if (i < print)
            {
                for(long int k = 0; k < i; k++)
                {
                    cout << rows.front() << "\n";
                    rows.pop();
                }
            }
            else
            {
                for(long int k = 0; k < print; k++)
                {
                    cout << rows.front() << "\n";
                    rows.pop();
                }
            }
        }
        file.close();
    }
    else if (argc > 4)
    {
        cerr << "Too many arguments" << endl;
        exit(INVALID_INPUT);
    }
    else if ((argument.compare(argv[1]) != 0 ))
    {
        cerr << "Wrong selector" << endl;
        exit(INVALID_INPUT);
    }
    else
    {
        cerr << "Something went wrong" << endl;
        exit(UNKNOWN_ERROR);
    }
    return OK;
}