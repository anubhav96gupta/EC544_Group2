#include <math.h>
#include <Wire.h>
#include <Servo.h>

int startupDelay = 1000;
const double delay_time = 10;

//SERVO FOR TURNING THE CAR AND REFERENCE
Servo Steering_Wheels;
Servo ESC;
double Angle;


// IR SENSOR VARIABLES
int IR_PIN = 2;         // analog pin for reading the IR sensor
float IR_Voltage, IR_Distance;

// LIDAR VARIABLES
unsigned long LIDAR_Left, LIDAR_Right;
double LIDAR_L, LIDAR_R;
double LIDAR_Diff, LIDAR_Diff_90, LAST_Error, Error, OUT, LIDAR_Feedback;
double Integral_Val, Derivative_Val;

//SETPOINT FOR CRAWLER
const double Point = 90;

double Kp = 1;
double Ki = 0;
double Kd = 0;

//SETPUT
void setup()
{
  // Controls steering
  LIDAR_Setup();
  LIDAR_Diff = LIDAR_Left - LIDAR_Right;
  LIDAR_Diff_90 = 90 * (LIDAR_Diff/180);
  LIDAR_Feedback = 90 - LIDAR_Diff_90;
  LAST_Error = Point - LIDAR_Feedback;
  
  Steering_Wheels.attach(8);    //ASSIGNING PIN TO WHEELS
  ESC.attach(9);                //ASSIGNING PIN TO ESC
  Calibrate_ESC();               //CALIBRFATING SPEED CONTROL

}
 
//SPEED CALIBRATION
void Calibrate_ESC(){
    ESC.write(180); // full backwards
    delay(startupDelay);
    ESC.write(0); // full forwards
    delay(startupDelay);
    ESC.write(90); // neutral
    delay(startupDelay);
    ESC.write(90); // reset the ESC to neutral (non-moving) value
}

//LIDAR SETUP
void LIDAR_Setup(){
  Serial.begin(9600); // Start serial communications
  pinMode(2, OUTPUT); // Set pin 2 as trigger pin
  pinMode(3, INPUT); // Set pin 3 as monitor pin
  digitalWrite(11, LOW); // Set trigger LOW for continuous read

  pinMode(6, OUTPUT); // Set pin 2 as trigger pin
  pinMode(5, INPUT); // Set pin 3 as monitor pin
  digitalWrite(2, LOW); // Set trigger LOW for continuous read
} 

//LIDAR OUTPUT
void LIDAR()
{
  LIDAR_Right = pulseIn(3, HIGH)/10; // Count how long the pulse is high in microseconds
  LIDAR_Left = pulseIn(5, HIGH)/10; // Count how long the pulse is high in microseconds
  LIDAR_Right = LIDAR_Right - 30;
  LIDAR_Left = LIDAR_Left - 25;
  /*if(LIDAR_Right > 90)
  {
     LIDAR_Right = 90;
  }
  if(LIDAR_Left > 90)
  {
     LIDAR_Left = 90;
  }*/
  Serial.println(LIDAR_Right);
  Serial.println("\t");
  Serial.println(LIDAR_Left);
  Serial.println("\n");
  
  LIDAR_L = double(LIDAR_Left);
  LIDAR_R = double(LIDAR_Right);
  LIDAR_Diff = LIDAR_L - LIDAR_R;
  LIDAR_Diff_90 = 90 * (LIDAR_Diff/180);
  LIDAR_Feedback = 90 - LIDAR_Diff_90;
}

//IR OUTPUT
void IR()
{
  IR_Voltage= analogRead(IR_PIN)*0.00322265625;
  IR_Distance = 65*pow(IR_Voltage, -1.10);
}

//Steering Crawler
void Steer_Crawler()
{
  if (IR_Distance <= 40) 
  {
    ESC.write(90);
    delay(1);
    Steering_Wheels.write(90); 
  }
  else
  {
    if (LIDAR_Diff > 90) //if diff greater than 200 we lost a wall
        { 
          Steering_Wheels.write(90); //keep moving straight 
          delay(1);
          ESC.write(75);  //slow down a little bit
        } 
     else
     {         
    //Case One Wheel Angle Adj is small
      if (abs(OUT) < 10)
        {
           Steering_Wheels.write(Angle);
           delay(1);
           ESC.write(65); //can move pretty fast here
        }
      
     //Case two Wheel adj medium
      else 
      if (10 < abs(OUT) < 20)
      {
        Steering_Wheels.write(Angle);
        delay(1);
        ESC.write(75); //slow down to make this adjs
      } 
      //Case three wheel adj large
      else 
      {
        Steering_Wheels.write(Angle);
        delay(1);
        ESC.write(78); // go real slow to make this adjustment
      }
     }
  } 
}       
//PID CONTROLLER
void Controller()
{
  delay(delay_time);
  Error = Point - LIDAR_Feedback;
  Integral_Val = Integral_Val + (Error * delay_time);
  Derivative_Val = (Error - LAST_Error) / delay_time;
  OUT = (Kp * Error) + (Ki * Integral_Val) + (Kd * Derivative_Val);
  LAST_Error = Error;
  if ((OUT >= -0.000001) && (OUT <= 0.000001))
  { 
    OUT = 0;
  }
  Angle = 90 + OUT;
  Steer_Crawler();
}

//MAIN LOOP
void loop()
{
  IR();
  LIDAR();
  Controller();
  //delay(2000);
}

