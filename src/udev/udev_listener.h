#ifndef _UdevListener_H
#define _UdevListener_H

#include "atrributes.h"

#include <atomic>
#include <list>
#include <memory>
#include <thread>

#include "singleton.h"

using namespace std;

class PdmNetlinkEvent;

class UdevListener : public Singleton<UdevListener> {
private:
    friend class Singleton<UdevListener>;
    static atomic<bool> m_listenerRunning;
    struct udev* m_udev;
    thread m_listenerThread;
    list<string> m_usbDevicePath;

    UdevListener();
    UdevListener(const UdevListener& src) = delete;
    UdevListener& operator=(const UdevListener& rhs) = delete;

public:
    ~UdevListener();

    bool initListener();
    bool startListener();
    bool stopListener();
    //virtual void onEvent(PdmNetlinkEvent *evn) = 0;

private:
    bool init();
    void threadStart();
    void enumerate_devices();
    void enumerate_subsystem_devices(string subSys);
    bool checkExternalUsbDevice(string &path);
};

#endif //_UdevListener_H
