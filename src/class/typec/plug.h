#ifndef PLUG_H_
#define PLUG_H_

#include <algorithm>
#include "sysfs.h"

class Plug : public SysFS {
private:
    const string name = "typec_plug";
public:
    Plug(string path);
};

#endif
