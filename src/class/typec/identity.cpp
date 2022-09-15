#include "identity.h"

Identity::Identity(string path)
	: SysFS(path) {
	// id_header
	SysFSValue<bitset<32>>* id_header = new SysFSValue<bitset<32>>("id_header", false);
	addMap(id_header);
	
	// cert_stat
	SysFSValue<bitset<32>>* cert_stat = new SysFSValue<bitset<32>>("cert_stat", false);
	addMap(cert_stat);

	// product
	SysFSValue<bitset<32>>* product = new SysFSValue<bitset<32>>("product", false);
	addMap(product);

	// product_type_vdo1
	SysFSValue<bitset<32>>* product_type_vdo1 = new SysFSValue<bitset<32>>("product_type_vdo1", false);
	addMap(product_type_vdo1);

	// product_type_vdo2
	SysFSValue<bitset<32>>* product_type_vdo2 = new SysFSValue<bitset<32>>("product_type_vdo2", false);
	addMap(product_type_vdo2);

	// product_type_vdo3
	SysFSValue<bitset<32>>* product_type_vdo3 = new SysFSValue<bitset<32>>("product_type_vdo3", false);
	addMap(product_type_vdo3);

	getSysFSAll();
}
