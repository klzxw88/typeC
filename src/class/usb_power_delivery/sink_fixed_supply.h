#ifndef SINK_FIXED_SUPPLY_H_
#define SINK_FIXED_SUPPLY_H_

#include <algorithm>
#include "sysfs.h"

class SinkFixedSupply : public SysFS {
private:
	const string name = "pdo";
public:
	SinkFixedSupply(string path);
};

#endif
