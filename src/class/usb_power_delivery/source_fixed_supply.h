#ifndef SOURCE_FIXED_SUPPLY_H_
#define SOURCE_FIXED_SUPPLY_H_

#include <algorithm>
#include "sysfs.h"

class SourceFixedSupply : public SysFS {
private:
	const string name = "pdo";
public:
	SourceFixedSupply(string path);
};

#endif
