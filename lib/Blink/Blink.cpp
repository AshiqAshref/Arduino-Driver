#include "Blink.h"

#include <Led_Indicator.h>

extern Led_Indicator led_indicator;
void Blink::blink(){
    if((millis() - this->blinkPrevioustime_) > this->delay_) {
        this->blinkPrevioustime_=millis();
        if(this->blinkState_){
            if(color_single) blink_ch();
            else blink_st();
        }else
            led_indicator.setColor(this->boxNo_, COLOR::CLEAR);
        this->blinkState_ = !this->blinkState_;

    }
}

void Blink::blink_ch() const {
    led_indicator.setColor(this->boxNo_, this->color_a);
}
void Blink::blink_st() const {
    const COLOR color[2] = {this->color_a,this->color_b};
    led_indicator.setColor(this->boxNo_, color);
}


