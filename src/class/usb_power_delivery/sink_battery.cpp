#include "sink_battery.h"

SinkBattery::SinkBattery(string path)
	: SysFS(path) {
	// maximum_voltage ("%umV\n")
	SysFSValue<string>* maximum_voltage = new SysFSValue<string>("maximum_voltage", false);
	addMap(maximum_voltage);

	// minimum_voltage ("%umV\n")
	SysFSValue<string>* minimum_voltage = new SysFSValue<string>("minimum_voltage", false);
	addMap(minimum_voltage);

	// operational_power ("%umW\n")
	SysFSValue<string>* operational_power = new SysFSValue<string>("operational_power", false);
	addMap(operational_power);

	getSysFSAll();
/*
	cout << toJson().toStyledString() << endl;
	cout << sysFStoJson().toStyledString() << endl;
*/
}
/*
int main() {
	SinkBattery* p = new SinkBattery("./sys/class/typec/port0/usb_power_delivery/sink_capabilities/3:battery/");
	cout << "maximum_voltage: " <<  p->getValue("maximum_voltage") << endl;
	cout << "minimum_voltage: " << p->getValue("minimum_voltage") << endl;
	cout << "operational_power: " << p->getValue("operational_power") << endl;
	delete p;
	return 0;
}
*/
