#include "utils.h"

template bool writeToFile(std::string fileToWrite, string &usbData);
template bool writeToFile(std::string fileToWrite, int &usbData);

bool readFromFile(string fileToRead, string &usbData) {
    ifstream readFile (fileToRead);
    if (readFile.fail()) {
        return false;
    }
    ostringstream ss;
    ss << readFile.rdbuf();
    usbData = ss.str();
    readFile.close();
    return true;
}

template<typename T>
bool writeToFile(std::string fileToWrite, T &usbData)
{
    std::ofstream countFile (fileToWrite, std::ofstream::out);
    if(countFile.is_open()){
        countFile << usbData;
        countFile.close();
        return true;
    } else {
        return false;
    }
}
