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
	virtual bool processUdevEvent(UdevEvent* pUE) = 0;
};

template <typename T>
class DeviceHandler : public IDeviceHandler {
private:
	std::list<T*> devices;
	std::recursive_mutex mtx;
public:
	DeviceHandler(string n):IDeviceHandler(n) {};
	~DeviceHandler() {};
	bool processUdevEvent(UdevEvent* pUE) override;
};

template <typename T>
bool DeviceHandler<T>::processUdevEvent(UdevEvent* pUE) {
	if (pUE->getDevAttribute(DEVTYPE) == name) {
		string devpath = pUE->getDevAttribute(DEVPATH);
		std::lock_guard<std::recursive_mutex> lock(mtx);
		auto device = find_if(devices.begin(), devices.end(), [&devpath](T* d) { return (d->getDevPath() == devpath); });
		if (pUE->getDevAttribute(ACTION) == DEVICE_REMOVE) {	
			devices.remove(*device);
			return true;
		}
		else if (pUE->getDevAttribute(ACTION) == DEVICE_ADD) {
			if (*device == nullptr) {
				T* d = new T(devpath);
				devices.push_back(d);
			}
			else {
				(*device)->getSysFSAll();
			}
			return true;
		}
		else if (pUE->getDevAttribute(ACTION) == DEVICE_CHANGE) {
			(*device)->getSysFSAll();
			return true;
		}
	}
	return false;
}

	// getList();
#endif



