// Paint example specifically for the TFTLCD breakout board.
// If using the Arduino shield, use the tftpaint_shield.pde sketch instead!
// DOES NOT CURRENTLY WORK ON ARDUINO LEONARDO

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <TouchScreen.h>

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7

// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 22
//   D1 connects to digital pin 23
//   D2 connects to digital pin 24
//   D3 connects to digital pin 25
//   D4 connects to digital pin 26
//   D5 connects to digital pin 27
//   D6 connects to digital pin 28
//   D7 connects to digital pin 29

// For the Arduino Due, use digital pins 33 through 40
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 33
//   D1 connects to digital pin 34
//   D2 connects to digital pin 35
//   D3 connects to digital pin 36
//   D4 connects to digital pin 37
//   D5 connects to digital pin 38
//   D6 connects to digital pin 39
//   D7 connects to digital pin 40

#define YP A3  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 9   // can be a digital pin
#define XP 8   // can be a digital pin

#define TS_MINX 920
#define TS_MINY 120
#define TS_MAXX 150
#define TS_MAXY 940

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define ORANGE  0xFD20  

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);


byte inData;
char inChar;
String BuildINString="";
String DisplayString="";
String WorkingString="";
String WorkingStringA="";
String WorkingStringB="";
long DisplayValue;
float fdata=0;
long A;
long B;
float tempa;
float tempb;

void setup(void) {
  tft.begin(9600);
  
  Serial.begin(9600);
  Serial.println("ATZ");  
  tft.reset();
  
  uint16_t identifier = tft.readID();

  tft.begin(identifier);
  pinMode(13, OUTPUT);
//  delay(10000);
  tft.print("ELM327 TZ    ");
  delay(500);
   ReadData();
  if (BuildINString.substring(1,3)=="TZ")    // MIATA RESPONSE TO ATZ IS ATZ[[[ELM327 V1.5  OR AT LEAST THAT IS WHAT ARDUINO HAS IN THE BUFFER
      {
        tft.println("Connection OK         ");
      }
    else
      tft.println("No Connection!         ");
delay(500);
  Serial.println("0100");          // Works with This  only
 
  tft.print("Initialzing.....");          //Initialize BUS  //lcd.print("0100 Sent"); 
  delay(400);
   ReadData();

   tft.print("Initialized.....");  //Added 12-10-2016
   delay(500);
   
  tft.reset();
  tft.begin(identifier);
  tft.setRotation(45);
  tft.fillScreen(BLACK);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.fillRect(225, 0, 2, 320, GREEN);
  tft.fillRect(225, 60, 340, 2, GREEN);
  tft.fillRect(225, 120, 340, 2, GREEN);
  tft.fillRect(0, 180, 340, 2, GREEN);
  
  tft.setCursor(14,20);
  tft.println("RPM:");
  tft.setCursor(230,5);
  tft.println("TEMP");
  tft.setCursor(230,65);
  tft.println("Voltage");
  tft.setCursor(230,125);
  tft.println("In.Temp");
  tft.setCursor(230,185);
  tft.println("Am.Temp");
  tft.setCursor(0,205);
  tft.println("Speed: ");

 
}


void loop()
{
  String rpm=getRPM();
  tft.fillRect(12,50,100,23,BLACK);
  tft.setCursor(12,50);
  tft.println(rpm);
  drawRPM(rpm);

  String temp=getTEMP();
  tft.fillRect(230,35,70,23,BLACK);
  tft.setCursor(230,35);
  tft.println(temp);
  
  rpm=getRPM();
  tft.fillRect(12,50,100,23,BLACK);
  tft.setCursor(12,50);
  tft.println(rpm);
  drawRPM(rpm);
  
  String voltage=getVOLTAGE();
  tft.fillRect(230,95,70,23,BLACK);
  tft.setCursor(230,95);
  tft.println(voltage);

  rpm=getRPM();
  tft.fillRect(12,50,100,23,BLACK);
  tft.setCursor(12,50);
  tft.println(rpm);
  drawRPM(rpm);
  
  String inttemp=getINTEMP();
  tft.fillRect(230,155,70,23,BLACK);
  tft.setCursor(230,155);
  tft.println(inttemp);

  rpm=getRPM();
  tft.fillRect(12,50,100,23,BLACK);
  tft.setCursor(12,50);
  tft.println(rpm);
  drawRPM(rpm);
  
  String ambtemp=ambTEMP();
  tft.fillRect(230,215,70,23,BLACK);
  tft.setCursor(230,215);
  tft.println(ambtemp);
  
  rpm=getRPM();
  tft.fillRect(12,50,100,23,BLACK);
  tft.setCursor(12,50);
  tft.println(rpm);
  drawRPM(rpm);
  
  String Speed=getSPEED();
  tft.fillRect(95,205,90,23,BLACK);
  tft.setCursor(95,205);
  tft.println(Speed);

  
}

