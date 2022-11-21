#include "source_fixed_supply.h"

SourceFixedSupply::SourceFixedSupply(string path)
    : SysFS(path) {
    // dule_role_power
    SysFSValue<bool>* dual_role_power = new SysFSValue<bool>(path, "dual_role_power", false);
    addMap(dual_role_power);

    // usb_suspend_supported
    SysFSValue<bool>* usb_suspend_supported = new SysFSValue<bool>(path, "usb_suspend_supported", false);
    addMap(usb_suspend_supported);

    // unconstrained_power
    SysFSValue<bool>* unconstrained_power = new SysFSValue<bool>(path, "unconstrained_power", false);
    addMap(unconstrained_power);

    // usb_communication_capable
    SysFSValue<bool>* usb_communication_capable = new SysFSValue<bool>(path, "usb_communication_capable", false);
    addMap(usb_communication_capable);

    // dual_role_data
    SysFSValue<bool>* dual_role_data = new SysFSValue<bool>(path, "dual_role_data", false);
    addMap(dual_role_data);

    // unchunked_extended_messages_supported
    SysFSValue<bool>* unchunked_extended_messages_supported = new SysFSValue<bool>(path, "unchunked_extended_messages_supported", false);
    addMap(unchunked_extended_messages_supported);

    // voltage ("%umV\n")
    SysFSValue<string>* voltage = new SysFSValue<string>(path, "voltage", false);
    addMap(voltage);

    // maximum_current ("%umA\n")
    SysFSValue<string>* maximum_current = new SysFSValue<string>(path, "maximum_current", false);
    addMap(maximum_current);

    getSysFSAll();
/*
    cout << toJson().toStyledString() << endl;
    cout << sysFStoJson().toStyledString() << endl;
*/
}
/*
int main() {
    SourceFixedSupply* p = new SourceFixedSupply("./sys/class/typec/port0/usb_power_delivery/source_capabilities/1:fixed_supply/");
    cout << "dual_role_power: " <<  p->getValue("dual_role_power") << endl;
    cout << "usb_suspend_supported: " << p->getValue("usb_suspend_supported") << endl;
    cout << "unconstrained_power: " << p->getValue("unconstrained_power") << endl;
    cout << "usb_communication_capable: " << p->getValue("usb_communication_capable") << endl;
    cout << "dual_role_data: " << p->getValue("dual_role_data") << endl;
    cout << "unchunked_extended_messages_supported: " << p->getValue("unchunked_extended_messages_supported") << endl;
    cout << "maximum_current: " << p->getValue("maximum_current") << endl;
    cout << "voltage: " << p->getValue("voltage") << endl;
    delete p;
    return 0;
}
*/
