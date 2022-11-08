#include "power_delivery.h"

PowerDelivery::PowerDelivery(string path)
    : SysFS(path) {
    // revision ("%u.%u\n")
    SysFSValue<string>* revision = new SysFSValue<string>(path, "revision", false);
    addMap(revision);

    // version ("%u.%u\n")
    SysFSValue<string>* version = new SysFSValue<string>(path, "version", false);
    addMap(version);

    getSysFSAll();
/*    
    cout << toJson().toStyledString() << endl;
    cout << sysFStoJson().toStyledString() << endl;
*/
}
/*
int main() {
    Plug* p = new Plug("./sys/class/typec/port0/port0-plug0/");
    cout << p->getValue("revision") << endl;
    cout << p->getValue("version") << endl;
    delete p;
    return 0;
}
*/
