// ROCHAO_KeyGenerator.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "KeyGenerator.h"
#include <curl/curl.h>

using namespace std;


string postURL = "URL";

const string CSRFCookieName = "XSRF-TOKEN";

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

string getCSRFCookie() {
    CURL* curl = curl_easy_init();
    CURLcode responseCode;
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, postURL.c_str());
        curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
        responseCode = curl_easy_perform(curl);

        if (!responseCode) {
            curl_slist* cookies = NULL;
            curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
            for (curl_slist* each = cookies; each != nullptr; each = each->next) {
               string cookieData = each->data;
               if (cookieData.find(CSRFCookieName) != std::string::npos) {
                   cookieData.erase(remove_if(cookieData.begin(), cookieData.end(), isspace), cookieData.end());
                   size_t posCookieName = cookieData.find(CSRFCookieName) + CSRFCookieName.size();
                   cookieData.erase(0, posCookieName);
                  // std::cout << cookieData + "\n";
                   return cookieData;   
               }
               // std::cout << each->data;
            }
            curl_slist_free_all(cookies);
        }
        curl_easy_cleanup(curl);
    }

    return "";
}
void runKeyCurlRquest(string keyString) {
    CURL* postCurl;
    CURLcode responseCode;
    curl_slist* headers = NULL;
    postCurl = curl_easy_init();

    if (postCurl) {
        //string finalString = postURL + keyString;
        curl_easy_setopt(postCurl, CURLOPT_URL, postURL.c_str()); //Todo: Add CURLCODE check

        string headerString1 = "X-XSRF-TOKEN: " + getCSRFCookie();
        headers = curl_slist_append(headers, headerString1.c_str());

        curl_easy_setopt(postCurl, CURLOPT_HTTPHEADER, headers);

        //string finalString = postBaseFields + keyString;
        curl_easy_setopt(postCurl, CURLOPT_POST, 1L);
        curl_easy_setopt(postCurl, CURLOPT_POSTFIELDS, "key: " + keyString); //Todo: Add CURLCODE check

       responseCode = curl_easy_perform(postCurl);

       curl_slist_free_all(headers);

        if (responseCode != CURLE_OK) {
            string curlStringError = curl_easy_strerror(responseCode);
            setErrorMessage("curl_easy_perform() failed:" + curlStringError);
        }

        curl_easy_cleanup(postCurl);

    }
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
    curl_global_init(CURL_GLOBAL_ALL); //TODO: Add CURLCode check
    
    KeyGenerator* keyGen = new KeyGenerator(sectionLength, sectionCount);
    string generatedKey = keyGen->GenerateKeyString();
   

    std::cout << generatedKey + "\n";
        
    runKeyCurlRquest(generatedKey);

    curl_global_cleanup();
    delete keyGen;
}