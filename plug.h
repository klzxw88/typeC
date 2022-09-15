#ifndef PLUG_H_
#define PLUG_H_

#include <algorithm>
#include "sysfs.h"

class Plug : public SysFS {
public:
	Plug(string path);
};

#endif
