#include "configuration.h"
#include "utils.h"

Configuration::Configuration() {
    m_conf = readJson("file/conf/type-c.conf");
}

Json::Value Configuration::getAttribute(string name) {
    Json::Value value;
    if (m_conf.isMember("attributes")) {
        value = m_conf["attributes"][name];
    }
    return value;
}

Json::Value Configuration::getSysValues(string name) {
    Json::Value value;
    if (m_conf.isMember(name)) {
        value = m_conf[name];
    }
    return value;
}
