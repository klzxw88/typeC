#ifndef SINK_PPS_H_
#define SINK_PPS_H_

#include <algorithm>
#include "sysfs.h"

// Programmable Power Supply (PPS)

class SinkPPS : public SysFS {
private:
    const string name = "pdo";
public:
    SinkPPS(string path);
};

#endif
