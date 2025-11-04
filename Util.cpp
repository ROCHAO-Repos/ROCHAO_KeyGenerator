#include "Util.h"
#include <iostream>;

using namespace std;

void setErrorMessage(string message) {
    std::cout << "Error: " + message + "\n";
    exit(0);
}

void help() {
    std::cout << "Help:\n";
    std::cout << "-KeyAmount [integer]: Sets the amount of keys generated. Default is 1.\n";
    exit(0);

}