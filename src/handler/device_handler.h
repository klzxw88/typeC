#ifndef DEVICEHANDLER_H_
#define DEVICEHANDLER_H_

#include <mutex>
#include <list>
#include <string>
#include "udev_event.h"
#include "sysfs.h"

using namespace std;
using namespace DevAttributes;

class IDeviceHandler {
protected:
	string name;
public:
	IDeviceHandler(string n):name(n) {};
	virtual ~IDeviceHandler() {};
	virtual void get(string path) = 0;
	virtual bool processUdevEvent(UdevEvent* pUE) = 0;
	virtual Json::Value getList() = 0;
};

template <typename T>
class DeviceHandler : public IDeviceHandler {
private:
	std::list<shared_ptr<T>> devices;
	std::recursive_mutex mtx;
public:
	DeviceHandler(string n):IDeviceHandler(n) {};
	~DeviceHandler() {
		devices.clear();
	};
	bool processUdevEvent(UdevEvent* pUE) override;
	void get(string path) override;
	Json::Value getList() override;
};

template <typename T>
bool DeviceHandler<T>::processUdevEvent(UdevEvent* pUE) {
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
				devices.push_back(make_shared<T>(devpath));
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

template <typename T>
void DeviceHandler<T>::get(string devpath) {
	std::lock_guard<std::recursive_mutex> lock(mtx);
	auto device = find_if(devices.begin(), devices.end(), [&devpath](auto d) { return (d->getDevPath() == devpath); });
	if (devices.end() == device) {
		devices.push_back(make_shared<T>(devpath));
	}
	else {
		(*device)->getSysFSAll();
	}
}

template <typename T>
Json::Value DeviceHandler<T>::getList() {
	Json::Value lists;
	for (auto d : devices) {
		lists.append(d->toJson());
	}
	return lists;
}

#endif
