#include "partner.h"

Partner::Partner(string path)
	: SysFS(path) {
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
	addMap(type);

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

	/*
	mapSysFsBitset = {
		{"id_header", {[&](){return identity.id_header;}, [&](bitset<32> header){identity.id_header=header;}, false}},
		{"cert_stat", {[&](){return identity.cert_stat;}, [&](bitset<32> stat){identity.cert_stat=stat;}, false}},
		{"product", {[&](){return identity.product;}, [&](bitset<32> product){identity.product=product;}, false}},
		{"product_type_vdo1", {[&](){return identity.product_type_vdo1;}, [&](bitset<32> vdo1){identity.product_type_vdo1=vdo1;}, false}},
		{"product_type_vdo2", {[&](){return identity.product_type_vdo2;}, [&](bitset<32> vdo2){identity.product_type_vdo2=vdo2;}, false}},
		{"product_type_vdo3", {[&](){return identity.product_type_vdo3;}, [&](bitset<32> vdo3){identity.product_type_vdo3=vdo3;}, false}},
	};
	*/

	getSysFSAll();
	getSysFSAll(devpath+"identity/");
}

int main() {
	Partner* p = new Partner("./sys/class/typec/port0/port0-partner/");
	cout << p->getValue("type") << endl;
	cout << p->getValue("number_of_alternate_modes") << endl;
	cout << p->getValue("usb_power_delivery_revision") << endl;
	cout << p->getValue("accessory_mode") << endl;
	cout << p->getValue("supports_usb_power_delivery") << endl;
	//cout << p->getValue("id_header") << endl;
	//cout << p->getValue("cert_stat") << endl;
	//cout << p->getValue("product") << endl;
	//cout << p->getValue("product_type_vdo1") << endl;
	//cout << p->getValue("product_type_vdo2") << endl;
	//cout << p->getValue("product_type_vdo3") << endl;
	delete p;
	return 0;
}

