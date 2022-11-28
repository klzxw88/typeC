#ifndef DEVICEHANDLER_H_
#define DEVICEHANDLER_H_

#include <mutex>
#include <list>
#include <string>
#include "udev_event.h"
#include "sysfs.h"

using namespace std;
using namespace DevAttributes;

class DeviceHandler {
private:
    string name;
    std::list<shared_ptr<SysFS>> devices;
    std::recursive_mutex mtx;
public:
    DeviceHandler(string n):name(n) {};
    ~DeviceHandler() {
        devices.clear();
    };
    bool processUdevEvent(UdevEvent* pUE);
    void get(string path);
    Json::Value getList();
    bool set(string devpath, string attr, string value);
    void clearList() { devices.clear(); };
};
#endif