void drawRPM(String rpm){
  int intrpm=rpm.toInt();
  
  tft.fillRect(20,160,20,10,BLACK);
  tft.fillRect(45,145,20,25,BLACK);
  tft.fillRect(70,130,20,40,BLACK);
  tft.fillRect(70,130,20,40,BLACK);
  tft.fillRect(95,115,20,55,BLACK);
  tft.fillRect(120,100,20,70,BLACK);
  tft.fillRect(145,80,20,90,BLACK);
  tft.fillRect(170,60,20,110,BLACK);
  tft.fillRect(195,30,20,140,BLACK);
    
  if(intrpm>0)
    tft.fillRect(20,160,20,10,GREEN);
    
  if(intrpm>=1000)
    tft.fillRect(45,145,20,25,GREEN);
  
  if(intrpm>=1500)
    tft.fillRect(70,130,20,40,GREEN);
    
  if(intrpm>=2200)
    tft.fillRect(95,115,20,55,YELLOW);
    
  if(intrpm>=2800)
    tft.fillRect(120,100,20,70,ORANGE);
  
  if(intrpm>=3600)
    tft.fillRect(145,80,20,90,ORANGE);
    
  if(intrpm>=4500)
    tft.fillRect(170,60,20,110,RED);
  
  if(intrpm>=5700)
    tft.fillRect(195,30,20,140,RED);
}


String getRPM(){
  Serial.println("010c");  // Send RPM PID request 0105
  
   delay(300);   
   ReadData();  //replaced below code

   BuildINString.replace(" ", "");

    WorkingStringA = BuildINString.substring(9,11); 
    WorkingStringB = BuildINString.substring(11,13);  
    A = strtol(WorkingStringA.c_str(),NULL,16);  //convert hex to decimnal
    B = strtol(WorkingStringB.c_str(),NULL,16);  //convert hex to decimnal
   DisplayValue = ((255*A)+B)/4;
   DisplayString = String(DisplayValue);  // Subtract 40 from decimal to get the right temperature
   return DisplayString + " RPM";
}
String getSPEED(){ 
  Serial.println("010d");  // Send Speed PID request 0105
  
   delay(300);   
   ReadData();  //replaced below code

   BuildINString.replace(" ", "");
    WorkingString = BuildINString.substring(9);  

    A = strtol(WorkingString.c_str(),NULL,16);  //convert hex to decimnal

   DisplayValue = A;
   DisplayString = String(DisplayValue); 
   return DisplayString + " km/h";
}
String getTEMP(){   

   Serial.println("0105");  // Send temp PID request 010c
  
   delay(300);
   ReadData();  //replaced below code

   BuildINString.replace(" ", "");
   WorkingString = BuildINString.substring(9,11);   

   A = strtol(WorkingString.c_str(),NULL,16);  //convert hex to decimnal

   DisplayValue = A;
   DisplayString = String(DisplayValue - 40);  // Subtract 40 from decimal to get the right temperature
   return DisplayString + " C";
}
String getINTEMP(){    

   Serial.println("010f");  // Send intake temp PID request 010c
  
   delay(300);
   ReadData();  //replaced below code

   BuildINString.replace(" ", "");
   WorkingString = BuildINString.substring(9,11);   

   A = strtol(WorkingString.c_str(),NULL,16);  //convert hex to decimnal

   DisplayValue = A;
   DisplayString = String(DisplayValue - 40);  // Subtract 40 from decimal to get the right temperature
   return DisplayString + " C";
}

String ambTEMP(){  

   Serial.println("0146");  // Send ambient temperature PID request 010c
  
   delay(300);
   ReadData();  //replaced below code

   BuildINString.replace(" ", "");
   WorkingString = BuildINString.substring(9,11);   

   A = strtol(WorkingString.c_str(),NULL,16);  //convert hex to decimnal

   DisplayValue = A;
   DisplayString = String(DisplayValue - 40);  // Subtract 40 from decimal to get the right temperature
   return DisplayString + " C";
}

String getVOLTAGE(){ 
  Serial.println("0142");  // Send Volatage PID request 0105
  delay(300);   
   ReadData();  //replaced below code

   BuildINString.replace(" ", "");

    WorkingStringA = BuildINString.substring(9,11); 
    WorkingStringB = BuildINString.substring(11,13);  
    tempa = strtol(WorkingStringA.c_str(),NULL,16);  //convert hex to decimnal
    tempb = strtol(WorkingStringB.c_str(),NULL,16);  //convert hex to decimnal
   fdata= ((255*tempa)+tempb)/1000;
   DisplayString = String(fdata);  // Subtract 40 from decimal to get the right temperature
   return DisplayString + " V";
}
void ReadData()
{
BuildINString="";  
  while(Serial.available() > 0)
  {
    inData=0;
    inChar=0;
    inData = Serial.read();
    inChar=char(inData);
    BuildINString = BuildINString + inChar;
  }

}
