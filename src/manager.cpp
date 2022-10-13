#include "manager.h"

Manager::Manager(string path) : devpath(path) {
};

void Manager::registerPort(string path) {
	// exit directory
	// check list
		// if not exist add
		// if exist none
}

void Manager::unregisterPort(string path) {
	// check list
		// if not exist none
		// if exist delete
}

int main() {
	Manager* m = new Manager(ROOT_PATH);
	m->registerPort("./sys/class/typec/port0/");
	return 0;
}

