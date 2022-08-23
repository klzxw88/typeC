#include "utils.h"

template bool writeToFile(string fileToWrite, string &usbData);
template bool writeToFile(string fileToWrite, int &usbData);

string &rtrim(string &s) {
        s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
        return s;
}

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
bool writeToFile(string fileToWrite, T &usbData)
{
    ofstream countFile (fileToWrite, ofstream::out);
    if(countFile.is_open()){
        countFile << usbData;
        countFile.close();
        return true;
    } else {
        return false;
    }
}
