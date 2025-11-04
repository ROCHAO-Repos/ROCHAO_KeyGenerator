// ROCHAO_KeyGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <curl/curl.h>
#include "Util.h"
#include "KeyGenerator.h"

using namespace std;

int main(int argc, char* argv[])
{
    int keyCount = 1;

    for (int i = 0; i < argc; i++)
    {
        if (_stricmp(argv[i], "-h") == 0 || _stricmp(argv[i], "-help") == 0)
            help();

        if (_stricmp(argv[i], "-KeyAmount") == 0)
            keyCount = atoi(argv[i + 1]);

    }
    curl_global_init(CURL_GLOBAL_ALL); //TODO: Add CURLCode check
    
    KeyGenerator* keyGen = new KeyGenerator("./config.ini");
    keyGen->GenerateKeyStrings(keyCount);
 
    curl_global_cleanup();
    delete keyGen;

    while (true)
    {

    }
}