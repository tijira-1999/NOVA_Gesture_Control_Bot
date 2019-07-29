#include <SPI.h>      //SPI library for communicate with the nRF24L01+
#include "RF24.h"     //The main library of the nRF24L01+
#include <Servo.h>
#include "pitches.h"


const int led=A1;
int noteDuration;
int pauseBetweenNotes = 0;
int negra;

//Define packet for the direction (X axis and Y axis)
int data[5];
int curDist = 0;
const int soundpin=A0;

//Define object from RF24 library - 9 and 10 are a digital pin numbers to which signals CE and CSN are connected
RF24 radio(7,8);

//Create a pipe addresses for the communicate
const uint64_t pipe1 = 0xE8E8F0F0D2LL;

Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;

int data1[5];

void setup() 
{
  Serial.begin(9600);
  radio.begin();                    //Start the nRF24 communicate            
  radio.openReadingPipe(1, pipe1);   //Sets the address of the transmitter to which the program will receive data.
  //radio.setPALevel(RF24_PA_MIN);
  radio.startListening(); 

  pinMode(3,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(9,OUTPUT);
  
  pinMode(soundpin,OUTPUT);
  digitalWrite(soundpin,LOW);

  myservo1.attach(3);
  myservo2.attach(5);  
  myservo3.attach(6);
  myservo4.attach(9);
  
  myservo1.write(15);
  myservo2.write(112);
  myservo3.write(10);
  myservo4.write(85);

  /*myservo1.write(73);
  myservo2.write(15);
  myservo3.write(112);
  myservo4.write(10);*/
  delay(150);// put your setup code here, to run once:

  //Despacito tune
  pinMode(led, OUTPUT);   
  digitalWrite(led, LOW);
  negra=640;
  chorus();
  chorus();

  delay(150);
}

void loop() 
{
  
  if (radio.available())
  {
    radio.read(data1, sizeof(data1));
    
    int val1=data1[0];
    val1=map(val1,1023,0,10,143);
    myservo1.write(val1);
    delay(10);
    //Serial.print("val1  ");
    //Serial.println(val1);
    //delay(500);

    int val2=data1[1];
    val2=map(val2,1023,0,33,143);
    myservo2.write(val2);
    delay(10);
    //Serial.print("val2  ");
    //Serial.println(val2);
    //delay(500);

    int val3=data1[2];
    val3=map(val3,0,1023,0,180);
    myservo3.write(val3);
    delay(10);
    //Serial.print("val3  ");
    //Serial.println(val3);
    //delay(500);

    int val4=data1[3];
    val4=map(val4,0,1023,0,180);
    myservo4.write(val4);
    delay(10);
    //Serial.print("val4  ");
    //Serial.println(val4);
    //delay(500); 

    curDist=data1[4];

      if(curDist < 30 && curDist >= 20) 
        {
          sound3();       
        }                 
      else if(curDist  < 20 && curDist >= 10)
        {
          sound2(); 
        }
      else if(curDist  < 10)
        {
          sound1(); 
        }
    
  }
  /*else
  {
    digitalWrite(soundpin,HIGH);
    delay(200);
    digitalWrite(soundpin,LOW);
    delay(1000);
  }*/
      
}

void sound1()
{
  digitalWrite(soundpin,HIGH);
  delay(100);
  digitalWrite(soundpin,LOW);
}

void sound2()
{
  digitalWrite(soundpin,HIGH);
  delay(100);
  digitalWrite(soundpin,LOW);
  delay(100);
  digitalWrite(soundpin,HIGH);
  delay(100);
  digitalWrite(soundpin,LOW);
}

void sound3()
{
  digitalWrite(soundpin,HIGH);
  delay(100);
  digitalWrite(soundpin,LOW);
  delay(100);
  digitalWrite(soundpin,HIGH);
  delay(100);
  digitalWrite(soundpin,LOW);
  delay(100);
  digitalWrite(soundpin,HIGH);
  delay(100);
  digitalWrite(soundpin,LOW);
}

void chorus(){
     notars(negra, NOTE_D5);
     notars(negra, NOTE_CS5);digitalWrite(led, HIGH);   // (1)
     notars(negra/2, NOTE_B4);
     notars(negra/4, NOTE_FS4);digitalWrite(led, LOW);    // (2)
     delay(negra/4); digitalWrite(led, HIGH);   // (3)
     notars(negra/4, NOTE_FS4);
     notars(negra/4, NOTE_FS4);
digitalWrite(led, LOW);    // (4)
     notars(negra/4, NOTE_FS4);
     notars(negra/4, NOTE_FS4);digitalWrite(led, HIGH);   //  (1) 
     
     notars(negra/4, NOTE_FS4);

     notars(negra/4, NOTE_B4);
     notars(negra/4, NOTE_B4);digitalWrite(led, LOW);    //  (2) 
     notars(negra/4, NOTE_B4);digitalWrite(led, HIGH);   //  (3)  
     notars(negra/2, NOTE_B4);digitalWrite(led, LOW);    //  (4)   

     notars(negra/4, NOTE_A4);  
          
     notars(negra/4, NOTE_B4);digitalWrite(led, HIGH);   //  (1) 
     delay(negra/4); 
     delay(negra/4); 
     notars(negra/4, NOTE_G4);  digitalWrite(led, LOW);    //  (2) 
     delay(negra/4);digitalWrite(led, HIGH);   //  (3) 
//......
     notars(negra/4, NOTE_G4);  
     notars(negra/4, NOTE_G4);digitalWrite(led, LOW);    //  (4)   
     notars(negra/4, NOTE_G4);  
     notars(negra/4, NOTE_G4);digitalWrite(led, HIGH);   //  (1)   
     notars(negra/4, NOTE_G4);  

     notars(negra/4, NOTE_B4);  
     notars(negra/4, NOTE_B4);  digitalWrite(led, LOW);    //  (2) 
     notars(negra/4, NOTE_B4);digitalWrite(led, HIGH);   //  (3) 
     notars(negra/2, NOTE_B4);digitalWrite(led, LOW);    //  (4) 
     
     notars(negra/4, NOTE_CS5);
     notars(negra/4, NOTE_D5);digitalWrite(led, HIGH);   //  (1) 
     delay(negra/4);
     delay(negra/4);
     notars(negra/4, NOTE_A4);digitalWrite(led, LOW);    //  (2)      
     delay(negra/4);digitalWrite(led, HIGH);   //  (3) 
     
     notars(negra/4, NOTE_A4);     
     notars(negra/4, NOTE_A4);digitalWrite(led, LOW);    //  (4)           
     notars(negra/4, NOTE_A4);               
     notars(negra/4, NOTE_A4);digitalWrite(led, HIGH);   //  (1)                
     
     notars(negra/4, NOTE_D5);          
     notars(negra/4, NOTE_CS5);               
     notars(negra/4, NOTE_D5);digitalWrite(led, LOW);    //  (2)           
     notars(negra/4, NOTE_CS5);digitalWrite(led, HIGH);   //  (3)          
     notars(negra/2, NOTE_D5);digitalWrite(led, LOW);    //  (4)                
     //
     notars(negra/4, NOTE_E5);
     notars_jai(negra/2, NOTE_E5,negra/4);  // (1)
//     notars(negra/2, NOTE_E5);  

    //-----------------------------------------------
      tone(soundpin,NOTE_CS5 ,negra*3/4);
    //-----------------------------------------------
      delay(negra/4);
      delay(negra/4);
     digitalWrite(led, LOW);    //  (2)                     
      delay(negra/4);
     digitalWrite(led, HIGH);   //  (3)                     
     noTone(soundpin);
     delay(negra/4);
     
     //--------------
     delay(negra/4);
     digitalWrite(led, LOW);    //  (4) 

     delay(negra/4);
     delay(negra/4);       
  
}
void notars(int noteDuration,int LaNota ){

    //noteDuration=negra/4;
  tone(soundpin,LaNota ,noteDuration*0.9);
//      pauseBetweenNotes = noteDuration * 1.30;
      pauseBetweenNotes = noteDuration * 1.1;
      delay(pauseBetweenNotes);
      noTone(soundpin);  


}
void notars_jai(int noteDuration,int LaNota ,int Desfase){

    //noteDuration=negra/4;
  tone(soundpin,LaNota ,noteDuration*0.9);
//      pauseBetweenNotes = noteDuration * 1.30;
      pauseBetweenNotes = noteDuration * 1.1;
      delay(Desfase);
      
       digitalWrite(led, HIGH);   //  (1)   
      
        delay(pauseBetweenNotes-Desfase);
      noTone(soundpin);  


}
