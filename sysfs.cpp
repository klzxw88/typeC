#include "sysfs.h"

SysFS::SysFS(string path) : devpath(path) {
}

inline string SysFS::getString(string name, int val) {
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

string SysFS::getValue(string name) {
	return getString(name, get<0>(mapSysFs[name])());
}

bool SysFS::setValue(string name, int value) {
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

void SysFS::getSysFS() {
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

void SysFS::getSysFS(string file) {
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
}
