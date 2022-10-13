#include "sink_pps.h"

SinkPPS::SinkPPS(string path)
	: SysFS(path) {
	// maximum_voltage ("%umV\n")
	SysFSValue<string>* maximum_voltage = new SysFSValue<string>("maximum_voltage", false);
	addMap(maximum_voltage);

	// minimum_voltage ("%umV\n")
	SysFSValue<string>* minimum_voltage = new SysFSValue<string>("minimum_voltage", false);
	addMap(minimum_voltage);

	// maximum_current ("%umA\n")
	SysFSValue<string>* maximum_current = new SysFSValue<string>("maximum_current", false);
	addMap(maximum_current);

	getSysFSAll();
}
/*
int main() {
	SinkPPS* p = new SinkPPS("./sys/class/typec/port0/usb_power_delivery/sink_capabilities/4:pps/");
	cout << "maximum_voltage: " <<  p->getValue("maximum_voltage") << endl;
	cout << "minimum_voltage: " << p->getValue("minimum_voltage") << endl;
	cout << "maximum_current: " << p->getValue("maximum_current") << endl;
	delete p;
	return 0;
}
*/
