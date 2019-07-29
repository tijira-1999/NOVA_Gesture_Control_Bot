//Add the necessary libraries
//You can find all the necessary library links in the video description.
#include <SPI.h>        //SPI library for communicate with the nRF24L01+
#include "RF24.h"       //The main library of the nRF24L01+
#include "Wire.h"       //For communicate
#include "I2Cdev.h"     //For communicate with MPU6050
#include "MPU6050.h"    //The main library of the MPU6050
#include <pitches.h> //add note library

//notes in the melody
int melody[]={NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};

//note durations. 4=quarter note / 8=eighth note
int noteDurations[]={4, 8, 8, 4, 4, 4, 4, 4};

//Define the object to access and cotrol the Gyro and Accelerometer (We don't use the Gyro data)
MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

//Define packet for the direction (X axis and Y axis)
int data[6];
const int buzz=3;

//Define object from RF24 library - 9 and 10 are a digital pin numbers to which signals CE and CSN are connected.
RF24 radio(9,10);

//Create a pipe addresses for the communicate                                    
const uint64_t pipe = 0xE8E8F0F0E1LL;

const int a=A0;
const int b=A1;
const int c=A2;
const int d=A3;

const int L1=4;
const int L2=5;
const int L3=6;
const int L4=7;
const int L5=8;

void setup(void)
{
  
  for (int thisNote=0; thisNote <8; thisNote++)
  {

      //to calculate the note duration, take one second. Divided by the note type
      int noteDuration = 1000 / noteDurations [thisNote];
      tone(buzz, melody [thisNote], noteDuration);

      //to distinguish the notes, set a minimum time between them
      //the note's duration +30% seems to work well
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      //stop the tone playing
      noTone(buzz);
    }

    
  pinMode(a,INPUT);
  pinMode(b,INPUT);
  pinMode(c,INPUT);
  pinMode(d,INPUT);
  
  pinMode(L1,OUTPUT);
  pinMode(L2,OUTPUT);
  pinMode(L3,OUTPUT);
  pinMode(L4,OUTPUT);
  pinMode(L5,OUTPUT);

  digitalWrite(L1,HIGH);
  delay(100);
  digitalWrite(L2,HIGH);
  delay(100);
  digitalWrite(L3,HIGH);
  delay(100);
  digitalWrite(L4,HIGH);
  delay(100);
  digitalWrite(L5,HIGH);
  delay(100);

  digitalWrite(L5,LOW);
  delay(100);
  digitalWrite(L4,LOW);
  delay(100);
  digitalWrite(L3,LOW);
  delay(100);
  digitalWrite(L2,LOW);
  delay(100);
  digitalWrite(L1,LOW);
  delay(100);
  
  digitalWrite(L1,HIGH);
  delay(100);
  digitalWrite(L2,HIGH);
  delay(100);
  digitalWrite(L3,HIGH);
  delay(100);
  digitalWrite(L4,HIGH);
  delay(100);
  digitalWrite(L5,HIGH);
  delay(100);
  
  digitalWrite(L1,LOW);
  digitalWrite(L2,LOW);
  digitalWrite(L3,LOW);
  digitalWrite(L4,LOW);
  digitalWrite(L5,LOW);
  
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();              //Initialize the MPU object
  radio.begin();                 //Start the nRF24 communicate     
  radio.openWritingPipe(pipe);   //Sets the address of the receiver to which the program will send data.
  //radio.setPALevel(RF24_PA_MAX);
}

void loop(void){
  
  //With this function, the acceleration and gyro values of the axes are taken. 
  //If you want to control the car axis differently, you can change the axis name in the map command.
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
 
  //In two-way control, the X axis (data [0]) of the MPU6050 allows the robot to move forward and backward. 
  //Y axis (data [0]) allows the robot to right and left turn.
  data[0] = map(ax, -10000, 10000, 300, 400 ); //Send X axis data
  data[1] = map(ay, -10000, 10000, 100, 200);  //Send Y axis data

  data[2]= analogRead(a);
  data[3] = analogRead(b);
  data[4]= analogRead(c);
  data[5] = analogRead(d);

  if(data[0] > 380)
    {
      //right
      digitalWrite(L2,HIGH);
      digitalWrite(L1,LOW);
      digitalWrite(L3,LOW);
      digitalWrite(L4,LOW);
      digitalWrite(L5,LOW);
  
    }
    
  if(data[0] < 312)
    {
      //left
      digitalWrite(L3,HIGH);
      digitalWrite(L1,LOW);
      digitalWrite(L2,LOW);
      digitalWrite(L4,LOW);
      digitalWrite(L5,LOW);
    }
     
  if(data[1] > 190)
    {
     //forward            
     digitalWrite(L5,HIGH);
     digitalWrite(L1,LOW);
     digitalWrite(L3,LOW);
     digitalWrite(L4,LOW);
     digitalWrite(L2,LOW);
    }

  if(data[1] < 120)
    {
     //backward              
     digitalWrite(L1,HIGH);
     digitalWrite(L4,LOW);
     digitalWrite(L3,LOW);
     digitalWrite(L2,LOW);
     digitalWrite(L5,LOW);
    }

  if(data[0] > 312 && data[0] < 380 && data[1] > 120 && data[1] < 190)
    {
      //stop car
      digitalWrite(L4,HIGH);
      digitalWrite(L1,LOW);
      digitalWrite(L3,LOW);
      digitalWrite(L2,LOW);
      digitalWrite(L5,LOW);
    }
   
  /*Serial.print("A0 ");
  Serial.println(data[2]);
  Serial.print(" | A1 ");
  Serial.println(data[3]);
  Serial.print("A2 ");
  Serial.println(data[4]);
  Serial.print(" | A3 ");
  Serial.println(data[5]);
  delay(1000);
  Serial.println(data[0]);
  delay(500);
  Serial.println(" ");
  Serial.println(data[1]);
  delay(500);*/
  radio.write(data, sizeof(data));
}
