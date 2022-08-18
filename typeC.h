#include <iostream>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <map>
using namespace std;
namespace fs = std::filesystem;

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

enum DataRoles {
	DATA_ROLE_NONE = 0,
	DATA_ROLE_DEVICE,
	DATA_ROLE_HOST,
	DATA_ROLE_DUAL_HOST,
	DATA_ROLE_DUAL_DEVICE,
};

enum PowerRoles {
	POWER_ROLE_NONE = 0,
	POWER_ROLE_SOURCE,
	POWER_ROLE_SINK,
	POWER_ROLE_DUAL_SOURCE,
	POWER_ROLE_DUAL_SINK,
};

enum PortTypes {
	PORT_TYPE_NONE = 0,
	PORT_TYPE_SOURCE,
	PORT_TYPE_SINK,
	PORT_TYPE_DUAL_SOURCE,
	PORT_TYPE_DUAL_SINK,
};

class Port {
	string devpath;
	DataRoles data_role;
	PowerRoles power_role;
	PortTypes port_type;
	map<string, string> sysfsValue;
	map<string, DataRoles> mapDataRole;
	map<string, PowerRoles> mapPowerRole;
	map<string, PortTypes> mapPortType;
	map<string, function<void()>> mapSysFSFunc;

	void getSysFS();
	void getSysFS(string file);
	void getSysFSDataRole();
	void getSysFSPowerRole();
	void getSysFSPortType();
	string getDataRole(DataRoles role);
	string getPowerRole(PowerRoles role);
	string getPortType(PortTypes type);
public:
	Port(string path);
	string getDataRole();
	string getPowerRole();
	string getPortType();
	void setDataRole(DataRoles role);
	void setPowerRole(PowerRoles role);
	void setPortType(PortTypes type);
};
