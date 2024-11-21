#ifndef BOX_H
#define BOX_H
#include <Arduino.h>
#include <Led_Coordinate.h>
#include <Pos_Coordinate.h>
#include <Status_Directive.h>

class Box {
    const byte box_no_;
    Pos_Coordinate position= Pos_Coordinate();
    Led_Coordinate led_address=Led_Coordinate();

    unsigned int med_id_ = 0;
    String medicine_name_="";
    unsigned short medicine_amount_=0;
    boolean isOpen_=false;
    BoxStatus status_ = BOX_STATUS_DEFAULT ;

public:
    Box();
    explicit Box(const byte box_no, const Pos_Coordinate &pos_coordinate)
        : box_no_(box_no), position(pos_coordinate) {}

    void set_box(
        const unsigned int med_id,
        const String& medicine_name,
        const unsigned short medicine_amount
    ) {
        this->med_id_ = med_id;
        this->medicine_name_ = medicine_name;
        this->medicine_amount_ = medicine_amount;
    }


    Pos_Coordinate coordinate() const {return position;}
    byte box_no() const {return box_no_;}
    boolean isOpen() const {return isOpen_;}
    void isOpen(const boolean value) {
        if(value)
            Status_Directive::set_mode(this->box_no_, BOX_STATUS_REMOVED);
        else if(this->isOpen_)
            set_status(status_);
        this->isOpen_=value;
    }
    void unlocking(const boolean value) {
        if(value)
            Status_Directive::set_mode(this->box_no_, BOX_STATUS_UNLOCKING);
        else if(this->isOpen_)
            set_status(status_);
    }
    unsigned int med_id() const {return this->med_id_;}
    void set_med_id(const unsigned int med_id) {this->med_id_ = med_id;}

    String name() {return medicine_name_;}
    void set_name(String const& medicine_name) {this->medicine_name_ = medicine_name;}

    unsigned short medicne_amount() const {return medicine_amount_;}
    void set_medicne_amount(unsigned short const medicne_amount) {this->medicine_amount_ = medicne_amount;}

    BoxStatus &status() {return status_;}
    void set_status(const BoxStatus status) {
        this->status_ = status;
        Status_Directive::set_mode(this->box_no_, status);
    }

    String toString() const{
        return "{"
                R"("name" : ")" + medicine_name_ + "\""
                R"("box_no" : ")"+ static_cast<String>(box_no_) +
                R"("medicine_amount" : ")"+ static_cast<String>(medicine_amount_) +
                    "}";
    }
};

#endif //BOX_H



