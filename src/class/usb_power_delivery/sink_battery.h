#ifndef SINK_BATTERY_H_
#define SINK_BATTERY_H_

#include <algorithm>
#include "sysfs.h"

class SinkBattery : public SysFS {
private:
	const string name = "pdo";
public:
	SinkBattery(string path);
};

#endif
