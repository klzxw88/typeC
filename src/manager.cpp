#include "manager.h"
#include "atrributes.h"
#include "device_handler.h"
#include "altmode.h"
#include "displayport.h"
#include "power_delivery.h"
#include "port.h"
#include "partner.h"
#include "plug.h"
#include "cable.h"
#include "identity.h"
#include "source_fixed_supply.h"
#include "source_variable_supply.h"
#include "source_battery.h"
#include "source_pps.h"
#include "sink_fixed_supply.h"
#include "sink_variable_supply.h"
#include "sink_battery.h"
#include "sink_pps.h"
#include "udev_listener.h"

using namespace DevAttributes;

Manager::Manager() {
    deviceHandlers.insert(make_pair(DEVTYPE_TYPEC_PORT, shared_ptr<IDeviceHandler>(new DeviceHandler<Port>(DEVTYPE_TYPEC_PORT))));
    deviceHandlers.insert(make_pair(DEVTYPE_TYPEC_PARTNER, shared_ptr<IDeviceHandler>(new DeviceHandler<Partner>(DEVTYPE_TYPEC_PARTNER))));
    deviceHandlers.insert(make_pair(DEVTYPE_TYPEC_CABLE, shared_ptr<IDeviceHandler>(new DeviceHandler<Cable>(DEVTYPE_TYPEC_CABLE))));
    deviceHandlers.insert(make_pair(DEVTYPE_TYPEC_PLUG, shared_ptr<IDeviceHandler>(new DeviceHandler<Plug>(DEVTYPE_TYPEC_PLUG))));
    deviceHandlers.insert(make_pair(DEVTYPE_IDENTITY, shared_ptr<IDeviceHandler>(new DeviceHandler<Identity>(DEVTYPE_IDENTITY))));
    deviceHandlers.insert(make_pair(DEVTYPE_TYPEC_ALTMODE, shared_ptr<IDeviceHandler>(new DeviceHandler<AlternateMode>(DEVTYPE_TYPEC_ALTMODE))));
    deviceHandlers.insert(make_pair(DEVTYPE_DP, shared_ptr<IDeviceHandler>(new DeviceHandler<DisplayPort>(DEVTYPE_DP))));
    deviceHandlers.insert(make_pair(DEVTYPE_USB_POWER_DELIVERY, shared_ptr<IDeviceHandler>(new DeviceHandler<PowerDelivery>(DEVTYPE_USB_POWER_DELIVERY))));
    deviceHandlers.insert(make_pair(PDO_SOURCE_FIXED, shared_ptr<IDeviceHandler>(new DeviceHandler<SourceFixedSupply>(DEVTYPE_PDO))));
    deviceHandlers.insert(make_pair(PDO_SOURCE_VARIABLE, shared_ptr<IDeviceHandler>(new DeviceHandler<SourceVariableSupply>(DEVTYPE_PDO))));
    deviceHandlers.insert(make_pair(PDO_SOURCE_BATTERY, shared_ptr<IDeviceHandler>(new DeviceHandler<SourceBattery>(DEVTYPE_PDO))));
    deviceHandlers.insert(make_pair(PDO_SOURCE_PPS, shared_ptr<IDeviceHandler>(new DeviceHandler<SourcePPS>(DEVTYPE_PDO))));
    deviceHandlers.insert(make_pair(PDO_SINK_FIXED, shared_ptr<IDeviceHandler>(new DeviceHandler<SinkFixedSupply>(DEVTYPE_PDO))));
    deviceHandlers.insert(make_pair(PDO_SINK_VARIABLE, shared_ptr<IDeviceHandler>(new DeviceHandler<SinkVariableSupply>(DEVTYPE_PDO))));
    deviceHandlers.insert(make_pair(PDO_SINK_BATTERY, shared_ptr<IDeviceHandler>(new DeviceHandler<SinkBattery>(DEVTYPE_PDO))));
    deviceHandlers.insert(make_pair(PDO_SINK_PPS, shared_ptr<IDeviceHandler>(new DeviceHandler<SinkPPS>(DEVTYPE_PDO))));
};

void Manager::processUdevEvent(UdevEvent* pUE) {
    string type = pUE->getDevAttribute(DEVTYPE);
    if (type == DEVTYPE_PDO) {
        string path = pUE->getDevAttribute(DEVPATH);
        if (path.find("source_capabilities") != string::npos) {
            type += "_source";
        }
        else if (path.find("sink_capabilities") != string::npos) {
            type += "_sink";
        }
        else {
            cout << "invalid path : " << path << endl;
        }
        
        if (path.find("fixed_supply") != string::npos) {
            type += "_fixed";
        }
        else if (path.find("variable_supply") != string::npos) {
            type += "_variable";
        }
        else if (path.find("battery") != string::npos) {
            type += "_battery";
        }
        else if (path.find("pps") != string::npos) {
            type += "_pps";
        }
        else {
            cout << "invalid path : " << path << endl;
        }

    }
    if (deviceHandlers.find(type) != deviceHandlers.end()) {
        deviceHandlers[type]->processUdevEvent(pUE);
    }
}

