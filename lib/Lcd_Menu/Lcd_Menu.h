#ifndef LCD_MENU_H
#define LCD_MENU_H
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

class Lcd_Menu {
private:
    LiquidCrystal_I2C * lcd; //(addr, row, col)
    byte SYSTEM_IP[4]={ 192,168,4,28 };
    byte SYSTEM_SB[4]={ 255,255,255,0 };

    byte enterButton;
    byte leftButton;
    byte downButton;
    byte rightButton;
    byte upButton;
    byte frontButton;
    byte backButton;
public:
    Lcd_Menu(const byte & enterButton_, const byte & leftButton_,
        const byte & downButton_, const byte & rightButton_, const byte & upButton_,
        const byte & frontButton_, const byte & backButton_);
    void initializeLcd()const;
    void initializePins()const;

    void ipMenu();
    void setupPage();
    void menuPage();
    boolean confirm(const byte *iP) const;
    static String getIp(const byte *a);
    static String getIpBig(const byte *a);
    void setIp(byte *temp_SYS_IP) const;
    static void bigToSmall(byte *temp_SYS_IP, const byte *bigIp);
    static void smallToBig(byte const *temp_SYS_IP, byte *bigIp);
    static String ipSort(const String & ip);
    void lcdClear(byte col) const;
    LiquidCrystal_I2C & getLcd() const;









};



#endif //LCD_MENU_H
