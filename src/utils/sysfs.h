#ifndef SYSFS_H_
#define SYSFS_H_

#include <filesystem>
#include <list>
#include <memory>
#include <json/json.h>
#include "utils.h"
#include "sysfsvalue.h"

using namespace placeholders;

namespace fs = std::filesystem;

class SysFS {
private:
	map<string, string> sysfsValue;

protected:
	list<string> subDirs;
	map<string, shared_ptr<ISysFSValue>> mapSysValue;
	string devpath;

	void getSysFSAll(string path);
	void getSysFS(string file);
	void getSysFS(string file, string path);
public:
	SysFS(string path);
	~SysFS() = default;

	void getSysFSAll();
	string getDevPath() { return devpath; };
	string getValue(string name);
	void addMap(ISysFSValue* iSysFSValue) { mapSysValue.insert(make_pair(iSysFSValue->getName(), shared_ptr<ISysFSValue>(iSysFSValue))); };
	Json::Value toJson();
	Json::Value sysFStoJson();
	template<typename T> bool setValue(string name, T value);
	template<typename T> bool setValue(string name, T value, string path);
};

template<typename T>
bool SysFS::setValue(string name, T value) {
	return setValue(name, value, devpath);
}

template<typename T>
bool SysFS::setValue(string name, T value, string path) {
	if (!mapSysValue[name]->writable()) {
		return false;
	}

	string val;
	if constexpr (is_same_v<T, string>) {
		val = value;
	}
	else if constexpr (is_same_v<T, const char*>) {
		val = value;
	}
	else if constexpr (is_same_v<T, bitset<32>>) {
		return "";
	}
	else if constexpr (is_same_v<T, bool>) {
		if (value) {
			val = "yes\n";
		}
		else {
			val = "no\n";
		}
	}
	else if (mapSysValue[name]->empty()) {
		val = to_string(value);
	}
	else {
		val = mapSysValue[name]->getString(value);
	}

	writeToFile(path+name, val);
	getSysFS(name, path);
	if (mapSysValue[name]->getString() == val) {
		return true;
	}
	return false;
}

#endif
