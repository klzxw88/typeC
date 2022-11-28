#include "device_handler.h"

bool DeviceHandler::processUdevEvent(UdevEvent* pUE) {
    if (pUE->getDevAttribute(DEVTYPE) == name) {
        string devpath = pUE->getDevAttribute(DEVPATH);
        std::lock_guard<std::recursive_mutex> lock(mtx);
        auto device = find_if(devices.begin(), devices.end(), [&devpath](auto d) { return (d->getDevPath() == devpath); });
        if (pUE->getDevAttribute(ACTION) == DEVICE_REMOVE) {
            if (devices.end() != device) {
                devices.remove(*device);
                return true;
            }
            return false;
        }
        else if (pUE->getDevAttribute(ACTION) == DEVICE_ADD) {
            if (devices.end() == device) {
                devices.push_back(make_shared<SysFS>(name, devpath));
            }
            else {
                (*device)->getSysFSAll();
            }
            return true;
        }
        else if (pUE->getDevAttribute(ACTION) == DEVICE_CHANGE) {
            if (devices.end() != device) {
                (*device)->getSysFSAll();
                return true;
            }
            return false;
        }
    }
    return false;
}

void DeviceHandler::get(string devpath) {
    std::lock_guard<std::recursive_mutex> lock(mtx);
    auto device = find_if(devices.begin(), devices.end(), [&devpath](auto d) { return (d->getDevPath() == devpath); });
    if (devices.end() == device) {
        devices.push_back(make_shared<SysFS>(name, devpath));
    }
    else {
        (*device)->getSysFSAll();
    }
}

Json::Value DeviceHandler::getList() {
    Json::Value lists;
    for (auto d : devices) {
        lists.append(d->toJson());
    }
    return lists;
}

bool DeviceHandler::set(string devpath, string attr, string value) {
    auto device = find_if(devices.begin(), devices.end(), [&devpath](auto d) { return (d->getDevPath() == devpath); });
    if (devices.end() == device) {
        cout << "device(" << devpath << ") not found" << endl;
        return false;
    }
    else {
        return (*device)->set(attr, value);
    }
}
