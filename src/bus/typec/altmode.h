#ifndef ALTMODE_H_
#define ALTMODE_H_

#include <algorithm>
#include "common.h"
#include "sysfs.h"
#include "identity.h"

class AlternateMode : public SysFS {
public:
	AlternateMode(string path);
};

#endif
