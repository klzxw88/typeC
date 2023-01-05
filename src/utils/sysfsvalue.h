#ifndef SYSFSVALUE_H_
#define SYSFSVALUE_H_

#include <bitset>
#include <ctype.h>
#include <json/json.h>
#include <map>
#include <string>
#include <type_traits>
#include "utils.h"

using namespace placeholders;

class ISysFSValue {
private:
    bool isWritable;
protected:
    bool isHit;
    string name;
public:
    ISysFSValue(string n, bool b) : name(n), isWritable(b), isHit(false) {};
    virtual ~ISysFSValue() {};

    virtual bool empty() = 0;
    virtual bool load(string s) = 0;
    virtual string to_string() = 0;
    virtual string to_string(int i) = 0;
    virtual Json::Value toJson() = 0;
    void setHit() { isHit = true; };
    bool getHit() { return isHit; };
    bool writable() { return isWritable; };
    string getName() { return name; };
};

template <typename T>
class SysFSValue : public ISysFSValue {
private:
    map<string, T> table;
    T value;
    string devpath;
public:
    using value_type = T;
    SysFSValue(string path, string n, bool b) : ISysFSValue(n, b), devpath(path) {};
    ~SysFSValue() {};

    T get() { return value; };
    void set(T val) { value = val; };
    void add(string s, T t) { table.insert(make_pair(s,t)); };
    bool load(string s) override;
    bool empty() override { return table.empty(); };
    string to_string() override;
    string to_string(int i) override;
    Json::Value toJson() override;
};

template<typename T>
bool SysFSValue<T>::load(string s) {
    if constexpr (is_same_v<T, string>) {
        value = s;
        return true;
    }
    else if constexpr (is_same_v<T, bitset<32>>) {
        if (s[0] == '0' && s[1] == 'x') {
            s = s.substr(2, s.length()-2);
            stringstream ss;
            unsigned int iVal;
            ss << std::hex << s;
            ss >> iVal;
            value = iVal;
            return true;
        }
        else if (std::all_of(s.begin(), s.end()-2, ::isxdigit)) {
            stringstream ss;
            unsigned int iVal;
            ss << std::hex << s;
            ss >> iVal;
            value = iVal;
            return true;
        }
        else {
            return false;
        }
    }
    else if constexpr (is_same_v<T, bool>) {
        if (s == "yes\n" || s == "1\n" || s == "true\n") {
            value = true;
        }
        else {
            value = false;
        }
        return true;
    }
    else if (table.empty()) {
        if(std::all_of(s.begin(), s.end()-2, ::isxdigit)) {
            stringstream ss;
            unsigned int iVal;
            ss << std::hex << s;
            ss >> iVal;
            value = iVal;
            return true;
        }
        else {
            try {
                value = stoi(s);
                return true;
            } catch(std::invalid_argument const& ex) {
                std::cout << "std::invalid_argument::what(): " << ex.what() << '\n';
                return false;
            } catch(std::out_of_range const& ex) {
                 std::cout << "std::out_of_range::what(): " << ex.what() << '\n';
                return false;
            }
        }
    }
    else {
        if (table.find(s) != table.end()) {
            value = table[s];
            return true;
        }
        else {
            return false;
        }
    }
}

template<typename T>
string SysFSValue<T>::to_string() {
    if constexpr (is_same_v<T, string>) {
        return value;
    }
    else if constexpr (is_same_v<T, bitset<32>>) {
        return value.to_string();
    }
    else if constexpr (is_same_v<T, bool>) {
        return value?"yes\n":"no\n";
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
string SysFSValue<T>::to_string(int i) {
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

template<typename T>
Json::Value SysFSValue<T>::toJson() {
    Json::Value obj;
    if constexpr (is_same_v<T, string>) {
        obj[name] = rtrim(value);
    }
    else if constexpr (is_same_v<T, bitset<32>>) {
        obj[name] = to_string();
    }
    else if constexpr (is_same_v<T, bool>) {
        obj[name] = value;
    }
    else if (table.empty()) {
        obj[name] = value;
    }
    else {
        auto result = find_if(
                table.begin(),
                table.end(),
                [&](const auto& mo) {return mo.second == value; });
        if (result != table.end()) {
            string retVal = result->first;
            obj[name] = rtrim(retVal);
        }
        else {
            obj[name] = "";
        }
    }
    return obj;
}

#endif
