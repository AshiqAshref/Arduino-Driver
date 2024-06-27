#include "Lcd_Menu.h"

Lcd_Menu::Lcd_Menu(const byte &enterButton_, const byte &leftButton_,
  const byte &downButton_, const byte &rightButton_, const byte &upButton_,
  const byte &frontButton_, const byte &backButton_) {

  lcd = new LiquidCrystal_I2C(0x27, 16, 2);
  enterButton=enterButton_;
  leftButton=leftButton_;
  downButton=downButton_;
  rightButton=rightButton_;
  upButton=upButton_;
  frontButton=frontButton_;
  backButton=backButton_;

  initializeLcd();
}


LiquidCrystal_I2C &Lcd_Menu::getLcd() const {
  return *lcd;
}


void Lcd_Menu::initializeLcd()const{
    lcd->init();
    lcd->backlight();
    lcd->clear();
    lcd->setCursor(2,0);
    lcd->print("Initialized");
}


void Lcd_Menu::initializePins()const{
  pinMode(enterButton,INPUT);
  pinMode(leftButton,INPUT);
  pinMode(rightButton,INPUT);
  pinMode(upButton,INPUT);
  pinMode(downButton,INPUT);
}


void Lcd_Menu::ipMenu(){
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

  lcd->createChar(2, ip);
  lcd->createChar(3, sb);

  boolean idleFlag=true;
  byte caseInt=0;
  boolean showIpFlag=false;
  boolean setIpFlag=false;
  boolean setSbFlag=false;
  boolean ipFlag=true;

  boolean enterState=false;
  boolean upState=false;
  boolean downState=false;

  while(ipFlag==1){
    constexpr byte totalCase=3;
    byte temp_SYS_IP[4];
    byte temp_SYS_SB[4];

    for(int i=0;i<4;i++){
      temp_SYS_IP[i]=SYSTEM_IP[i];
      temp_SYS_SB[i]=SYSTEM_SB[i];
    }

    while(digitalRead(upButton))    upState=true;
    while(digitalRead(downButton))  downState=true;
    while(digitalRead(enterButton)) enterState=true;

    if(upState==1){
      if(caseInt<=0){
        caseInt=totalCase;
      }else{
        caseInt--;
      }

      idleFlag=true;
      upState=false;
    }

    if(downState==1){
      if(caseInt>=totalCase){
        caseInt=0;
      }else{
        caseInt++;
      }

      idleFlag=true;
      downState=false;
    }

    if(enterState==1){

      if(caseInt==totalCase){
        ipFlag=false;
        idleFlag=false;
      }else if(caseInt==0){
        showIpFlag=true;
        idleFlag=true;
      }else if(caseInt==1){
        setIpFlag=true;
        idleFlag=true;
      }else if(caseInt==2){
        setSbFlag=true;
        idleFlag=true;
      }
      enterState=false;
    }

    if(idleFlag==1){
      switch(caseInt){
        case 0:
          while(showIpFlag==1){
            if(idleFlag==1){
              lcd->clear();
              lcd->setCursor(0,0);
              lcd->write(2);
              lcd->print(getIp(SYSTEM_IP));

              lcd->setCursor(0,1);
              lcd->write(3);
              lcd->print(getIp(SYSTEM_SB));

              idleFlag=false;
            }
            while(digitalRead(enterButton))enterState=true;
            if(enterState==1){
              enterState=false;
              showIpFlag=false;
            }
          }
          lcd->clear();
          lcd->setCursor(4,0);
          lcd->print(">Show Ip<");
          lcd->setCursor(5,1);
          lcd->print("Set Ip");
          idleFlag=false;
          break;

        case 1:
          while(setIpFlag==1){
            setIp(temp_SYS_IP);
            if(confirm(temp_SYS_IP)){
              for(int i=0;i<4;i++)
                SYSTEM_IP[i]=temp_SYS_IP[i];
            }

            setIpFlag=false;
          }
          lcd->clear();
          lcd->setCursor(5,0);
          lcd->print("Show Ip");
          lcd->setCursor(4,1);
          lcd->print(">Set Ip<");
          idleFlag=false;
          break;

        case 2:
          while(setSbFlag==1){
            setIp(temp_SYS_SB);
            if(confirm(temp_SYS_SB)){
              for(int i=0;i<4;i++)
                SYSTEM_SB[i]=temp_SYS_SB[i];
            }
            setSbFlag=false;
          }
          lcd->clear();
          lcd->setCursor(5,0);
          lcd->print("Set Ip");
          lcd->setCursor(2,1);
          lcd->print(">Set Subnet<");
          idleFlag=false;
          break;

        case 3:
          lcd->clear();
          lcd->setCursor(3,0);
          lcd->print("Set Subnet");
          lcd->setCursor(5,1);
          lcd->print(">back<");
          idleFlag=false;
          break;

        default:
          break;
      }
      delay(1);
    }
  }
}


