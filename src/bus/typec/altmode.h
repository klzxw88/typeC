#ifndef ALTMODE_H_
#define ALTMODE_H_

#include <algorithm>
#include "common.h"
#include "sysfs.h"

class AlternateMode : public SysFS {
private:
	const string name = "typec_alternate_mode";
public:
	AlternateMode(string path);
};

#endif
