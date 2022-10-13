#include "source_pps.h"

SourcePPS::SourcePPS(string path)
	: SysFS(path) {
	// pps_power_limited
	SysFSValue<bool>* pps_power_limited = new SysFSValue<bool>("pps_power_limited", false);
	addMap(pps_power_limited);

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
	SourcePPS* p = new SourcePPS("./sys/class/typec/port0/usb_power_delivery/source_capabilities/4:pps/");
	cout << "pps_power_limited: " <<  p->getValue("pps_power_limited") << endl;
	cout << "maximum_voltage: " <<  p->getValue("maximum_voltage") << endl;
	cout << "minimum_voltage: " << p->getValue("minimum_voltage") << endl;
	cout << "maximum_current: " << p->getValue("maximum_current") << endl;
	delete p;
	return 0;
}
*/
