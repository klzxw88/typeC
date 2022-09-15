#include "cable.h"
// https://www.kernel.org/doc/Documentation/ABI/testing/sysfs-class-typec

Cable::Cable(string path)
	: SysFS(path) {
	// usb_power_delivery_revision
	SysFSValue<int>* usb_power_delivery_revision = new SysFSValue<int>("usb_power_delivery_revision", false);
	usb_power_delivery_revision->add("2.0\n", POWER_DELIVERY_REVISION_2_0);
	usb_power_delivery_revision->add("3.0\n", POWER_DELIVERY_REVISION_3_0);
	usb_power_delivery_revision->add("3.1\n", POWER_DELIVERY_REVISION_3_1);
	addMap(usb_power_delivery_revision);

	// type
	SysFSValue<int>* type = new SysFSValue<int>("type", false);
	type->add("not_cable\n", CABLE_TYPE_NOT_CABLE);
	type->add("passive\n", CABLE_TYPE_PASSIVE);
	type->add("active\n", CABLE_TYPE_ACTIVE);
	type->add("vpd\n", CABLE_TYPE_VPD);
	addMap(type);

	// plug_type
	SysFSValue<int>* plug_type = new SysFSValue<int>("plug_type", false);
	plug_type->add("unknown", PLUG_TYPE_UNKNOWN);
	plug_type->add("type-a", PLUG_TYPE_A);
	plug_type->add("type-b", PLUG_TYPE_B);
	plug_type->add("type-c", PLUG_TYPE_C);
	plug_type->add("captive", PLUG_TYPE_CAPTIVE);
	addMap(plug_type);

	// number_of_alternate_modes
	SysFSValue<int>* number_of_alternate_modes = new SysFSValue<int>("number_of_alternate_modes", false);
	addMap(number_of_alternate_modes);

	getSysFSAll();
	identity = make_shared<Identity>(devpath+"identity/");
}
/*
int main() {
	Cable* p = new Cable("./sys/class/typec/port0/port0-cable/");
	cout << p->getValue("type") << endl;
	cout << p->getValue("number_of_alternate_modes") << endl;
	cout << p->getValue("usb_power_delivery_revision") << endl;
	cout << p->getValue("plug_type") << endl;
	cout << p->getIdentityValue("id_header") << endl;
	cout << p->getIdentityValue("cert_stat") << endl;
	cout << p->getIdentityValue("product") << endl;
	cout << p->getIdentityValue("product_type_vdo1") << endl;
	cout << p->getIdentityValue("product_type_vdo2") << endl;
	cout << p->getIdentityValue("product_type_vdo3") << endl;
	delete p;
	return 0;
}
*/