void Lcd_Menu::setupPage(){
  boolean idleFlag=true;
  byte caseInt=0;
  boolean ipFlag=false;
  boolean setupFlag=true;

  boolean enterState=false;
  boolean upState=false;
  boolean downState=false;

  while(setupFlag==1){
    constexpr byte totalCase=1;

    while(digitalRead(upButton))    upState=true;
    while(digitalRead(downButton))  downState=true;
    while(digitalRead(enterButton)) enterState=true;

    if(upState==1){
      if(caseInt<=0){
        caseInt=totalCase;
      }else{
        caseInt--;
      }

      idleFlag=true;
      upState=false;
    }

    if(downState==1){
      if(caseInt>=totalCase){
        caseInt=0;
      }else{
        caseInt++;
      }

      idleFlag=true;
      downState=false;
    }

    if(enterState==1){

      if(caseInt==totalCase){
        setupFlag=false;
        idleFlag=false;
      }else if(caseInt==0){
        ipFlag=true;
        idleFlag=true;
      }
      enterState=false;
    }

    if(idleFlag==1){
      switch(caseInt){
        case 0:
          if(ipFlag==1){
            ipMenu();
            ipFlag=false;
          }
        lcd->clear();
        lcd->setCursor(2,0);
        lcd->print(">Ip address<");
        lcd->setCursor(6,1);
        lcd->print("Back");

        idleFlag=false;
        break;

        case 1:
          lcd->clear();
        lcd->setCursor(3,0);
        lcd->print("Ip address");
        lcd->setCursor(5,1);
        lcd->print(">Back<");
        idleFlag=false;
        break;

        default:
          break;
      }
      delay(1);
    }
  }
}


void Lcd_Menu::menuPage(){
  boolean idleFlag=true;
  boolean setupFlag=false;
  byte caseInt=0;
  boolean menuFlag=true;

  boolean enterState=false;
  boolean upState=false;
  boolean downState=false;
  // boolean mainDisplayFlag=false;
  while(menuFlag==1){
    constexpr byte totalCase=2;
    while(digitalRead(upButton))    upState=true;
    while(digitalRead(downButton))  downState=true;
    while(digitalRead(enterButton)) enterState=true;

    if(upState==1){
      if(caseInt<=0){
        caseInt=totalCase;
      }else{
        caseInt--;
      }

      idleFlag=true;
      upState=false;
    }

    if(downState==1){
      if(caseInt>=totalCase){
        caseInt=0;
      }else{
        caseInt++;
      }

      idleFlag=true;
      downState=false;
    }

    if(enterState==1){
      if(caseInt==totalCase){
        menuFlag=false;
        // mainDisplayFlag=true;
      }else if(caseInt==0){
        setupFlag=true;
        idleFlag=true;
      }
      enterState=false;
    }

    if(idleFlag==1){
      switch(caseInt){
        case 0:
          if(setupFlag==1){
            setupPage();
          }

        lcd->clear();
        lcd->setCursor(5,0);
        lcd->print(">Setup<");
        lcd->setCursor(6,1);
        lcd->print("Help");

        setupFlag=false;
        idleFlag=false;
        break;

        case 1:
          lcd->clear();
        lcd->setCursor(6,0);
        lcd->print("Setup");
        lcd->setCursor(5,1);
        lcd->print(">Help<");
        idleFlag=false;
        break;

        case 2:
          lcd->clear();
        lcd->setCursor(6,0);
        lcd->print("Help");
        lcd->setCursor(5,1);
        lcd->print(">Back<");
        idleFlag=false;
        break;

        default:
          break;
      }
      delay(1);
    }
  }
}


boolean Lcd_Menu::confirm(const byte *iP) const {
  boolean rightState=false;
  boolean leftState=false;
  boolean enterState=false;

  boolean idleFlag=true;
  boolean confirmFlag=false;
  while(true){
    while(digitalRead(enterButton)) enterState=true;
    while(digitalRead(leftButton))  leftState=true;
    while(digitalRead(rightButton)) rightState=true;

    if(leftState){
      leftState=false;
      idleFlag=true;
      confirmFlag = !confirmFlag;
    }else if(rightState){
      rightState=false;
      idleFlag=true;
      confirmFlag = !confirmFlag;
    }else if(enterState){
      return confirmFlag;
    }
    if(idleFlag==1){
      if(!confirmFlag){
        lcd->clear();
        lcd->setCursor(1,0);
        lcd->print(getIp(iP));
        lcd->setCursor(0,1);
        lcd->print("Confirm?  Y >N");
        idleFlag=false;
      }else{
        lcd->clear();
        lcd->setCursor(1,0);
        lcd->print(getIp(iP));
        lcd->setCursor(0,1);
        lcd->print("Confirm? >Y  N");
        idleFlag=false;
      }
    }
  }
}


String Lcd_Menu::getIp(const byte *a){
  String temp;
  for(int i=0;i<4;i++){
    temp+=String(a[i]);
    if(i!=3)
      temp+='.';
  }
  return temp;
}


