#ifndef SOURCE_BATTERY_H_
#define SOURCE_BATTERY_H_

#include <algorithm>
#include "sysfs.h"

class SourceBattery : public SysFS {
private:
    const string name = "pdo";
public:
    SourceBattery(string path);
};

#endif
