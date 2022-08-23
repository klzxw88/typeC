#include "sysfs.h"

SysFS::SysFS(string path) : devpath(path) {
}

inline string SysFS::getString(string name, int val) {
	auto map = get<2>(mapSysFsEnum[name]);
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
	string retVal;
	if (mapSysFsEnum.find(name) != mapSysFsEnum.end()) {
		retVal = getString(name, get<0>(mapSysFsEnum[name])());
	}
	else if (mapSysFsBitset.find(name) != mapSysFsBitset.end()) {
		retVal = get<0>(mapSysFsBitset[name])().to_string();
	}
	else if (mapSysFsBool.find(name) != mapSysFsBool.end()) {
		retVal = get<0>(mapSysFsBool[name])() ? "yes" : "no";
	}
	else if (mapSysFsInt.find(name) != mapSysFsInt.end()) {
		retVal = to_string(get<0>(mapSysFsInt[name])());
	}
	else {
		retVal = get<0>(mapSysFsString[name])();
	}
	rtrim(retVal);
	return retVal;
}

bool SysFS::setValue(string name, int value) {
	return setValue(name, value, devpath);
}

bool SysFS::setValue(string name, int value, string path) {
	if (mapSysFsEnum.find(name) != mapSysFsEnum.end()) {
		if (!get<3>(mapSysFsEnum[name])) {
			return false;
		}
	}
	else if (mapSysFsInt.find(name) != mapSysFsInt.end()) {
		if (!get<2>(mapSysFsInt[name])) {
			return false;
		}
	}
	string val = getString(name, value);
	if (val.empty()) {
		return false;
	}
	writeToFile(path+name, val);
	getSysFS(name, path);
	if (mapSysFsEnum.find(name) != mapSysFsEnum.end()) {
		if (get<0>(mapSysFsEnum[name])() == value) {
			return true;
		}
	}
	else if (mapSysFsInt.find(name) != mapSysFsInt.end()) {
		if (get<0>(mapSysFsInt[name])() == value) {
			return true;
		}
	}
	return false;
}

bool SysFS::setValue(string name, string value) {
	return setValue(name, value, devpath);
}

bool SysFS::setValue(string name, string value, string path) {
	if (!get<2>(mapSysFsString[name])) {
		return false;
	}
	writeToFile(path+name, value);
	getSysFS(name, path);
	if (get<0>(mapSysFsString[name])() == value) {
		return true;
	}
	return false;
}

bool SysFS::setValue(string name, bool value) {
	return setValue(name, value, devpath);
}

bool SysFS::setValue(string name, bool value, string path) {
	if (!get<2>(mapSysFsBool[name])) {
		return false;
	}
	string val = value?"yes":"no";
	writeToFile(path+name, val);
	getSysFS(name, path);
	if (get<0>(mapSysFsBool[name])() == value) {
		return true;
	}
	return false;
}

void SysFS::getSysFSAll() {
	getSysFSAll(devpath);
}

void SysFS::getSysFSAll(string path) {
	for (const auto & file : fs::directory_iterator(path)) {
		string data;
		fs::path p;
		if (fs::is_symlink(file.path())) {
			p = fs::read_symlink(file.path());
		}
		else if(fs::is_directory(file.path())) {
			continue;
		}
		else {
			p = file;
		}
		getSysFS(p.filename(), path);
	}
}

void SysFS::getSysFS(string file) {
	getSysFS(file, devpath);
}

void SysFS::getSysFS(string file, string path) {
	if (sysfsValue.find(file) == sysfsValue.end()) {
		string data;
		if (readFromFile(path+file, data)) {
			sysfsValue[file] = data;
		}
	}

	if (mapSysFsEnum.find(file) != mapSysFsEnum.end()) {
		auto map = get<2>(mapSysFsEnum[file]);
		auto setter = get<1>(mapSysFsEnum[file]);
		if (map.find(sysfsValue[file]) != map.end()) {
			setter(map[sysfsValue[file]]);
		}
		else {
			cout << "not matched" << endl;
		}
	}
	else if (mapSysFsBool.find(file) != mapSysFsBool.end()) {
		auto setter = get<1>(mapSysFsBool[file]);
		bool val;
		if (sysfsValue[file] == "yes\n" || sysfsValue[file] == "true\n" || sysfsValue[file] == "1\n") {
			val = true;
		}
		else {
			val = false;
		}
		setter(val);
	}
	else if (mapSysFsBitset.find(file) != mapSysFsBitset.end()) {
		string sVal = sysfsValue[file];
		if (sVal[0] == '0' && sVal[1] == 'x') {
			sVal = sVal.substr(2, sVal.length()-2);
			stringstream ss;
			unsigned int iVal;
			ss << std::hex << sVal;
			ss >> iVal;
			bitset<32> val(iVal);
			auto setter = get<1>(mapSysFsBitset[file]);
			setter(val);
		}
	}
	else if (mapSysFsInt.find(file) != mapSysFsInt.end()) {
		auto setter = get<1>(mapSysFsInt[file]);
		setter(stoi(sysfsValue[file]));
	}
	else {
		auto setter = get<1>(mapSysFsString[file]);
		setter(sysfsValue[file]);
	}
}
