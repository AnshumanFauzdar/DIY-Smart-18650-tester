/* ________  ________   ________  ___  ___  ___  ___  _____ ______   ________  ________           ________ ________  ___  ___  ________  ________  ________  ________     
|\   __  \|\   ___  \|\   ____\|\  \|\  \|\  \|\  \|\   _ \  _   \|\   __  \|\   ___  \        |\  _____\\   __  \|\  \|\  \|\_____  \|\   ___ \|\   __  \|\   __  \    
\ \  \|\  \ \  \\ \  \ \  \___|\ \  \\\  \ \  \\\  \ \  \\\__\ \  \ \  \|\  \ \  \\ \  \       \ \  \__/\ \  \|\  \ \  \\\  \\|___/  /\ \  \_|\ \ \  \|\  \ \  \|\  \   
 \ \   __  \ \  \\ \  \ \_____  \ \   __  \ \  \\\  \ \  \\|__| \  \ \   __  \ \  \\ \  \       \ \   __\\ \   __  \ \  \\\  \   /  / /\ \  \ \\ \ \   __  \ \   _  _\  
  \ \  \ \  \ \  \\ \  \|____|\  \ \  \ \  \ \  \\\  \ \  \    \ \  \ \  \ \  \ \  \\ \  \       \ \  \_| \ \  \ \  \ \  \\\  \ /  /_/__\ \  \_\\ \ \  \ \  \ \  \\  \| 
   \ \__\ \__\ \__\\ \__\____\_\  \ \__\ \__\ \_______\ \__\    \ \__\ \__\ \__\ \__\\ \__\       \ \__\   \ \__\ \__\ \_______\\________\ \_______\ \__\ \__\ \__\\ _\ 
    \|__|\|__|\|__| \|__|\_________\|__|\|__|\|_______|\|__|     \|__|\|__|\|__|\|__| \|__|        \|__|    \|__|\|__|\|_______|\|_______|\|_______|\|__|\|__|\|__|\|__|
                        \|_________|                                                                                                                                    */
                                                                                                                                                                        
                                                                                                                                                                        
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define BMP_SCK  (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS   (10)

Adafruit_BMP280 bmp; // I2C

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define MOSFET_Pin 2
#define Bat_Pin A0
#define Res_Pin A1
#define Buzzer_Pin 9
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
float Capacity = 0.0; // Capacity in mAh
float Res_Value = 10.0;  // Resistor Value in Ohm
float Vcc = 4.76; // Voltage of Arduino 5V pin ( Mesured by Multimeter )
float Current = 0.0; // Current in Amp
float mA=0;         // Current in mA
float Bat_Volt = 0.0;  // Battery Voltage 
float Res_Volt = 0.0;  // Voltage at lower end of the Resistor 
float Bat_High = 4.4; // Battery High Voltage
float Bat_Low = 2.9; // Discharge Cut Off Voltage
unsigned long previousMillis = 0; // Previous time in ms
unsigned long millisPassed = 0;  // Current time in ms
float sample1 =0;
float sample2= 0;
int x = 0;
int row = 0;



//*******************************Setup Function ***************************************************************
  
  void setup() {
   Serial.begin(115200);
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.setTextSize(2);
  display.setTextColor(WHITE);
 
  Serial.println(F("BMP280 test"));

  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
 
   pinMode(MOSFET_Pin, OUTPUT);
   pinMode(Buzzer_Pin, OUTPUT);
   digitalWrite(MOSFET_Pin, LOW);  // MOSFET is off during the start
   Serial.println("CLEARDATA");
   Serial.println("LABEL,Time,Bat_Volt,capacity");
   
   //Serial.println("Arduino Battery Capacity Tester v1.0");
   Serial.println("BattVolt Current mAh");
  }
  //********************************Main Loop Function***********************************************************
  void loop() {
//Vcc = readVcc()/1000.0; // Conevrrt mV to Volt

  
//Voltage devider Out = Bat_Volt * R2/(R1+R2 ) // R1 =10K and R2 =10K 
  
  //************ Measuring Battery Voltage ***********


if( Bat_Volt < 1){
  display.setCursor(0,10);
  display.println("No Battery");
   display.display();  
    }
else if ( Bat_Volt > Bat_High){
  display.setCursor(5,10);
  display.println("High Voltage"); 
  display.display(); 

  
  } 
else if(Bat_Volt < Bat_Low){
  display.setCursor(10,20);
  display.println("Low Voltage"); 
  display.display(); 
  }
else if(Bat_Volt >= Bat_Low && Bat_Volt < Bat_High){
  display.setCursor(0,10);
  display.println("Volt:");
  display.setCursor(0,10);
  display.println("Current:");
   display.setCursor(0,10);
  display.println("mAh:");
  
  display.setCursor(10,20);
  display.print(Bat_Volt,2);
  display.println("V");
  display.display(); 
  display.setCursor(10,30);
  display.print(mA,0);
  display.println("mA");
  display.display(); 
  display.setCursor(10,40);
  display.print(Capacity, 1);
  display.display(); 
  }

 display.setCursor(0,35);
 display.println(bmp.readTemperature());
 display.display(); 
 display.setCursor(60,35);
 display.println((char)247);
 display.display(); 
 display.setCursor(70,35);
 display.println("C");
 display.display();
 display.clearDisplay();


    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
    Serial.println(" m");

    Serial.println();
    delay(100);


  
  for(int i=0;i< 100;i++)
  {
   sample1=sample1+analogRead(Bat_Pin); //read the voltage from the divider circuit
   delay (2);
  }
  sample1=sample1/100; 
  Bat_Volt = 2* sample1 *Vcc/ 1024.0; 

  // *********  Measuring Resistor Voltage ***********

   for(int i=0;i< 100;i++)
  {
   sample2=sample2+analogRead(Res_Pin); //read the voltage from the divider circuit
   delay (2);
  }
  sample2=sample2/100;
  Res_Volt = 2* sample2 * Vcc/ 1024.0;

  //********************* Checking the different conditions *************
  
  if ( Bat_Volt > Bat_High){
    digitalWrite(MOSFET_Pin, LOW); // Turned Off the MOSFET // No discharge 
    Serial.println( "Warning High-V! ");
    delay(1000);
   }
   
   else if(Bat_Volt < Bat_Low){
      digitalWrite(MOSFET_Pin, LOW);      
      Serial.println( "Warning Low-V! ");
      delay(1000);
  }
  else if(Bat_Volt > Bat_Low && Bat_Volt < Bat_High  ) { // Check if the battery voltage is within the safe limit
      digitalWrite(MOSFET_Pin, HIGH);
      millisPassed = millis() - previousMillis;
      Current = (Bat_Volt - Res_Volt) / Res_Value;
      mA = Current * 1000.0 ;
      Capacity = Capacity + mA * (millisPassed / 3600000.0); // 1 Hour = 3600000ms
      previousMillis = millis();
      Serial.print("DATA,TIME,"); Serial.print(Bat_Volt); Serial.print(","); Serial.println(Capacity);
      row++;
      x++;
      delay(4000); 
 
     }
  
   //**************************************************


  
 //*************************************************
 }    
