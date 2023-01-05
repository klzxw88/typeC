#include <iostream>
#include "manager.h"
#include "udev_listener.h"
#include "configuration.h"

using namespace std;

void printDebugMenu() {
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
    cout << "@@@@@@@@@@@@@@@@@@@  WELCOME to USB type-C DEBUG MENU @@@@@@@@@@@@@@@@" << endl;
    cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@" << endl;
    cout << endl;
    cout << "======================================================================" << endl;
    cout << "0x01:   print all sysfs values for usb type-C (/sys/class/typec)" << endl;
    cout << "0x02:   print all ports" << endl;
    cout << "0x03:   print all partners" << endl;
    cout << "0x04:   print all cables" << endl;
    cout << "0x05:   print all plugs" << endl;
    cout << "0x06:   print all PD(Power Delivery)s" << endl;
    cout << "0x07:   print all altmodes" << endl;
    cout << "0x08:   print all DP(Display Port) alt modes" << endl;
    cout << "0x09:   print all identities" << endl;
    cout << "0x0a:   print all PDO(Power Delivery Object) values" << endl;
    cout << "0x0b:   print all PD sources" << endl;
    cout << "0x0c:   print all PD fixed sources" << endl;
    cout << "0x0d:   print all PD variable sources" << endl;
    cout << "0x0e:   print all PD battery sources" << endl;
    cout << "0x0f:   print all PD pps sources" << endl;
    cout << "0x10:   print all PD sinks" << endl;
    cout << "0x11:   print all PD fixed sinks" << endl;
    cout << "0x12:   print all PD variable sinks" << endl;
    cout << "0x13:   print all PD battery sinks" << endl;
    cout << "0x14:   print all PD pps sinks" << endl;
    cout << "0x20:   get port values" << endl;
    cout << "0x21:   get partner values" << endl;
    cout << "0x22:   get partner Identity values" << endl;
    cout << "0x23:   get cable values" << endl;
    cout << "0x24:   get cable identity values" << endl;
    cout << "0x25:   get plug values" << endl;
    cout << "0x26:   get altmode values" << endl;
    cout << "0x27:   get DP values" << endl;
    cout << "0x28:   get PD(Power Delivery) values" << endl;
    cout << "0x29:   get PDO(Power Delivery Object) values" << endl;
    cout << "0x2a:   get PD sources" << endl;
    cout << "0x2b:   get PD fixed sources" << endl;
    cout << "0x2c:   get PD variable sources" << endl;
    cout << "0x2d:   get PD battery sources" << endl;
    cout << "0x2e:   get PD pps sources" << endl;
    cout << "0x2f:   get PD sinks" << endl;
    cout << "0x30:   get PD fixed sinks" << endl;
    cout << "0x31:   get PD variable sinks" << endl;
    cout << "0x32:   get PD battery sinks" << endl;
    cout << "0x33:   get PD pps sinks" << endl;
    cout << "0x40:   get all sysfs values for usb type-C with path" << endl;
    cout << "0x41:   start udev monitor" << endl;
    cout << "0x42:   stop udev monitor" << endl;
    cout << "0x50:   set port data_role value" << endl;
    cout << "0x51:   set port power_role value" << endl;
    cout << "0x52:   set port port_type value" << endl;
    cout << "0x53:   set port vconn_source value" << endl;
    cout << "0x54:   set port preferred_role value" << endl;
    cout << "0x55:   set altmode active value" << endl;
    cout << "0x56:   set displayport configuration value" << endl;
    cout << "0x57:   set displayport pin_assignment value" << endl;
    cout << "======================================================================" << endl;
    cout << "0xff:   Exit [usb type-c debug menu]" << endl;
    cout << "======================================================================" << endl;
    cout << "Enter CMD >>  0x";
}

int hexStringToInt(string line) {
    int val;
    std::istringstream iss(line);
    iss >> std::hex >> val;
    return val;
}

int stringToInt(string line) {
    int val;
    std::istringstream iss(line);
    iss >> val;
    return val;
}

