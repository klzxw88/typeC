#include "identity.h"

Identity::Identity(string path)
    : SysFS(path) {
    // id_header
    SysFSValue<bitset<32>>* id_header = new SysFSValue<bitset<32>>(path, "id_header", false);
    addMap(id_header);

    // cert_stat
    SysFSValue<bitset<32>>* cert_stat = new SysFSValue<bitset<32>>(path, "cert_stat", false);
    addMap(cert_stat);

    // product
    SysFSValue<bitset<32>>* product = new SysFSValue<bitset<32>>(path, "product", false);
    addMap(product);

    // product_type_vdo1
    SysFSValue<bitset<32>>* product_type_vdo1 = new SysFSValue<bitset<32>>(path, "product_type_vdo1", false);
    addMap(product_type_vdo1);

    // product_type_vdo2
    SysFSValue<bitset<32>>* product_type_vdo2 = new SysFSValue<bitset<32>>(path, "product_type_vdo2", false);
    addMap(product_type_vdo2);

    // product_type_vdo3
    SysFSValue<bitset<32>>* product_type_vdo3 = new SysFSValue<bitset<32>>(path, "product_type_vdo3", false);
    addMap(product_type_vdo3);

    getSysFSAll();
/*
    cout << toJson().toStyledString() << endl;
    cout << sysFStoJson().toStyledString() << endl;
*/
}
/*
int main() {
    Partner* p = new Partner("./sys/class/typec/port0/port0-partner/identity/");
    cout << p->getValue("id_header") << endl;
    cout << p->getValue("cert_stat") << endl;
    cout << p->getValue("product") << endl;
    cout << p->getValue("product_type_vdo1") << endl;
    cout << p->getValue("product_type_vdo2") << endl;
    cout << p->getValue("product_type_vdo3") << endl;
    delete p;
    return 0;
}
*/
