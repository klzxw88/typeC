#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

using getter = function<int()>;
using setter = function<void(int)>;

bool readFromFile(string fileToRead, string &usbData);
template<typename T> bool writeToFile(std::string fileToWrite, T &usbData);
