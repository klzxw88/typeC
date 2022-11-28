#ifndef SYSFS_H_
#define SYSFS_H_

#include <filesystem>
#include <list>
#include <memory>
#include "utils.h"
#include "sysfsvalue.h"

using namespace placeholders;

namespace fs = std::filesystem;

class SysFS {
private:
    string devtype;
    map<string, string> sysfsValue;

protected:
    list<string> subDirs;
    map<string, shared_ptr<ISysFSValue>> mapSysValue;
    string devpath;

    void getSysFSAll(string path);
    void getSysFS(string file);
    void getSysFS(string file, string path);
public:
    SysFS(string type, string path);
    ~SysFS() = default;

    void getSysFSAll();
    string getDevPath() { return devpath; };
    string getValue(string name);
    void addMap(ISysFSValue* iSysFSValue) { mapSysValue.insert(make_pair(iSysFSValue->getName(), shared_ptr<ISysFSValue>(iSysFSValue))); };
    Json::Value toJson();
    Json::Value sysFStoJson();
    bool set(string name, string value);
    bool set(string name, string value, string path);
    template<typename T> bool setValue(string name, T value);
    template<typename T> bool setValue(string name, T value, string path);
    string get(string name);
};

template<typename T>
bool SysFS::setValue(string name, T value) {
    return setValue(name, value, devpath);
}

template<typename T>
bool SysFS::setValue(string name, T value, string path) {
    if (!mapSysValue[name]->writable()) {
        cout << "attribute(" << name << ") is read-only" << endl;
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
        // bitset<32> to hex
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
        val = mapSysValue[name]->to_string(value);
        if (val.empty()) {
            return false;
        }
    }

    writeToFile(path+name, val);
    getSysFS(name, path);
    if (mapSysValue[name]->to_string() == val) {
        return true;
    }
    return false;
}

#endif
