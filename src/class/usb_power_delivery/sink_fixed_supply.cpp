#include "sink_fixed_supply.h"

SinkFixedSupply::SinkFixedSupply(string path)
	: SysFS(path) {
	// dual_role_power
	SysFSValue<bool>* dual_role_power = new SysFSValue<bool>("dual_role_power", false);
	addMap(dual_role_power);

	// usb_suspend_supported
	SysFSValue<bool>* usb_suspend_supported = new SysFSValue<bool>("usb_suspend_supported", false);
	addMap(usb_suspend_supported);

	// unconstrained_power
	SysFSValue<bool>* unconstrained_power = new SysFSValue<bool>("unconstrained_power", false);
	addMap(unconstrained_power);

	// usb_communication_capable
	SysFSValue<bool>* usb_communication_capable = new SysFSValue<bool>("usb_communication_capable", false);
	addMap(usb_communication_capable);

	// dual_role_data
	SysFSValue<bool>* dual_role_data = new SysFSValue<bool>("dual_role_data", false);
	addMap(dual_role_data);

	// unchunked_extended_messages_supported
	SysFSValue<bool>* unchunked_extended_messages_supported = new SysFSValue<bool>("unchunked_extended_messages_supported", false);
	addMap(unchunked_extended_messages_supported);

	// fast_role_swap (2bits)
	SysFSValue<int>* fast_role_swap = new SysFSValue<int>("fast_role_swap", false);
	addMap(fast_role_swap);

	// voltage ("%umV\n")
	SysFSValue<string>* voltage = new SysFSValue<string>("voltage", false);
	addMap(voltage);

	// operational_current ("%umA\n")
	SysFSValue<string>* operational_current = new SysFSValue<string>("operational_current", false);
	addMap(operational_current);

	getSysFSAll();
}
/*
int main() {
	SinkFixedSupply* p = new SinkFixedSupply("./sys/class/typec/port0/usb_power_delivery/sink_capabilitie/1:fixed_supply/");
	cout << "dual_role_power: " <<  p->getValue("dual_role_power") << endl;
	cout << "usb_suspend_supported: " << p->getValue("usb_suspend_supported") << endl;
	cout << "unconstrained_power: " << p->getValue("unconstrained_power") << endl;
	cout << "usb_communication_capable: " << p->getValue("usb_communication_capable") << endl;
	cout << "dual_role_data: " << p->getValue("dual_role_data") << endl;
	cout << "unchunked_extended_messages_supported: " << p->getValue("unchunked_extended_messages_supported") << endl;
	cout << "fast_role_swap: " << p->getValue("fast_role_swap") << endl;
	cout << "operational_current: " << p->getValue("operational_current") << endl;
	cout << "voltage: " << p->getValue("voltage") << endl;
	delete p;
	return 0;
}
*/
