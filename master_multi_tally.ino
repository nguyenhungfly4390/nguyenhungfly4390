//================================================================================================
// ██████████████▓▓▓▓▓▒▒▒   INISIALISASI   ▒▒▒▓▓▓▓▓██████████████
//================================================================================================
//-------------------------------------------------------------------------------------- Library
#include <Firmata.h>
#include <RF24.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <avr/wdt.h>
//---------------------------------------------------------------------------- Config LCD & NRF
LiquidCrystal_I2C lcd(0x27,16,2);  //Alamat 0x27 LCD 1602
RF24 radio(9,10); // CE, CSN
const uint64_t pipe = 0xE8E8F0F0E1LL;
byte previousPIN[TOTAL_PORTS];  // PIN means PORT for input
byte previousPORT[TOTAL_PORTS];
int Bank[5] = {0,1,2,3,4};
int Mem[5] = {0,0,0,0,0};

const int interval = 800;
long PinOut = 0x00;
int Input[12]={2,3,4,5,6,7,8,16,15,14,0,1} ;
int MENU = A6;
int MENU_MODE=0; 
int ValMENU=0;
unsigned long TimerMenu;
int PA= A7;
int MIDI_IN=2;
unsigned long SIGNAL;
byte PIN[2];
char SIGN = 'S';
String DATAPIN="";
char my_data[20]={'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
byte Virtua_Port [4]={0,0,0,0};
int MODE=0;
String Modes[6]={"AVMatrix","Sony MCX500","DV SE-500","vMix","DeviceWell"};
//                                                        "                "
String ModeOpsi[6]={"AVMatrix VS-0601","  Sony MCX-500  ","DataVideo SE-500","      vMix      ","DeviceWell H9106"};
byte sinyal[8] = {B00000,  B00000,  B11111,  B01110,  B00100,  B00100,  B00100,};
byte sinyal1[8] = {  B00000,  B00000,  B00000,  B00001,  B00101,  B10101,  B10101,};
byte sinyal0[8] = {  B00000,  B00000,  B10001,  B01010,  B00100,  B01010,  B10001,};



//============================================================================================
//████████████████████████████████████████████ FIRMATA SCRIPT 
//============================================================================================
void setPinModeCallback(byte pin, int mode) {
  if (IS_PIN_DIGITAL(pin)) {
    pinMode(PIN_TO_DIGITAL(pin), mode);
    }}

void digitalWriteCallback(byte port, int value){
  byte i;
  byte currentPinValue, previousPinValue;
  
  if (port < TOTAL_PORTS && value != previousPORT[port]) {
    Virtua_Port[port]=value; //String (value,BIN);
    for (i = 2; i < 8; i++)
    {
    bitWrite(PIN[0],i-2,bitRead(Virtua_Port[0],i));
    }
   
    for (i = 0; i < 6; i++)
    {
    bitWrite(PIN[1],i,bitRead(Virtua_Port[1],i));
    }
  
  previousPORT[port] = value;
  }}

//============================================================================================
//▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ SET MODE
//============================================================================================
void MODE_SET (int mode){
  lcd.clear();
  switch(mode){
  case 0:
    for (int x=0 ;x<12;x++){
      pinMode (Input[x],INPUT_PULLUP);   
      }
    break;
  case 1:
    for (int x=0 ;x<12;x++){
      pinMode (Input[x],INPUT_PULLUP);   
      }
    break;
  case 2:
    for (int x=0 ;x<12;x++){
      pinMode (Input[x],INPUT_PULLUP);   
      }
    break;

  case 3:
    Firmata.setFirmwareVersion(FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);
    Firmata.attach(DIGITAL_MESSAGE, digitalWriteCallback);
    Firmata.attach(SET_PIN_MODE, setPinModeCallback);
    Firmata.begin(57600);
    break;
  case 4:
    for (int x=0 ;x<12;x++){
      pinMode (Input[x],INPUT_PULLUP);   
      }
    break;
  }

  lcd.home();
  lcd.print(Modes[mode]);   
}
//============================================================================================
//▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ PA CHECK
//============================================================================================
void PA_Check(){
  if (analogRead(PA)>=500){
     lcd.setCursor (14,0); 
     lcd.write(byte(0));
     lcd.write(byte(1));
   
  } else {
    lcd.setCursor (14,0); 
    lcd.write(byte(0));
    lcd.write(byte(2));
  }}
//==============================================================================================
// ██████████████▓▓▓▓▓▒▒▒   CHEK PIN     ▒▒▒▓▓▓▓▓██████████████
//==============================================================================================
void Check_pin(){
  
  switch (MODE){
    case 0: //►►►►►►►►►►►►►►►►►►►►►►►►►►►►►►►►► MODE 0 (AVMatrix)
      PIN[0]=0;
      PIN[1]=0;
      for (int x=0 ;x<6;x++){
      bitWrite(PIN[0],x,not(digitalRead(Input[x])));
      }
      for (int x=6 ;x<12;x++){
      bitWrite(PIN[1],x-6,not(digitalRead(Input[x])));
      }
      break;
    case 1: //►►►►►►►►►►►►►►►►►►►►►►►►►►►►►►►►► MODE 1 (Sony MCX500)
      PIN[0]=0;
      PIN[1]=0;
      for (int x=0 ;x<6;x++){
      bitWrite(PIN[0],x,not(digitalRead(Input[x])));
      }
      for (int x=6 ;x<12;x++){
      bitWrite(PIN[1],x-6,not(digitalRead(Input[x])));
      }
      break;
    case 2: //►►►►►►►►►►►►►►►►►►►►►►►►►►►►►►►►► MODE 2 (DV SE-500)
      PIN[0]=0;
      PIN[1]=0;
      for (int x=0 ;x<6;x++){
      bitWrite(PIN[0],x,not(digitalRead(Input[x])));
      }
      for (int x=6 ;x<12;x++){
      bitWrite(PIN[1],x-6,not(digitalRead(Input[x])));
      }
      break;
   
    case 3://►►►►►►►►►►►►►►►►►►►►►►►►►►►►►►►►► MODE 4 (Vmix)
      byte i;
      for (i = 0; i < TOTAL_PORTS; i++) {
      //outputPort(i, readPort(i, 0xff));
      }
      while (Firmata.available()) {
        Firmata.processInput(); 
      }
      break;
     case 4://►►►►►►►►►►►►►►►►►►►►►►►►►►►►►►►►► MODE 4 (DeviceWell)
      PIN[0]=0;
      PIN[1]=0;
      for (int x=0 ;x<6;x++){
      bitWrite(PIN[0],x,not(digitalRead(Input[x])));
      }
      for (int x=6 ;x<12;x++){
      bitWrite(PIN[1],x-6,not(digitalRead(Input[x])));
      }
      break;
    
    }
    
    radio.write(&PIN, 2); 
    lcd.setCursor (0,1);   //  "
    lcd.print("ACTIV:   PVIEW: ");
    lcd.setCursor (6,1); 
    if (bitRead(PIN[0],1)==1)  lcd.print("1");
    if (bitRead(PIN[0],3)==1)  lcd.print("2");
    if (bitRead(PIN[0],5)==1)  lcd.print("3");
    if (bitRead(PIN[1],1)==1)  lcd.print("4");
    if (bitRead(PIN[1],3)==1)  lcd.print("5");
    if (bitRead(PIN[1],5)==1)  lcd.print("6");
    lcd.setCursor (15,1); 
    if (bitRead(PIN[0],0)==1)  lcd.print("1");
    if (bitRead(PIN[0],2)==1)  lcd.print("2");
    if (bitRead(PIN[0],4)==1)  lcd.print("3");
    if (bitRead(PIN[1],0)==1)  lcd.print("4");
    if (bitRead(PIN[1],2)==1)  lcd.print("5");
    if (bitRead(PIN[1],4)==1)  lcd.print("6");

    lcd.print(" ");    

    
}
//================================================================================================
// ██████████████▓▓▓▓▓▒▒▒      SETUP       ▒▒▒▓▓▓▓▓██████████████
//================================================================================================
void setup(){ 
  int i;
  for (i= 0; i < 5; i++){
  Mem[i]=EEPROM.read(Bank[i]);
  }

  if (Mem[0]>=6){
    EEPROM.update(Bank[0],0);
    Mem[0]=0;
    }
  MODE=Mem[0];   
  
  

 //----------------------------------------- NRF SETUP
      radio.begin();
      radio.openWritingPipe(pipe);
      radio.setPALevel(RF24_PA_MAX);
      radio.setDataRate(RF24_1MBPS);
      radio.setChannel(77);
      radio.stopListening();  
 //----------------------------------------- LCD SETUP
      lcd.init();                      // initialize the lcd 
      lcd.backlight();
      lcd.createChar(0, sinyal);
      lcd.createChar(1, sinyal1);
      lcd.createChar(2, sinyal0);
      lcd.home (); // go home
      lcd.clear();
      lcd.home (); // go home
      lcd.print("UNIVERSAL TALLY ");
      lcd.setCursor (0,1);   
      lcd.print("   Versi 1.2   ");
      delay(2000) ;
      lcd.setCursor (0,1);  
      lcd.print(" by Cara Caknan ");
      delay(1000) ;
      lcd.home (); // go home
//------------------------------------------- Cek Mode         
      MODE_SET(MODE);
//------------------------------------------- Cek PA    
      PA_Check();
      
}

void loop()
{  

if (MENU_MODE==0) Check_pin();
opsi();
}

//void Header(){
  
//}

void opsi(){
  int z; 
  ValMENU=analogRead(MENU);
  if (ValMENU < 100) {
    MENU_MODE=1;
    lcd.home();
    lcd.print("   PILIH MODE   "); 
    TimerMenu = millis();
    if (MODE < 5) {
      MODE++;
      if (MODE==5) MODE=0;
      lcd.setCursor (0,1);
      lcd.print("                ");
    for (z= 16; z >= 0; z--){ 
      lcd.setCursor (z,1);
      lcd.print(ModeOpsi[MODE]); 
      delay (10);
    }
      
      delay(interval); 
    }}
  
  if ((MENU_MODE==1) && ((millis()-TimerMenu)>=6000)) {
    lcd.home();
    lcd.print(ModeOpsi[MODE]);
    lcd.setCursor (0,1);
    lcd.print(" MENYIMPAN.     ");
    EEPROM.update(Bank[0],MODE);
    lcd.setCursor (9,1);
    int i;
    for (i= 9; i < 14; i++){
      lcd.print(".");
      delay(200); 
      }
    MENU_MODE=0;
    MODE_SET(MODE);
    PA_Check();
   }

}
