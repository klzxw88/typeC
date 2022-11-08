#include "displayport.h"
// https://www.kernel.org/doc/Documentation/ABI/testing/sysfs-driver-typec-displayport

DisplayPort::DisplayPort(string path)
    : SysFS(path) {
    // configuration
    SysFSValue<int>* configuration = new SysFSValue<int>(path, "configuration", true);
    configuration->add("[USB] source sink\n", DP_CONF_DUAL_USB);
    configuration->add("USB [source] sink\n", DP_CONF_DUAL_SOURCE);
    configuration->add("USB source [sink]\n", DP_CONF_DUAL_SINK);
    configuration->add("[USB] source\n", DP_CONF_SOURCE_USB);
    configuration->add("USB [source]\n", DP_CONF_SOURCE_SOURCE);
    configuration->add("[USB] sink\n", DP_CONF_SINK_USB);
    configuration->add("USB [sink]\n", DP_CONF_SINK_SINK);
    addMap(configuration);

    // pin_assignment
    SysFSValue<string>* pin_assignment = new SysFSValue<string>(path, "pin_assignment", true);
    addMap(pin_assignment);

    getSysFSAll();
/*
    cout << toJson().toStyledString() << endl;
    cout << sysFStoJson().toStyledString() << endl;
*/
}
/*
int main() {
    DisplayPort* p = new DisplayPort("./sys/bus/typec/devices/1-1/mode1/displayport/");
    cout << p->getValue("configuration") << endl;
    cout << p->getValue("pin_assignment") << endl;
    p->setValue("configuration", DP_CONF_DUAL_USB);
    p->setValue("pin_assignment", "A [B] C\n");
    cout << p->getValue("configuration") << endl;
    cout << p->getValue("pin_assignment") << endl;
    p->setValue("configuration", DP_CONF_DUAL_SOURCE);
    p->setValue("pin_assignment", "[C] D\n");
    cout << p->getValue("configuration") << endl;
    cout << p->getValue("pin_assignment") << endl;
    delete p;
    return 0;
}
*/
