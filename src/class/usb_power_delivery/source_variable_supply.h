#ifndef SOURCE_VARIABLE_SUPPLY_H_
#define SOURCE_VARIABLE_SUPPLY_H_

#include <algorithm>
#include "sysfs.h"

class SourceVariableSupply : public SysFS {
private:
    const string name = "pdo";
public:
    SourceVariableSupply(string path);
};

#endif
