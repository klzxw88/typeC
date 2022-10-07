#include "sink_variable_supply.h"

SinkVariableSupply::SinkVariableSupply(string path)
	: SysFS(path) {
	// maximum_voltage ("%umV\n")
	SysFSValue<string>* maximum_voltage = new SysFSValue<string>("maximum_voltage", false);
	addMap(maximum_voltage);

	// minimum_voltage ("%umV\n")
	SysFSValue<string>* minimum_voltage = new SysFSValue<string>("minimum_voltage", false);
	addMap(minimum_voltage);

	// operational_current ("%umA\n")
	SysFSValue<string>* operational_current = new SysFSValue<string>("operational_current", false);
	addMap(operational_current);

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
