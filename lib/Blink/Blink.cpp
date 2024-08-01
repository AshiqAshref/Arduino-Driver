#include "Blink.h"
#include <Led_Indicator.h>

extern Led_Indicator led_indicator;
void Blink::blink(){
    if((millis() - this->blinkPrevioustime_) > this->delay_){
        this->blinkPrevioustime_=millis();
        if(this->blinkState_)
            led_indicator.setColor(this->boxNo_, this->color_);
        else
            led_indicator.setColor(this->boxNo_, 'c');
        this->blinkState_ = !this->blinkState_;
    }
}
