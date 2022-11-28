#ifndef UTILS_H_
#define UTILS_H_

#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include <json/json.h>

using namespace std;

Json::Value readJson(string filePath);
bool readFromFile(string fileToRead, string &usbData);
template<typename T> bool writeToFile(std::string fileToWrite, T &usbData);
string &rtrim(string &s);
string getRealPath(string path);

#endif
