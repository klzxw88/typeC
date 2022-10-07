#ifndef CABLE_H_
#define CABLE_H_

#include <algorithm>
#include "common.h"
#include "sysfs.h"
#include "identity.h"

enum CABLE_TYPES {
	CABLE_TYPE_NOT_CABLE = 120,
	CABLE_TYPE_PASSIVE = 121,
	CABLE_TYPE_ACTIVE = 122,
	CABLE_TYPE_VPD = 123,
};

enum PLUG_TYPES {
	PLUG_TYPE_UNKNOWN = 130,
	PLUG_TYPE_A = 131,
	PLUG_TYPE_B = 132,
	PLUG_TYPE_C = 133,
	PLUG_TYPE_CAPTIVE = 134,
};

class Cable : public SysFS {
private:
	const string name = "typec_cable";
	shared_ptr<Identity> identity;

public:
	Cable(string path);

	string getIdentityValue(string name) { return identity->getValue(name); };
};

#endif
