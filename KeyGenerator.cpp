#include <random>
#include "KeyGenerator.h"

using namespace std;

const string allowedString = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const string baseName = "ROCHAO-";

KeyGenerator::KeyGenerator()
	:mSectionLength(NULL),
	mSectionCount(NULL)
{}

KeyGenerator::KeyGenerator(size_t sectionLength, int sectionCount)
	:mSectionLength(sectionLength),
	mSectionCount(sectionCount)
{}

void KeyGenerator::setGenerationValues(size_t sectionLength, int sectionCount) {
	mSectionLength = sectionLength;
	mSectionCount = sectionCount;
}

string KeyGenerator::GenerateKeyString() {
	string randomString;
	random_device rand;
	mt19937 randomStringGen(rand());
	uniform_int_distribution<> dist(0, allowedString.size() - 1);

	for (int i = 0; i < mSectionCount; i++) {
		for (size_t i = 0; i < mSectionLength; i++) {
			randomString += allowedString[dist(randomStringGen)];
		}

		if (i != (mSectionCount - 1))
			randomString += "-";
	}

	return baseName + randomString + "\n";
}

string KeyGenerator::GenerateKeyStrings(int count)
{
	string finalString = "";

	for (int i = 0; i < count; i++) {
		finalString += GenerateKeyString();
	}
	return finalString;
}