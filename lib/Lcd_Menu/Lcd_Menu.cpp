#include "Lcd_Menu.h"
#include <AV_Functions.h>
#include <IO_PINS.h>
#include <Command_deactivate_ap.h>
#include <Command_activate_ap.h>
#include <Command_daylight_sav.h>
#include <Command_get_network_inf.h>
#include <Command_server_ip.h>
#include <LiquidCrystal_I2C.h>
#include <Network_info.h>
#include <RTClib.h>

extern LiquidCrystal_I2C lcd;
extern Command_activate_AP command_activate_AP;
extern Command_deactivate_ap command_deactivate_ap;
extern Command_get_network_inf command_get_network_inf;
extern Network_info network_info;
extern Command_daylight_sav command_daylight_sav;
extern Command_server_ip command_server_ip;
extern RTC_DS1307 rtc;


constexpr uint8_t buttons_horizontal_size = 3;
constexpr uint8_t horizontal_button_config[buttons_horizontal_size] = { //left, right, enter
  BUTTON_LEFT,
  BUTTON_RIGHT,
  BUTTON_ENTER
};

constexpr uint8_t buttons_vertical_size = buttons_horizontal_size;
constexpr uint8_t vertical_button_config[buttons_horizontal_size] = { //up, down, enter
  BUTTON_UP,
  BUTTON_DOWN,
  BUTTON_ENTER
};

constexpr uint8_t buttons_all_size = 5;
constexpr uint8_t buttons_all[buttons_all_size] = {
  BUTTON_LEFT,
  BUTTON_RIGHT,
  BUTTON_UP,
  BUTTON_DOWN,
  BUTTON_ENTER
};




void Lcd_Menu::initializeLcd(){
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(2,0);
  lcd.print("Initialized");
  byte ip[8]={
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b10111,
    0b10101,
    0b10111,
    0b10100
  };
  byte sb[8]={
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11010,
    0b10010,
    0b01011,
    0b11011
  };
  byte arrow[8]={
    0b00100,
    0b00100,
    0b01010,
    0b01010,
    0b10001,
    0b10001,
    0b00000,
    0b00000
  };
  byte blank[8]={
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000
  };

  lcd.createChar(0, arrow);
  lcd.createChar(1, blank);
  lcd.createChar(2, ip);
  lcd.createChar(3, sb);
  // AV_Functions::beepFor(100);
}

void Lcd_Menu::handleMenu() {
  if(AV_Functions::checkForButtonPress(BUTTON_ENTER)) {
    menuPage();
  }
}

void Lcd_Menu::menuPage(){
  constexpr byte total_menu_items=4;
  const String menu_items[total_menu_items] = {"Setup", "Network", "Help", "Back"};
  byte current_menu_item=0;

  printOptions(menu_items,total_menu_items,current_menu_item);
  boolean exit=false;
  while(!exit){
    const uint8_t button = AV_Functions::checkForButtonPress(vertical_button_config, buttons_vertical_size);
    if(button!=0) {
      if(button==BUTTON_UP)  {
        if(current_menu_item==0)current_menu_item= total_menu_items-1;
        else current_menu_item--;
        printOptions(menu_items,total_menu_items,current_menu_item);
      }else if(button==BUTTON_DOWN) {
        current_menu_item++;
        if(current_menu_item>total_menu_items-1) current_menu_item=0;
        printOptions(menu_items,total_menu_items,current_menu_item);
      }else if(button==BUTTON_ENTER) {
        if(current_menu_item == 0) {setupPage(); exit = true;}
        else if(current_menu_item==1){; exit = true; network_page();}
        else if(current_menu_item==2) helpPage();
        else if(current_menu_item==total_menu_items-1) exit = true;
        if(!exit)
          printOptions(menu_items,total_menu_items,current_menu_item);
      }
    }
  }
  lcd.clear();
}

void Lcd_Menu::setupPage(){
  constexpr byte total_menu_items=3;
  const String menu_items[total_menu_items] = {"daylight_sav","Server ip", "Back"};
  byte current_menu_item=0;
  printOptions(menu_items,total_menu_items,current_menu_item);

  boolean exit=false;
  while(!exit){
    const uint8_t button = AV_Functions::checkForButtonPress(vertical_button_config, buttons_vertical_size);
    if(button!=0) {
      if(button == BUTTON_UP) {
        if(current_menu_item==0)current_menu_item= total_menu_items-1;
        else current_menu_item--;
        printOptions(menu_items,total_menu_items,current_menu_item);
      }else if(button == BUTTON_DOWN) {
        current_menu_item++;
        if(current_menu_item>total_menu_items-1) current_menu_item=0;
        printOptions(menu_items,total_menu_items,current_menu_item);
      }else if(button==BUTTON_ENTER) {
        if(current_menu_item == 0) {dls_page(); exit = true;}
        else if(current_menu_item == 1) {changeIpPage(); exit = true;}
        else if(current_menu_item==total_menu_items-1) exit = true;
        printOptions(menu_items,total_menu_items,current_menu_item);
      }
    }
  }
}

