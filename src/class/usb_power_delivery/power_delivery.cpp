#include "power_delivery.h"

PowerDelivery::PowerDelivery(string path)
	: SysFS(path) {
	// revision ("%u.%u\n")
	SysFSValue<string>* revision = new SysFSValue<string>("revision", false);
	addMap(revision);

	// version ("%u.%u\n")
	SysFSValue<string>* version = new SysFSValue<string>("version", false);
	addMap(version);

	getSysFSAll();
}
/*
int main() {
	Plug* p = new Plug("./sys/class/typec/port0/port0-plug0/");
	cout << p->getValue("number_of_alternate_modes") << endl;
	delete p;
	return 0;
}
*/
