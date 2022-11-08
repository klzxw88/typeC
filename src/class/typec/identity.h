#ifndef IDENTITY_H_
#define IDENTITY_H_

#include <algorithm>
#include "sysfs.h"

class Identity : public SysFS {
private:
    const string name = "identity";
public:
    bitset<32> id_header;
    bitset<32> cert_stat;
    bitset<32> product;
    bitset<32> product_type_vdo1;
    bitset<32> product_type_vdo2;
    bitset<32> product_type_vdo3;

    Identity(string path);
};

#endif
