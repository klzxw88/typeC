#include "utils.h"

#include <filesystem>

namespace fs = std::filesystem;

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
bool writeToFile(string fileToWrite, T &usbData) {
    ofstream countFile (fileToWrite, ofstream::out);
    if(countFile.is_open()){
        countFile << usbData;
        countFile.close();
        return true;
    } else {
        return false;
    }
}
	
string getRealPath(string path) {
	fs::path p(path);
    if (!fs::exists(p)) {
		std::cout << "no exist" << std::endl;
		return "";
    }
	fs::path devpath;
	if (fs::is_symlink(p)) {
		try {
			devpath = fs::canonical(p.parent_path()/fs::read_symlink(p));
		} catch (std::exception &e) {
			cout << "exception: "<< e.what() << endl;
			return "";
		}
	}
	else {
		devpath = p;
	}
	return devpath;
}
