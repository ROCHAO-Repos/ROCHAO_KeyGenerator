#pragma once

#include <iostream>

using namespace std;

extern const string allowedString;
extern const string baseName;

class KeyGenerator
{
private:
	size_t mSectionLength;
	int mSectionCount;
public:
	KeyGenerator();
	KeyGenerator(size_t sectionLength, int sectionCount);

	void setGenerationValues(size_t sectionLength, int sectionCount);
	string GenerateKeyString();

	string GenerateKeyStrings(int count);

};