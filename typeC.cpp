#include "typeC.h"

Port::Port(string path):devpath(path), data_role(DataRoles::DATA_ROLE_NONE), power_role(PowerRoles::POWER_ROLE_NONE), port_type(PORT_TYPE_NONE) {
	mapDataRole = {
		{"[device]\n", DataRoles::DATA_ROLE_DEVICE},
		{"[host]\n", DataRoles::DATA_ROLE_HOST},
		{"[host] device\n", DataRoles::DATA_ROLE_DUAL_HOST},
		{"host [device]\n", DataRoles::DATA_ROLE_DUAL_DEVICE},
	};
	mapPowerRole = {
		{"[sink]\n", PowerRoles::POWER_ROLE_SINK},
		{"[source]\n", PowerRoles::POWER_ROLE_SOURCE},
		{"[source] sink\n", PowerRoles::POWER_ROLE_DUAL_SINK},
		{"source [sink]\n", PowerRoles::POWER_ROLE_DUAL_SOURCE},
	};
	mapPortType = {
		{"[sink]\n", PortTypes::PORT_TYPE_SINK},
		{"[source]\n", PortTypes::PORT_TYPE_SOURCE},
		{"dual [source] sink\n", PortTypes::PORT_TYPE_DUAL_SOURCE},
		{"dual source [sink]\n", PortTypes::PORT_TYPE_DUAL_SINK},
	};
	mapSysFSFunc = {
		{"data_role", bind(&Port::getSysFSDataRole, this)},
		{"power_role", bind(&Port::getSysFSPowerRole, this)},
		{"port_type", bind(&Port::getSysFSPortType, this)},
	};

	getSysFS();
	for (auto p : mapSysFSFunc) {
		p.second();
	}
}

string Port::getDataRole() {
	return getDataRole(data_role);
}

string Port::getDataRole(DataRoles role) {
	auto result = find_if(
          mapDataRole.begin(),
          mapDataRole.end(),
          [role](const auto& mo) {return mo.second == role; });
	if (result != mapDataRole.end()) {
		return result->first;
	}
	return "";
}

void Port::setDataRole(DataRoles role) {
	string dataRole = getDataRole(role);
	if (dataRole.empty()) {
		return;
	}
	writeToFile(devpath+"data_role", dataRole); 
	getSysFS("data_role");
}

string Port::getPowerRole() {
	return getPowerRole(power_role);
}

string Port::getPowerRole(PowerRoles role) {
	auto result = find_if(
          mapPowerRole.begin(),
          mapPowerRole.end(),
          [role](const auto& mo) {return mo.second == role; });
	if (result != mapPowerRole.end()) {
		return result->first;
	}
	return "";
}

void Port::setPowerRole(PowerRoles role) {
	string powerRole = getPowerRole(role);
	if (powerRole.empty()) {
		return;
	}
	writeToFile(devpath+"power_role", powerRole); 
	getSysFS("power_role");
}

string Port::getPortType() {
	return getPortType(port_type);
}

string Port::getPortType(PortTypes type) {
	auto result = find_if(
          mapPortType.begin(),
          mapPortType.end(),
          [type](const auto& mo) {return mo.second == type; });
	if (result != mapPortType.end()) {
		return result->first;
	}
	return "";
}

void Port::setPortType(PortTypes type) {
	string portType = getPortType(type);
	if (portType.empty()) {
		return;
	}
	writeToFile(devpath+"port_type", portType); 
	getSysFS("port_type");
}

void Port::getSysFS() {
	for (const auto & file : fs::directory_iterator(devpath)) {
		string data;
		fs::path p;
		if (fs::is_symlink(file.path())) {
			p = fs::read_symlink(file.path());
		}
		else {
			p = file;
		}
		if (readFromFile(p.string(), data)) {
			sysfsValue[p.filename()] = data;
		}
	}
}

void Port::getSysFS(string file) {
	string data;
	if (readFromFile(devpath+file, data)) {
		sysfsValue[file] = data;
	}
	mapSysFSFunc[file]();
}

void Port::getSysFSDataRole() {
	if (sysfsValue.find("data_role") != sysfsValue.end() && mapDataRole.find(sysfsValue["data_role"]) != mapDataRole.end()){
		data_role = mapDataRole[sysfsValue["data_role"]];
	}
	else {
		data_role = DATA_ROLE_NONE;
	}
}

void Port::getSysFSPowerRole() {
	if (sysfsValue.find("power_role") != sysfsValue.end() && mapPowerRole.find(sysfsValue["power_role"]) != mapPowerRole.end()){
		power_role = mapPowerRole[sysfsValue["power_role"]];
	}
	else {
		power_role = POWER_ROLE_NONE;
	}
}

void Port::getSysFSPortType() {
	if (sysfsValue.find("port_type") != sysfsValue.end() && mapPortType.find(sysfsValue["port_type"]) != mapPortType.end()){
		port_type = mapPortType[sysfsValue["port_type"]];
	}
	else {
		port_type = PORT_TYPE_NONE;
	}
	cout << "port_type:" << port_type << endl;
}

int main() {
	Port* p = new Port("./sys/class/typec/port0/");
	//p->setDataRole(DATA_ROLE_DEVICE);
	//p->setPowerRole(POWER_ROLE_SOURCE);
	//p->setPortType(PORT_TYPE_SINK);
	return 0;
}