String Lcd_Menu::getIpBig(const byte *a){
  String temp;
  for(int i=0;i<12;i++){
    temp+=String(a[i]);
    if(i!=11 && (i+1)%3==0)
      temp+='.';
  }
  return temp;
}

void Lcd_Menu::setIp(byte *temp_SYS_IP) const {
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
  lcd->createChar(0, arrow);
  lcd->createChar(1, blank);

  byte temp_IP[12];
  smallToBig(temp_SYS_IP,temp_IP);
  Serial.println(getIpBig(temp_IP));

  boolean runFlag=true;
  boolean idleFlag=true;
  byte curPos=0;
  unsigned long blinkTime=0;
  boolean currentState=false;
  byte dataPos=0;

  boolean enterState=false;
  boolean upState=false;
  boolean downState=false;
  boolean leftState=false;
  boolean rightState=false;

  while(runFlag==1){
    while(digitalRead(enterButton)) enterState=true;
    while(digitalRead(upButton))    upState=true;
    while(digitalRead(downButton))  downState=true;
    while(digitalRead(leftButton))    leftState=true;
    while(digitalRead(rightButton))  rightState=true;

    if(enterState==1){
      enterState=false;
      runFlag=false;
      idleFlag=false;
    }else if(rightState==1){
      lcd->setCursor(curPos, 1);
      lcd->write(1);

      if(curPos>=14){
        curPos=0;
        dataPos=0;
      }else if((curPos+2)%4==0){
        curPos+=2;
        dataPos++;
      }else{
        curPos++;
        dataPos++;
      }

      rightState=false;
    }else if(leftState==1){
      lcd->setCursor(curPos, 1);
      lcd->write(1);

      if(curPos<=0){
        curPos=14;
        dataPos=12;
      }else if((curPos+4)%4==0){
        curPos-=2;
        dataPos--;
      }else{
        curPos--;
        dataPos--;
      }

      leftState=false;
    }
    else if(upState==1){
      if(temp_IP[dataPos]>=9)
        temp_IP[dataPos]=0;
      else
        temp_IP[dataPos]++;

      idleFlag=true;
      upState=false;
    }else if(downState==1){
      if(temp_IP[dataPos]<=0)
        temp_IP[dataPos]=9;
      else
        temp_IP[dataPos]--;

      idleFlag=true;
      downState=false;
    }

    if(millis()-blinkTime>=500){
      if(currentState==0){
        currentState=true;
        lcd->setCursor(curPos, 1);
        lcd->write(0);
      }else{
        currentState=false;
        lcd->setCursor(curPos, 1);
        lcd->write(1);
      }
      blinkTime=millis();
    }

    if(idleFlag==1){
      lcd->clear();
      byte cursor=0;
      for(int i=0;i<12;i++){
        if(i>0 && i%3==0){
          cursor++;
          lcd->setCursor(cursor,0);
          lcd->print('.');
        }
        if(i!=0)
          cursor++;
        lcd->setCursor(cursor,0);
        lcd->print(String(temp_IP[i]));
      }
      idleFlag=false;
    }
  }
  bigToSmall(temp_SYS_IP,temp_IP);
  Serial.println(getIp(temp_SYS_IP));
}


// Converts 12 byte IP into 4 Byte
// STILL TEST
void Lcd_Menu::bigToSmall(byte *temp_SYS_IP, const byte *bigIp){
  String temp;
  byte i=0;
  byte count=0;
  byte pass=0;
  while(i<=12){
    temp+=String(bigIp[i]);
    count++;
    if(count==3){
      temp_SYS_IP[pass]=temp.toInt();
      Serial.println(temp.toInt());
      pass++;
      count=0;
      temp="";
    }
    i++;
  }
}


// Converts 4 byte IP into 12 Byte
// STILL TEST
void Lcd_Menu::smallToBig(byte const *temp_SYS_IP, byte *bigIp){
  byte bigIpCounter=0;
  for(int i=0;i<4;i++){
    String temp;
    temp=String(temp_SYS_IP[i]);
    for(int k=temp.length();(k-3)!=0;k++){
      bigIp[bigIpCounter]=0;
      bigIpCounter++;
    }
    for(unsigned int j=0;j<temp.length();j++){
      bigIp[bigIpCounter]=temp.charAt(j)-'0';
      bigIpCounter++;
    }
  }
}


//GETS RID OF WHITE ZEROES
// FOR EXAMPLE
// 192.003.045.200 WILL BE CONVERTED INTO
//192.3.45.200
String Lcd_Menu::ipSort(const String & ip){
  String sortedIp;
  for(int i=0;i<=14;i++){
    if(ip.charAt(i)!='0'&& ip.charAt(i)!='.'){
      for(int k=i; (k+1)%4!=0; k++){
        sortedIp+=ip.charAt(k);
        i=k;
      }
      if(!(i>=14)) sortedIp+='.';

    }
  }
  return sortedIp;
}

void Lcd_Menu::lcdClear(byte const col) const {
  lcd->setCursor(0, col);
  lcd->print("                ");
  lcd->setCursor(0, col);
}



