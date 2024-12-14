#include "AV_Functions.h"
#include <RTClib.h>

extern RTC_DS1307 rtc;

String AV_Functions::get_current_formated_Time(const byte mode) {
    return get_formated_Time(rtc.now(),mode);
}

uint32_t AV_Functions::get_current_unix_time() {
    return rtc.now().unixtime();
}
