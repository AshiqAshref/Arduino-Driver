#ifndef LCD_MENU_H
#define LCD_MENU_H
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>


class Lcd_Menu {

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

static void initializeLcd();

    static void handleMenu();
    static void menuPage();
    static void ap_page();
    static void start_ap_Page();
    static void stop_ap_Page();
    static void refresh_network_status();
    static void net_status_Page();
static void ap_status_Page();
    static void network_page();

static void setupPage();
    static void showIpPage() ;
    static void changeIpPage();

static bool checkIPAddrValid(const byte *ip_small);

static bool checkSBAddrValid(const byte *sb_small);

static bool IP_compare(const byte *ip_small_1, const byte *ip_small_2);

static void setIpPage(byte *temp_SYS_IP);
    static void setSubnetPage(byte *temp_SYS_SB);

    static void helpPage();

    static uint8_t checkForButtonPress(const uint8_t * buttons, uint8_t size);
    static uint8_t checkForButtonPress(uint8_t button);

    static boolean confirm(const byte *iP) ;



static String get_IP_as_string(const byte *a);
    static String get_IP_big_as_string(const byte *a);

    static byte* setIp(byte *temp_SYS_IP_small) ;

static byte calculate_cursor_position(byte dataPos);

static void lcd_print_ip_big(byte x, byte y, const byte *IP_big);
static void print_confirm_message(bool state);
static void printOptions(const String *a, uint8_t size, uint8_t index);
static void printCenter(const String &a, uint8_t column);
static void printLeft(const String &a, uint8_t column);

static void blink_cursor(byte x, byte y);

static void lcd_clear_block(byte x, byte y);

    static void bigToSmall(byte *temp_SYS_IP, const byte *bigIp);
    static void smallToBig(byte const *temp_SYS_IP, byte *bigIp);

    static String ipSort(const String &ip);

    static void lcdClear(byte col) ;
    static void lcdClear();

};

#endif //LCD_MENU_H
