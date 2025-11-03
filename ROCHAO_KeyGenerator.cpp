// ROCHAO_KeyGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "KeyGenerator.h"

using namespace std;

void setErrorMessage(string message) {
    std::cout << "Error: " + message + "\n";
    exit(0);
}

void help() {
    std::cout << "Help:\n";
    std::cout << "-SectionCount [integer]: Sets the amount of randomly generated sections your key(s) has(have). Default is 3.\n";
    std::cout << "-SectionLength [integer]: Sets the length of each of the randomly generated sections your key(s) has(have). Default is 10.\n";
    std::cout << "-KeyAmount [integer]: Sets the amount of keys generated. Default is 1.\n";
    exit(0);

}

int main(int argc, char* argv[])
{
    size_t sectionLength = 10;
    int sectionCount = 3;
    int keyCount = 1;

    for (int i = 0; i < argc; i++)
    {
        if (_stricmp(argv[i], "-h") == 0 || _stricmp(argv[i], "-help") == 0)
            help();

        if (_stricmp(argv[i], "-SectionCount") == 0)
            sectionCount = atoi(argv[i + 1]);

        if (_stricmp(argv[i], "-SectionLength") == 0)
            sectionLength = atoi(argv[i + 1]);

        if (_stricmp(argv[i], "-KeyAmount") == 0)
            keyCount = atoi(argv[i + 1]);

    }

    KeyGenerator* keyGen = new KeyGenerator(sectionLength, sectionCount);

    std::cout << keyGen->GenerateKeyStrings(keyCount);

    delete keyGen;
}