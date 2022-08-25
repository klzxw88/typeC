#ifndef SYSFSVALUE_H_
#define SYSFSVALUE_H_

#include <bitset>
#include <map>
#include <string>
#include <type_traits>
#include "utils.h"

using namespace placeholders;

class ISysFSValue {
private:
	string name;
	bool isWritable;
public:
	ISysFSValue(string n, bool b) : name(n), isWritable(b) {};
	virtual ~ISysFSValue() {};

	virtual bool empty() = 0;
	virtual string getString() = 0;
	virtual string getString(int i) = 0;
	virtual void set(string s) = 0;
	bool writable() { return isWritable; };
	string getName() { return name; };
};

template <typename T>
class SysFSValue : public ISysFSValue {
private:
	map<string, T> table;
	T value;
public:
	SysFSValue(string n, bool b) : ISysFSValue(n, b) {};
	~SysFSValue() {};

	T get() { return value; };
	void set(T val) { value = val; };
	void set(string s) override;
	void add(string s, T t) { table.insert(make_pair(s,t)); };
	bool empty() override { return table.empty(); };
	string getString() override;
	string getString(int i) override;
};

template<typename T>
void SysFSValue<T>::set(string s) {
	if constexpr (is_same_v<T, string>) {
		value = s;
	}
	else if (is_same_v<T, bitset<32>>) {
		if (s[0] == '0' && s[1] == 'x') {
			s = s.substr(2, s.length()-2);
			stringstream ss;
			unsigned int iVal;
			ss << std::hex << s;
			ss >> iVal;
			value = iVal;
		}
	}
	else if (is_same_v<T, bool>) {
		if (s == "yes\n" || s == "1\n" || s == "true\n") {
			value = true;
		}
		else {
			value = false;
		}
	}
	else if (table.empty()) {
		value = stoi(s);
	}
	else {
		value = table[s];
	}
}

template<typename T>
string SysFSValue<T>::getString() {
	if constexpr (is_same_v<T, string>) {
		return value;
	}
	else if constexpr (is_same_v<T, bitset<32>>) {
		return value.to_string();
	}
	else if constexpr (is_same_v<T, bool>) {
		return value?"yes":"no";
	}
	else if (table.empty()) {
		return to_string(value);	
	}
	else {
		auto result = find_if(
				table.begin(),
				table.end(),
				[&](const auto& mo) {return mo.second == value; });
		if (result != table.end()) {
			return result->first;	
		}
		return "";
	}
}

template<typename T>
string SysFSValue<T>::getString(int i) {
	if constexpr (is_same_v<T, string>) {
		return "";
	}
	else if constexpr (is_same_v<T, bitset<32>>) {
		return "";
	}
	else if constexpr (is_same_v<T, bool>) {
		return "";
	}
	else if (table.empty()) {
		return "";
	}
	else {
		auto result = find_if(
				table.begin(),
				table.end(),
				[&](const auto& mo) {return mo.second == i; });
		if (result != table.end()) {
			return result->first;	
		}
		return "";
	}
}

#endif