string getValue(string name) {
    string line;
    Json::Value attribute = Configuration::instance()->getAttribute(name);
    string valueType = attribute["type"].asString();
    if (valueType == "string") {
        cout << "Enter new " + name + " value >> ";
        getline(cin, line);
        return line+"\n";
    }
    else if (valueType == "bitset<32>") {
        cout << "Enter new " + name + " value >> ";
        getline(cin, line);
        if (line[0] == '0' && line[1] == 'x') {
            line = line.substr(2, line.length()-2);
            stringstream ss;
            unsigned int iVal;
            ss << std::hex << line;
            ss >> iVal;
            return to_string(iVal);
        }
        else if (std::all_of(line.begin(), line.end()-2, ::isxdigit)) {
            stringstream ss;
            unsigned int iVal;
            ss << std::hex << line;
            ss >> iVal;
            return to_string(iVal);
        }
        else {
            cout << "invalid value : " << line << endl;
            return "";
        }
    }
    else if (valueType == "bool") {
        cout << "Enter new " + name + " value (yes/no) >> ";
        getline(cin, line);
        if (line != "yes" && line != "no") {
            cout << "invalid value : " << line << endl;
            return "";
        }
        return line+"\n";
    }
    else if (valueType == "int") {
        cout << "Enter new " + name + " value >> ";
        getline(cin, line);
        if (!line.empty() && std::all_of(line.begin(), line.end(), ::isdigit)) {
            return line;
        }
        else {
            cout << "invalid value : " << line << endl;
            return "";
        }
    }
    else if (valueType == "enum") {
        int i=0;
        map<int, string> values;
        for (string item : attribute["values"].getMemberNames()) {
            values.insert(make_pair(i, item));
            cout << "[" << i++ << "]\t" << item.erase(item.length()-1) << endl;
        }
        cout << "Enter new " + name + " value >> ";
        getline(cin, line);
        int index = stringToInt(line);
        if (0 > index || index >= i) {
            cout << "invalid index selected" << endl;
            return "";
        }
        return values[index];
    }
    return "";
}

