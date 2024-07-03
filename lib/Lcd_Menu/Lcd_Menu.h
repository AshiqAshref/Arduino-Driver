#ifndef LCD_MENU_H
#define LCD_MENU_H
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <AV_Functions.h>

class Lcd_Menu {
    byte SYSTEM_IP[4]={ 192,168,4,28 };
    byte SYSTEM_SB[4]={ 255,255,255,0 };
protected:
    LiquidCrystal_I2C *lcd; //(addr, row, col)
public:
    explicit Lcd_Menu();
    void initializeLcd()const;
    void menuPage();
    void setupPage();
    void showIpPage() const;
    void changeIpPage();
    void setIpPage(byte *temp_SYS_IP);
    void setSubnetPage(byte *temp_SYS_SB);
    static void helpPage();

    static uint8_t checkForInputTest(const uint8_t * buttons, uint8_t size);
    static uint8_t checkForInputTest(uint8_t button);

    boolean confirm(const byte *iP) const;
    static String getIp(const byte *a);
    static String getIpBig(const byte *a);
    void setIp(byte &temp_SYS_IP) const;
    static void bigToSmall(byte *temp_SYS_IP, const byte *bigIp);
    static void smallToBig(byte const *temp_SYS_IP, byte *bigIp);
    static String ipSort(const String &ip);
    void lcdClear(byte col) const;
    LiquidCrystal_I2C &getLcd() const;
};

#endif //LCD_MENU_H
