#include "manager.h"
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

Manager::Manager(string path) : devpath(path) {
	deviceHandlers.insert(make_pair("typec_port", shared_ptr<IDeviceHandler>(new DeviceHandler<Port>("typec_port"))));
	deviceHandlers.insert(make_pair("typec_partner", shared_ptr<IDeviceHandler>(new DeviceHandler<Partner>("typec_partner"))));
	deviceHandlers.insert(make_pair("typec_cable", shared_ptr<IDeviceHandler>(new DeviceHandler<Cable>("typec_cable"))));
	deviceHandlers.insert(make_pair("typec_plug", shared_ptr<IDeviceHandler>(new DeviceHandler<Plug>("typec_plug"))));
	deviceHandlers.insert(make_pair("identity", shared_ptr<IDeviceHandler>(new DeviceHandler<Identity>("identity"))));
	deviceHandlers.insert(make_pair("typec_alternate_mode", shared_ptr<IDeviceHandler>(new DeviceHandler<AlternateMode>("typec_alternate_mode"))));
	deviceHandlers.insert(make_pair("displayport", shared_ptr<IDeviceHandler>(new DeviceHandler<DisplayPort>("displayport"))));
	deviceHandlers.insert(make_pair("usb_power_delivery", shared_ptr<IDeviceHandler>(new DeviceHandler<PowerDelivery>("usb_power_delivery"))));
	deviceHandlers.insert(make_pair("pdo_source_fixed", shared_ptr<IDeviceHandler>(new DeviceHandler<SourceFixedSupply>("pdo"))));
	deviceHandlers.insert(make_pair("pdo_source_variable", shared_ptr<IDeviceHandler>(new DeviceHandler<SourceVariableSupply>("pdo"))));
	deviceHandlers.insert(make_pair("pdo_source_battery", shared_ptr<IDeviceHandler>(new DeviceHandler<SourceBattery>("pdo"))));
	deviceHandlers.insert(make_pair("pdo_source_pps", shared_ptr<IDeviceHandler>(new DeviceHandler<SourcePPS>("pdo"))));
	deviceHandlers.insert(make_pair("pdo_sink_fixed", shared_ptr<IDeviceHandler>(new DeviceHandler<SinkFixedSupply>("pdo"))));
	deviceHandlers.insert(make_pair("pdo_sink_variable", shared_ptr<IDeviceHandler>(new DeviceHandler<SinkVariableSupply>("pdo"))));
	deviceHandlers.insert(make_pair("pdo_sink_battery", shared_ptr<IDeviceHandler>(new DeviceHandler<SinkBattery>("pdo"))));
	deviceHandlers.insert(make_pair("pdo_sink_pps", shared_ptr<IDeviceHandler>(new DeviceHandler<SinkPPS>("pdo"))));
};

