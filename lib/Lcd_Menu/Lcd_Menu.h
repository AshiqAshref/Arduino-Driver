#ifndef LCD_MENU_H
#define LCD_MENU_H
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <AV_Functions.h>


class Lcd_Menu {
    byte SYSTEM_IP[4]={ 192,168,4,28 };
    byte SYSTEM_SB[4]={ 255,255,255,0 };
// protected:
    // LiquidCrystal_I2C *lcd; //(addr, row, col)
     // LiquidCrystal_I2C lcd; //(addr, row, col)
public:
    Lcd_Menu()=default;
    // explicit Lcd_Menu(LiquidCrystal_I2C &lcd)
    //     : lcd(lcd) {
    //     AV_Functions::beepFor(100);
    //     initializeLcd();
    //     AV_Functions::beepFor(100);
    // }

    // extern Lcd_Menu lcd_menu;
    ~Lcd_Menu(){Serial.println("Destroyed");}

    void initializeLcd();
    void menuPage();
    void setupPage();
    void showIpPage() const;
    void changeIpPage();
    void setIpPage(byte *temp_SYS_IP);
    void setSubnetPage(byte *temp_SYS_SB);

    static void helpPage();

    static uint8_t checkForInputTest(const uint8_t * buttons, uint8_t size);
    static uint8_t checkForInputTest(uint8_t button);

    static boolean confirm(const byte *iP) ;

    static String getIp(const byte *a);
    static String getIpBig(const byte *a);

    static void setIp(byte &temp_SYS_IP) ;

    static void bigToSmall(byte *temp_SYS_IP, const byte *bigIp);
    static void smallToBig(byte const *temp_SYS_IP, byte *bigIp);

    static String ipSort(const String &ip);

    static void lcdClear(byte col) ;

};

#endif //LCD_MENU_H
