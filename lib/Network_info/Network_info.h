//
// Created by user on 25/10/2024.
//

#ifndef NETWORK_INFO_H
#define NETWORK_INFO_H
#include <IPAddress.h>

class Network_info {
    bool AP_active=false;
    IPAddress IP;
public:
    Network_info()=default;
    bool is_ap_active() const {return this->AP_active;}
    void set_ap_active(const bool aP_status) {
        this-> AP_active=aP_status;
        if(!aP_status)
            this->IP = IPAddress();
    }

    void set_IP(const IPAddress &ip_address) {
        this->IP = ip_address;
        this->AP_active= true;
    }

    IPAddress get_IP() {
        return this->IP;
    }
    String get_ip_string() const {
        return this->IP.toString();
    }
};



#endif //NETWORK_INFO_H
