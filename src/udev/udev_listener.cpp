#include <glib.h>
#include <libudev.h>
#include <stdarg.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <cstring>
#include <chrono>
#include <thread>
#include <iostream>

#include "udev_listener.h"
#include "udev_event.h"
#include "manager.h"

#define memzero(x,l) (memset((x), 0, (l)))
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

using namespace DevAttributes;

atomic<bool> UdevListener::m_listenerRunning = false;
atomic<bool> UdevListener::m_verbose = false;

UdevListener::UdevListener(): m_udev(nullptr) {
}

UdevListener::~UdevListener() {
    stopListener();
}

bool UdevListener::initListener(bool verbose) {
    //cout << "UdevListener::" << __FUNCTION__ << endl;
    bool initDone = false;
    if (init()) {
        if (startListener(verbose)) {
            enumerate_devices();
        }
        initDone = true;
    }
    return initDone;
}

bool UdevListener::startListener(bool verbose) {
    //cout << "UdevListener::" << __FUNCTION__ << endl;
	if (m_listenerRunning) {
		return false;
	}
	m_verbose = verbose;
    m_listenerRunning = true;
    m_listenerThread = thread(&UdevListener::threadStart,this);
    return true;
}

bool UdevListener::stopListener() {
    //cout << "UdevListener::" << __FUNCTION__ << endl;
    if (!m_listenerRunning) {
        return false;
    }
    udev_unref(m_udev);
    m_udev = nullptr;
    if (m_listenerThread.joinable()) {
        try {
            m_listenerRunning = false;
            m_listenerThread.join();
           }
           catch (exception &e) {
            cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << " caught system_error: " << e.what() << endl;
           }
    }
    return true;
}

/*  To get the new udev instances
*/
bool UdevListener::init() {
    //cout << "UdevListener::" << __FUNCTION__ << endl;
    m_udev = udev_new();
    if (!m_udev) {
        cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << " udev_new() failed" << endl;
        return false;
    }
    return true;
}

void UdevListener::enumerate_devices() {
    //cout << "UdevListener::" << __FUNCTION__ << endl;
    if (m_udev == nullptr) {
        cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << " m_udev is null" << endl;
        return;
    }
    enumerate_subsystem_devices("usb");
    enumerate_subsystem_devices("typec");
}

void UdevListener::threadStart() {

    struct udev_monitor* monitor = NULL;
    int fd_ep;
    int fd_udev = -1;
    struct epoll_event ep_udev;

    fd_ep = epoll_create1(EPOLL_CLOEXEC);
    if (fd_ep < 0) {
        cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << " error creating epoll fd: " << strerror(errno) << endl;
        goto out;
    }
    monitor = udev_monitor_new_from_netlink(m_udev, "udev");
    if (monitor == NULL) {
        cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << " no socket" << endl;
        goto out;
    }

    //if (udev_monitor_filter_add_match_subsystem_devtype(monitor, "usb", "usb_device") < 0 ||
    if (udev_monitor_filter_add_match_subsystem_devtype(monitor, "typec", NULL) < 0) {
        cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << " filter failed" << endl;
        goto out;
    }

    //if (udev_monitor_set_receive_buffer_size(monitor, 128*1024*1024) < 0) {
    if (udev_monitor_set_receive_buffer_size(monitor, 128*1024) < 0) {
        cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << " failed to set buffer size" << endl;
        goto out;
    }

    if (udev_monitor_enable_receiving(monitor) < 0) {
        cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << " bind failed" << endl;
        goto out;
    }
    fd_udev = udev_monitor_get_fd(monitor);
    memzero(&ep_udev, sizeof(struct epoll_event));
    ep_udev.events = EPOLLIN;
    ep_udev.data.fd = fd_udev;
    if (epoll_ctl(fd_ep, EPOLL_CTL_ADD, fd_udev, &ep_udev) < 0) {
        cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << " fail to add fd to epoll: " << strerror(errno) << endl;
        goto out;
    }

    for (;;) {
        int fdcount;
        struct epoll_event ev[4];
        struct udev_device *device;

        if (!m_listenerRunning) {
            goto out;
        }
        fdcount = epoll_wait(fd_ep, ev, ARRAY_SIZE(ev), 100);

        for (int i = 0; i < fdcount; i++) {
            if (ev[i].data.fd == fd_udev && ev[i].events & EPOLLIN) {
                device = udev_monitor_receive_device(monitor);
                if (device == NULL) {
                    cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << "no device from socket" << endl;
                    continue;
                }
                UdevEvent *pNE = new (nothrow) UdevEvent;
                if (pNE) {
                    pNE->parser(device, true);
					Manager::instance()->processUdevEvent(pNE, m_verbose);
            		delete pNE;
                }
                else {
                    cout << "UdevListener::" << __FUNCTION__ << " line: " << __LINE__ << " memory failure for PdmNetlinkEvent" << endl;
                }
                delete pNE;
                udev_device_unref(device);
            }
        }
    }

    out:
    if (fd_ep >= 0) {
        close(fd_ep);
    }
    udev_monitor_unref(monitor);
}

bool UdevListener::checkExternalUsbDevice(string &path) {

    cout << "UdevListener::" << __FUNCTION__ << " line:" << __LINE__ << " Device PATH = [" << path.c_str() << "]" << endl;
    size_t found = path.find("usb");
    if (found == string::npos) {
        cout << "UdevListener::" << __FUNCTION__ << " line:" << __LINE__ << " Device PATH = [" << path << "] not found" << endl;
        return false;
    }

    for (auto devPath : m_usbDevicePath) {
        if (path.compare(0,devPath.length(),devPath) == 0) {
            return true;
        }
    }
    m_usbDevicePath.push_back(path);
    //cout << "UdevListener::" << __FUNCTION__ << " line:" << __LINE__ << " Device PATH = [" << path << "] pushed inside list" << endl;
    return true;
}

void UdevListener::enumerate_subsystem_devices(string subSys) {
    if (m_udev == nullptr) {
        cout << "UdevListener::" << __FUNCTION__ << " line:" << __LINE__ << " m_udev is null" << endl;
        return;
    }
    struct udev_enumerate* enumerate = udev_enumerate_new(m_udev);
    //cout << "UdevListener::" << __FUNCTION__ << endl;
    udev_enumerate_add_match_subsystem(enumerate, subSys.c_str());

    udev_enumerate_scan_devices(enumerate);

    struct udev_list_entry* devices = udev_enumerate_get_list_entry(enumerate);
    struct udev_list_entry* entry;

    udev_list_entry_foreach(entry, devices) {
        const char* path = udev_list_entry_get_name(entry);
        struct udev_device* device = udev_device_new_from_syspath(m_udev, path);
        string pathfinder = path;
        struct udev_device* parent_dev = udev_device_get_parent(device);

        if (parent_dev) {
            //cout << "UdevListener::" << __FUNCTION__ << " line:" << __LINE__ << " Parent device " << path << endl;
            UdevEvent *pNE = new UdevEvent;
            pNE->parser(device, false);
			Manager::instance()->processUdevEvent(pNE, m_verbose);
            delete pNE;
        }
        udev_device_unref(device);
    }

    udev_enumerate_unref(enumerate);
}
