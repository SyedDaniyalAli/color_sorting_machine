#include <LiquidCrystal_SR3W.h>
#include <LiquidCrystal_I2C.h>
#include <FastIO.h>
#include <LCD.h>
#include <I2CIO.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_SR2W.h>
#include <LiquidCrystal_SR.h>

#include <LiquidCrystal_I2C.h>

#include <LiquidCrystal_I2C.h>

/*     Arduino Project - Color Sorting Machine
 *
 *  developed by Syed Daniyal Ali  #SDA
 *
 */

 #include <Wire.h>


/*-----( Inport library )-----*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//i2c pins
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); //
#include <Servo.h>
#define S0 2
#define S1 3
#define S2 4
#define S3 5
#define sensorOut 6
const int analogInPin = A3; //LDR Pin
Servo topServo;
Servo bottomServo;
int frequency = 0;
int color=0;
int buttonState = 0; 
int OrangeCount = 0;
int PurpleCount = 0;
int YellowCount = 0;
int BlackCount = 0;
int R = frequency;
int G = frequency;
int B = frequency;
void setup() {
 //WE define our LCD 16 columns and 2 rows
lcd.begin(20,4);
lcd.backlight();//Power on the back light
//lcd.backlight(); Power off the back light
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A2, INPUT);
  pinMode(sensorOut, INPUT);
  // Setting frequency-scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  digitalWrite(A1, HIGH);
  
  topServo.attach(7);
  bottomServo.attach(8);
  Serial.begin(9600);

lcd.clear();//Clean the screen
//Write your text:
//lcd.setCursor(0,1); //we start writing from the first row first column
//lcd.print("      WELCOME"); //16 characters poer line
//delay(3000);//Delay used to give a dinamic effect
//lcd.clear();//Clean the screen
//lcd.setCursor(0,1); //we start writing from the first row first column
//lcd.print("Color Sorting System"); //16 characters poer line
//delay(4000);//Delay used to give a dinamic effect
//lcd.clear();//Clean the screen
//
//for(int i =0 ;i<5 ; i++)
//{
//lcd.setCursor(0,0);
//lcd.print(" Loading");
//delay(120);
//lcd.setCursor(0,0);
//lcd.print(" Loading.");
//delay(120);
//lcd.setCursor(0,0);
//lcd.print(" Loading..");
//delay(120);
//lcd.setCursor(0,0);
//lcd.print(" Loading...");
//delay(120);
//lcd.setCursor(0,0);
//lcd.print(" Loading....");
//delay(120);
//lcd.setCursor(0,0);
//lcd.print(" Loading.....");
//delay(120);
//lcd.setCursor(0,0);
//lcd.print(" Loading......");
//delay(120);
//lcd.clear();
//}
//lcd.setCursor(3,1);
//lcd.print("System Ready");
//delay(120);

}
void loop() {
int Ldr = analogRead(analogInPin);
buttonState = digitalRead(A0);
int Resetbutton = digitalRead(A2);
if(buttonState==HIGH & Ldr<400){
  
  topServo.write(85);
  delay(500);
Ldr = analogRead(analogInPin);
delay(10);
  if(Ldr<400){
  for(int i = 85; i > 48; i--) {
    topServo.write(i);
    delay(10);
  }
  delay(500);
  
  color = readColor();
  delay(10);  
  switch (color) {
    case 1:
    bottomServo.write(32);
    break;
    case 2:
    bottomServo.write(60);
    break;
    case 3:
    bottomServo.write(88);
    break;
    case 4:
    bottomServo.write(120);
   
  }
  delay(300);
  
  for(int i = 48; i > 12; i--) {
    topServo.write(i);
    delay(2);
    
  } 
  lcd.clear();//Clean the screen
    lcd.setCursor(0,0); 
    lcd.print("Ora:");
    lcd.print(OrangeCount);
    lcd.setCursor(0,1); 
    lcd.print("Pur:");
    lcd.print(PurpleCount);
    lcd.setCursor(9,0); 
    lcd.print("Yel:");
    lcd.print(YellowCount);
    lcd.setCursor(9,1); 
    lcd.print("Bla:");
    lcd.print(BlackCount);
  delay(200);
  
  for(int i = 12; i < 85; i++) {
    topServo.write(i);
    delay(2);
  }
  }
  color=0;

}
if(buttonState==LOW )
{
  lcd.clear();//Clean the screen
    lcd.setCursor(7,1); //we start writing from the first row first column
    lcd.print("(Stop)"); 
    delay(20);
    lcd.clear();
}
if(Resetbutton==HIGH)
{
OrangeCount = 0;
PurpleCount = 0;
YellowCount = 0;
BlackCount = 0;
   for(int i =0 ;i<5 ; i++)
{
  lcd.clear();
lcd.setCursor(0,0);
lcd.print("Resetting.");
delay(120);
lcd.setCursor(0,0);
lcd.print("Resetting..");
delay(120);
lcd.setCursor(0,0);
lcd.print("Resetting...");
delay(120);
lcd.setCursor(0,0);
lcd.print("Resetting....");
delay(120);
lcd.setCursor(0,0);
lcd.print("Resetting.....");
delay(120);
lcd.clear();

}
lcd.setCursor(0,0);
lcd.print("     Ready");
delay(1000);
    
}
if (buttonState==HIGH & Ldr>400)
{
lcd.clear();
lcd.setCursor(0,1);
lcd.print("No Object Detect");
delay(20);
lcd.clear();
}

}
// Custom Function - readColor()
int readColor() {
  // Setting red filtered photodiodes to be read
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  int R = frequency;
  // Printing the value on the serial monitor
  Serial.print("R= ");//printing name
  Serial.print(frequency);
  Serial.print("  ");
  delay(50);
  // Setting Green filtered photodiodes to be read
  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  int G = frequency;
  // Printing the value on the serial monitor
  Serial.print("G= ");//printing name
  Serial.print(frequency);
  Serial.print("  ");
  delay(50);
  
  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  int B = frequency;
  // Printing the value on the serial monitor
  Serial.print("B= ");//printing name
  Serial.print(frequency);
  Serial.println("  ");
  delay(50);
  
  
  if(R<62 & R>22 & G<113 & G>63 & B<93 & B>53){
    
    lcd.clear();//Clean the screen
    lcd.setCursor(0,0); 
    lcd.print("     Orange");

    color = 1; // R= 31  G= 71  B= 60  
    
    OrangeCount = OrangeCount+1;
    
    lcd.setCursor(8,1); 
    lcd.print(OrangeCount); 
  }
  if(R<84 & R>44 & G<100 & G>50 & B<64 & B>24){
     
    lcd.clear();//Clean the screen
    lcd.setCursor(0,0); 
    lcd.print("    Purple"); 

    color = 2; // R= 55  G= 53  B= 51 

   PurpleCount = PurpleCount+1;
    
    lcd.setCursor(8,1); 
    lcd.print(PurpleCount); 
  }
  if(R<53 & R>13 & G<69 & G>29 & B<88 & B>48){
    
    lcd.clear();//Clean the screen
    lcd.setCursor(0,0); 
    lcd.print("     Yellow"); 

    color = 3; // Yellow R= 33  G= 49  B= 68 

    YellowCount = YellowCount+1;
    
    lcd.setCursor(8,1); 
    lcd.print(YellowCount); 
  }
  if(R<110 & R>66 & G<130 & G>80 & B<105 & B>60 ){
    
    lcd.clear();//Clean the screen
    lcd.setCursor(0,0); 
    lcd.print("     Black"); 

    color = 4; //  R= 69  G= 85  B= 67  
    BlackCount = BlackCount+1;
    
    lcd.setCursor(8,1); 
    lcd.print(BlackCount); 
  }

 

  return color;  
}
