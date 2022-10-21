#include "altmode.h"
// https://www.kernel.org/doc/Documentation/ABI/testing/sysfs-bus-typec

AlternateMode::AlternateMode(string path)
	: SysFS(path) {
	// active
	SysFSValue<bool>* active = new SysFSValue<bool>("active", true);
	addMap(active);

	// description
	SysFSValue<string>* description = new SysFSValue<string>("description", false);
	addMap(description);

	// mode
	SysFSValue<int>* mode = new SysFSValue<int>("mode", false);
	addMap(mode);

	// svid
	SysFSValue<int>* svid = new SysFSValue<int>("svid", false);
	addMap(svid);

	// vdo
	SysFSValue<bitset<32>>* vdo = new SysFSValue<bitset<32>>("vdo", false);
	addMap(vdo);

	getSysFSAll();
/*
	cout << toJson().toStyledString() << endl;
	cout << sysFStoJson().toStyledString() << endl;
*/
}
/*
int main() {
	AlternateMode* p = new AlternateMode("./sys/bus/typec/devices/port0/mode1/");
	cout << p->getValue("active") << endl;
	cout << p->getValue("description") << endl;
	cout << p->getValue("mode") << endl;
	cout << p->getValue("svid") << endl;
	cout << p->getValue("vdo") << endl;
	cout << p->setValue("active",false) << endl;
	delete p;
	return 0;
}
*/
