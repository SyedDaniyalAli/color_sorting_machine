#include <HX711.h>
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

    developed by Syed Daniyal Ali  #SDA

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
#define calibration_factor -7050.0 //This value is obtained using the SparkFun_HX711_Calibration sketch

#define LOADCELL_DOUT_PIN  9
#define LOADCELL_SCK_PIN  10

HX711 scale;



const int analogInPin = A3; //LDR Pin
Servo topServo;
Servo bottomServo;
int frequency = 0;
int color = 0;
int buttonState = 0;
int RedCount = 0;
int RedHeavyCount = 0;
int YellowCount = 0;
int Unidentified = 0;
int R = frequency;
int G = frequency;
int B = frequency;

float current_weight = 0;
float total_weight = 0;

void setup() {
  //WE define our LCD 16 columns and 2 rows
  lcd.begin(20, 4);
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
  topServo.write(180);
  bottomServo.attach(8);
  bottomServo.write(30);
  Serial.begin(9600);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  lcd.clear();//Clean the screen
  //  Write your text/:
  lcd.setCursor(6, 1); //we start writing from the first row first column
  lcd.print("WELCOME"); //16 characters poer line
  delay(3000);//Delay used to give a dinamic effect
  lcd.clear();//Clean the screen
  lcd.setCursor(0, 1); //we start writing from the first row first column
  lcd.print("Color Sorting System"); //16 characters poer line
  delay(4000);//Delay used to give a dinamic effect
  lcd.clear();//Clean the screen

  for (int i = 0 ; i < 5 ; i++)
  {
    lcd.setCursor(0, 0);
    lcd.print(" Loading");
    delay(120);
    lcd.setCursor(0, 0);
    lcd.print(" Loading.");
    delay(120);
    lcd.setCursor(0, 0);
    lcd.print(" Loading..");
    delay(120);
    lcd.setCursor(0, 0);
    lcd.print(" Loading...");
    delay(120);
    lcd.setCursor(0, 0);
    lcd.print(" Loading....");
    delay(120);
    lcd.setCursor(0, 0);
    lcd.print(" Loading.....");
    delay(120);
    lcd.setCursor(0, 0);
    lcd.print(" Loading......");
    delay(120);
    lcd.clear();
  }
  lcd.setCursor(3, 1);
  lcd.print("System Ready");
  delay(120);

}
void loop() {
  current_weight = 0;
  int Ldr = analogRead(analogInPin);
  buttonState = digitalRead(A0);
  int Resetbutton = digitalRead(A2);
  if (buttonState == HIGH & Ldr < 400) {

    topServo.write(180);
    delay(500);
    Ldr = analogRead(analogInPin);
    delay(10);
    if (Ldr < 400) {
      //    move servo from 180 to 0~~~~~~~~~~~~~~~~~~~~~~
      for (int i = 180; i > 65; i--) {
        topServo.write(i);
        delay(50);
      }

      current_weight = scale.get_units() * -453.59237; // Reading Weight

      delay(1000);

      Serial.print("\n current weight:");
      Serial.print(current_weight, 1); //scale.get_units() returns a float
      total_weight += current_weight;

      color = readColor(); // Reading Color

      delay(500);
      switch (color) {
        case 1:
          bottomServo.write(30);
          break;
        case 2:
          bottomServo.write(70);
          break;
        case 3:
          bottomServo.write(118);
          break;
        case 4:
          bottomServo.write(150);

      }
      delay(300);

      for (int i = 65; i > 0; i--) {
        topServo.write(i);
        delay(50);

      }
      lcd.clear();//Clean the screen
      lcd.setCursor(0, 0);
      lcd.print("Red:");
      lcd.print(RedCount);

      lcd.setCursor(0, 1);
      lcd.print("Yell:");
      lcd.print(YellowCount);

      lcd.setCursor(9, 0);
      lcd.print("ReH:");
      lcd.print(RedHeavyCount);

      lcd.setCursor(9, 1);
      lcd.print("UnI:");
      lcd.print(Unidentified);

      lcd.setCursor(0, 3);
      lcd.print("CWt:");
      lcd.print(current_weight, 0);
      lcd.print("g ");

      lcd.print("TWt:");
      lcd.print(total_weight, 0);
      lcd.print("g");


      delay(2000);

      for (int i = 0; i < 180; i++) {
        topServo.write(i);
        delay(2);
      }
    }
    color = 0;

  }
  if (buttonState == LOW )
  {
    lcd.clear();//Clean the screen
    lcd.setCursor(7, 1); //we start writing from the first row first column
    lcd.print("(Stop)");
    delay(20);
    lcd.clear();
  }
  if (Resetbutton == HIGH)
  {
    RedCount = 0;
    YellowCount = 0;
    RedHeavyCount = 0;
    for (int i = 0 ; i < 5 ; i++)
    {
      lcd.clear();
      lcd.setCursor(2, 1);
      lcd.print("Resetting.");
      delay(120);
      lcd.setCursor(2, 1);
      lcd.print("Resetting..");
      delay(120);
      lcd.setCursor(2, 1);
      lcd.print("Resetting...");
      delay(120);
      lcd.setCursor(2, 1);
      lcd.print("Resetting....");
      delay(120);
      lcd.setCursor(2, 1);
      lcd.print("Resetting.....");
      delay(120);
      lcd.clear();

    }
    lcd.setCursor(6, 1);
    lcd.print("(Ready)");
    delay(1000);

  }
  if (buttonState == HIGH & Ldr > 400)
  {
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.print("No Object Detect");
    delay(2000);
    lcd.clear();
  }

}

// Custom Function - readColor()~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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


  //  Checking RGB values~~~~~~~~~~~~~~~~~~~~~~~~~~

  //  For RED~~~~~~~~~~~~~~~~~~~~~~~
  if (R<90 & R>70 & G<280 & G>200) {

    lcd.clear();//Clean the screen
    Serial.print(current_weight);
    if (current_weight > 250) {

      lcd.setCursor(1, 1);
      lcd.print("Heavy Red: ");

      RedHeavyCount += 1;
      lcd.print(RedHeavyCount);
      color = 3;
    }
    else {
      lcd.setCursor(3, 1);
      lcd.print("Red: ");

      RedCount += 1;
      lcd.print(RedCount);

      color = 1;
    }

  } else
    //  For YELLOW~~~~~~~~~~~~~~~~~~~~~~~
    if (R<70 & R>35 & G<85 & G>45) {

      lcd.clear();//Clean the screen
      lcd.setCursor(1, 1);
      lcd.print("Yellow: ");

      color = 2;

      YellowCount += 1;
      lcd.print(YellowCount);
    }

  //  For Heavy RED~~~~~~~~~~~~~~~~~~~~~~~
    else {

      lcd.setCursor(1, 1);
      lcd.print("Unidentified: ");
      lcd.print(Unidentified);
      Unidentified += 1;
      color = 4;
    }



  return color;
}
