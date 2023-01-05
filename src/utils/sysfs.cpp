#include "sysfs.h"
#include "configuration.h"

SysFS::SysFS(string type, string path) : devtype(type), devpath(path) {
    Json::Value sysFSValues = Configuration::instance()->getSysValues(type);
    if (!sysFSValues.isNull()) {
        for (string name : sysFSValues.getMemberNames()) {
            Json::Value attribute = Configuration::instance()->getAttribute(sysFSValues[name].asString());
            if (!attribute.isMember("type")) {
                cout << "attribute[\"" << name << "\"] is not has \"type\" member" << endl;
                break;
            }
            if (!attribute.isMember("writable")) {
                cout << "attribute[\"" << name << "\"] is not has \"writable\" member" << endl;
                break;
            }
            string valueType = attribute["type"].asString();
            if (valueType == "string") {
                SysFSValue<string>* value = new SysFSValue<string>(path, name, attribute["writable"].asBool());
                mapSysValue.insert(make_pair(name, shared_ptr<ISysFSValue>(value)));
            }
            else if (valueType == "bitset<32>") {
                SysFSValue<bitset<32>>* value = new SysFSValue<bitset<32>>(path, name, attribute["writable"].asBool());
                mapSysValue.insert(make_pair(name, shared_ptr<ISysFSValue>(value)));
            }
            else if (valueType == "bool") {
                SysFSValue<bool>* value = new SysFSValue<bool>(path, name, attribute["writable"].asBool());
                mapSysValue.insert(make_pair(name, shared_ptr<ISysFSValue>(value)));
            }
            else if (valueType == "int") {
                SysFSValue<int>* value = new SysFSValue<int>(path, name, attribute["writable"].asBool());
                mapSysValue.insert(make_pair(name, shared_ptr<ISysFSValue>(value)));
            }
            else if (valueType == "enum") {
                if (!attribute.isMember("values")) {
                    cout << "attribute[\"" << name << "\"] is not has \"values\" member" << endl;
                    break;
                }
                SysFSValue<int>* value = new SysFSValue<int>(path, name, attribute["writable"].asBool());
                for (string item : attribute["values"].getMemberNames()) {
                    value->add(item, attribute["values"][item].asInt());
                }
                mapSysValue.insert(make_pair(name, shared_ptr<ISysFSValue>(value)));
            }
        }
        getSysFSAll();
    }
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
            root[key] = value->toJson()[key];
        }
    }
    return root;
}

Json::Value SysFS::sysFStoJson() {
    Json::Value root;
    root["path"] = devpath;
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
        if ((fs::status(path+name).permissions() & fs::perms::owner_write) == fs::perms::none) {
            cout << path+name << " didn't have owner write permission" << endl;
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
        return false;
    }
    else {
        cout << name << " is not attribute in path(" << devpath << ")" << endl;
    }
    return false;
}

string SysFS::get(string name) {
    return "";
}
