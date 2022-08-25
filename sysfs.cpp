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
	string data;
	if (readFromFile(path+file, data)) {
		sysfsValue[file] = data;
	}

	if (mapSysValue.find(file) != mapSysValue.end()) {
		mapSysValue[file]->set(sysfsValue[file]);
	}
}