void selectMenu() {
    int portIdx = -1, plugIdx = -1, modeIdx = -1;
    int selected;
    do {
        string line;
        printDebugMenu();
        getline(cin, line);
        selected = hexStringToInt(line);
        switch(selected) {
            case 1:
                Manager::instance()->clearList();
                Manager::instance()->getAll();
                cout << Manager::instance()->getList().toStyledString() << endl;
                break;
            case 2:
                Manager::instance()->clearList();
                Manager::instance()->getAll();
                cout << Manager::instance()->getList(DEVTYPE_TYPEC_PORT).toStyledString() << endl;
                break;
            case 3:
                Manager::instance()->clearList();
                Manager::instance()->getAll();
                cout << Manager::instance()->getList(DEVTYPE_TYPEC_PARTNER).toStyledString() << endl;
                break;
            case 4:
                Manager::instance()->clearList();
                Manager::instance()->getAll();
                cout << Manager::instance()->getList(DEVTYPE_TYPEC_CABLE).toStyledString() << endl;
                break;
            case 5:
                Manager::instance()->clearList();
                Manager::instance()->getAll();
                cout << Manager::instance()->getList(DEVTYPE_TYPEC_PLUG).toStyledString() << endl;
                break;
            case 6:
                Manager::instance()->clearList();
                Manager::instance()->getAll();
                cout << Manager::instance()->getList(DEVTYPE_USB_POWER_DELIVERY).toStyledString() << endl;
                break;
            case 7:
                Manager::instance()->clearList();
                Manager::instance()->getAll();
                cout << Manager::instance()->getList(DEVTYPE_TYPEC_ALTMODE).toStyledString() << endl;
                break;
            case 8:
                Manager::instance()->clearList();
                Manager::instance()->getAll();
                cout << Manager::instance()->getList(DEVTYPE_DP).toStyledString() << endl;
                break;
            case 9:
                Manager::instance()->clearList();
                Manager::instance()->getAll();
                cout << Manager::instance()->getList(DEVTYPE_IDENTITY).toStyledString() << endl;
                break;
            case 10:
                Manager::instance()->clearList();
                Manager::instance()->getAll();
                cout << Manager::instance()->getList(DEVTYPE_PDO).toStyledString() << endl;
                break;
            case 11:
                Manager::instance()->clearList();
                Manager::instance()->getAll();
                cout << Manager::instance()->getList(PDO_SOURCE).toStyledString() << endl;
                break;
            case 12:
                Manager::instance()->clearList();
                Manager::instance()->getAll();
                cout << Manager::instance()->getList(PDO_SOURCE_FIXED).toStyledString() << endl;
                break;
            case 13:
                Manager::instance()->clearList();
                Manager::instance()->getAll();
                cout << Manager::instance()->getList(PDO_SOURCE_VARIABLE).toStyledString() << endl;
                break;
            case 14:
                Manager::instance()->clearList();
                Manager::instance()->getAll();
                cout << Manager::instance()->getList(PDO_SOURCE_BATTERY).toStyledString() << endl;
                break;
            case 15:
                Manager::instance()->clearList();
                Manager::instance()->getAll();
                cout << Manager::instance()->getList(PDO_SOURCE_PPS).toStyledString() << endl;
                break;
            case 16:
                Manager::instance()->clearList();
                Manager::instance()->getAll();
                cout << Manager::instance()->getList(PDO_SINK).toStyledString() << endl;
                break;
            case 17:
                Manager::instance()->clearList();
                Manager::instance()->getAll();
                cout << Manager::instance()->getList(PDO_SINK_FIXED).toStyledString() << endl;
                break;
            case 18:
                Manager::instance()->clearList();
                Manager::instance()->getAll();
                cout << Manager::instance()->getList(PDO_SINK_VARIABLE).toStyledString() << endl;
                break;
            case 19:
                Manager::instance()->clearList();
                Manager::instance()->getAll();
                cout << Manager::instance()->getList(PDO_SINK_BATTERY).toStyledString() << endl;
                break;
            case 20:
                Manager::instance()->clearList();
                Manager::instance()->getAll();
                cout << Manager::instance()->getList(PDO_SINK_PPS).toStyledString() << endl;
                break;
            case 32:
                Manager::instance()->clearList(DEVTYPE_TYPEC_PORT);
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPort(portIdx)) {
                    cout << "print port(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(DEVTYPE_TYPEC_PORT).toStyledString() << endl;
                break;
            case 33:
                Manager::instance()->clearList(DEVTYPE_TYPEC_PARTNER);
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPartner(portIdx)) {
                    cout << "print partner(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(DEVTYPE_TYPEC_PARTNER).toStyledString() << endl;
                break;
            case 34:
                Manager::instance()->clearList(DEVTYPE_IDENTITY);
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPartnerIdentity(portIdx)) {
                    cout << "print partner Identity(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(DEVTYPE_IDENTITY).toStyledString() << endl;
                break;
            case 35:
                Manager::instance()->clearList(DEVTYPE_TYPEC_CABLE);
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getCable(portIdx)) {
                    cout << "print cable(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(DEVTYPE_TYPEC_CABLE).toStyledString() << endl;
                break;
            case 36:
                Manager::instance()->clearList(DEVTYPE_IDENTITY);
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getCableIdentity(portIdx)) {
                    cout << "print Cable Identity(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(DEVTYPE_IDENTITY).toStyledString() << endl;
                break;
            case 37:
                Manager::instance()->clearList(DEVTYPE_TYPEC_PLUG);
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                cout << "Enter plug index >> ";
                getline(cin, line);
                plugIdx = stringToInt(line);
                if (!Manager::instance()->getPlug(portIdx, plugIdx)) {
                    cout << "print Cable Identity(portIdx:" << portIdx << ", plugIdx:" << plugIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(DEVTYPE_TYPEC_PLUG).toStyledString() << endl;
                break;
            case 38:
                Manager::instance()->clearList(DEVTYPE_TYPEC_ALTMODE);
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                cout << "Enter mode index >> ";
                getline(cin, line);
                modeIdx = stringToInt(line);
                if (!Manager::instance()->getAltMode(portIdx, modeIdx)) {
                    cout << "print altmode(portIdx:" << portIdx << ", modeIdx:" << modeIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(DEVTYPE_TYPEC_ALTMODE).toStyledString() << endl;
                break;
            case 39:
                Manager::instance()->clearList(DEVTYPE_DP);
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                cout << "Enter mode index >> ";
                getline(cin, line);
                modeIdx = stringToInt(line);
                if (!Manager::instance()->getDP(portIdx, modeIdx)) {
                    cout << "print DP(portIdx:" << portIdx << ", modeIdx:" << modeIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(DEVTYPE_DP).toStyledString() << endl;
                break;
            case 40:
                Manager::instance()->clearList(DEVTYPE_USB_POWER_DELIVERY);
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPowerDelivery(portIdx)) {
                    cout << "print PD(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(DEVTYPE_USB_POWER_DELIVERY).toStyledString() << endl;
                break;
            case 41:
                Manager::instance()->clearList(DEVTYPE_PDO);
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPowerDelivery(portIdx)) {
                    cout << "print PD(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(DEVTYPE_PDO).toStyledString() << endl;
                break;
            case 42:
                Manager::instance()->clearList(PDO_SOURCE);
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPowerDeliverySource(portIdx)) {
                    cout << "print PD sources(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(PDO_SOURCE).toStyledString() << endl;
                break;
            case 43:
                Manager::instance()->clearList(PDO_SOURCE_FIXED);
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPowerDeliverySourceFixed(portIdx)) {
                    cout << "print PD fixed sources(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(PDO_SOURCE_FIXED).toStyledString() << endl;
                break;
            case 44:
                Manager::instance()->clearList(PDO_SOURCE_VARIABLE);
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPowerDeliverySourceVariable(portIdx)) {
                    cout << "print PD variable sources(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(PDO_SOURCE_VARIABLE).toStyledString() << endl;
                break;
            case 45:
                Manager::instance()->clearList(PDO_SOURCE_BATTERY);
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPowerDeliverySourceBattery(portIdx)) {
                    cout << "print PD battery sources(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(PDO_SOURCE_BATTERY).toStyledString() << endl;
                break;
            case 46:
                Manager::instance()->clearList(PDO_SOURCE_PPS);
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPowerDeliverySourcePPS(portIdx)) {
                    cout << "print PD pps sources(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(PDO_SOURCE_PPS).toStyledString() << endl;
                break;
            case 47:
                Manager::instance()->clearList(PDO_SINK);
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPowerDeliverySink(portIdx)) {
                    cout << "print PD sinks(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(PDO_SINK).toStyledString() << endl;
                break;
            case 48:
                Manager::instance()->clearList(PDO_SINK_FIXED);
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPowerDeliverySinkFixed(portIdx)) {
                    cout << "print PD fixed sinks(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(PDO_SINK_FIXED).toStyledString() << endl;
                break;
            case 49:
                Manager::instance()->clearList(PDO_SINK_VARIABLE);
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPowerDeliverySinkVariable(portIdx)) {
                    cout << "print PD variable sinks(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(PDO_SINK_VARIABLE).toStyledString() << endl;
                break;
            case 50:
                Manager::instance()->clearList(PDO_SINK_BATTERY);
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPowerDeliverySinkBattery(portIdx)) {
                    cout << "print PD battery sinks(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(PDO_SINK_BATTERY).toStyledString() << endl;
                break;
            case 51:
                Manager::instance()->clearList(PDO_SINK_PPS);
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPowerDeliverySinkPPS(portIdx)) {
                    cout << "print PD pps sinks(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(PDO_SINK_PPS).toStyledString() << endl;
                break;
            case 64:
                cout << "Enter path >> ";
                getline(cin, line);
                Manager::instance()->getAll(line);
                cout << Manager::instance()->getList().toStyledString() << endl;
                break;
            case 65:
                UdevListener::instance()->initListener(true);
                break;
            case 66:
                UdevListener::instance()->stopListener();
                break;
            case 80:
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPort(portIdx)) {
                    cout << "get port(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(DEVTYPE_TYPEC_PORT).toStyledString() << endl;
                if ((line = getValue("data_role")).empty()) {
                    break;
                }
                if (Manager::instance()->setPort(portIdx, "data_role", line)) {
                    cout << "retValue : true" << endl;
                    cout << Manager::instance()->getList(DEVTYPE_TYPEC_PORT).toStyledString() << endl;
                }
                else {
                    cout << "retValue : false" << endl;
                }
                break;
            case 81:
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPort(portIdx)) {
                    cout << "get port(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(DEVTYPE_TYPEC_PORT).toStyledString() << endl;
                if ((line = getValue("power_role")).empty()) {
                    break;
                }
                if (Manager::instance()->setPort(portIdx, "power_role", line)) {
                    cout << "retValue : true" << endl;
                    cout << Manager::instance()->getList(DEVTYPE_TYPEC_PORT).toStyledString() << endl;
                }
                else {
                    cout << "retValue : false" << endl;
                }
                break;
            case 82:
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPort(portIdx)) {
                    cout << "get port(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(DEVTYPE_TYPEC_PORT).toStyledString() << endl;
                if ((line = getValue("port_type")).empty()) {
                    break;
                }
                if (Manager::instance()->setPort(portIdx, "port_type", line)) {
                    cout << "retValue : true" << endl;
                    cout << Manager::instance()->getList(DEVTYPE_TYPEC_PORT).toStyledString() << endl;
                }
                else {
                    cout << "retValue : false" << endl;
                }
                break;
            case 83:
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPort(portIdx)) {
                    cout << "get port(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(DEVTYPE_TYPEC_PORT).toStyledString() << endl;
                if ((line = getValue("vconn_source")).empty()) {
                    break;
                }
                if (Manager::instance()->setPort(portIdx, "vconn_source", line)) {
                    cout << "retValue : true" << endl;
                    cout << Manager::instance()->getList(DEVTYPE_TYPEC_PORT).toStyledString() << endl;
                }
                else {
                    cout << "retValue : false" << endl;
                }
                break;
            case 84:
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPort(portIdx)) {
                    cout << "get port(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(DEVTYPE_TYPEC_PORT).toStyledString() << endl;
                if ((line = getValue("preferred_role")).empty()) {
                    break;
                }
                if (Manager::instance()->setPort(portIdx, "preferred_role", line)) {
                    cout << "retValue : true" << endl;
                    cout << Manager::instance()->getList(DEVTYPE_TYPEC_PORT).toStyledString() << endl;
                }
                else {
                    cout << "retValue : false" << endl;
                }
                break;
            case 85:
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                if (!Manager::instance()->getPartner(portIdx)) {
                    cout << "print partner(portIdx:" << portIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(DEVTYPE_TYPEC_PARTNER).toStyledString() << endl;
                if ((line = getValue("supports_usb_power_delivery")).empty()) {
                    break;
                }
                if (Manager::instance()->setPartner(portIdx, "supports_usb_power_delivery", line)) {
                    cout << "retValue : true" << endl;
                    cout << Manager::instance()->getList(DEVTYPE_TYPEC_PARTNER).toStyledString() << endl;
                }
                else {
                    cout << "retValue : false" << endl;
                }
                break;
            case 86:
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                cout << "Enter mode index >> ";
                getline(cin, line);
                modeIdx = stringToInt(line);
                if (!Manager::instance()->getAltMode(portIdx, modeIdx)) {
                    cout << "print altmode(portIdx:" << portIdx << ", modeIdx:" << modeIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(DEVTYPE_TYPEC_ALTMODE).toStyledString() << endl;
                if ((line = getValue("active")).empty()) {
                    break;
                }
                if (Manager::instance()->setAltMode(portIdx, modeIdx, "active", line)) {
                    cout << "retValue : true" << endl;
                    cout << Manager::instance()->getList(DEVTYPE_TYPEC_ALTMODE).toStyledString() << endl;
                }
                else {
                    cout << "retValue : false" << endl;
                }
                break;
            case 87:
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                cout << "Enter mode index >> ";
                getline(cin, line);
                modeIdx = stringToInt(line);
                if (!Manager::instance()->getDP(portIdx, modeIdx)) {
                    cout << "print DP(portIdx:" << portIdx << ", modeIdx:" << modeIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(DEVTYPE_DP).toStyledString() << endl;
                if ((line = getValue("configuration")).empty()) {
                    break;
                }
                if (Manager::instance()->setDP(portIdx, modeIdx, "configuration", line)) {
                    cout << "retValue : true" << endl;
                    cout << Manager::instance()->getList(DEVTYPE_DP).toStyledString() << endl;
                }
                else {
                    cout << "retValue : false" << endl;
                }
				break;
            case 88:
                cout << "Enter port index >> ";
                getline(cin, line);
                portIdx = stringToInt(line);
                cout << "Enter mode index >> ";
                getline(cin, line);
                modeIdx = stringToInt(line);
                if (!Manager::instance()->getDP(portIdx, modeIdx)) {
                    cout << "print DP(portIdx:" << portIdx << ", modeIdx:" << modeIdx << ") values failed" << endl;
                    break;
                }
                cout << Manager::instance()->getList(DEVTYPE_DP).toStyledString() << endl;
                if ((line = getValue("pin_assignment")).empty()) {
                    break;
                }
                if (Manager::instance()->setDP(portIdx, modeIdx, "pin_assignment", line)) {
                    cout << "retValue : true" << endl;
                    cout << Manager::instance()->getList(DEVTYPE_DP).toStyledString() << endl;
                }
                else {
                    cout << "retValue : false" << endl;
                }
				break;
            case 255:
                break;
            default:
                cout << "invalid index (" << selected << ")" << endl;
                break;
        }
        cout << endl;
    } while (selected != 255);
}

int main() {
    selectMenu();
    return 0;
}
