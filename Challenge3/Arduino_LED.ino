#include <SoftwareSerial.h>
SoftwareSerial XBee(2, 3);

String str;
int led1, led2, led3, led4, tmp;
int status1, status2, status3, status4;
char status_buffer[4];

void setup(){
  
  Serial.begin(9600);
  XBee.begin(9600);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);

  char buf[4];

  if (digitalRead(4) == HIGH)
    led1 = 1;
  else
    led1 = 0;
  if (digitalRead(5) == HIGH)
    led2 = 1;
  else
    led2 = 0;
  if (digitalRead(8) == HIGH)
    led3 = 1;
  else
    led3 = 0;
  if (digitalRead(9) == HIGH)
    led4 = 1;
  else
    led4 = 0;
   
   sprintf(buf, "%d%d%d%d\n", led1, led2, led3, led4);
   XBee.write(buf);
   delay(1000);

}

void loop(){

  if(XBee.available()){
    
    int tmp = 0;
    while (tmp != 48 && tmp != 49){
      tmp =XBee.read();
      if (tmp == 115){
        sprintf(status_buffer, "%d%d%d%d\n", status1, status2, status3, status4);
        Serial.print(status_buffer);
        XBee.write(status_buffer);        
      }
      else
        led1 = tmp;
      }
      
      tmp = 0;
      
      while (tmp != 48 && tmp != 49){
        tmp =XBee.read();
        led2 = tmp;
      }
      
      tmp = 0;
      
      while (tmp != 48 && tmp != 49){
        tmp =XBee.read();
        led3 = tmp;
      }
      
      tmp = 0;
      
      while (tmp != 48 && tmp != 49){
        tmp =XBee.read();
        led4 = tmp;
      }
      
      //Turn the LEDs on/off based on the command sent by the server 
      //LED 1
      if(led1==49)
      {
        digitalWrite(4, HIGH);
        status1 = 1;
      }
      else 
      if(led1==48)
      {
        digitalWrite(4, LOW);
        status1 = 0;
      }
      //LED2
      if(led2==49)
      {
        digitalWrite(5, HIGH); 
        status2 = 1;
      }
      else 
      if(led2==48)
      {
        digitalWrite(5, LOW);
        status2 = 0;
      }
      //LED3
      if(led3==49)
      {
        digitalWrite(8, HIGH);
        status3 = 1;
      }
      else 
      if(led3==48)
      {
        digitalWrite(8, LOW);
        status3 = 0;
      }
      //LED4
      if(led4==49)
      {
        digitalWrite(9, HIGH);
        status4 = 1;
      }
      else 
      if(led4==48)
      {
        digitalWrite(9, LOW);
        status4 = 0;
      }
  }
}
