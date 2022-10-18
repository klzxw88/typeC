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

Manager::Manager(string path) : devpath(path) {
	deviceHandlers.insert(make_pair("typec_port", shared_ptr<IDeviceHandler>(new DeviceHandler<Port>("typec_port"))));
	deviceHandlers.insert(make_pair("typec_partner", shared_ptr<IDeviceHandler>(new DeviceHandler<Partner>("typec_partner"))));
	deviceHandlers.insert(make_pair("typec_cable", shared_ptr<IDeviceHandler>(new DeviceHandler<Cable>("typec_cable"))));
	deviceHandlers.insert(make_pair("typec_plug", shared_ptr<IDeviceHandler>(new DeviceHandler<Plug>("typec_plug"))));
	deviceHandlers.insert(make_pair("identity", shared_ptr<IDeviceHandler>(new DeviceHandler<Identity>("identity"))));
	deviceHandlers.insert(make_pair("typec_alternate_mode", shared_ptr<IDeviceHandler>(new DeviceHandler<AlternateMode>("typec_alternate_mode"))));
	deviceHandlers.insert(make_pair("displayport", shared_ptr<IDeviceHandler>(new DeviceHandler<DisplayPort>("displayport"))));
	deviceHandlers.insert(make_pair("usb_power_delivery", shared_ptr<IDeviceHandler>(new DeviceHandler<PowerDelivery>("usb_power_delivery"))));

	UdevEvent* event = new UdevEvent();
	event->mdeviceInfo.insert(make_pair(DEVPATH,"./sys/class/typec/port0/"));
	event->mdeviceInfo.insert(make_pair(DEVTYPE,"typec_port"));
	event->mdeviceInfo.insert(make_pair(ACTION,DEVICE_ADD));
	
	processUdevEvent(event);
	delete event;
};

void Manager::processUdevEvent(UdevEvent* pUE) {
	string type = pUE->getDevAttribute(DEVTYPE);
	if (deviceHandlers.find(type) != deviceHandlers.end()) {
		deviceHandlers[type]->processUdevEvent(pUE);
	}
}
/*
void Manager::getList(string type) {

	if (deviceHandlers.find(type) != deviceHandlers.end()) {
		deviceHandlers[type]->getList();
	}
}
*/
int main() {
	Manager* m = new Manager(ROOT_PATH);
	delete m;
	return 0;
}
