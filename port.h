#ifndef PORT_H_
#define PORT_H_

#include <algorithm>
#include "sysfs.h"
#include "partner.h"

enum DATA_ROLES {
	DATA_ROLE_NONE = 0,
	DATA_ROLE_DEVICE = 1,
	DATA_ROLE_HOST = 2,
	DATA_ROLE_DUAL_HOST = 3,
	DATA_ROLE_DUAL_DEVICE = 4,
};

enum POWER_ROLES {
	POWER_ROLE_NONE = 10,
	POWER_ROLE_SOURCE = 11,
	POWER_ROLE_SINK = 12,
	POWER_ROLE_DUAL_SOURCE = 13,
	POWER_ROLE_DUAL_SINK = 14,
};

enum PORT_TYPES {
	PORT_TYPE_NONE = 20,
	PORT_TYPE_SOURCE = 21,
	PORT_TYPE_SINK = 22,
	PORT_TYPE_DUAL_SOURCE = 23,
	PORT_TYPE_DUAL_SINK = 24,
};

enum POWER_OPERATION_MODES {
	POWER_OPERATION_MODE_NONE = 40,
	POWER_OPERATION_MODE_DEFAULT = 41,
	POWER_OPERATION_MODE_1_5A = 42,
	POWER_OPERATION_MODE_3A = 43,
	POWER_OPERATION_MODE_PD = 44,
};

enum PREFERRED_ROLES {
	PREFERRED_ROLE_NONE = 50,
	PREFERRED_ROLE_SINK = 51,
	PREFERRED_ROLE_SOURCE = 52,
};

enum TYPEC_REVISIONS {
	TYPEC_REVISION_NONE = 70,
	TYPEC_REVISION_1_1 = 71,
	TYPEC_REVISION_1_2 = 72,
	TYPEC_REVISION_1_3 = 73,
	TYPEC_REVISION_1_4 = 74,
	TYPEC_REVISION_2_0 = 75,
};

enum ORIENTATIONS {
	ORIENTATION_NONE = 80,
	ORIENTATION_UNKNOWN = 81,
	ORIENTATION_REVERSE = 82,
	ORIENTATION_NORMAL = 83,
};

class Port : public SysFS {
	Partner *partner;
	//Cable *cable;
	bool vconn_source;
	DATA_ROLES data_role;
	POWER_ROLES power_role;
	PORT_TYPES port_type;
	POWER_OPERATION_MODES power_operation_mode;
	PREFERRED_ROLES preferred_role;
	POWER_DELIVERY_REVISIONS usb_power_delivery_revision;
	TYPEC_REVISIONS usb_typec_revision;
	ORIENTATIONS orientation;
	map<string, int> mapDataRole;
	map<string, int> mapPowerRole;
	map<string, int> mapPortType;
	map<string, int> mapPowerOperationMode;
	map<string, int> mapPreferredRole;
	map<string, int> mapPowerDeliveryRev;
	map<string, int> mapTypeCRev;
	map<string, int> mapOrientation;

public:
	Port(string path);
};

#endif
