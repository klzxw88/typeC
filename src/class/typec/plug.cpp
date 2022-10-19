#include "plug.h"

Plug::Plug(string path)
	: SysFS(path) {
	// number_of_alternate_modes
	SysFSValue<int>* number_of_alternate_modes = new SysFSValue<int>("number_of_alternate_modes", false);
	addMap(number_of_alternate_modes);

	getSysFSAll();
/*
	cout << toJson().toStyledString() << endl;
	cout << sysFStoJson().toStyledString() << endl;
*/
}
/*
int main() {
	Plug* p = new Plug("./sys/class/typec/port0/port0-plug0/");
	cout << p->getValue("number_of_alternate_modes") << endl;
	delete p;
	return 0;
}
*/