bool Manager::get(string path, string type) {
    string devpath = getRealPath(path);
    if (devpath.empty()) {
        return false;
    }
    devpath += "/";
    deviceHandlers[type]->get(devpath);
    return true;
}

bool Manager::getPort(int portIdx) {
    return get(string(ROOT_PATH) + "/port" + to_string(portIdx), DEVTYPE_TYPEC_PORT);
}

bool Manager::getPartner(int portIdx) {
    return get(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/port" + to_string(portIdx) + "-partner", DEVTYPE_TYPEC_PARTNER);
}

bool Manager::getPartnerIdentity(int portIdx) {
    return get(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/port" + to_string(portIdx) + "-partner/identity", DEVTYPE_IDENTITY);
}

bool Manager::getCable(int portIdx) {
    return get(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/port" + to_string(portIdx) + "-cable", DEVTYPE_TYPEC_CABLE);
}

bool Manager::getCableIdentity(int portIdx) {
    return get(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/port" + to_string(portIdx) + "-cable/identity", DEVTYPE_IDENTITY);
}

bool Manager::getPlug(int portIdx, int plugIdx) {
    return get(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/port" + to_string(portIdx) + "-plug" + to_string(plugIdx), DEVTYPE_TYPEC_PLUG);
}

bool Manager::getAltMode(int portIdx, int modeIdx) {
    return get(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/mode" + to_string(modeIdx), DEVTYPE_TYPEC_ALTMODE);
}

bool Manager::getDP(int portIdx, int modeIdx) {
    return get(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/mode" + to_string(modeIdx) + "/displayport", DEVTYPE_DP);
}

bool Manager::getPowerDeliverySinkFixed(int portIdx) {
    return get(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/usb_power_delivery/sink_capabilities/1:fixed_supply", PDO_SINK_FIXED);
}

bool Manager::getPowerDeliverySinkVariable(int portIdx) {
    return get(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/usb_power_delivery/sink_capabilities/2:variable_supply", PDO_SINK_VARIABLE);
}

bool Manager::getPowerDeliverySinkBattery(int portIdx) {
    return get(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/usb_power_delivery/sink_capabilities/3:battery", PDO_SINK_BATTERY);
}

bool Manager::getPowerDeliverySinkPPS(int portIdx) {
    return get(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/usb_power_delivery/sink_capabilities/4:pps", PDO_SINK_PPS);
}

bool Manager::getPowerDeliverySink(int portIdx) {
    bool ret = getPowerDeliverySinkFixed(portIdx);
    ret = ret && getPowerDeliverySinkVariable(portIdx);
    ret = ret && getPowerDeliverySinkBattery(portIdx);
    ret = ret && getPowerDeliverySinkPPS(portIdx);
    return ret;
}

bool Manager::getPowerDeliverySourceFixed(int portIdx) {
    return get(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/usb_power_delivery/source_capabilities/1:fixed_supply", PDO_SOURCE_FIXED);
}

bool Manager::getPowerDeliverySourceVariable(int portIdx) {
    return get(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/usb_power_delivery/source_capabilities/2:variable_supply", PDO_SOURCE_VARIABLE);
}

bool Manager::getPowerDeliverySourceBattery(int portIdx) {
    return get(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/usb_power_delivery/source_capabilities/3:battery", PDO_SOURCE_BATTERY);
}

bool Manager::getPowerDeliverySourcePPS(int portIdx) {
    return get(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/usb_power_delivery/source_capabilities/4:pps", PDO_SOURCE_PPS);
}

bool Manager::getPowerDeliverySource(int portIdx) {
    bool ret = getPowerDeliverySourceFixed(portIdx);
    ret = ret && getPowerDeliverySourceVariable(portIdx);
    ret = ret && getPowerDeliverySourceBattery(portIdx);
    ret = ret && getPowerDeliverySourcePPS(portIdx);
    return ret;
}

bool Manager::getPowerDelivery(int portIdx) {
    bool ret = get(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/usb_power_delivery", DEVTYPE_USB_POWER_DELIVERY);
    ret = ret && getPowerDeliverySource(portIdx);
    ret = ret && getPowerDeliverySink(portIdx);
    return ret;
}

bool Manager::getWithPath(string p){
    if (!fs::exists(p)) {
        return false;
    }
    if (!fs::is_directory(p)) {
        return false;
    }
    if (p[p.length()-1] == '/') {
        p=p.substr(0,p.length()-1);
    }
    fs::path devpath(p);
    string filename = devpath.filename();
    if (filename.find("displayport") != string::npos) {
        return Manager::get(devpath.string(), DEVTYPE_DP);
    }
    else if (filename.find("partner") != string::npos) {
        return Manager::get(devpath.string(), DEVTYPE_TYPEC_PARTNER);
    }
    else if (filename.find("cable") != string::npos) {
        return Manager::get(devpath.string(), DEVTYPE_TYPEC_CABLE);
    }
    else if (filename.find("plug") != string::npos) {
        return Manager::get(devpath.string(), DEVTYPE_TYPEC_PLUG);
    }
    else if (filename.find("port") != string::npos) {
        return Manager::get(devpath.string(), DEVTYPE_TYPEC_PORT);
    }
    else if (filename.find("identity") != string::npos) {
        return Manager::get(devpath.string(), DEVTYPE_IDENTITY);
    }
    else if (filename.find("mode") != string::npos) {
        return Manager::get(devpath.string(), DEVTYPE_TYPEC_ALTMODE);
    }
    else if (filename.find("usb_power_delivery") != string::npos) {
        return Manager::get(devpath.string(), DEVTYPE_USB_POWER_DELIVERY);
    }
    else if (devpath.parent_path().filename().string().find("source_capabilities") != string::npos) {
        if (filename.find("1:fixed_supply") != string::npos) {
            return Manager::get(devpath.string(), PDO_SOURCE_FIXED);
        }
        else if (filename.find("2:variable_supply") != string::npos) {
            return Manager::get(devpath.string(), PDO_SOURCE_VARIABLE);
        }
        else if (filename.find("3:battery") != string::npos) {
            return Manager::get(devpath.string(), PDO_SOURCE_BATTERY);
        }
        else if (filename.find("4:pps") != string::npos) {
            return Manager::get(devpath.string(), PDO_SOURCE_PPS);
        }
        else {
            return false;
        }
    }
    else if (devpath.parent_path().filename().string().find("sink_capabilities") != string::npos) {
        if (filename.find("1:fixed_supply") != string::npos) {
            return Manager::get(devpath.string(), PDO_SINK_FIXED);
        }
        else if (filename.find("2:variable_supply") != string::npos) {
            return Manager::get(devpath.string(), PDO_SINK_VARIABLE);
        }
        else if (filename.find("3:battery") != string::npos) {
            return Manager::get(devpath.string(), PDO_SINK_BATTERY);
        }
        else if (filename.find("4:pps") != string::npos) {
            return Manager::get(devpath.string(), PDO_SINK_PPS);
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

void Manager::getAll() {
    for (auto const& dir_entry : fs::recursive_directory_iterator(ROOT_PATH, fs::directory_options::follow_directory_symlink)) {
        if (fs::is_directory(dir_entry)) {
            if (dir_entry.path().filename() == "source_capabilities")
                continue;
            if (dir_entry.path().filename() == "sink_capabilities")
                continue;
            getWithPath(dir_entry.path().string());
        }
    }
}


Json::Value Manager::getList(string type) {
    Json::Value root;
    if (type.empty()) {
        for (const auto& [key, value] : deviceHandlers) {
            root[key] = value->getList();
        }
    }
    else {
        if (type == DEVTYPE_PDO) {
            root[PDO_SOURCE_FIXED] = deviceHandlers[PDO_SOURCE_FIXED]->getList();
            root[PDO_SOURCE_VARIABLE] = deviceHandlers[PDO_SOURCE_VARIABLE]->getList();
            root[PDO_SOURCE_BATTERY] = deviceHandlers[PDO_SOURCE_BATTERY]->getList();
            root[PDO_SOURCE_PPS] = deviceHandlers[PDO_SOURCE_PPS]->getList();
            root[PDO_SINK_FIXED] = deviceHandlers[PDO_SINK_FIXED]->getList();
            root[PDO_SINK_VARIABLE] = deviceHandlers[PDO_SINK_VARIABLE]->getList();
            root[PDO_SINK_BATTERY] = deviceHandlers[PDO_SINK_BATTERY]->getList();
            root[PDO_SINK_PPS] = deviceHandlers[PDO_SINK_PPS]->getList();
        }
        else if (deviceHandlers.find(type) != deviceHandlers.end()) {
            root[type] = deviceHandlers[type]->getList();
        }
    }
    return root;
}

bool Manager::setWithPath(string p, string attr, string value){
    if (!fs::exists(p)) {
        return false;
    }
    if (!fs::is_directory(p)) {
        return false;
    }
    if (p[p.length()-1] == '/') {
        p=p.substr(0,p.length()-1);
    }
    fs::path devpath(p);
    string filename = devpath.filename();
    if (filename.find("displayport") != string::npos) {
        return Manager::set(devpath.string(), DEVTYPE_DP, attr, value);
    }
    else if (filename.find("partner") != string::npos) {
        return Manager::set(devpath.string(), DEVTYPE_TYPEC_PARTNER, attr, value);
    }
    else if (filename.find("cable") != string::npos) {
        return Manager::set(devpath.string(), DEVTYPE_TYPEC_CABLE, attr, value);
    }
    else if (filename.find("plug") != string::npos) {
        return Manager::set(devpath.string(), DEVTYPE_TYPEC_PLUG, attr, value);
    }
    else if (filename.find("port") != string::npos) {
        return Manager::set(devpath.string(), DEVTYPE_TYPEC_PORT, attr, value);
    }
    else if (filename.find("identity") != string::npos) {
        return Manager::set(devpath.string(), DEVTYPE_IDENTITY, attr, value);
    }
    else if (filename.find("mode") != string::npos) {
        return Manager::set(devpath.string(), DEVTYPE_TYPEC_ALTMODE, attr, value);
    }
    else if (filename.find("usb_power_delivery") != string::npos) {
        return Manager::set(devpath.string(), DEVTYPE_USB_POWER_DELIVERY, attr, value);
    }
    else if (devpath.parent_path().filename().string().find("source_capabilities") == string::npos) {
        if (filename.find("1:fixed_supply") != string::npos) {
            return Manager::set(devpath.string(), PDO_SOURCE_FIXED, attr, value);
        }
        else if (filename.find("2:variable_supply") != string::npos) {
            return Manager::set(devpath.string(), PDO_SOURCE_VARIABLE, attr, value);
        }
        else if (filename.find("3:battery") != string::npos) {
            return Manager::set(devpath.string(), PDO_SOURCE_BATTERY, attr, value);
        }
        else if (filename.find("4:pps") != string::npos) {
            return Manager::set(devpath.string(), PDO_SOURCE_PPS, attr, value);
        }
        else {
            return false;
        }
    }
    else if (devpath.parent_path().filename().string().find("sink_capabilities") == string::npos) {
        if (filename.find("1:fixed_supply") != string::npos) {
            return Manager::set(devpath.string(), PDO_SINK_FIXED, attr, value);
        }
        else if (filename.find("2:variable_supply") != string::npos) {
            return Manager::set(devpath.string(), PDO_SINK_VARIABLE, attr, value);
        }
        else if (filename.find("3:battery") != string::npos) {
            return Manager::set(devpath.string(), PDO_SINK_BATTERY, attr, value);
        }
        else if (filename.find("4:pps") != string::npos) {
            return Manager::set(devpath.string(), PDO_SOURCE_PPS, attr, value);
        }
        else {
            return false;
        }
    }
    return false;
}

bool Manager::set(string path, string type, string attr, string value) {
    string devpath = getRealPath(path);
    if (devpath.empty()) {
        return false;
    }
    devpath += "/";
    deviceHandlers[type]->set(devpath, attr, value);
    return true;
}

bool Manager::setPort(int portIdx, string attr, string value) {
    return set(string(ROOT_PATH) + "/port" + to_string(portIdx), DEVTYPE_TYPEC_PORT, attr, value);
}

bool Manager::setPartner(int portIdx, string attr, string value) {
    return set(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/port" + to_string(portIdx) + "-partner", DEVTYPE_TYPEC_PARTNER, attr, value);
}

bool Manager::setPartnerIdentity(int portIdx, string attr, string value) {
    return set(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/port" + to_string(portIdx) + "-partner/identity", DEVTYPE_IDENTITY, attr, value);
}

bool Manager::setCable(int portIdx, string attr, string value) {
    return set(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/port" + to_string(portIdx) + "-cable", DEVTYPE_TYPEC_CABLE, attr, value);
}

bool Manager::setCableIdentity(int portIdx, string attr, string value) {
    return set(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/port" + to_string(portIdx) + "-cable/identity", DEVTYPE_IDENTITY, attr, value);
}

bool Manager::setPlug(int portIdx, int plugIdx, string attr, string value) {
    return set(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/port" + to_string(portIdx) + "-plug" + to_string(plugIdx), DEVTYPE_TYPEC_PLUG, attr, value);
}

bool Manager::setAltMode(int portIdx, int modeIdx, string attr, string value) {
    return set(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/mode" + to_string(modeIdx), DEVTYPE_TYPEC_ALTMODE, attr, value);
}

bool Manager::setDP(int portIdx, int modeIdx, string attr, string value) {
    return set(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/mode" + to_string(modeIdx) + "/displayport", DEVTYPE_DP, attr, value);
}

bool Manager::setPowerDeliverySinkFixed(int portIdx, string attr, string value) {
    return set(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/usb_power_delivery/sink_capabilities/1:fixed_supply", PDO_SINK_FIXED, attr, value);
}

bool Manager::setPowerDeliverySinkVariable(int portIdx, string attr, string value) {
    return set(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/usb_power_delivery/sink_capabilities/2:variable_supply", PDO_SINK_VARIABLE, attr, value);
}

bool Manager::setPowerDeliverySinkBattery(int portIdx, string attr, string value) {
    return set(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/usb_power_delivery/sink_capabilities/3:battery", PDO_SINK_BATTERY, attr, value);
}

bool Manager::setPowerDeliverySinkPPS(int portIdx, string attr, string value) {
    return set(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/usb_power_delivery/sink_capabilities/4:pps", PDO_SINK_PPS, attr, value);
}

bool Manager::setPowerDeliverySourceFixed(int portIdx, string attr, string value) {
    return set(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/usb_power_delivery/source_capabilities/1:fixed_supply", PDO_SOURCE_FIXED, attr, value);
}

bool Manager::setPowerDeliverySourceVariable(int portIdx, string attr, string value) {
    return set(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/usb_power_delivery/source_capabilities/2:variable_supply", PDO_SOURCE_VARIABLE, attr, value);
}

bool Manager::setPowerDeliverySourceBattery(int portIdx, string attr, string value) {
    return set(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/usb_power_delivery/source_capabilities/3:battery", PDO_SOURCE_BATTERY, attr, value);
}

bool Manager::setPowerDeliverySourcePPS(int portIdx, string attr, string value) {
    return set(string(ROOT_PATH) + "/port" + to_string(portIdx) + "/usb_power_delivery/source_capabilities/4:pps", PDO_SOURCE_PPS, attr, value);
}

/*
void testUdevEvent() {
    shared_ptr<UdevEvent> event = make_shared<UdevEvent>();
    event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/"));
    event->mdeviceInfo.insert(make_pair(DEVTYPE,DEVTYPE_TYPEC_PORT));
    event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
    Manager::instance()->processUdevEvent(event.get());
    event->mdeviceInfo.clear();

    event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/port0-partner/"));
    event->mdeviceInfo.insert(make_pair(DEVTYPE,DEVTYPE_TYPEC_PARTNER));
    event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
    Manager::instance()->processUdevEvent(event.get());
    event->mdeviceInfo.clear();

    event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/port0-partner/identity/"));
    event->mdeviceInfo.insert(make_pair(DEVTYPE,DEVTYPE_IDENTITY));
    event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
    Manager::instance()->processUdevEvent(event.get());
    event->mdeviceInfo.clear();
    
    event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/port0-cable/"));
    event->mdeviceInfo.insert(make_pair(DEVTYPE,DEVTYPE_TYPEC_CABLE));
    event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
    Manager::instance()->processUdevEvent(event.get());
    event->mdeviceInfo.clear();

    event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/port0-cable/identity/"));
    event->mdeviceInfo.insert(make_pair(DEVTYPE,DEVTYPE_IDENTITY));
    event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
    Manager::instance()->processUdevEvent(event.get());
    event->mdeviceInfo.clear();

    event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/port0-cable/identity/"));
    event->mdeviceInfo.insert(make_pair(DEVTYPE,"identity"));
    event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_CHANGE));
    Manager::instance()->processUdevEvent(event.get());
    event->mdeviceInfo.clear();
    
    event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/port0-cable/"));
    event->mdeviceInfo.insert(make_pair(DEVTYPE,DEVTYPE_TYPEC_CABLE));
    event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_REMOVE));
    Manager::instance()->processUdevEvent(event.get());
    event->mdeviceInfo.clear();

    event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/port0-plug0/"));
    event->mdeviceInfo.insert(make_pair(DEVTYPE,DEVTYPE_TYPEC_PLUG));
    event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
    Manager::instance()->processUdevEvent(event.get());
    event->mdeviceInfo.clear();

    event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/bus/typec/devices/port0/mode1/"));
    event->mdeviceInfo.insert(make_pair(DEVTYPE,DEVTYPE_TYPEC_ALTMODE));
    event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
    Manager::instance()->processUdevEvent(event.get());
    event->mdeviceInfo.clear();

    event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/bus/typec/devices/port0/mode1/displayport/"));
    event->mdeviceInfo.insert(make_pair(DEVTYPE,DEVTYPE_DP));
    event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
    Manager::instance()->processUdevEvent(event.get());
    event->mdeviceInfo.clear();
    
    event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/usb_power_delivery/"));
    event->mdeviceInfo.insert(make_pair(DEVTYPE,DEVTYPE_USB_POWER_DELIVERY));
    event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
    Manager::instance()->processUdevEvent(event.get());
    event->mdeviceInfo.clear();

    event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/usb_power_delivery/source_capabilities/1:fixed_supply/"));
    event->mdeviceInfo.insert(make_pair(DEVTYPE,DEVTYPE_PDO));
    event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
    Manager::instance()->processUdevEvent(event.get());
    event->mdeviceInfo.clear();

    event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/usb_power_delivery/source_capabilities/2:variable_supply/"));
    event->mdeviceInfo.insert(make_pair(DEVTYPE,DEVTYPE_PDO));
    event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
    Manager::instance()->processUdevEvent(event.get());
    event->mdeviceInfo.clear();

    event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/usb_power_delivery/source_capabilities/3:battery/"));
    event->mdeviceInfo.insert(make_pair(DEVTYPE,DEVTYPE_PDO));
    event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
    Manager::instance()->processUdevEvent(event.get());
    event->mdeviceInfo.clear();

    event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/usb_power_delivery/source_capabilities/4:pps/"));
    event->mdeviceInfo.insert(make_pair(DEVTYPE,DEVTYPE_PDO));
    event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
    Manager::instance()->processUdevEvent(event.get());
    event->mdeviceInfo.clear();

    event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/usb_power_delivery/sink_capabilities/1:fixed_supply/"));
    event->mdeviceInfo.insert(make_pair(DEVTYPE,DEVTYPE_PDO));
    event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
    Manager::instance()->processUdevEvent(event.get());
    event->mdeviceInfo.clear();

    event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/usb_power_delivery/sink_capabilities/2:variable_supply/"));
    event->mdeviceInfo.insert(make_pair(DEVTYPE,DEVTYPE_PDO));
    event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
    Manager::instance()->processUdevEvent(event.get());
    event->mdeviceInfo.clear();

    event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/usb_power_delivery/sink_capabilities/3:battery/"));
    event->mdeviceInfo.insert(make_pair(DEVTYPE,DEVTYPE_PDO));
    event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
    Manager::instance()->processUdevEvent(event.get());
    event->mdeviceInfo.clear();

    event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/usb_power_delivery/sink_capabilities/4:pps/"));
    event->mdeviceInfo.insert(make_pair(DEVTYPE,DEVTYPE_PDO));
    event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
    Manager::instance()->processUdevEvent(event.get());
    event->mdeviceInfo.clear();

    cout << Manager::instance()->getList().toStyledString() << endl;
    cout << Manager::instance()->getList(DEVTYPE_PDO).toStyledString() << endl;
    //cout << Manager::instance()->getList(DEVTYPE_TYPEC_CABLE).toStyledString() << endl;
}

void testInterfaces() {
    if (Manager::instance()->getPort(0)) {
        cout << Manager::instance()->getList(DEVTYPE_TYPEC_PORT).toStyledString() << endl;
    }
    if (Manager::instance()->getPartner(0)) {
        cout << Manager::instance()->getList(DEVTYPE_TYPEC_PARTNER).toStyledString() << endl;
    }
    if (Manager::instance()->getPartnerIdentity(0)) {
        cout << Manager::instance()->getList(DEVTYPE_IDENTITY).toStyledString() << endl;
    }
    if (Manager::instance()->getCable(0)) {
        cout << Manager::instance()->getList(DEVTYPE_TYPEC_CABLE).toStyledString() << endl;
    }
    if (Manager::instance()->getCableIdentity(0)) {
        cout << Manager::instance()->getList(DEVTYPE_IDENTITY).toStyledString() << endl;
    }
    if (Manager::instance()->getPlug(0,0)) {
        cout << Manager::instance()->getList(DEVTYPE_TYPEC_PLUG).toStyledString() << endl;
    }
    if (Manager::instance()->getPowerDelivery(0)) {
        cout << Manager::instance()->getList(DEVTYPE_PDO).toStyledString() << endl;
    }
    if (Manager::instance()->getAltMode(0,1)) {
        cout << Manager::instance()->getList(DEVTYPE_TYPEC_ALTMODE).toStyledString() << endl;
    }
    if (Manager::instance()->getDP(0,1)) {
        cout << Manager::instance()->getList(DEVTYPE_DP).toStyledString() << endl;
    }
}

void testInterfacesPath() {
    if (Manager::instance()->getWithPath("./sys/class/typec/port0/")) {
        cout << Manager::instance()->getList(DEVTYPE_TYPEC_PORT).toStyledString() << endl;
    }
    if (Manager::instance()->getWithPath("./sys/class/typec/port0/port0-partner/")) {
        cout << Manager::instance()->getList(DEVTYPE_TYPEC_PARTNER).toStyledString() << endl;
    }
    if (Manager::instance()->getWithPath("./sys/class/typec/port0/port0-partner/identity/")) {
        cout << Manager::instance()->getList(DEVTYPE_IDENTITY).toStyledString() << endl;
    }
    if (Manager::instance()->getWithPath("./sys/class/typec/port0/port0-cable/")) {
        cout << Manager::instance()->getList(DEVTYPE_TYPEC_CABLE).toStyledString() << endl;
    }
    if (Manager::instance()->getWithPath("./sys/class/typec/port0/port0-cable/identity/")) {
        cout << Manager::instance()->getList(DEVTYPE_IDENTITY).toStyledString() << endl;
    }
    if (Manager::instance()->getWithPath("./sys/class/typec/port0/port0-plug0/")) {
        cout << Manager::instance()->getList(DEVTYPE_TYPEC_PLUG).toStyledString() << endl;
    }
    if (Manager::instance()->getWithPath("./sys/bus/typec/devices/port0/mode1/")) {
        cout << Manager::instance()->getList(DEVTYPE_TYPEC_ALTMODE).toStyledString() << endl;
    }
    if (Manager::instance()->getWithPath("./sys/bus/typec/devices/port0/mode1/displayport/")) {
        cout << Manager::instance()->getList(DEVTYPE_DP).toStyledString() << endl;
    }
    if (Manager::instance()->getWithPath("./sys/class/typec/port0/usb_power_delivery/")) {
        cout << Manager::instance()->getList(DEVTYPE_USB_POWER_DELIVERY).toStyledString() << endl;
    }
    if (Manager::instance()->getWithPath("./sys/class/typec/port0/usb_power_delivery/source_capabilities/1:fixed_supply/")) {
        cout << Manager::instance()->getList(DEVTYPE_PDO).toStyledString() << endl;
    }
    if (Manager::instance()->getWithPath("./sys/class/typec/port0/usb_power_delivery/source_capabilities/2:variable_supply/")) {
        cout << Manager::instance()->getList(DEVTYPE_PDO).toStyledString() << endl;
    }
    if (Manager::instance()->getWithPath("./sys/class/typec/port0/usb_power_delivery/source_capabilities/3:battery/")) {
        cout << Manager::instance()->getList(DEVTYPE_PDO).toStyledString() << endl;
    }
    if (Manager::instance()->getWithPath("./sys/class/typec/port0/usb_power_delivery/source_capabilities/4:pps/")) {
        cout << Manager::instance()->getList(DEVTYPE_PDO).toStyledString() << endl;
    }
    if (Manager::instance()->getWithPath("./sys/class/typec/port0/usb_power_delivery/sink_capabilities/1:fixed_supply/")) {
        cout << Manager::instance()->getList(DEVTYPE_PDO).toStyledString() << endl;
    }
    if (Manager::instance()->getWithPath("./sys/class/typec/port0/usb_power_delivery/sink_capabilities/2:variable_supply/")) {
        cout << Manager::instance()->getList(DEVTYPE_PDO).toStyledString() << endl;
    }
    if (Manager::instance()->getWithPath("./sys/class/typec/port0/usb_power_delivery/sink_capabilities/3:battery/")) {
        cout << Manager::instance()->getList(DEVTYPE_PDO).toStyledString() << endl;
    }
    if (Manager::instance()->getWithPath("./sys/class/typec/port0/usb_power_delivery/sink_capabilities/4:pps/")) {
        cout << Manager::instance()->getList(DEVTYPE_PDO).toStyledString() << endl;
    }
}
*/
int main() {
    UdevListener::instance()->initListener();

    //testUdevEvent();
    //testInterfaces();
    //testInterfacesPath();
    Manager::instance()->getAll();
    cout << Manager::instance()->getList().toStyledString() << endl;
    /*
    Manager::instance()->setWithPath("./sys/bus/typec/devices/port0/mode1/displayport/", "configuration", "[USB] source sink\n");
    Manager::instance()->setWithPath("./sys/bus/typec/devices/port0/mode1/displayport/", "pin_assignment", "A [B] C\n");
    //Manager::instance()->setWithPath("./sys/bus/typec/devices/port0/mode1/displayport/", "abc", "A [B] C\n");
    //Manager::instance()->setWithPath("./sys/bus/typec/devices/port0/mode1/displayport/", "configuration", "abc");
    Manager::instance()->setWithPath("./sys/bus/typec/devices/port0/mode1/", "active", "yes\n");
    Manager::instance()->setWithPath("./sys/class/typec/port0/", "data_role", "[host]\n");
    Manager::instance()->setWithPath("./sys/class/typec/port0/", "power_role", "[source]\n");
    Manager::instance()->setWithPath("./sys/class/typec/port0/", "port_type", "[source]\n");
    Manager::instance()->setWithPath("./sys/class/typec/port0/", "vconn_source", "no\n");
    Manager::instance()->setWithPath("./sys/class/typec/port0/", "preferred_role", "sink\n");
    Manager::instance()->getAll();
    cout << Manager::instance()->getList().toStyledString() << endl;
    Manager::instance()->setWithPath("./sys/bus/typec/devices/port0/mode1/displayport/", "configuration", "USB [source] sink\n");
    Manager::instance()->setWithPath("./sys/bus/typec/devices/port0/mode1/displayport/", "pin_assignment", "[C] D\n");
    Manager::instance()->setWithPath("./sys/bus/typec/devices/port0/mode1/", "active", "no\n");
    Manager::instance()->setWithPath("./sys/class/typec/port0/", "data_role", "[device]\n");
    Manager::instance()->setWithPath("./sys/class/typec/port0/", "power_role", "source [sink]\n");
    Manager::instance()->setWithPath("./sys/class/typec/port0/", "port_type", "dual [source] sink\n");
    Manager::instance()->setWithPath("./sys/class/typec/port0/", "vconn_source", "yes\n");
    Manager::instance()->setWithPath("./sys/class/typec/port0/", "preferred_role", "source\n");
    Manager::instance()->getAll();
    cout << Manager::instance()->getList().toStyledString() << endl;
    */
    Manager::instance()->setPort(0, "data_role", "[host]\n");
    Manager::instance()->setPort(0, "power_role", "[source]\n");
    Manager::instance()->setPort(0, "port_type", "[source]\n");
    Manager::instance()->setPort(0, "vconn_source", "no\n");
    Manager::instance()->setPort(0, "preferred_role", "sink\n");
    //Manager::instance()->setPartner(0, "accessory_mode", "none\n");
    //Manager::instance()->setPartnerIdentity(0, "product_type_vdo1", "0x00000000\n");
    //Manager::instance()->setCable(0, "plug_type", "unkown\n");
    //Manager::instance()->setCableIdentity(0, "product_type_vdo1", "0x00000000\n");
    //Manager::instance()->setPlug(0, 0, "number_of_alternate_modes", "1");
    Manager::instance()->setAltMode(0, 1, "active", "yes\n");
    Manager::instance()->setDP(0, 1, "configuration", "[USB] source sink\n");
    Manager::instance()->setDP(0, 1, "pin_assignment", "A [B] C\n");
    //Manager::instance()->setPowerDeliverySinkFixed(0, "dual_role_power", "yes\n");
    //Manager::instance()->setPowerDeliverySinkVariable(0, "operational_current", "4000mA\n");
    //Manager::instance()->setPowerDeliverySinkBattery(0, "operational_power", "200000mW\n");
    //Manager::instance()->setPowerDeliverySinkPPS(0, "maximum_voltage", "30000mV\n");
    //Manager::instance()->setPowerDeliverySourceFixed(0, "dual_role_power", "no\n");
    //Manager::instance()->setPowerDeliverySourceVariable(0, "maximum_current", "4000mA\n");
    //Manager::instance()->setPowerDeliverySourceBattery(0, "maximum_power", "200000mW\n");
    //Manager::instance()->setPowerDeliverySourcePPS(0, "maximum_voltage", "30000mV\n");
    Manager::instance()->getAll();
    cout << Manager::instance()->getList().toStyledString() << endl;
    Manager::instance()->setPort(0, "data_role", "[device]\n");
    Manager::instance()->setPort(0, "power_role", "source [sink]\n");
    Manager::instance()->setPort(0, "port_type", "dual [source] sink\n");
    Manager::instance()->setPort(0, "vconn_source", "yes\n");
    Manager::instance()->setPort(0, "preferred_role", "source\n");
    Manager::instance()->setAltMode(0, 1, "active", "no\n");
    Manager::instance()->setDP(0, 1, "configuration", "USB [source] sink\n");
    Manager::instance()->setDP(0, 1, "pin_assignment", "[C] D\n");
    Manager::instance()->getAll();
    cout << Manager::instance()->getList().toStyledString() << endl;

    // null -> empty array
    // fix power delivery capability json

    UdevListener::instance()->stopListener();

    return 0;
}
