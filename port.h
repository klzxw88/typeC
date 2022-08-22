#include <algorithm>
#include <filesystem>
#include <map>
#include <string>
#include "sysfs.h"

using namespace placeholders;

namespace fs = std::filesystem;

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

enum VCONN_SOURCE_EN {
	VCONN_SOURCE_NONE = 30,
	VCONN_SOURCE_YES = 31,
	VCONN_SOURCE_NO = 32,
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

enum POWER_DELIVERY_REVISIONS {
	POWER_DELIVERY_REVISION_NONE = 60,
	POWER_DELIVERY_REVISION_2_0 = 61,
	POWER_DELIVERY_REVISION_3_0 = 62,
	POWER_DELIVERY_REVISION_3_1 = 63,
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
	int data_role;
	int power_role;
	int port_type;
	int vconn_source;
	int power_operation_mode;
	int preferred_role;
	int usb_power_delivery_revision;
	int usb_typec_revision;
	int orientation;
	map<string, int> mapDataRole;
	map<string, int> mapPowerRole;
	map<string, int> mapPortType;
	map<string, int> mapVconnSource;
	map<string, int> mapPowerOperationMode;
	map<string, int> mapPreferredRole;
	map<string, int> mapPowerDeliveryRev;
	map<string, int> mapTypeCRev;
	map<string, int> mapOrientation;

public:
	Port(string path);
};
