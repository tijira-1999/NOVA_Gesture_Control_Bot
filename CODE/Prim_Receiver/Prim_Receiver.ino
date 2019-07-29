//Add the necessary libraries
//You can find all the necessary library links in the video description
#include <SPI.h>      //SPI library for communicate with the nRF24L01+
#include "RF24.h"     //The main library of the nRF24L01+
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//temp control
int ThermistorPin = A1;
int Vo;
float R1 = 10000;
float logR2, R2, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;

#define TRIG_PIN A3 // Pin A0 on the Motor Drive Shield soldered to the ultrasonic sensor
#define ECHO_PIN A2 // Pin A1 on the Motor Drive Shield soldered to the ultrasonic sensor

int curDist = 0;
const int soundpin = 8;
const int dangerpin = A0;

//Define enable pins of the Motors
const int enbA = 3;
const int enbB = 5;

//Define control pins of the Motors
//If the motors rotate in the opposite direction, you can change the positions of the following pin numbers
const int IN1 = 2;    //Right Motor (-)
const int IN2 = 4;    //Right Motor (+)
const int IN3 = 7;    //Left Motor (+)
const int IN4 = 6;    //Right Motor (-)

//Define variable for the motors speeds
//I have defined a variable for each of the two motors 
//This way you can synchronize the rotation speed difference between the two motors
int RightSpd = 130;
int LeftSpd = 130;

//Define packet for the direction (X axis and Y axis)
int data[6];
int data1[5];

//Define object from RF24 library - 9 and 10 are a digital pin numbers to which signals CE and CSN are connected
RF24 radio(9,10);

//Create a pipe addresses for the communicate
const uint64_t pipe = 0xE8E8F0F0E1LL;
const uint64_t pipe1 = 0xE8E8F0F0D2LL;

void setup()
{
  pinMode(soundpin,OUTPUT);
  digitalWrite(soundpin,HIGH);
  delay(100);
  digitalWrite(soundpin,LOW);
  delay(100);
  
  lcd.begin(16, 2);
  de();  

  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);
  pinMode(soundpin,OUTPUT);

  pinMode(dangerpin,OUTPUT);
  
  digitalWrite(soundpin,HIGH);
  delay(100);
  digitalWrite(soundpin,LOW);
  delay(100);
  digitalWrite(soundpin,HIGH);
  delay(100);
  digitalWrite(soundpin,LOW);

  digitalWrite(dangerpin,LOW);
  
  //Define the motor pins as OUTPUT
  pinMode(enbA, OUTPUT);
  pinMode(enbB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  Serial.begin(9600);
  radio.begin();                    //Start the nRF24 communicate            
  radio.openReadingPipe(1, pipe);   //Sets the address of the transmitter to which the program will receive data.
  radio.openWritingPipe(pipe1);   //Sets the address of the receiver to which the program will send data.
  //radio.setPALevel(RF24_PA_MIN);
    
  }

void loop()
{ 
  radio.startListening();
  fol();  
  if (radio.available())
  {
    radio.read(data, sizeof(data));
    digitalWrite(soundpin,LOW);
    lcd.setCursor(4,0);
    lcd.print(" ON");

    /*Serial.println(data[0]);
    delay(2000);
    Serial.println(" ");
    Serial.println(data[1]);
    delay(2000);*/
    if(data[0] > 380){
      //right
      analogWrite(enbA, RightSpd);
      analogWrite(enbB, LeftSpd);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);

      lcd.setCursor(11, 1);
      lcd.print("RIGHT");
    }
    
    if(data[0] < 312){
      //left
      analogWrite(enbA, RightSpd);
      analogWrite(enbB, LeftSpd);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);

      lcd.setCursor(11, 1);
      lcd.print("LEFT");
    }
     
    if(data[1] < 120)
    { 
      //backward          
      analogWrite(enbA, RightSpd);
      analogWrite(enbB, LeftSpd);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
            
      lcd.setCursor(11, 1);
      lcd.print("BACK ");
    }

    if(data[1] > 190)
    {
      
      //forward             
      analogWrite(enbA, RightSpd);
      analogWrite(enbB, LeftSpd);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);

      lcd.setCursor(11, 1);
      lcd.print("FOR  ");
    }

    if(data[0] > 312 && data[0] < 380 && data[1] > 120 && data[1] < 190){
      //stop car
      analogWrite(enbA, 0);
      analogWrite(enbB, 0);

      lcd.setCursor(11, 1);
      lcd.print("REST ");
    }

    data1[0]=data[2];
    data1[1]=data[3];
    data1[2]=data[4];
    data1[3]=data[5];
    data1[4]=curDist;

    /*Serial.println(" ");
    Serial.println(data[2]);
    //delay(1000);
    
    Serial.println(data[3]);
    Serial.println(data[4]);
    Serial.println(data[5]);
    delay(1000);*/

    delay(5);
    radio.stopListening();
    radio.write(data1,sizeof(data1));

  }
  else
  {
    lcd.setCursor(4,0);
    lcd.print("OFF");
    digitalWrite(soundpin,HIGH); 
    
    //STOPS THE CAR ON SIGNAL LOSS
    analogWrite(enbA, 0);
    analogWrite(enbB, 0);
  }

  
}

