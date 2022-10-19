#include "partner.h"
// https://www.kernel.org/doc/Documentation/ABI/testing/sysfs-class-typec

Partner::Partner(string path)
	: SysFS(path) {
	// usb_power_delivery_revision
	SysFSValue<int>* usb_power_delivery_revision = new SysFSValue<int>("usb_power_delivery_revision", false);
	usb_power_delivery_revision->add("2.0\n", POWER_DELIVERY_REVISION_2_0);
	usb_power_delivery_revision->add("3.0\n", POWER_DELIVERY_REVISION_3_0);
	usb_power_delivery_revision->add("3.1\n", POWER_DELIVERY_REVISION_3_1);
	addMap(usb_power_delivery_revision);

	// accessory_mode
	SysFSValue<int>* accessory_mode = new SysFSValue<int>("accessory_mode", false);
	accessory_mode->add("none\n", ACCESSORY_MODE_NONE);
	accessory_mode->add("analog_audio\n", ACCESSORY_MODE_ANALOG_AUDIO);
	accessory_mode->add("debug\n", ACCESSORY_MODE_DEBUG);
	addMap(accessory_mode);

	// supports_usb_power_delivery
	SysFSValue<bool>* supports_usb_power_delivery = new SysFSValue<bool>("supports_usb_power_delivery", false);
	addMap(supports_usb_power_delivery);

	// number_of_alternate_modes
	SysFSValue<int>* number_of_alternate_modes = new SysFSValue<int>("number_of_alternate_modes", false);
	addMap(number_of_alternate_modes);

	// type
	SysFSValue<int>* type = new SysFSValue<int>("type", false);
	type->add("not_ufp\n", TYPE_UFP_NOT_UFP);
	type->add("hub\n", TYPE_HUB);
	type->add("peripheral\n", TYPE_UFP_PERIPHERAL);
	type->add("PSD\n", TYPE_UFP_PSD); //Power Sink Device
	type->add("ama\n", TYPE_UFP_AMA); // Alternate Mode Adapter
	type->add("not_dfp\n", TYPE_DFP_NOT_DFP);
	type->add("host\n", TYPE_DFP_HOST);
	type->add("power_brick\n", TYPE_DFP_POWER_BRICK); // wall charger
	type->add("amc\n", TYPE_DFP_AMC); // Alternate Mode Controller
	addMap(type);

	getSysFSAll();
/*
	cout << toJson().toStyledString() << endl;
	cout << sysFStoJson().toStyledString() << endl;
*/
}
/*
int main() {
	Partner* p = new Partner("./sys/class/typec/port0/port0-partner/");
	cout << p->getValue("type") << endl;
	cout << p->getValue("number_of_alternate_modes") << endl;
	cout << p->getValue("usb_power_delivery_revision") << endl;
	cout << p->getValue("accessory_mode") << endl;
	cout << p->getValue("supports_usb_power_delivery") << endl;
	delete p;
	return 0;
}
*/
