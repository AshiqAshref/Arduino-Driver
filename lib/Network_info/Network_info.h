//
// Created by user on 25/10/2024.
//

#ifndef NETWORK_INFO_H
#define NETWORK_INFO_H
#include <IPAddress.h>

class Network_info {
    bool AP_active=false;
    bool wifi_active=false;
    bool isConnected = false;
    bool needs_updating = true;
    IPAddress IP;
public:
    Network_info()=default;
    bool is_ap_active() const {return this->AP_active;}
    bool is_wifi_active() const {return this->wifi_active;}
    bool connected() const {return this->isConnected;}
    void set_ap_active(const IPAddress &ip_address) {
        this-> AP_active=true;
        this->wifi_active=false;
        this->isConnected=false;
        this->IP = ip_address;
        this->needs_updating=false;
    }
    void set_wifi_active(const IPAddress &ip_address) {
        this-> AP_active=false;
        this->wifi_active=true;
        this->IP = ip_address;
        ip_address.toString() == "1.1.1.1"?
            this->isConnected=false:
            this->isConnected=true;

        this->needs_updating=false;
    }
    void set_needs_update() {
        this->set_none_active();
        this->needs_updating=true;
    }
    bool needs_update() const {return this->needs_updating;}
    void set_none_active() {
        this-> AP_active=false;
        this->wifi_active=false;
        this->isConnected=false;
        this->IP = IPAddress();
        this->needs_updating=false;
    }



    IPAddress get_IP() {
        return this->IP;
    }
    String get_ip_string() const {
        return this->IP.toString();
    }
};



#endif //NETWORK_INFO_H
