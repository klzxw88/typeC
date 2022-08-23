#ifndef SYSFS_H_
#define SYSFS_H_

#include <filesystem>
#include <list>
#include <map>
#include <string>
#include <bitset>
#include "utils.h"

using namespace placeholders;

namespace fs = std::filesystem;

using getterEnum = function<int()>;
using setterEnum = function<void(int)>;
using getterInt = function<int()>;
using setterInt = function<void(int)>;
using getterString = function<string()>;
using setterString = function<void(string)>;
using getterBool = function<bool()>;
using setterBool = function<void(bool)>;
using getterBitset = function<bitset<32>()>;
using setterBitset = function<void(bitset<32>)>;

class SysFS {
protected:
	map<string, tuple<getterEnum, setterEnum, map<string, int>, bool>> mapSysFsEnum;
	map<string, tuple<getterString, setterString, bool>> mapSysFsString;
	map<string, tuple<getterBool, setterBool, bool>> mapSysFsBool;
	map<string, tuple<getterInt, setterInt, bool>> mapSysFsInt;
	map<string, tuple<getterBitset, setterBitset, bool>> mapSysFsBitset;
	list<string> subDirs;
	string devpath;
	map<string, string> sysfsValue;

	void getSysFSAll();
	void getSysFSAll(string path);
	void getSysFS(string file);
	void getSysFS(string file, string path);

	string getString(string name, int val);
	string getString(string name, string val);
public:
	SysFS(string path);

	string getValue(string name);
	bool setValue(string name, int value);
	bool setValue(string name, int value, string path);
	bool setValue(string name, string value);
	bool setValue(string name, string value, string path);
	bool setValue(string name, bool value);
	bool setValue(string name, bool value, string path);
};

#endif
