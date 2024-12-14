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

    char medicine_name_[9]{};
    uint16_t medicine_amount_=0;
    boolean isOpen_=false;
    BoxStatus box_status_ = BOX_STATUS_NOT_IN_USE ;
    BoxStatus amount_status_ = BOX_STATUS_DEFAULT;

    void set_amount_status(BoxStatus status){this->amount_status_=status;}
    bool medicine_amount_set=false;

public:
    Box();
    explicit Box(const byte box_no, const Pos_Coordinate &pos_coordinate)
        : box_no_(box_no), position(pos_coordinate) {}


    BoxStatus amount_status() const {return this->amount_status_;}
    Pos_Coordinate coordinate() const {return position;}
    byte box_no() const {return box_no_;}
    boolean isOpen() const {return isOpen_;}
    void setOpen(const boolean value) {
        if(value)
            Status_Directive::set_mode(this->box_no_, BOX_STATUS_REMOVED);
        else if(this->isOpen_)
            set_box_status(box_status_);
        this->isOpen_=value;
    }
    void unlocking(const boolean value) {
        if(value)
            Status_Directive::set_mode(this->box_no_, BOX_STATUS_UNLOCKING);
        else
            set_box_status(box_status_);
    }


    void set_medicine_name(const String &medicine_name) {
        for (int i = 0; i < 8; i++)
            this->medicine_name_[i] = medicine_name.charAt(i);
        this->medicine_name_[8]='\0';
    }
    char* medicine_name() {return this->medicine_name_;}


    void reduce_amount(const uint16_t dosage) {
        if(medicine_amount_set) {
            if(this->medicine_amount()>dosage) {
                this->medicine_amount_ = this->medicine_amount_-dosage;
            }else {
                this->medicine_amount_=0;
            }

            if(this->medicine_amount_==0) {
                set_amount_status(BOX_STATUS_EMPTY);
            }else if(this->medicine_amount_<=8) {
                set_amount_status(BOX_STATUS_RUNNING_LOW);
            }else {
                set_amount_status(BOX_STATUS_DEFAULT);
            }
        }
    }


    uint16_t medicine_amount() const {return medicine_amount_;}
    void set_medicine_amount(const uint16_t medicne_amount) {
        medicine_amount_set=true;
        this->medicine_amount_ = medicne_amount;
        reduce_amount(0);

    }


    BoxStatus &box_status() {return box_status_;}
    void set_box_status(const BoxStatus status) {
        this->box_status_ = status;
        if(this->amount_status_==BOX_STATUS_EMPTY) {
                if( this->box_status_==BOX_STATUS_DEFAULT ||
                    this->box_status_==BOX_STATUS_COMPLETED ||
                    this->box_status_==BOX_STATUS_MISSED ||
                    this->box_status_==BOX_STATUS_CURRENT
                ) {
                    Status_Directive::set_mode(this->box_no_, BOX_STATUS_EMPTY);
                    this->box_status_= BOX_STATUS_EMPTY;
                  }
        }else if(this->amount_status_==BOX_STATUS_RUNNING_LOW) {
                if( this->box_status_==BOX_STATUS_DEFAULT ||
                    this->box_status_==BOX_STATUS_COMPLETED
                ) {
                    Status_Directive::set_mode(this->box_no_, BOX_STATUS_RUNNING_LOW);
                  }
        }else {
            Status_Directive::set_mode(this->box_no_, status);
        }
    }


    void toStringPrint() const{
        Serial.print("{");
        Serial.print( R"(", box_no" : ")"); Serial.print(this->box_no_);
        Serial.print( R"(", med_amt" : ")");Serial.print(this->medicine_amount_);
        Serial.print( "} ");
    }


    String toString() const{
        return "{"
                R"(", bno" : ")"+ static_cast<String>(box_no_) +
                R"(", amt" : ")"+ static_cast<String>(medicine_amount_) +
                    "} ";
    }
};

#endif //BOX_H