void Manager::processUdevEvent(UdevEvent* pUE) {
	string type = pUE->getDevAttribute(DEVTYPE);
	if (type == "pdo") {
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

Json::Value Manager::getList(string type) {
	Json::Value root;
	if (type.empty()) {
		for (const auto& [key, value] : deviceHandlers) {
			root[key] = value->getList();
		}
	}
	else {
		if (type == "pdo") {
			root["pdo_source_fixed"] = deviceHandlers["pdo_source_fixed"]->getList();
			root["pdo_source_variable"] = deviceHandlers["pdo_source_variable"]->getList();
			root["pdo_source_battery"] = deviceHandlers["pdo_source_battery"]->getList();
			root["pdo_source_pps"] = deviceHandlers["pdo_source_pps"]->getList();
			root["pdo_sink_fixed"] = deviceHandlers["pdo_sink_fixed"]->getList();
			root["pdo_sink_variable"] = deviceHandlers["pdo_sink_variable"]->getList();
			root["pdo_sink_battery"] = deviceHandlers["pdo_sink_battery"]->getList();
			root["pdo_sink_pps"] = deviceHandlers["pdo_sink_pps"]->getList();
		}
		else if (deviceHandlers.find(type) != deviceHandlers.end()) {
			root[type] = deviceHandlers[type]->getList();
		}
	}
	return root;
}

int main() {
	shared_ptr<Manager> m = make_shared<Manager>(ROOT_PATH);
	UdevListener::getInstance()->initListener();

	shared_ptr<UdevEvent> event = make_shared<UdevEvent>();

	event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/"));
	event->mdeviceInfo.insert(make_pair(DEVTYPE,"typec_port"));
	event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
	m->processUdevEvent(event.get());
	event->mdeviceInfo.clear();

	event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/port0-partner/"));
	event->mdeviceInfo.insert(make_pair(DEVTYPE,"typec_partner"));
	event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
	m->processUdevEvent(event.get());
	event->mdeviceInfo.clear();

	event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/port0-partner/identity/"));
	event->mdeviceInfo.insert(make_pair(DEVTYPE,"identity"));
	event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
	m->processUdevEvent(event.get());
	event->mdeviceInfo.clear();
	
	event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/port0-cable/"));
	event->mdeviceInfo.insert(make_pair(DEVTYPE,"typec_cable"));
	event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
	m->processUdevEvent(event.get());
	event->mdeviceInfo.clear();

	event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/port0-cable/identity/"));
	event->mdeviceInfo.insert(make_pair(DEVTYPE,"identity"));
	event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
	m->processUdevEvent(event.get());
	event->mdeviceInfo.clear();
	
	event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/port0-cable/identity/"));
	event->mdeviceInfo.insert(make_pair(DEVTYPE,"identity"));
	event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_CHANGE));
	m->processUdevEvent(event.get());
	event->mdeviceInfo.clear();
/*	
	event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/port0-cable/"));
	event->mdeviceInfo.insert(make_pair(DEVTYPE,"typec_cable"));
	event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_REMOVE));
	m->processUdevEvent(event.get());
	event->mdeviceInfo.clear();
*/
	event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/port0-plug0/"));
	event->mdeviceInfo.insert(make_pair(DEVTYPE,"typec_plug"));
	event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
	m->processUdevEvent(event.get());
	event->mdeviceInfo.clear();
	
	event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/bus/typec/devices/1-1/mode1/"));
	event->mdeviceInfo.insert(make_pair(DEVTYPE,"typec_alternate_mode"));
	event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
	m->processUdevEvent(event.get());
	event->mdeviceInfo.clear();

	event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/bus/typec/devices/1-1/mode1/displayport/"));
	event->mdeviceInfo.insert(make_pair(DEVTYPE,"displayport"));
	event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
	m->processUdevEvent(event.get());
	event->mdeviceInfo.clear();

	event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/usb_power_delivery/"));
	event->mdeviceInfo.insert(make_pair(DEVTYPE,"usb_power_delivery"));
	event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
	m->processUdevEvent(event.get());
	event->mdeviceInfo.clear();

	event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/usb_power_delivery/source_capabilities/1:fixed_supply/"));
	event->mdeviceInfo.insert(make_pair(DEVTYPE,"pdo"));
	event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
	m->processUdevEvent(event.get());
	event->mdeviceInfo.clear();

	event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/usb_power_delivery/source_capabilities/2:variable_supply/"));
	event->mdeviceInfo.insert(make_pair(DEVTYPE,"pdo"));
	event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
	m->processUdevEvent(event.get());
	event->mdeviceInfo.clear();

	event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/usb_power_delivery/source_capabilities/3:battery/"));
	event->mdeviceInfo.insert(make_pair(DEVTYPE,"pdo"));
	event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
	m->processUdevEvent(event.get());
	event->mdeviceInfo.clear();

	event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/usb_power_delivery/source_capabilities/4:pps/"));
	event->mdeviceInfo.insert(make_pair(DEVTYPE,"pdo"));
	event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
	m->processUdevEvent(event.get());
	event->mdeviceInfo.clear();

	event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/usb_power_delivery/sink_capabilities/1:fixed_supply/"));
	event->mdeviceInfo.insert(make_pair(DEVTYPE,"pdo"));
	event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
	m->processUdevEvent(event.get());
	event->mdeviceInfo.clear();

	event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/usb_power_delivery/sink_capabilities/2:variable_supply/"));
	event->mdeviceInfo.insert(make_pair(DEVTYPE,"pdo"));
	event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
	m->processUdevEvent(event.get());
	event->mdeviceInfo.clear();

	event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/usb_power_delivery/sink_capabilities/3:battery/"));
	event->mdeviceInfo.insert(make_pair(DEVTYPE,"pdo"));
	event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
	m->processUdevEvent(event.get());
	event->mdeviceInfo.clear();

	event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/usb_power_delivery/sink_capabilities/4:pps/"));
	event->mdeviceInfo.insert(make_pair(DEVTYPE,"pdo"));
	event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
	m->processUdevEvent(event.get());
	event->mdeviceInfo.clear();

	cout << m->getList().toStyledString() << endl;
	cout << m->getList("pdo").toStyledString() << endl;
	//cout << m->getList("typec_cable").toStyledString() << endl;
	UdevListener::getInstance()->stopListener();

	return 0;
}
