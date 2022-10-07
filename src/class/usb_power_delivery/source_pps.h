#ifndef SOURCE_PPS_H_
#define SOURCE_PPS_H_

#include <algorithm>
#include "sysfs.h"

// Programmable Power Supply (PPS)

class SourcePPS : public SysFS {
private:
	const string name = "pdo";
public:
	SourcePPS(string path);
};

#endif