void Lcd_Menu::dls_page() {
  const bool new_dls = confirm("Daylight Save","acvt?", network_info.daylight_saving());
  // const bool cur_dls = network_info.daylight_saving();
  // if(new_dls != cur_dls)
  //   adjust_daylight_saving_page(new_dls);
  network_info.set_daylight_saving(new_dls);
  command_daylight_sav.send_request(network_info.daylight_saving(), RES_SET);

  lcd.clear();
  printCenter("OK",0);
  delay(700);
  lcd.clear();
}

void Lcd_Menu::adjust_daylight_saving_page(const bool dls) {
  constexpr int hour_as_millisec = 3600;
  dls?
      rtc.adjust(DateTime(rtc.now().unixtime()+hour_as_millisec)):
      rtc.adjust(DateTime(rtc.now().unixtime()-hour_as_millisec));
}

void Lcd_Menu::network_page() {
  constexpr byte total_menu_items=5;
  const String menu_items[total_menu_items] = {"Net status","Refresh stat","Activate AP", "Deactivate AP", "Back"};
  byte current_menu_item=0;

  printOptions(menu_items,total_menu_items,current_menu_item);
  boolean exit=false;
  while(!exit){
    const uint8_t button = AV_Functions::checkForButtonPress(vertical_button_config, buttons_vertical_size);
    if(button!=0) {
      if(button==BUTTON_UP)  {
        if(current_menu_item==0)current_menu_item= total_menu_items-1;
        else current_menu_item--;
        printOptions(menu_items,total_menu_items,current_menu_item);
      }else if(button==BUTTON_DOWN) {
        current_menu_item++;
        if(current_menu_item>total_menu_items-1) current_menu_item=0;
        printOptions(menu_items,total_menu_items,current_menu_item);
      }else if(button==BUTTON_ENTER) {
        if(current_menu_item == 0) {net_status_Page();exit = true;}
        else if(current_menu_item==1){refresh_network_status_page();exit = true;}
        else if(current_menu_item==2){start_ap_Page();exit = true;}
        else if(current_menu_item==3){stop_ap_Page();exit = true;}
        else if(current_menu_item==total_menu_items-1) exit = true;
        if(!exit)
          printOptions(menu_items,total_menu_items,current_menu_item);
      }
    }
  }
  lcd.clear();
}

void Lcd_Menu::start_ap_Page() {
  lcdClear();
  command_activate_AP.send_request();
  printCenter("start req sent",0);
  delay(1000);
}

void Lcd_Menu::stop_ap_Page() {
  lcdClear();
  command_deactivate_ap.send_request();
  printCenter("stop req sent",0);
  delay(1000);
}

void Lcd_Menu::refresh_network_status_page() {
  printCenter("updating_info",0);
  command_get_network_inf.send_request();
  delay(700);
}

void Lcd_Menu::net_status_Page() {
  lcd.clear();
  if(network_info.needs_update()) {
    refresh_network_status_page();
    return;
  }
  if(network_info.is_ap_active()) {
    printCenter("AP_ACTIVE",0);
    printCenter(network_info.get_esp_IP_string(),1);
  }else if(network_info.is_wifi_active()) {
    printCenter("Wifi_ACTIVE",0);
    if(network_info.connected()) {
      printCenter(network_info.get_esp_IP_string(),1);
    }else {
      printCenter("NOT_CONNECTED",1);
    }
  }else {
    printCenter("NET_IDLE",1);
  }
  while(AV_Functions::checkForButtonPress(BUTTON_ENTER)!=BUTTON_ENTER){}
}

void Lcd_Menu::helpPage() {
  lcd.clear();
  printCenter("help page",0);
  delay(1000);
}

void Lcd_Menu::changeIpPage(){
  constexpr byte total_menu_items=4;
  const String menu_items[total_menu_items] = {"Show IP", "Set IP", "Set Subnet", "Back"};
  byte current_menu_item=0;
  printOptions(menu_items,total_menu_items,current_menu_item);

  IPAddress temp_SYS_IP = network_info.server_IP();
  IPAddress temp_SYS_SB = network_info.server_SB();

  boolean exit=false;
  while(!exit){
    const uint8_t button = AV_Functions::checkForButtonPress(vertical_button_config, buttons_vertical_size);
    if(button!=0) {
      if(button == BUTTON_UP) {
        if(current_menu_item==0)current_menu_item= total_menu_items-1;
        else current_menu_item--;
        printOptions(menu_items,total_menu_items,current_menu_item);
      }else if(button == BUTTON_DOWN) {
        current_menu_item++;
        if(current_menu_item>total_menu_items-1) current_menu_item=0;
        printOptions(menu_items,total_menu_items,current_menu_item);
      }else if(button==BUTTON_ENTER) {
        if(current_menu_item == 0) showIpPage();
        else if(current_menu_item==1) {serverIpPage(temp_SYS_IP); exit = true;}
        else if(current_menu_item==2) setSubnetPage(temp_SYS_SB);
        else if(current_menu_item==total_menu_items-1) exit = true;
        printOptions(menu_items,total_menu_items,current_menu_item);
      }
    }
  }
}

