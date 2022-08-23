#ifndef UTILS_H_
#define UTILS_H_

#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

bool readFromFile(string fileToRead, string &usbData);
template<typename T> bool writeToFile(std::string fileToWrite, T &usbData);
string &rtrim(string &s);

#endif
