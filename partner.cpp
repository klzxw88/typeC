#include "partner.h"

Partner::Partner(string path)
	: SysFS(path),
	type(TYPE_NONE),
	usb_power_delivery_revision(POWER_DELIVERY_REVISION_NONE),
	accessory_mode(ACCESSORY_MODE_NONE),
	supports_usb_power_delivery(false),
	number_of_alternate_modes(0) {
	mapPowerDeliveryRev = {
		{"2.0\n", POWER_DELIVERY_REVISION_2_0},
		{"3.0\n", POWER_DELIVERY_REVISION_3_0},
		{"3.1\n", POWER_DELIVERY_REVISION_3_1},
	};
	mapAccessoryMode = {
		{"none\n", ACCESSORY_MODE_NONE},
		{"analog_audio\n", ACCESSORY_MODE_ANALOG_AUDIO},
		{"debug\n", ACCESSORY_MODE_DEBUG},
	};
	mapType = {
		{"not_ufp\n", TYPE_UFP_NOT_UFP},
		{"hub\n", TYPE_HUB},
		{"peripheral\n", TYPE_UFP_PERIPHERAL},
		{"PSD\n", TYPE_UFP_PSD}, // Power Sink Device
		{"ama\n", TYPE_UFP_AMA}, // Alternate Mode Adapter
		{"not_dfp\n", TYPE_DFP_NOT_DFP},
		{"host\n", TYPE_DFP_HOST},
		{"power_brick\n", TYPE_DFP_POWER_BRICK}, // wall charger
	};

	mapSysFsEnum = {
		{"usb_power_delivery_revision", {bind(&Partner::usb_power_delivery_revision, this), [&](int rev){usb_power_delivery_revision=static_cast<POWER_DELIVERY_REVISIONS>(rev);}, mapPowerDeliveryRev, false}},
		{"accessory_mode", {bind(&Partner::accessory_mode, this), [&](int mode){accessory_mode=static_cast<ACCESSORY_MODES>(mode);}, mapAccessoryMode, false}},
		{"type", {bind(&Partner::type, this), [&](int val){type=static_cast<PARTNER_TYPES>(val);}, mapType, false}},
	};
	mapSysFsBitset = {
		{"id_header", {[&](){return identity.id_header;}, [&](bitset<32> header){identity.id_header=header;}, false}},
		{"cert_stat", {[&](){return identity.cert_stat;}, [&](bitset<32> stat){identity.cert_stat=stat;}, false}},
		{"product", {[&](){return identity.product;}, [&](bitset<32> product){identity.product=product;}, false}},
		{"product_type_vdo1", {[&](){return identity.product_type_vdo1;}, [&](bitset<32> vdo1){identity.product_type_vdo1=vdo1;}, false}},
		{"product_type_vdo2", {[&](){return identity.product_type_vdo2;}, [&](bitset<32> vdo2){identity.product_type_vdo2=vdo2;}, false}},
		{"product_type_vdo3", {[&](){return identity.product_type_vdo3;}, [&](bitset<32> vdo3){identity.product_type_vdo3=vdo3;}, false}},
	};
	mapSysFsBool = {
		{"supports_usb_power_delivery", {bind(&Partner::supports_usb_power_delivery, this), [&](int support){supports_usb_power_delivery=support;}, false}},
	};
	mapSysFsInt = {
		{"number_of_alternate_modes", {bind(&Partner::number_of_alternate_modes, this), [&](int num){number_of_alternate_modes=num;}, false}},
	};

	getSysFSAll();
	getSysFSAll(devpath+"identity/");
}
/*
int main() {
	Partner* p = new Partner("./sys/class/typec/port0/port0-partner/");
	cout << p->getValue("type") << endl;
	cout << p->getValue("number_of_alternate_modes") << endl;
	cout << p->getValue("usb_power_delivery_revision") << endl;
	cout << p->getValue("accessory_mode") << endl;
	cout << p->getValue("supports_usb_power_delivery") << endl;
	cout << p->getValue("id_header") << endl;
	cout << p->getValue("cert_stat") << endl;
	cout << p->getValue("product") << endl;
	cout << p->getValue("product_type_vdo1") << endl;
	cout << p->getValue("product_type_vdo2") << endl;
	cout << p->getValue("product_type_vdo3") << endl;
	return 0;
}
*/
