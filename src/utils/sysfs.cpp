#include "sysfs.h"

SysFS::SysFS(string path) : devpath(path) {
}

string SysFS::getValue(string name) {
    if (mapSysValue.find(name) != mapSysValue.end()) {
        string retVal = mapSysValue[name]->to_string();
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
        if (mapSysValue[file]->load(sysfsValue[file])) {
            mapSysValue[file]->setHit();
        }
    }
}

Json::Value SysFS::toJson() {
    Json::Value root;
    root["path"] = devpath;
    for (const auto& [key, value] : mapSysValue) {
        if (mapSysValue[key]->getHit()) {
            root[key] = getValue(key);
        }
    }
    return root;
}

Json::Value SysFS::sysFStoJson() {
    Json::Value root;
    for (const auto& [key, value] : sysfsValue) {
        root[key] = value;
    }
    return root;
}
    
bool SysFS::set(string name, string value) {
    return set(name, value, devpath);
}

bool SysFS::set(string name, string value, string path) {
    if (mapSysValue.find(name) != mapSysValue.end()) {
        if (!mapSysValue[name]->writable()) {
            cout << "attribute(" << name << ") is read-only" << endl;
            return false;
        }
        if (!mapSysValue[name]->load(value)) {
            cout << "invalid value(" << value << ") for attribute(" << name << ")" << endl;
            return false;
        }
        writeToFile(path+name, value);
        getSysFS(name, path);
        if (mapSysValue[name]->to_string() == value) {
            return true;
        }
        return true;
    }
    else {
        cout << name << " is not attribute in path(" << devpath << ")" << endl;
    }
    return false;
}

string SysFS::get(string name) {
    return "";
}
