#include "source_battery.h"

SourceBattery::SourceBattery(string path)
	: SysFS(path) {
	// maximum_voltage ("%umV\n")
	SysFSValue<string>* maximum_voltage = new SysFSValue<string>("maximum_voltage", false);
	addMap(maximum_voltage);

	// minimum_voltage ("%umV\n")
	SysFSValue<string>* minimum_voltage = new SysFSValue<string>("minimum_voltage", false);
	addMap(minimum_voltage);

	// maximum_power ("%umW\n")
	SysFSValue<string>* maximum_power = new SysFSValue<string>("maximum_power", false);
	addMap(maximum_power);

	getSysFSAll();
/*
	cout << toJson().toStyledString() << endl;
	cout << sysFStoJson().toStyledString() << endl;
*/
}
/*
int main() {
	SourceBattery* p = new SourceBattery("./sys/class/typec/port0/usb_power_delivery/source_capabilities/3:battery/");
	cout << "maximum_voltage: " <<  p->getValue("maximum_voltage") << endl;
	cout << "minimum_voltage: " << p->getValue("minimum_voltage") << endl;
	cout << "maximum_power: " << p->getValue("maximum_power") << endl;
	delete p;
	return 0;
}
*/
