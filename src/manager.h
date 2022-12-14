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
    Manager();
    map<string, shared_ptr<DeviceHandler>> deviceHandlers;
    bool get(string path, string type);
    bool set(string path, string type, string attr, string value);

public:
    void processUdevEvent(UdevEvent* pUE, bool verbose = false);
    void clearList(string type="");
    Json::Value getList(string type="");
    Json::Value getListWithPath(string path, string type="");
    void getAll(string path=ROOT_PATH);
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
    bool setWithPath(string p, string attr, string value);
    bool setPort(int portIdx, string attr, string value);
    bool setPartner(int portIdx, string attr, string value);
    bool setPartnerIdentity(int portIdx, string attr, string value);
    bool setCable(int portIdx, string attr, string value);
    bool setCableIdentity(int portIdx, string attr, string value);
    bool setPlug(int portIdx, int plugIdx, string attr, string value);
    bool setAltMode(int portIdx, int modeIdx, string attr, string value);
    bool setDP(int portIdx, int modeIdx, string attr, string value);
    bool setPowerDelivery(int portIdx, string attr, string value);
    bool setPowerDeliverySinkFixed(int portIdx, string attr, string value);
    bool setPowerDeliverySinkVariable(int portIdx, string attr, string value);
    bool setPowerDeliverySinkBattery(int portIdx, string attr, string value);
    bool setPowerDeliverySinkPPS(int portIdx, string attr, string value);
    bool setPowerDeliverySourceFixed(int portIdx, string attr, string value);
    bool setPowerDeliverySourceVariable(int portIdx, string attr, string value);
    bool setPowerDeliverySourceBattery(int portIdx, string attr, string value);
    bool setPowerDeliverySourcePPS(int portIdx, string attr, string value);
};

#endif
