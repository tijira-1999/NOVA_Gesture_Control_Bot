# NOVA_Gesture_Control_Bot

A gesture control bot with real time display of data and equipped with various sensors

modules/Sensors used
  arduino nano *2
  mpu6050
  nrf24l01 *3
  I2C module
  16*2 lcd
  ultrasonic sensor
  l298n motor driver
  thermistor
  nrf24l01 socket base
  
Other components include
  potentiometers 10k *4
  wires
  voltage regulators lm7805 *1
                     lm7806 *3
  buzzer *2
  battery holder(3 cell) *2
  leds 
  relay
  robot arm kit
  double sided tape
  screwdriver
  sunboard
  metal chasis
  gloves *1
  zip lock ties
  soldering iron
  solder flux
  solder wire
  resistors
  header pins(both male and female)
  zero pcb
  dc socket and jack
  switches
  
  
  
  this robot can be controlled wirelessly with its transmitter by gestures which are indicated by the leds mounted on the transmitter
  the robotic arm can be controlled by 4 potentiometers
  
  the main receiver (that is the primary arduino) receives all data through a read only hex pipe and transmits potentiometer values to the secondary arduino which controls arm positions and also
  sends distance and temperature to the primary arduino through a read/write hex pipe.
  
  the primary arduino controls robot movements (forward, backward,right left) and displays all fetched real time data from transmitter and secondary
  arduino on lcd using I2C protocol.
