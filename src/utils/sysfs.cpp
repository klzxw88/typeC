#include "sysfs.h"

SysFS::SysFS(string path) : devpath(path) {
}

string SysFS::getValue(string name) {
	if (mapSysValue.find(name) != mapSysValue.end()) {
		string retVal = mapSysValue[name]->getString();
		return rtrim(retVal);
	}
	else {
		return "";
	}
}

void SysFS::getSysFSAll() {
	getSysFSAll(devpath);
}

void SysFS::getSysFSAll(string path) {
	if (!fs::exists(path)) {
		return;
	}
	if (!fs::is_directory(path)) {
		return;
	}
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
	if (!fs::exists(path+file)) {
		return;
	}
	string data;
	if (readFromFile(path+file, data)) {
		sysfsValue[file] = data;
	}

	if (mapSysValue.find(file) != mapSysValue.end()) {
		mapSysValue[file]->setString(sysfsValue[file]);
		mapSysValue[file]->setHit();
	}
}

Json::Value SysFS::toJson() {
	Json::Value root;
	for (const auto& [key, value] : mapSysValue) {
		if (mapSysValue[key]->getHit()) {
			root[key] = getValue(key);
		}
	}
	return root;
}
