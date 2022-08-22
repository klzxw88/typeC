#include "port.h"

Port::Port(string path)
	: SysFS(path), 
	data_role(DATA_ROLE_NONE),
	power_role(POWER_ROLE_NONE),
	port_type(PORT_TYPE_NONE),
	vconn_source(VCONN_SOURCE_NONE),
	power_operation_mode(POWER_OPERATION_MODE_NONE),
	preferred_role(PREFERRED_ROLE_NONE),
	usb_power_delivery_revision(POWER_DELIVERY_REVISION_NONE),
	usb_typec_revision(TYPEC_REVISION_NONE),
	orientation(ORIENTATION_NONE) {
	mapDataRole = {
		{"[device]\n", DATA_ROLE_DEVICE},
		{"[host]\n", DATA_ROLE_HOST},
		{"[host] device\n", DATA_ROLE_DUAL_HOST},
		{"host [device]\n", DATA_ROLE_DUAL_DEVICE},
	};
	mapPowerRole = {
		{"[sink]\n", POWER_ROLE_SINK},
		{"[source]\n", POWER_ROLE_SOURCE},
		{"[source] sink\n", POWER_ROLE_DUAL_SINK},
		{"source [sink]\n", POWER_ROLE_DUAL_SOURCE},
	};
	mapPortType = {
		{"[sink]\n", PORT_TYPE_SINK},
		{"[source]\n", PORT_TYPE_SOURCE},
		{"dual [source] sink\n", PORT_TYPE_DUAL_SOURCE},
		{"dual source [sink]\n", PORT_TYPE_DUAL_SINK},
	};
	mapVconnSource = {
		{"yes\n", VCONN_SOURCE_YES},
		{"no\n", VCONN_SOURCE_NO},
	};
	mapPowerOperationMode = {
		{"default\n", POWER_OPERATION_MODE_DEFAULT},
		{"1.5A\n", POWER_OPERATION_MODE_1_5A},
		{"3.0A\n", POWER_OPERATION_MODE_3A},
		{"usb_power_delivery\n", POWER_OPERATION_MODE_PD},
	};
	mapPreferredRole = {
		{"sink\n", PREFERRED_ROLE_SINK},
		{"source\n", PREFERRED_ROLE_SOURCE},
	};
	mapPowerDeliveryRev = {
		{"2.0\n", POWER_DELIVERY_REVISION_2_0},
		{"3.0\n", POWER_DELIVERY_REVISION_3_0},
		{"3.1\n", POWER_DELIVERY_REVISION_3_1},
	};
	mapTypeCRev = {
		{"1.1\n", TYPEC_REVISION_1_1},
		{"1.2\n", TYPEC_REVISION_1_2},
		{"1.3\n", TYPEC_REVISION_1_3},
		{"1.4\n", TYPEC_REVISION_1_4},
		{"2.0\n", TYPEC_REVISION_2_0},
	};
	mapOrientation = {
		{"unknown\n", ORIENTATION_UNKNOWN},
		{"reverse\n", ORIENTATION_REVERSE},
		{"normal\n", ORIENTATION_NORMAL},
	};

	mapSysFs = {
		{"data_role", {bind(&Port::data_role, this), [&](int role){data_role=role;}, mapDataRole, true}},
		{"power_role", {bind(&Port::power_role, this), [&](int role){power_role=role;}, mapPowerRole, true}},
		{"port_type", {bind(&Port::port_type, this), [&](int type){port_type=type;}, mapPortType, true}},
		{"vconn_source", {bind(&Port::vconn_source, this), [&](int vconn){vconn_source=vconn;}, mapVconnSource, true}},
		{"power_operation_mode", {bind(&Port::power_operation_mode, this), [&](int mode){power_operation_mode=mode;}, mapPowerOperationMode, false}},
		{"preferred_role", {bind(&Port::preferred_role, this), [&](int role){preferred_role=role;}, mapPreferredRole, true}},
		{"usb_power_delivery_revision", {bind(&Port::usb_power_delivery_revision, this), [&](int rev){usb_power_delivery_revision=rev;}, mapPowerDeliveryRev, false}},
		{"usb_typec_revision", {bind(&Port::usb_typec_revision, this), [&](int rev){usb_typec_revision=rev;}, mapTypeCRev, false}},
		{"orientation", {bind(&Port::orientation, this), [&](int ori){orientation=ori;}, mapOrientation, false}},
	};

	getSysFS();
	for (auto p : mapSysFs) {
		getSysFS(p.first);
	}
}

int main() {
	Port* p = new Port("./sys/class/typec/port0/");
	//p->setValue("data_role", DATA_ROLE_DEVICE);
	//p->setValue("power_role", POWER_ROLE_SOURCE);
	//p->setValue("port_type", PORT_TYPE_SINK);
	//p->setValue("vconn_source", VCONN_SOURCE_YES);
	//p->setValue("power_operation_mode", POWER_OPERATION_MODE_DEFAULT);
	return 0;
}
