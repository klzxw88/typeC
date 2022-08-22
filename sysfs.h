#include <algorithm>
#include <filesystem>
#include <map>
#include <string>
#include "utils.h"

using namespace placeholders;

namespace fs = std::filesystem;

class SysFS {
protected:
	map<string, tuple<getter, setter, map<string, int>, bool>> mapSysFs;
	string devpath;
	map<string, string> sysfsValue;

	void getSysFS();
	void getSysFS(string file);

	string getString(string name, int val);
public:
	SysFS(string path);

	string getValue(string name);
	bool setValue(string name, int value);
};
