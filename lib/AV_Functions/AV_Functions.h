#ifndef AV_FUNCTIONS_H
#define AV_FUNCTIONS_H
#include <Arduino.h>
#include <COLOR.h>

class AV_Functions {
public:
    AV_Functions()=default;
    ~AV_Functions()=default;
    static void beepFor(int time);
    static void beepFor(int delay1, int delay2);
    static void beepFor(int delay1, int delay2, byte repeat);
    static void waitForInput();
    static void flush_();
    static COLOR char_to_enum(char a);
    static char enum_to_char(COLOR color);
};
#endif //AV_FUNCTIONS_H
