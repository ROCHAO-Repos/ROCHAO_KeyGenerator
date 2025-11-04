#include <random>
#include "KeyGenerator.h"
#include <curl/curl.h>
#include "util.h"

using namespace std;

const string allowedString = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

KeyGenerator::KeyGenerator(string INIReaderPath) {
	INIReader reader(INIReaderPath);

	if (reader.ParseError() < 0) {
		setErrorMessage("Config.ini not found!");
		exit(0);
	}
	mSectionLength = reader.GetInteger("keySettings", "sectionLength", 10);
	mSectionCount = reader.GetInteger("keySettings", "sectionCount", 3);
	mKeyBaseName = reader.GetString("keySettings", "baseName", "YOURREVIVAL-");
	mPostURL = reader.GetString("HTTPPostData", "postURL", "");
	mPostFields = reader.GetString("HTTPPostData", "postFields", "");
	mIsCSRFEnabled = reader.GetBoolean("CSRFData", "CSRFEnabled", false);
	mCSRFCookieName = reader.GetString("CSRFData", "CSRFCookieName", "");
	mCSRFHeaderName = reader.GetString("CSRFData", "CSRFHeaderName", "");
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

	return mKeyBaseName + randomString;
}

void KeyGenerator::GenerateKeyStrings(int count)
{

	for (int i = 0; i < count; i++) {
		string curKey = GenerateKeyString();

		runKeyCurlRquest(curKey);

		std::cout << curKey + "\n";

	}
}


string KeyGenerator::getCSRFCookie() {
	CURL* curl = curl_easy_init();
	CURLcode responseCode;
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, mPostURL.c_str());
		curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
		responseCode = curl_easy_perform(curl);

		if (!responseCode) {
			curl_slist* cookies = NULL;
			curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
			for (curl_slist* each = cookies; each != nullptr; each = each->next) {
				string cookieData = each->data;
				if (cookieData.find(mCSRFCookieName) != std::string::npos) {
					cookieData.erase(remove_if(cookieData.begin(), cookieData.end(), isspace), cookieData.end());
					size_t posCookieName = cookieData.find(mCSRFCookieName) + mCSRFCookieName.size();
					cookieData.erase(0, posCookieName);
					return cookieData;
				}
			}
			curl_slist_free_all(cookies);
		}
		curl_easy_cleanup(curl);
	}

	return "";
}

void KeyGenerator::runKeyCurlRquest(string keyString) {
	CURL* postCurl;
	CURLcode responseCode;
	curl_slist* headers = NULL;
	postCurl = curl_easy_init();

	if (postCurl) {
		curl_easy_setopt(postCurl, CURLOPT_URL, mPostURL.c_str()); //Todo: Add CURLCODE check

		if (mIsCSRFEnabled) {
			string headerString1 = mCSRFHeaderName + ": " + getCSRFCookie();
			std::cout << headerString1;
			headers = curl_slist_append(headers, headerString1.c_str());
			curl_easy_setopt(postCurl, CURLOPT_HTTPHEADER, headers);
		}

		string finalString = mPostFields + keyString;
		curl_easy_setopt(postCurl, CURLOPT_POSTFIELDSIZE, finalString.length());
		curl_easy_setopt(postCurl, CURLOPT_POSTFIELDS, finalString.c_str()); //Todo: Add CURLCODE check
		curl_easy_setopt(postCurl, CURLOPT_POST, 1L);

		responseCode = curl_easy_perform(postCurl);

		//if (mIsCSRFEnb)
		curl_slist_free_all(headers);

		if (responseCode != CURLE_OK) {
			string curlStringError = curl_easy_strerror(responseCode);
			setErrorMessage("curl_easy_perform() failed:" + curlStringError);
		}

		curl_easy_cleanup(postCurl);

	}
}