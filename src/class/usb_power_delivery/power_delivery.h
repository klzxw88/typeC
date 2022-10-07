#ifndef POWER_DELIVERY_H_
#define POWER_DELIVERY_H_

#include <algorithm>
#include "sysfs.h"

class PowerDelivery : public SysFS {
private:
	const string name = "usb_power_delivery";
public:
	PowerDelivery(string path);
};

#endif
