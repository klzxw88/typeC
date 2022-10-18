#include "port.h"
// https://www.kernel.org/doc/Documentation/ABI/testing/sysfs-class-typec

Port::Port(string path)	: SysFS(path) { 
	// data_role
	SysFSValue<int>* data_role = new SysFSValue<int>("data_role", true);
	data_role->add("[device]\n", DATA_ROLE_DEVICE);
	data_role->add("[host]\n", DATA_ROLE_HOST);
	data_role->add("[host] device\n", DATA_ROLE_DUAL_HOST);
	data_role->add("host [device]\n", DATA_ROLE_DUAL_DEVICE);
	addMap(data_role);

	// power_role
	SysFSValue<int>* power_role = new SysFSValue<int>("power_role", true);
	power_role->add("[sink]\n", POWER_ROLE_SINK);
	power_role->add("[source]\n", POWER_ROLE_SOURCE);
	power_role->add("[source] sink\n", POWER_ROLE_DUAL_SINK);
	power_role->add("source [sink]\n", POWER_ROLE_DUAL_SOURCE);
	addMap(power_role);

	// port_type
	SysFSValue<int>* port_type = new SysFSValue<int>("port_type", true);
	port_type->add("[sink]\n", PORT_TYPE_SINK);
	port_type->add("[source]\n", PORT_TYPE_SOURCE);
	port_type->add("dual [source] sink\n", PORT_TYPE_DUAL_SOURCE);
	port_type->add("dual source [sink]\n", PORT_TYPE_DUAL_SINK);
	addMap(port_type);

	// vconn_source
	SysFSValue<bool>* vconn_source = new SysFSValue<bool>("vconn_source", true);
	addMap(vconn_source);

	// power_operation_mode
	SysFSValue<int>* power_operation_mode = new SysFSValue<int>("power_operation_mode", false);
	power_operation_mode->add("default\n", POWER_OPERATION_MODE_DEFAULT);
	power_operation_mode->add("1.5A\n", POWER_OPERATION_MODE_1_5A);
	power_operation_mode->add("3.0A\n", POWER_OPERATION_MODE_3A);
	power_operation_mode->add("usb_power_delivery\n", POWER_OPERATION_MODE_PD);
	addMap(power_operation_mode);

	// preferred_role
	SysFSValue<int>* preferred_role = new SysFSValue<int>("preferred_role", true);
	preferred_role->add("sink\n", PREFERRED_ROLE_SINK);
	preferred_role->add("source\n", PREFERRED_ROLE_SOURCE);
	addMap(preferred_role);

    // supported_accessory_modes

	// usb_power_delivery_revision
	SysFSValue<int>* usb_power_delivery_revision = new SysFSValue<int>("usb_power_delivery_revision", false);
	usb_power_delivery_revision->add("2.0\n", POWER_DELIVERY_REVISION_2_0);
	usb_power_delivery_revision->add("3.0\n", POWER_DELIVERY_REVISION_3_0);
	usb_power_delivery_revision->add("3.1\n", POWER_DELIVERY_REVISION_3_1);
	addMap(usb_power_delivery_revision);

	// usb_typec_revision
	SysFSValue<int>* usb_typec_revision = new SysFSValue<int>("usb_typec_revision", false);
	usb_typec_revision->add("1.1\n", TYPEC_REVISION_1_1);
	usb_typec_revision->add("1.2\n", TYPEC_REVISION_1_2);
	usb_typec_revision->add("1.3\n", TYPEC_REVISION_1_3);
	usb_typec_revision->add("1.4\n", TYPEC_REVISION_1_4);
	usb_typec_revision->add("2.0\n", TYPEC_REVISION_2_0);
	addMap(usb_typec_revision);

	// orientation
	SysFSValue<int>* orientation = new SysFSValue<int>("orientation", false);
	orientation->add("unknown\n", ORIENTATION_UNKNOWN);
	orientation->add("reverse\n", ORIENTATION_REVERSE);
	orientation->add("normal\n", ORIENTATION_NORMAL);
	addMap(orientation);

	// select_usb_power_delivery

	getSysFSAll();

	//cout << toJson().toStyledString() << endl;
}
/*
int main() {
	Port* p = new Port("./sys/class/typec/port0/");
	cout << "------" << endl;
	cout << "data_role:" << p->getValue("data_role") << endl;
	cout << "vconn_source:" << p->getValue("vconn_source") << endl;
	cout << "power_role:" << p->getValue("power_role") << endl;
	cout << "usb_power_delivery_revision:" << p->getValue("usb_power_delivery_revision") << endl;
	cout << "power_operation_mode:" << p->getValue("power_operation_mode") << endl;
	cout << "port_type:" << p->getValue("port_type") << endl;
	cout << "preferred_role:" << p->getValue("preferred_role") << endl;
	cout << "usb_typec_revision:" << p->getValue("usb_typec_revision") << endl;
	cout << "orientation:" << p->getValue("orientation") << endl;
	cout << "------" << endl;
	p->setValue("data_role", DATA_ROLE_HOST);
	p->setValue("vconn_source", false);
	p->setValue("power_role", POWER_ROLE_SOURCE);
	p->setValue("port_type", PORT_TYPE_SOURCE);
	p->setValue("power_operation_mode", POWER_OPERATION_MODE_DEFAULT);
	p->setValue("preferred_role", PREFERRED_ROLE_SINK);
	cout << "------" << endl;
	cout << "data_role:" << p->getValue("data_role") << endl;
	cout << "vconn_source:" << p->getValue("vconn_source") << endl;
	cout << "power_role:" << p->getValue("power_role") << endl;
	cout << "port_type:" << p->getValue("port_type") << endl;
	cout << "power_operation_mode:" << p->getValue("power_operation_mode") << endl;
	cout << "preferred_role:" << p->getValue("preferred_role") << endl;
	cout << "------" << endl;
	p->setValue("data_role", DATA_ROLE_DEVICE);
	p->setValue("vconn_source", true);
	p->setValue("power_role", POWER_ROLE_DUAL_SOURCE);
	p->setValue("port_type", PORT_TYPE_DUAL_SOURCE);
	p->setValue("preferred_role", PREFERRED_ROLE_SOURCE);
	cout << "------" << endl;
	cout << "data_role:" << p->getValue("data_role") << endl;
	cout << "vconn_source:" << p->getValue("vconn_source") << endl;
	cout << "power_role:" << p->getValue("power_role") << endl;
	cout << "port_type:" << p->getValue("port_type") << endl;
	cout << "preferred_role:" << p->getValue("preferred_role") << endl;
	cout << "------" << endl;
	delete p;
	return 0;
}
*/
