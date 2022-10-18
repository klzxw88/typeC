#ifndef _UdevEvent_H
#define _UdevEvent_H

#include "atrributes.h"

#include <libudev.h>
#include <string>
#include <map>

class UdevEvent {
private:

    void getDeviceInfo(struct udev_device* dev, bool isInitDone);

public:
    std::map<std::string,std::string> mdeviceInfo;
    UdevEvent() {
    }
    ~UdevEvent() {
    }

    void parser(struct udev_device* dev, bool isInitDone);
    std::string getDevAttribute(std::string);
};
#endif //_UdevEvent_H