void Lcd_Menu::setSubnetPage(IPAddress &temp_SYS_SB) {
  temp_SYS_SB = setIpPage(temp_SYS_SB);
  if(!checkSBAddrValid(temp_SYS_SB)) {
    printCenter("INVALID SUBNET", 0);
    return;
  }
  if(confirm(temp_SYS_SB.toString(), "Confirm?")){
      network_info.set_server_SB(temp_SYS_SB);
}
}

void Lcd_Menu::serverIpPage(IPAddress &temp_SYS_IP) {
  temp_SYS_IP = setIpPage(temp_SYS_IP);
  Serial.println("New Ip: ");
  Serial.println(temp_SYS_IP);
  if(!checkIPAddrValid(temp_SYS_IP)) {
    lcdClear();
    printCenter(temp_SYS_IP.toString(), 0);
    printCenter("INVALID IP", 1);
    delay(700);
    return;
  }
  if(confirm(temp_SYS_IP.toString(),"Confirm?")){
    network_info.set_server_IP(temp_SYS_IP);
    command_server_ip.send_request(network_info.server_IP(),RES_SET);

  }
}

void Lcd_Menu::showIpPage()  {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.write(2);
  lcd.print(network_info.server_IP());

  lcd.setCursor(0,1);
  lcd.write(3);
  lcd.print(network_info.server_SB());
  while(AV_Functions::checkForButtonPress(BUTTON_ENTER)!=BUTTON_ENTER){}

}

IPAddress Lcd_Menu::setIpPage(IPAddress &temp_SYS_IP_small)  {
  byte temp_IP_big[12];

  smallToBig(temp_SYS_IP_small,temp_IP_big);
  Serial.print("asBigIP: ");
  Serial.println(get_IP_big_as_string(temp_IP_big));

  lcd_print_ip_big(0,0,temp_IP_big);
  byte curPos=0;
  byte dataPos=0;
  while(true){
    const uint8_t button = AV_Functions::checkForButtonPress(buttons_all, buttons_all_size);
      if(button == BUTTON_UP) {
        if(temp_IP_big[dataPos]>=9)temp_IP_big[dataPos]=0;
        else temp_IP_big[dataPos]++;
        lcd_print_ip_big(0,0,temp_IP_big);
      }else if(button == BUTTON_DOWN) {
        if(temp_IP_big[dataPos]==0)temp_IP_big[dataPos]=9;
        else temp_IP_big[dataPos]--;
        lcd_print_ip_big(0,0,temp_IP_big);
      }else if(button == BUTTON_LEFT) {
        lcd_clear_block(curPos,1);
        if(dataPos==0) dataPos = 11;
        dataPos--;
        curPos = dataPos + (dataPos/3);
      }else if(button == BUTTON_RIGHT) {
        lcd_clear_block(curPos,1);
        if(dataPos==11) dataPos = 0;
        else dataPos++;
        curPos = dataPos + (dataPos/3);
      }else if(button==BUTTON_ENTER) {
        bigToSmall(temp_SYS_IP_small,temp_IP_big);
        return temp_SYS_IP_small;
      }
      blink_cursor(curPos,1);
  }
}


bool Lcd_Menu::checkIPAddrValid(const IPAddress &ip_small) {
  const IPAddress invalid_ip_2(127,0,0,1);
  if(ip_small.toString().equals(invalid_ip_2.toString()))
    return false;
  if(ip_small[0]<1 || ip_small[3]<1)
    return false;
  for(byte i=0;i<4;i++) {
    if(ip_small[i] > 254)
      return false;
  }
  return true;
}
bool Lcd_Menu::checkSBAddrValid(const IPAddress &sb_small) {
  byte occurrence_0=0;
  byte occurrence_255=0;
  if(sb_small[0]<255)
    return false;
  for(byte i=0;i<4;i++) {
    if(sb_small[i]!=255) {
      for(byte j=i-1; j>0; j--)
        if(sb_small[j] < 255)
          return false;
    }
    if(sb_small[i]==0)
      occurrence_0++;
    if(sb_small[i]==255)
      occurrence_255++;
  }
  if(occurrence_0>3 || occurrence_255>3)
    return false;
  return true;
}

