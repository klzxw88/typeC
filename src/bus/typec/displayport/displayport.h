#ifndef DISPLAYPORT_H_
#define DISPLAYPORT_H_

#include <algorithm>
#include "common.h"
#include "sysfs.h"

enum DP_CONF_TYPES {
    DP_CONF_DUAL_USB = 140,
    DP_CONF_DUAL_SOURCE = 141,
    DP_CONF_DUAL_SINK = 142,
    DP_CONF_SOURCE_USB = 143,
    DP_CONF_SOURCE_SOURCE = 144,
    DP_CONF_SINK_USB = 145,
    DP_CONF_SINK_SINK = 146,
};

class DisplayPort : public SysFS {
    const string name = "displayport";
public:
    DisplayPort(string path);
};

#endif
