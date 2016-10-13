#include <SoftwareSerial.h>
SoftwareSerial XBee(2, 3);

String str;
int led1, led2, led3, led4, tmp;
int status1, status2, status3, status4;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  XBee.begin(9600);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  if (digitalRead(4) == HIGH)
    led1 = 1;
   else
    led1 = 0;
  if (digitalRead(5) == HIGH)
    led2 = 1;
   else
    led2 = 0;
   if (digitalRead(6) == HIGH)
    led3 = 1;
   else
    led3 = 0;
   if (digitalRead(7) == HIGH)
    led4 = 1;
   else
    led4 = 0;

   
   char buf[4];
   sprintf(buf, "%d%d%d%d\n", led1, led2, led3, led4);
   Serial.write(buf);
   XBee.write(buf);
   delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  if (XBee.available()){
   
      int tmp = 0;
      while (tmp != 48 && tmp != 49){
        tmp =XBee.read();
        if (tmp == 115){
          if (digitalRead(4) == HIGH)
              led1 = 1;
          else
              led1 = 0;
          if (digitalRead(5) == HIGH)
              led2 = 1;
          else
              led2 = 0;
          if (digitalRead(6) == HIGH)
              led3 = 1;
          else
              led3 = 0;
          if (digitalRead(7) == HIGH)
              led4 = 1;
          else
              led4 = 0;
          char buf[4];
          sprintf(buf, "%d%d%d%d\n", led1, led2, led3, led4);
          Serial.write(buf);
          XBee.write(buf);
          tmp = 0;
          led1, led2, led3, led4 = 0;
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
    
      Serial.print(led1);
      Serial.print(led2);
      Serial.print(led3);
      Serial.print(led4);
      Serial.println("**");
      
      //Turn the LEDs on/off based on the command sent by the server 
      //LED 1
      if(led1==49) //if server sent 1 (on)
      {
        digitalWrite(4, HIGH);
        status1 = 1;// turn the LED on (HIGH is the voltage level)
       }
      else if(led1==48)
      {
        digitalWrite(4, LOW);
        status1 = 0;// turn the LED off by making the voltage LOW
                     // wait for a second
      }
      //LED2
      if(led2==49)
      {
        digitalWrite(5, HIGH); 
        status2 = 1;// turn the LED on (HIGH is the voltage level)
      }
      else if(led2==48)
      {
        digitalWrite(5, LOW);
        status2 = 0;// turn the LED off by making the voltage LOW
                     // wait for a second
      }
      //LED3
      if(led3==49)
      {
        digitalWrite(6, HIGH);
        status3 = 1;// turn the LED on (HIGH is the voltage level)
      }
      else if(led3==48)
      {
        digitalWrite(6, LOW);
        status3 = 0;// turn the LED off by making the voltage LOW
                     // wait for a second
      }
      //LED4
      if(led4==49)
      {
        digitalWrite(7, HIGH);
        status4 = 1;// turn the LED on (HIGH is the voltage level)
                      // wait for a second
      }
      else if(led4==48)
      {
        digitalWrite(7, LOW);
        status4 = 0;// turn the LED off by making the voltage LOW
                     // wait for a second
      }
  
   char status_buffer[4];
   sprintf(status_buffer, "%d%d%d%d\n", status1, status2, status3, status4);
   XBee.write(status_buffer);
  }
  
}

