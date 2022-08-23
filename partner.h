#ifndef PARTNER_H_
#define PARTNER_H_

#include <algorithm>
#include "sysfs.h"

enum POWER_DELIVERY_REVISIONS {
	POWER_DELIVERY_REVISION_NONE = 60,
	POWER_DELIVERY_REVISION_2_0 = 61,
	POWER_DELIVERY_REVISION_3_0 = 62,
	POWER_DELIVERY_REVISION_3_1 = 63,
};

enum ACCESSORY_MODES {
	ACCESSORY_MODE_NONE = 100,
	ACCESSORY_MODE_ANALOG_AUDIO = 101,
	ACCESSORY_MODE_DEBUG = 102,
};

enum PARTNER_TYPES {
	TYPE_NONE = 110,
	TYPE_UFP_NOT_UFP = 111,
	TYPE_HUB = 112,
	TYPE_UFP_PERIPHERAL = 113,
	TYPE_UFP_PSD = 114, // Power Sink Device
	TYPE_UFP_AMA = 115, // Alternate Mode Adapter
	TYPE_DFP_NOT_DFP = 116,
	TYPE_DFP_HOST = 117,
	TYPE_DFP_POWER_BRICK = 118, // wall charger
};

class Identity {
public:
	bitset<32> id_header;
	bitset<32> cert_stat;
	bitset<32> product;
	bitset<32> product_type_vdo1;
	bitset<32> product_type_vdo2;
	bitset<32> product_type_vdo3;
};

class Partner : public SysFS {
	bool supports_usb_power_delivery;
	POWER_DELIVERY_REVISIONS usb_power_delivery_revision;
	ACCESSORY_MODES accessory_mode;
	PARTNER_TYPES type;
	int number_of_alternate_modes;
	Identity identity;
	map<string, int> mapPowerDeliveryRev;
	map<string, int> mapAccessoryMode;
	map<string, int> mapType;

public:
	Partner(string path);
};

#endif
