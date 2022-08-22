#include "typeC.h"

Port::Port(string path):devpath(path), data_role(DATA_ROLE_NONE), power_role(POWER_ROLE_NONE), port_type(PORT_TYPE_NONE) {
	mapDataRole = {
		{"[device]\n", DATA_ROLE_DEVICE},
		{"[host]\n", DATA_ROLE_HOST},
		{"[host] device\n", DATA_ROLE_DUAL_HOST},
		{"host [device]\n", DATA_ROLE_DUAL_DEVICE},
	};
	mapPowerRole = {
		{"[sink]\n", POWER_ROLE_SINK},
		{"[source]\n", POWER_ROLE_SOURCE},
		{"[source] sink\n", POWER_ROLE_DUAL_SINK},
		{"source [sink]\n", POWER_ROLE_DUAL_SOURCE},
	};
	mapPortType = {
		{"[sink]\n", PORT_TYPE_SINK},
		{"[source]\n", PORT_TYPE_SOURCE},
		{"dual [source] sink\n", PORT_TYPE_DUAL_SOURCE},
		{"dual source [sink]\n", PORT_TYPE_DUAL_SINK},
	};
	mapVconnSource = {
		{"yes\n", VCONN_SOURCE_YES},
		{"no\n", VCONN_SOURCE_NO},
	};
	mapPowerOperationMode = {
		{"default\n", POWER_OPERATION_MODE_DEFAULT},
		{"1.5A\n", POWER_OPERATION_MODE_1_5A},
		{"3.0A\n", POWER_OPERATION_MODE_3A},
		{"usb_power_delivery\n", POWER_OPERATION_MODE_PD},
	};

	mapSysFs = {
		{"data_role", {bind(&Port::data_role, this), [&](unsigned int role){data_role=role;}, mapDataRole, true}},
		{"power_role", {bind(&Port::power_role, this), [&](unsigned int role){power_role=role;}, mapPowerRole, true}},
		{"port_type", {bind(&Port::port_type, this), [&](unsigned int type){port_type=type;}, mapPortType, true}},
		{"vconn_source", {bind(&Port::vconn_source, this), [&](unsigned int vconn){vconn_source=vconn;}, mapVconnSource, true}},
		{"power_operation_mode", {bind(&Port::power_operation_mode, this), [&](unsigned int mode){power_operation_mode=mode;}, mapPowerOperationMode, false}},
	};

	getSysFS();
	for (auto p : mapSysFs) {
		getSysFS(p.first);
	}
}

inline string Port::getString(string name, unsigned int val) {
	auto map = get<2>(mapSysFs[name]);
	auto result = find_if(
          map.begin(),
          map.end(),
          [&val](const auto& mo) {return mo.second == val; });
	if (result != map.end()) {
		return result->first;
	}
	return "";
}

string Port::getValue(string name) {
	return getString(name, get<0>(mapSysFs[name])());
}

bool Port::setValue(string name, unsigned int value) {
	if (!get<3>(mapSysFs[name])) {
		cout << "RO" << endl;
		return false;
	}
	string val = getString(name, value);
	if (val.empty()) {
		return false;
	}
	writeToFile(devpath+name, val);
	getSysFS(name);
	if (get<0>(mapSysFs[name])() == value) {
		cout << "set OK" << endl;
		return true;
	}
	return false;
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
	auto map = get<2>(mapSysFs[file]);
	auto setter = get<1>(mapSysFs[file]);
	if (sysfsValue.find(file) != sysfsValue.end() && map.find(sysfsValue[file]) != map.end()) {
		cout << file << ":" << map[sysfsValue[file]] << endl;
		setter(map[sysfsValue[file]]);
	}
	else {
		setter(DATA_ROLE_NONE);
	}
}

int main() {
	Port* p = new Port("./sys/class/typec/port0/");
	//p->setValue("data_role", DATA_ROLE_DEVICE);
	//p->setValue("power_role", POWER_ROLE_SOURCE);
	//p->setValue("port_type", PORT_TYPE_SINK);
	//p->setValue("vconn_source", VCONN_SOURCE_YES);
	//p->setValue("power_operation_mode", POWER_OPERATION_MODE_DEFAULT);
	return 0;
}
