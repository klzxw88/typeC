#ifndef MANAGER_H_
#define MANAGER_H_

#include <string>
#include <map>
#include <algorithm>

#include "singleton.h"
#include "device_handler.h"

#define ROOT_PATH "./sys/class/typec"

using namespace std;

class Manager : public Singleton<Manager> {
private:
	friend class Singleton<Manager>;
	const string PDO_SOURCE_FIXED = "pdo_source_fixed";
	const string PDO_SOURCE_VARIABLE = "pdo_source_variable";
	const string PDO_SOURCE_BATTERY = "pdo_source_battery";
	const string PDO_SOURCE_PPS = "pdo_source_pps";
	const string PDO_SINK_FIXED = "pdo_sink_fixed";
	const string PDO_SINK_VARIABLE = "pdo_sink_variable";
	const string PDO_SINK_BATTERY = "pdo_sink_battery";
	const string PDO_SINK_PPS = "pdo_sink_pps";
	Manager();
	map<string, shared_ptr<IDeviceHandler>> deviceHandlers;
	bool get(string path, string type);

public:
	void processUdevEvent(UdevEvent* pUE);
	Json::Value getList(string type="");
	Json::Value getListWithPath(string path, string type="");
	void getAll();
	bool getPort(int portIdx);
	bool getPartner(int portIdx);
	bool getPartnerIdentity(int portIdx);
	bool getCable(int portIdx);
	bool getCableIdentity(int portIdx);
	bool getPlug(int portIdx, int plugIdx);
	bool getPowerDelivery(int portIdx);
	bool getPowerDeliverySinkFixed(int portIdx);
	bool getPowerDeliverySinkVariable(int portIdx);
	bool getPowerDeliverySinkBattery(int portIdx);
	bool getPowerDeliverySinkPPS(int portIdx);
	bool getPowerDeliverySink(int portIdx);
	bool getPowerDeliverySourceFixed(int portIdx);
	bool getPowerDeliverySourceVariable(int portIdx);
	bool getPowerDeliverySourceBattery(int portIdx);
	bool getPowerDeliverySourcePPS(int portIdx);
	bool getPowerDeliverySource(int portIdx);
	bool getAltMode(int portIdx, int modeIdx);
	bool getDP(int portIdx, int modeIdx);
	bool getWithPath(string p);
};

#endif
