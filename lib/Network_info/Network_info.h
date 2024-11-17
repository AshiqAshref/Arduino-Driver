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
    bool needs_updating_ = true;
    bool daylight_saving_ = false;
    bool server_needs_updating_ = true;
    IPAddress esp_IP_;
    IPAddress server_IP_= IPAddress(192,168,004,001);
    IPAddress server_SB_= IPAddress(255,255,255,000);
public:
    Network_info()=default;
    bool is_ap_active() const {return this->AP_active; }
    bool is_wifi_active() const {return this->wifi_active;}
    bool connected() const {return this->isConnected;}
    void set_ap_active(const IPAddress &ip_address) {
        this-> AP_active=true;
        this->wifi_active=false;
        this->isConnected=false;
        this->esp_IP_ = ip_address;
        this->needs_updating_=false;
    }
    void set_wifi_active(const IPAddress &ip_address) {
        this-> AP_active=false;
        this->wifi_active=true;
        this->esp_IP_ = ip_address;
        ip_address.toString() == "1.1.1.1"?
            this->isConnected=false:
            this->isConnected=true;

        this->needs_updating_=false;
    }
    void set_needs_update() {
        this->set_none_active();
        this->needs_updating_=true;
    }
    bool needs_update() const {return this->needs_updating_;}
    void set_none_active() {
        this-> AP_active=false;
        this->wifi_active=false;
        this->isConnected=false;
        this->esp_IP_ = IPAddress();
        this->needs_updating_=false;
    }

    IPAddress server_IP(){return this->server_IP_;}
    void set_server_IP(const IPAddress &server_IP){this->server_IP_=server_IP;}

    IPAddress server_SB(){return this->server_SB_;}
    void set_server_SB(const IPAddress &server_SB){this->server_SB_=server_SB;}

    void set_daylight_saving(const bool new_dls) {this->daylight_saving_ = new_dls;}
    bool daylight_saving() const {return this->daylight_saving_;}

    IPAddress get_esp_IP() {return this->esp_IP_;}
    String get_esp_IP_string() const {return this->esp_IP_.toString();}
};




#endif //NETWORK_INFO_H
