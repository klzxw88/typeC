#include "udev_event.h"

#include <algorithm>
#include <iostream>

using namespace std;
using namespace DevAttributes;

void UdevEvent::parser(struct udev_device* dev, bool isInitDone) {
    getDeviceInfo(dev, isInitDone);

}

string UdevEvent::getDevAttribute(string attribute) {
    return mdeviceInfo[attribute];
}

void UdevEvent::getDeviceInfo(struct udev_device* device, bool isInitDone) {
    struct udev_list_entry *list_entry;

    udev_list_entry_foreach(list_entry, udev_device_get_properties_list_entry(device)) {
        string name = udev_list_entry_get_name(list_entry);
        string value = udev_list_entry_get_value(list_entry);
        if (name.compare("ID_MODEL") == 0) {
            replace( value.begin(), value.end(), '_', ' ');
        }
        if (name.compare("DEVNAME") == 0) {
            string dev = "/dev/";
            if (value.compare(0,dev.length(),dev) == 0) {
                value.erase(0,dev.length());
            }
        }
        if (name.compare("DEVPATH") == 0) {
            string sys = "/sys";
            if (value.compare(0,sys.length(),sys) != 0) {
                value=sys+value;
            }
            if (value[value.length()-1] != '/') {
                value+='/';
            }
        }
        mdeviceInfo[name] = value;
        cout << "UdevEvent::getDeviceInfo - Name: " << name.c_str() << " Value: " << value.c_str() << endl;
    }
    cout << endl;
    if (!isInitDone) {
        mdeviceInfo[ACTION] = DEVICE_ADD;
    }
}
