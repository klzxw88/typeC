#ifndef SINK_VARIABLE_SUPPLY_H_
#define SINK_VARIABLE_SUPPLY_H_

#include <algorithm>
#include "sysfs.h"

class SinkVariableSupply : public SysFS {
private:
    const string name = "pdo";
public:
    SinkVariableSupply(string path);
};

#endif
