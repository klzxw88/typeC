#ifndef MANAGER_H_
#define MANAGER_H_

#include <string>
#include <map>
#include <algorithm>

#include "device_handler.h"

#define ROOT_PATH "./sys/class/typec"

using namespace std;

class Manager {
private:
	string devpath;
	map<string, shared_ptr<IDeviceHandler>> deviceHandlers;

	void processUdevEvent(UdevEvent* pUE);

public:
	Manager(string path);
};

#endif