void fol()
{
  //read the ultrasonic sensor distance
  analogWrite(TRIG_PIN,0);
  delayMicroseconds(2);
  analogWrite(TRIG_PIN,255);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN,0);

  int duration = pulseIn(ECHO_PIN,255);
  int distance = (duration*.0343)/2;
  //Serial.print("Distance: ");
  //Serial.println(distance);
  curDist = distance;
  
  lcd.setCursor(12,0);
  lcd.print(curDist);
  lcd.print("   ");

  Vo = analogRead(ThermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  T = T - 273.15;
  //T = (T * 9.0)/ 5.0 + 32.0; 

  lcd.setCursor(4,1);
  lcd.print(T);
  lcd.print("C");
  
  /*Serial.print("Temperature: "); 
  Serial.print(T);
  Serial.println(" C");*/

  if(T>=40.00)
  {
    digitalWrite(dangerpin,HIGH);
  }
  else
  {
    digitalWrite(dangerpin,LOW);
  }
      
}

void de()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("PLEASE WAIT"); 
  lcd.setCursor(0, 1); 
  lcd.print("INITIALISING"); 
  
  //  1
  lcd.setCursor(12, 1);
  lcd.print(".");
  delay(300);
  lcd.setCursor(13, 1);
  lcd.print(".");
  delay(300);
  lcd.setCursor(14, 1);
  lcd.print(".");
  delay(300);
  lcd.setCursor(15, 1);
  lcd.print(".");
  delay(300);
  
  lcd.setCursor(12, 1);
  lcd.print(" ");
  delay(100);
  lcd.setCursor(13, 1);
  lcd.print(" ");
  delay(100);
  lcd.setCursor(14, 1);
  lcd.print(" ");
  delay(100);
  lcd.setCursor(15, 1);
  lcd.print(" ");
  //delay(100);
  
  lcd.setCursor(12, 1);
  lcd.print(".");
  delay(300);
  lcd.setCursor(13, 1);
  lcd.print(".");
  delay(300);
  lcd.setCursor(14, 1);
  lcd.print(".");
  delay(300);
  lcd.setCursor(15, 1);
  lcd.print(".");
  delay(300);

  //  2
  
  lcd.setCursor(12, 1);
  lcd.print(" ");
  delay(100);
  lcd.setCursor(13, 1);
  lcd.print(" ");
  delay(100);
  lcd.setCursor(14, 1);
  lcd.print(" ");
  delay(100);
  lcd.setCursor(15, 1);
  lcd.print(" ");
  //delay(100);
  
  lcd.setCursor(12, 1);
  lcd.print(".");
  delay(300);
  lcd.setCursor(13, 1);
  lcd.print(".");
  delay(300);
  lcd.setCursor(14, 1);
  lcd.print(".");
  delay(300);
  lcd.setCursor(15, 1);
  lcd.print(".");
  delay(300);

  lcd.setCursor(12, 1);
  lcd.print(" ");
  delay(100);
  lcd.setCursor(13, 1);
  lcd.print(" ");
  delay(100);
  lcd.setCursor(14, 1);
  lcd.print(" ");
  delay(100);
  lcd.setCursor(15, 1);
  lcd.print(" ");
  //delay(100);

  lcd.setCursor(12, 1);
  lcd.print(".");
  delay(300);
  lcd.setCursor(13, 1);
  lcd.print(".");
  delay(300);
  lcd.setCursor(14, 1);
  lcd.print(".");
  delay(300);
  lcd.setCursor(15, 1);
  lcd.print(".");
  delay(300);
 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("LISTEN DESPACITO"); 
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("TILL I INTRODUCE");
  delay(2000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" HELLO   FRIEND "); 
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("   I AM  NOVA   ");
  delay(2000); 
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  I AM A  GYRO  "); 
  delay(500);
  lcd.setCursor(0,1);
  lcd.print(" CONTROLLED BOT "); 
  delay(2000);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" I PICK OBJECTS ");
  delay(2000); 
  lcd.setCursor(0,1);
  lcd.print("I SENSE DISTANCE"); 
  delay(2000);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("  AND  I SENSE  ");
  delay(500); 
  lcd.setCursor(0,1);
  lcd.print("  TEMPERATURE.  "); 
  delay(2000);
  
  lcd.clear();
  lcd.print("SIG ");
  lcd.setCursor(8,0);
  lcd.print("DIS");
  lcd.setCursor(0,1);
  lcd.print("Tem:");
}

