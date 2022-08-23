#ifndef MANAGER_H_
#define MANAGER_H_

#include <list>
#include <string>
#include <algorithm>
#include "port.h"

#define ROOT_PATH "./sys/class/typec"

class Manager {
private:
	string devpath;
	list<Port> ports;

public:
	Manager(string path);
	void registerPort(string path);
	void unregisterPort(string path);
};

#endif
