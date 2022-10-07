#include "source_fixed_supply.h"

SourceFixedSupply::SourceFixedSupply(string path)
	: SysFS(path) {
	// dule_role_power
	SysFSValue<bool>* dule_role_power = new SysFSValue<bool>("dule_role_power", false);
	addMap(dule_role_power);

	// usb_suspend_supported
	SysFSValue<bool>* usb_suspend_supported = new SysFSValue<bool>("usb_suspend_supported", false);
	addMap(usb_suspend_supported);

	// unconstrained_power
	SysFSValue<bool>* unconstrained_power = new SysFSValue<bool>("unconstrained_power", false);
	addMap(unconstrained_power);

	// usb_communication_capable
	SysFSValue<bool>* usb_communication_capable = new SysFSValue<bool>("usb_communication_capable", false);
	addMap(unconstrained_power);

	// dual_role_data
	SysFSValue<bool>* dual_role_data = new SysFSValue<bool>("dual_role_data", false);
	addMap(dual_role_data);

	// unchunked_extended_messages_supported
	SysFSValue<bool>* unchunked_extended_messages_supported = new SysFSValue<bool>("unchunked_extended_messages_supported", false);
	addMap(unchunked_extended_messages_supported);

	// voltage ("%umV\n")
	SysFSValue<string>* voltage = new SysFSValue<string>("voltage", false);
	addMap(voltage);

	// maximum_current ("%umA\n")
	SysFSValue<string>* maximum_current = new SysFSValue<string>("maximum_current", false);
	addMap(maximum_current);

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
