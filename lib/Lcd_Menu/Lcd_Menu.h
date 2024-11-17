#ifndef LCD_MENU_H
#define LCD_MENU_H
#include <Arduino.h>
#include <IPAddress.h>

class Lcd_Menu {
public:
    Lcd_Menu()=default;
    static void initializeLcd();

    static void handleMenu();
    static void menuPage();
    static void ap_page();
    static void start_ap_Page();
    static void stop_ap_Page();
    static void refresh_network_status_page();
    static void net_status_Page();
    static void ap_status_Page();
    static void network_page();
    static void setupPage();
    static void dls_page();
    static void adjust_daylight_saving_page(bool dls);
    static void showIpPage() ;
    static void changeIpPage();
    static IPAddress setIpPage(IPAddress &temp_SYS_IP_small) ;
    static void serverIpPage(IPAddress &temp_SYS_IP);
    static void setSubnetPage(IPAddress &temp_SYS_SB);
    static void helpPage();

    static bool checkIPAddrValid(const IPAddress &ip_small);
    static bool checkSBAddrValid(const IPAddress &sb_small);

    static uint8_t checkForButtonPress(const uint8_t * buttons, uint8_t size);
    static uint8_t checkForButtonPress(uint8_t button);

    static String get_IP_big_as_string(const byte *a);
    static void lcd_print_ip_big(byte x, byte y, const byte *IP_big);

    static void bigToSmall(IPAddress &temp_SYS_IP, const byte *bigIp);
    static void smallToBig(const IPAddress &temp_SYS_IP, byte *bigIp);

    static boolean confirm(const String &confirm_object, const String &confirm_message, bool start_with=false) ;
    static void lcd_clear_block(byte x, byte y);
    static void blink_cursor(byte x, byte y);
    static void printOptions(const String *option_str_arr, uint8_t option_str_arr_size, uint8_t index);
    static void printCenter(const String &a, uint8_t column);
    static void printLeft(const String &a, uint8_t column);
    static void lcdClear(byte col) ;
    static void lcdClear();

};

#endif //LCD_MENU_H
