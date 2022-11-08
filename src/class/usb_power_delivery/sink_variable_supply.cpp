#include "sink_variable_supply.h"

SinkVariableSupply::SinkVariableSupply(string path)
    : SysFS(path) {
    // maximum_voltage ("%umV\n")
    SysFSValue<string>* maximum_voltage = new SysFSValue<string>(path, "maximum_voltage", false);
    addMap(maximum_voltage);

    // minimum_voltage ("%umV\n")
    SysFSValue<string>* minimum_voltage = new SysFSValue<string>(path, "minimum_voltage", false);
    addMap(minimum_voltage);

    // operational_current ("%umA\n")
    SysFSValue<string>* operational_current = new SysFSValue<string>(path, "operational_current", false);
    addMap(operational_current);

    getSysFSAll();
/*
    cout << toJson().toStyledString() << endl;
    cout << sysFStoJson().toStyledString() << endl;
*/
}
/*
int main() {
    SinkVariableSupply* p = new SinkVariableSupply("./sys/class/typec/port0/usb_power_delivery/sink_capabilities/2:variable_supply/");
    cout << "maximum_voltage: " <<  p->getValue("maximum_voltage") << endl;
    cout << "minimum_voltage: " << p->getValue("minimum_voltage") << endl;
    cout << "operational_current: " << p->getValue("operational_current") << endl;
    delete p;
    return 0;
}
*/
