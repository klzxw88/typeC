#ifndef PARTNER_H_
#define PARTNER_H_

#include <algorithm>
#include "common.h"
#include "sysfs.h"
#include "identity.h"

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
	TYPE_DFP_AMC = 119, // Alternate Mode Controller
};

class Partner : public SysFS {
private:
	const string name = "typec_partner";
	shared_ptr<Identity> identity;

public:
	Partner(string path);

	string getIdentityValue(string name) { return identity->getValue(name); };
};

#endif
