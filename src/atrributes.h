#ifndef DEVATTRIBUTES_H_
#define DEVATTRIBUTES_H_

#include <string>

using namespace std;

namespace DevAttributes {

//device status
    const string DEVICE_ADD = "add";
    const string DEVICE_REMOVE = "remove";
    const string DEVICE_CHANGE = "change";
    const string DEVICE_BIND = "bind";
    const string DEVICE_UNBIND = "unbind";
    const string YES = "1";
    const string NO = "0";

    const string DEVNUM = "DEVNUM";
    const string BUSNUM = "BUSNUM";
    const string ACTION  =  "ACTION";
    const string DEVLINKS  = "DEVLINKS";
    const string DEVNAME  =  "DEVNAME";
    const string DEVPATH  =  "DEVPATH";
    const string DEVTYPE  =  "DEVTYPE";
    const string ID_BUS  =  "ID_BUS";
    const string ID_MODEL  =  "ID_MODEL";
    const string ID_MODEL_ENC  =  "ID_MODEL_ENC";
    const string ID_MODEL_ID  =  "ID_MODEL_ID";
    const string ID_PATH  =  "ID_PATH";
    const string ID_PATH_TAG  =  "ID_PATH_TAG";
    const string ID_REVISION  =  "ID_REVISION";
    const string ID_SERIAL  =  "ID_SERIAL";
    const string ID_SERIAL_SHORT  =  "ID_SERIAL_SHORT";
    const string ID_TYPE  =  "ID_TYPE";
    const string ID_USB_DRIVER  =  "ID_USB_DRIVER";
    const string ID_USB_INTERFACES  =  "ID_USB_INTERFACES";
    const string ID_USB_INTERFACE_NUM  =  "ID_USB_INTERFACE_NUM";
    const string ID_VENDOR_ENC  =  "ID_VENDOR_ENC";
    const string ID_VENDOR  =  "ID_VENDOR";
    const string ID_VENDOR_ID  =  "ID_VENDOR_ID";
    const string MAJOR  =  "MAJOR";
    const string MINOR  =  "MINOR";
    const string SEQNUM  =  "SEQNUM";
    const string SUBSYSTEM  =  "SUBSYSTEM";
    const string TAGS  =  "TAGS";
    const string USEC_INITIALIZED  =  "USEC_INITIALIZED";
    const string SPEED = "SPEED";
    const string ID_VENDOR_FROM_DATABASE = "ID_VENDOR_FROM_DATABASE";

    const string MAX_CHILD = "MAX_CHILD";
    const string USB_VERSION = "USB_VERSION";
    const string DEVCLASS = "DEVICE_CLASS";
    const string DEVSUBCLASS = "DEVICE_SUBCLASS";
    const string INTERFACE = "INTERFACE";
    const string INTERFACE_COUNT = "INTERFACE_COUNT";
    const string PRODUCT_ID = "PRODUCT_ID";

// Unknown device
    const string DEV_TYPE_UNKNOWN = "UNKNOWN";

//USB device Speed
    const string USB_SUPER_SPEED = "SUPER";
    const string USB_HIGH_SPEED = "HIGH";
    const string USB_FULL_SPEED = "FULL";
    const string USB_LOW_SPEED = "LOW";

//Host Contoller Interface (HCD)
    const string XHCI = "xhci";
    const string EHCI = "ehci";
    const string OHCI = "ohci";
    const string UHCI = "uhci";

// DEVTYPE
	const string DEVTYPE_TYPEC_PORT = "typec_port";
	const string DEVTYPE_TYPEC_PARTNER = "typec_partner";
	const string DEVTYPE_TYPEC_CABLE = "typec_cable";
	const string DEVTYPE_TYPEC_PLUG = "typec_plug";
	const string DEVTYPE_IDENTITY = "identity";
	const string DEVTYPE_USB_POWER_DELIVERY = "usb_power_delivery";
	const string DEVTYPE_PDO = "pdo";
	const string DEVTYPE_TYPEC_ALTMODE = "typec_alternate_mode";
	const string DEVTYPE_DP = "displayport";
};

#endif /* DEVATTRIBUTES_H_ */