void Lcd_Menu::lcd_print_ip_big(const byte x, const byte y, const byte *IP_big) {
  lcd.clear();
  lcd.setCursor(x,y);
  lcd.print(get_IP_big_as_string(IP_big));
}
String Lcd_Menu::get_IP_big_as_string(const byte *a){
  String temp;
  for(int i=0;i<12;i++){
    temp+=String(a[i]);
    if(i!=11 && (i+1)%3==0)
      temp+='.';
  }
  return temp;
}

void Lcd_Menu::bigToSmall(IPAddress &temp_SYS_IP, const byte *bigIp){
/* Converts BigIP to SmallIP
 * FOR EXAMPLE
 * 192.003.045.200 -> 192.3.45.200
 */
  String octat_string = "";
  byte current_octat=0;
  for(int i=0;i<12;i++) {
    octat_string += String(bigIp[i]);
    if(i>0 && (i+1)%3==0) {
      const long to_int = octat_string.toInt();
      to_int<=255 && to_int>=0?
          temp_SYS_IP[current_octat]= to_int:
          temp_SYS_IP[current_octat]= 200;
      current_octat++;
      octat_string="";
    }
  }
}
void Lcd_Menu::smallToBig(const IPAddress& temp_SYS_IP, byte *bigIp){
/* Converts 4 byte IP into 12 Byte
 * example
 * 192.3.45.200 -> 192.003.045.200
 */
  byte bigIpCounter=0;
  for(int i=0;i<4;i++){
    String temp;
    temp=String(temp_SYS_IP[i]);
    for(uint16_t k=temp.length();(k-3)!=0;k++){
      bigIp[bigIpCounter]=0;
      bigIpCounter++;
    }
    for(uint16_t j=0;j<temp.length();j++){
      bigIp[bigIpCounter]=temp.charAt(j)-'0';
      bigIpCounter++;
    }
  }
}


bool lcd_menu_var_currentState = true;
unsigned long lcd_menu_var_blinkTime=0;
void Lcd_Menu::blink_cursor(const byte x, const byte y) {
  if(millis()-lcd_menu_var_blinkTime>=300){
    if(lcd_menu_var_currentState){
      lcd.setCursor(x, y);
      lcd.write(0);
    }else{
      lcd.setCursor(x, y);
      lcd.write(1);
    }
    lcd_menu_var_currentState=!lcd_menu_var_currentState;
    lcd_menu_var_blinkTime=millis();
  }
}
boolean Lcd_Menu::confirm(const String &confirm_object, const String &confirm_message, const bool start_with)  {
  lcd.clear();
  void(*print_confirm_message)(bool, const String&) = [](const bool state, const String& message_) {
    state?
      printCenter(message_ + " >Y  N",1):
      printCenter(message_ + "  Y >N",1);
  };

  printCenter(confirm_object,0);
  boolean confirmFlag=start_with;
  print_confirm_message(confirmFlag,confirm_message);

  while(true){
    const uint8_t button = AV_Functions::checkForButtonPress(horizontal_button_config, buttons_horizontal_size);
    if(button == BUTTON_LEFT || button == BUTTON_RIGHT) {
      confirmFlag = !confirmFlag;
      print_confirm_message(confirmFlag,confirm_message);
    }else if(button==BUTTON_ENTER) {
      return confirmFlag;
    }
  }
}
void Lcd_Menu::printOptions(const String* option_str_arr, const uint8_t option_str_arr_size, const uint8_t index) {
  lcd.clear();
  if(option_str_arr_size-1==index) {
    printCenter('>'+option_str_arr[index]+'<',0);
    printCenter(option_str_arr[0],1);
  }else{
    printCenter('>'+option_str_arr[index]+'<',0);
    printCenter(option_str_arr[index+1],1);
  }
}
void Lcd_Menu::printCenter(const String &a, const uint8_t column ) {
  uint16_t row=15;
  constexpr uint16_t lcd_width = 16;
  if (a.length()>16) return;
  if(a.length()<16) {
    row = lcd_width - a.length();
    row = row/2;
  }
  lcd.setCursor(row,column);
  lcd.print(a);
}
void Lcd_Menu::printLeft(const String &a, const uint8_t column) {
  lcd.setCursor(0,column);
  lcd.print(a);
}
void Lcd_Menu::lcd_clear_block(const byte x, const byte y) {
  lcd.setCursor(x,y);
  lcd.write(1);
}
void Lcd_Menu::lcdClear(byte const col)  {
    lcd.setCursor(0, col);
    lcd.print("                ");
    lcd.setCursor(0, col);
}
void Lcd_Menu::lcdClear() {
  lcd.clear();
}



