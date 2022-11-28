
#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <string>
#include <json/json.h>

#include "singleton.h"

#define CONF_PATH "./file/conf/"

using namespace std;

class Configuration : public Singleton<Configuration> {
private:
    friend class Singleton<Configuration>;
    Configuration();
    Json::Value m_conf;
public:
    Json::Value getAttribute(string name);
    Json::Value getSysValues(string name);
};

#endif
