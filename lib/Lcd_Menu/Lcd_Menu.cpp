// #include "Lcd_Menu.h"
// #include <BUTTON_PINS.h>
//
// Lcd_Menu::Lcd_Menu() {
//   lcd = new LiquidCrystal_I2C(0x27, 16, 2);
//   initializeLcd();
//   AV_Functions::beepFor(100);
// }
//
//
// LiquidCrystal_I2C &Lcd_Menu::getLcd() const {
//   return *lcd;
// }
//
//
// void Lcd_Menu::initializeLcd()const{
//   lcd->init();
//   lcd->backlight();
//   lcd->clear();
//   lcd->setCursor(2,0);
//   lcd->print("Initialized");
//   byte ip[8]={
//     0b00000,
//     0b00000,
//     0b00000,
//     0b00000,
//     0b10111,
//     0b10101,
//     0b10111,
//     0b10100
//   };
//   byte sb[8]={
//     0b00000,
//     0b00000,
//     0b00000,
//     0b00000,
//     0b11010,
//     0b10010,
//     0b01011,
//     0b11011
//   };
//   byte arrow[8]={
//     0b00100,
//     0b00100,
//     0b01010,
//     0b01010,
//     0b10001,
//     0b10001,
//     0b00000,
//     0b00000
//   };
//   byte blank[8]={
//     0b00000,
//     0b00000,
//     0b00000,
//     0b00000,
//     0b00000,
//     0b00000,
//     0b00000,
//     0b00000
//   };
//
//   lcd->createChar(0, arrow);
//   lcd->createChar(1, blank);
//   lcd->createChar(2, ip);
//   lcd->createChar(3, sb);
// }
//
//
// void Lcd_Menu::menuPage(){
//   constexpr byte total_menu_items=2;
//   byte menu_item=0;
//   boolean exit=false;
//   boolean menu_item_changed=true;
//   constexpr uint8_t buttons[3] = {
//     static_cast<uint8_t>(BUTTON_PINS::upButton),
//     static_cast<uint8_t>(BUTTON_PINS::downButton),
//     static_cast<uint8_t>(BUTTON_PINS::enterButton)
//   };
//   constexpr uint8_t button_size = sizeof(buttons);
//
//   while(!exit){
//     const uint8_t button = checkForInputTest(buttons, button_size);
//     if(button!=0) {
//       if(button==static_cast<uint8_t>(BUTTON_PINS::upButton))  {
//         if(menu_item==0)menu_item= total_menu_items;
//         else menu_item--;
//         menu_item_changed=true;
//       }else if(button==static_cast<uint8_t>(BUTTON_PINS::downButton)) {
//         menu_item++;
//         if(menu_item>total_menu_items) menu_item=0;
//         menu_item_changed=true;
//       }else if(button==static_cast<uint8_t>(BUTTON_PINS::enterButton)) {
//         menu_item_changed=true;
//         if(menu_item == 0) setupPage();
//         else if(menu_item==1) helpPage();
//         else if(menu_item==total_menu_items) exit = true;
//       }
//     }
//
//     if(menu_item_changed){
//       menu_item_changed=false;
//       switch(menu_item){
//         case 0:
//           lcd->clear();
//           lcd->setCursor(5,0);
//           lcd->print(">Setup<");
//           lcd->setCursor(6,1);
//           lcd->print("Help");
//           break;
//
//         case 1:
//           lcd->clear();
//           lcd->setCursor(6,0);
//           lcd->print("Setup");
//           lcd->setCursor(5,1);
//           lcd->print(">Help<");
//           break;
//
//         case 2:
//           lcd->clear();
//           lcd->setCursor(6,0);
//           lcd->print("Help");
//           lcd->setCursor(5,1);
//           lcd->print(">Back<");
//           break;
//
//         default:
//           break;
//       }
//       delay(1);
//     }
//   }
// }
//
//
// void Lcd_Menu::helpPage() {
//   Serial.println();
// }
//
//
//
//
// void Lcd_Menu::changeIpPage(){
//   constexpr byte total_menu_items=3;
//   constexpr uint8_t buttons[] = {
//     static_cast<uint8_t>(BUTTON_PINS::upButton),
//     static_cast<uint8_t>(BUTTON_PINS::downButton),
//     static_cast<uint8_t>(BUTTON_PINS::enterButton)
//   };
//   constexpr uint8_t button_size = sizeof(buttons);
//   byte menu_item=0;
//   boolean menu_item_changed=true;
//   boolean exit=false;
//   while(!exit){
//     byte temp_SYS_IP[4];
//     byte temp_SYS_SB[4];
//     for(int i=0;i<4;i++){
//       temp_SYS_IP[i]=SYSTEM_IP[i];
//       temp_SYS_SB[i]=SYSTEM_SB[i];
//     }
//
//     const uint8_t button = checkForInputTest(buttons, button_size);
//     if(button!=0) {
//       if(button == static_cast<uint8_t>(BUTTON_PINS::upButton)) {
//         if(menu_item==0)menu_item= total_menu_items;
//         else menu_item--;
//         menu_item_changed=true;
//       }else if(button == static_cast<uint8_t>(BUTTON_PINS::downButton)) {
//         menu_item++;
//         if(menu_item>total_menu_items) menu_item=0;
//         menu_item_changed=true;
//       }else if(button==static_cast<uint8_t>(BUTTON_PINS::enterButton)) {
//         menu_item_changed=true;
//         if(menu_item == 0) showIpPage();
//         else if(menu_item==1) setIpPage(temp_SYS_IP);
//         else if(menu_item==2) setSubnetPage(temp_SYS_SB);
//         else if(menu_item==total_menu_items) exit = true;
//       }
//     }
//
//
//     if(menu_item_changed==1){
//       switch(menu_item){
//         case 0:
//           lcd->clear();
//           lcd->setCursor(4,0);
//           lcd->print(">Show Ip<");
//           lcd->setCursor(5,1);
//           lcd->print("Set Ip");
//           menu_item_changed=false;
//           break;
//
//         case 1:
//           lcd->clear();
//           lcd->setCursor(5,0);
//           lcd->print("Show Ip");
//           lcd->setCursor(4,1);
//           lcd->print(">Set Ip<");
//           menu_item_changed=false;
//           break;
//
//         case 2:
//           lcd->clear();
//           lcd->setCursor(5,0);
//           lcd->print("Set Ip");
//           lcd->setCursor(2,1);
//           lcd->print(">Set Subnet<");
//           menu_item_changed=false;
//           break;
//
//         case 3:
//           lcd->clear();
//           lcd->setCursor(3,0);
//           lcd->print("Set Subnet");
//           lcd->setCursor(5,1);
//           lcd->print(">back<");
//           menu_item_changed=false;
//           break;
//
//         default:
//           break;
//       }
//       delay(1);
//     }
//   }
// }
//
//
// void Lcd_Menu::setSubnetPage(byte * temp_SYS_SB) {
//     setIp(*temp_SYS_SB);
//     if(confirm(temp_SYS_SB)){
//       for(int i=0;i<4;i++)
//         SYSTEM_SB[i]=temp_SYS_SB[i];
//     }
// }
//
//
// uint8_t Lcd_Menu::checkForInputTest(const uint8_t button) {
//   if(digitalRead(button)) {
//     AV_Functions::beepFor(100);
//     Serial.print("GOT THIS: ");
//     Serial.println(button);
//     while(digitalRead(button)){}
//     return button;
//   }return 0;
// }
//
//
// uint8_t Lcd_Menu::checkForInputTest(const uint8_t * buttons, const uint8_t size) {
//   for(uint8_t i=0; i<size; i++) {
//     const uint8_t button = checkForInputTest(buttons[i]);
//     if(button!=0) return button;
//   }return 0;
// }
//
//
//
// void Lcd_Menu::setIpPage(byte * temp_SYS_IP) {
//   setIp(*temp_SYS_IP);
//   if(confirm(temp_SYS_IP)){
//     for(int i=0;i<4;i++)
//       SYSTEM_IP[i]=temp_SYS_IP[i];
//   }
// }
//
//
// void Lcd_Menu::showIpPage() const {
//   constexpr auto button = static_cast<uint8_t>(BUTTON_PINS::enterButton);
//   lcd->clear();
//   lcd->setCursor(0,0);
//   lcd->write(2);
//   lcd->print(getIp(SYSTEM_IP));
//
//   lcd->setCursor(0,1);
//   lcd->write(3);
//   lcd->print(getIp(SYSTEM_SB));
//   while(checkForInputTest(button)!=button){}
// }
//
//
// void Lcd_Menu::setupPage(){
//   constexpr byte total_menu_items=1;
//   constexpr uint8_t buttons[3] = {
//     static_cast<uint8_t>(BUTTON_PINS::upButton),
//     static_cast<uint8_t>(BUTTON_PINS::downButton),
//     static_cast<uint8_t>(BUTTON_PINS::enterButton)
//   };
//   constexpr uint8_t button_size = sizeof(buttons);
//
//   boolean menu_item_changed=true;
//   byte menu_item=0;
//   boolean exit=false;
//   while(!exit){
//     const uint8_t button = checkForInputTest(buttons, button_size);
//     if(button!=0) {
//       if(button == static_cast<uint8_t>(BUTTON_PINS::upButton)) {
//         if(menu_item==0)menu_item= total_menu_items;
//         else menu_item--;
//         menu_item_changed=true;
//       }else if(button == static_cast<uint8_t>(BUTTON_PINS::downButton)) {
//         menu_item++;
//         if(menu_item>total_menu_items) menu_item=0;
//         menu_item_changed=true;
//       }else if(button==static_cast<uint8_t>(BUTTON_PINS::enterButton)) {
//         menu_item_changed=true;
//         if(menu_item == 0) changeIpPage();
//         else if(menu_item==total_menu_items) exit = true;
//       }
//     }
//
//     if(menu_item_changed==1){
//       switch(menu_item){
//         case 0:
//           lcd->clear();
//           lcd->setCursor(2,0);
//           lcd->print(">Ip address<");
//           lcd->setCursor(6,1);
//           lcd->print("Back");
//           menu_item_changed=false;
//           break;
//
//         case 1:
//           lcd->clear();
//           lcd->setCursor(3,0);
//           lcd->print("Ip address");
//           lcd->setCursor(5,1);
//           lcd->print(">Back<");
//           menu_item_changed=false;
//           break;
//
//         default:
//           break;
//       }
//       delay(1);
//     }
//   }
// }
//
//
// boolean Lcd_Menu::confirm(const byte *iP) const {
//   boolean menu_item_changed=true;
//   boolean confirmFlag=false;
//   constexpr uint8_t  buttons[] = {
//     static_cast<uint8_t>(BUTTON_PINS::leftButton),
//     static_cast<uint8_t>(BUTTON_PINS::rightButton),
//     static_cast<uint8_t>(BUTTON_PINS::enterButton)
//   };
//   constexpr uint8_t button_size = sizeof(buttons);
//   while(true){
//     const uint8_t button = checkForInputTest(buttons, button_size);
//     if(button!=0) {
//       if(button == static_cast<uint8_t>(BUTTON_PINS::leftButton)||
//          button == static_cast<uint8_t>(BUTTON_PINS::rightButton)) {
//         menu_item_changed = true;
//         confirmFlag = !confirmFlag;
//       }else if(button==static_cast<uint8_t>(BUTTON_PINS::enterButton)) {
//         return confirmFlag;
//       }
//     }
//
//     if(menu_item_changed){
//       if(!confirmFlag){
//         lcd->clear();
//         lcd->setCursor(1,0);
//         lcd->print(getIp(iP));
//         lcd->setCursor(0,1);
//         lcd->print("Confirm?  Y >N");
//         menu_item_changed=false;
//       }else{
//         lcd->clear();
//         lcd->setCursor(1,0);
//         lcd->print(getIp(iP));
//         lcd->setCursor(0,1);
//         lcd->print("Confirm? >Y  N");
//         menu_item_changed=false;
//       }
//     }
//   }
// }
//
//
// String Lcd_Menu::getIp(const byte *a){
//   String temp;
//   for(int i=0;i<4;i++){
//     temp+=String(a[i]);
//     if(i!=3)
//       temp+='.';
//   }
//   return temp;
// }
//
//
// String Lcd_Menu::getIpBig(const byte *a){
//   String temp;
//   for(int i=0;i<12;i++){
//     temp+=String(a[i]);
//     if(i!=11 && (i+1)%3==0)
//       temp+='.';
//   }
//   return temp;
// }
//
//
// void Lcd_Menu::setIp(byte &temp_SYS_IP) const {
//   byte temp_IP[12];
//
//   smallToBig(&temp_SYS_IP,temp_IP);
//   Serial.println(getIpBig(temp_IP));
//   constexpr uint8_t buttons[] = {
//     static_cast<uint8_t>(BUTTON_PINS::leftButton),
//     static_cast<uint8_t>(BUTTON_PINS::rightButton),
//     static_cast<uint8_t>(BUTTON_PINS::upButton),
//     static_cast<uint8_t>(BUTTON_PINS::downButton),
//     static_cast<uint8_t>(BUTTON_PINS::enterButton)
//   };
//   constexpr uint8_t button_size = sizeof(buttons);
//
//   boolean menu_item_changed=true;
//   byte curPos=0;
//   unsigned long blinkTime=0;
//   boolean currentState=false;
//   byte dataPos=0;
//   while(true){
//     const uint8_t button = checkForInputTest(buttons, button_size);
//     if(button!=0) {
//       if(button == static_cast<uint8_t>(BUTTON_PINS::upButton)) {
//         if(temp_IP[dataPos]>=9)temp_IP[dataPos]=0;
//         else temp_IP[dataPos]++;
//         menu_item_changed=true;
//         for(const unsigned char i : temp_IP) {
//           Serial.print(i);
//         }
//         Serial.println();
//
//       }else if(button == static_cast<uint8_t>(BUTTON_PINS::downButton)) {
//         if(temp_IP[dataPos]<=0)temp_IP[dataPos]=9;
//         else temp_IP[dataPos]--;
//         menu_item_changed=true;
//       }else if(button == static_cast<uint8_t>(BUTTON_PINS::leftButton)) {
//         lcd->setCursor(curPos, 1);
//         lcd->write(1);
//         if(curPos<=0){
//           curPos=14;
//           dataPos=12;
//         }else if((curPos+4)%4==0){
//           curPos-=2;
//           dataPos--;
//         }else{
//           curPos--;
//           dataPos--;
//         }
//       }else if(button == static_cast<uint8_t>(BUTTON_PINS::rightButton)) {
//         lcd->setCursor(curPos, 1);
//         lcd->write(1);
//         if(curPos>=14){
//           curPos=0;
//           dataPos=0;
//         }else if((curPos+2)%4==0){
//           curPos+=2;
//           dataPos++;
//         }else{
//           curPos++;
//           dataPos++;
//         }
//       }else if(button==static_cast<uint8_t>(BUTTON_PINS::enterButton)) {
//         bigToSmall(&temp_SYS_IP,temp_IP);
//         return;
//       }
//     }
//
//     if(millis()-blinkTime>=500){
//       if(currentState==0){
//         currentState=true;
//         lcd->setCursor(curPos, 1);
//         lcd->write(0);
//       }else{
//         currentState=false;
//         lcd->setCursor(curPos, 1);
//         lcd->write(1);
//       }
//       blinkTime=millis();
//     }
//
//     if(menu_item_changed==1){
//       lcd->clear();
//       byte cursor=0;
//       for(int i=0;i<12;i++){
//         if(i>0 && i%3==0){
//           cursor++;
//           lcd->setCursor(cursor,0);
//           lcd->print('.');
//         }
//         if(i!=0) cursor++;
//         lcd->setCursor(cursor,0);
//         lcd->print(String(temp_IP[i]));
//       }
//       menu_item_changed=false;
//     }
//
//   }
//   bigToSmall(&temp_SYS_IP,temp_IP);
//   Serial.println(getIp(&temp_SYS_IP));
// }
//
//
// // Converts 12 byte IP into 4 Byte
// // STILL TEST
// void Lcd_Menu::bigToSmall(byte *temp_SYS_IP, const byte *bigIp){
//   String temp;
//   byte i=0;
//   byte count=0;
//   byte pass=0;
//   while(i<=12){
//     temp+=String(bigIp[i]);
//     count++;
//     if(count==3){
//       temp_SYS_IP[pass]=temp.toInt();
//       Serial.println(temp.toInt());
//       pass++;
//       count=0;
//       temp="";
//     }
//     i++;
//   }
// }
//
//
// // Converts 4 byte IP into 12 Byte
// // STILL TEST
// void Lcd_Menu::smallToBig(byte const *temp_SYS_IP, byte *bigIp){
//   byte bigIpCounter=0;
//   for(int i=0;i<4;i++){
//     String temp;
//     temp=String(temp_SYS_IP[i]);
//     for(int k=temp.length();(k-3)!=0;k++){
//       bigIp[bigIpCounter]=0;
//       bigIpCounter++;
//     }
//     for(unsigned int j=0;j<temp.length();j++){
//       bigIp[bigIpCounter]=temp.charAt(j)-'0';
//       bigIpCounter++;
//     }
//   }
// }
//
//
// //GETS RID OF WHITE ZEROES
// // FOR EXAMPLE
// // 192.003.045.200 WILL BE CONVERTED INTO
// //192.3.45.200
// String Lcd_Menu::ipSort(const String & ip){
//   String sortedIp;
//   for(int i=0;i<=14;i++){
//     if(ip.charAt(i)!='0'&& ip.charAt(i)!='.'){
//       for(int k=i; (k+1)%4!=0; k++){
//         sortedIp+=ip.charAt(k);
//         i=k;
//       }
//       if(i<14) sortedIp+='.';
//     }
//   }
//   return sortedIp;
// }
//
//
// void Lcd_Menu::lcdClear(byte const col) const {
//     lcd->setCursor(0, col);
//     lcd->print("                ");
//     lcd->setCursor(0, col);
// }
//
//
//
