#pragma once

#include <iostream>
#include <INIReader.h>

using namespace std;

extern const string allowedString;

class KeyGenerator
{
private:
	//Key Generation Members
	size_t mSectionLength;
	int mSectionCount;
	string mKeyBaseName;

	//HTTP Post Members
	string mPostURL;
	string mPostFields;
	//CSRF Members

	bool mIsCSRFEnabled;
	string mCSRFCookieName;
	string mCSRFHeaderName;
public:
	KeyGenerator(string INIPath);


	string GenerateKeyString();

	void GenerateKeyStrings(int count);

	string getCSRFCookie();

	void runKeyCurlRquest(string keyString);
};