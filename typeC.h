#include <algorithm>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>

using namespace std;
using namespace placeholders;

namespace fs = std::filesystem;

const unsigned int DATA_ROLE_NONE = 0;
const unsigned int DATA_ROLE_DEVICE = 1;
const unsigned int DATA_ROLE_HOST = 2;
const unsigned int DATA_ROLE_DUAL_HOST = 3;
const unsigned int DATA_ROLE_DUAL_DEVICE = 4;

const unsigned int POWER_ROLE_NONE = 10;
const unsigned int POWER_ROLE_SOURCE = 11;
const unsigned int POWER_ROLE_SINK = 12;
const unsigned int POWER_ROLE_DUAL_SOURCE = 13;
const unsigned int POWER_ROLE_DUAL_SINK = 14;

const unsigned int PORT_TYPE_NONE = 20;
const unsigned int PORT_TYPE_SOURCE = 21;
const unsigned int PORT_TYPE_SINK = 22;
const unsigned int PORT_TYPE_DUAL_SOURCE = 23;
const unsigned int PORT_TYPE_DUAL_SINK = 24;

const unsigned int VCONN_SOURCE_YES = 30;
const unsigned int VCONN_SOURCE_NO = 31;

const unsigned int POWER_OPERATION_MODE_DEFAULT = 40;
const unsigned int POWER_OPERATION_MODE_1_5A = 41;
const unsigned int POWER_OPERATION_MODE_3A = 42;
const unsigned int POWER_OPERATION_MODE_PD = 43;

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

class Partner {
};

class Cable {

};

class AltMode {
	
};

class Port {
	using getter = function<unsigned int()>;
	using setter = function<void(unsigned int)>;
	map<string, tuple<getter, setter, map<string, unsigned int>, bool>> mapSysFs;
	string devpath;
	unsigned int data_role;
	unsigned int power_role;
	unsigned int port_type;
	unsigned int vconn_source;
	unsigned int power_operation_mode;
	map<string, string> sysfsValue;
	map<string, unsigned int> mapDataRole;
	map<string, unsigned int> mapPowerRole;
	map<string, unsigned int> mapPortType;
	map<string, unsigned int> mapVconnSource;
	map<string, unsigned int> mapPowerOperationMode;

	void getSysFS();
	void getSysFS(string file);

	string getString(string name, unsigned int val);
public:
	Port(string path);

	string getValue(string name);
	bool setValue(string name, unsigned int value);
};
