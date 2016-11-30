#include <math.h>
#include <Wire.h>
#include <Servo.h>
#include <SoftwareSerial.h>

// XBEE PINS
SoftwareSerial XBee(2, 3); // RX, TX
int Trigger = 0;

// DELAYS
int startupDelay = 2000;            //Calibration Delay
const double delay_time = 10;       //PID Delay

// SERVO FOR TURNING THE CAR AND REFERENCE
Servo Steering_Wheels;
Servo ESC;

// ULTRASONIC VARIABLES
const int ULTRA_PIN = 11;
long Pulse, Inc_Out , Cms_Out;

// LIDAR VARIABLES
unsigned long LIDAR_Left, LIDAR_Right;
double LIDAR_L, LIDAR_R;
double LIDAR_Diff, Per_Angle_Change, LAST_Angle_Error, Angle_Error, OUT_Angle, Angle_Feedback;
double Integral_Val, Derivative_Val;
double Angle;

//SETPOINT FOR CRAWLER
const double Point = 90;

double Kp = 1;
double Ki = 0;
double Kd = 0;

// SETUP
void setup()
{
  XBee.begin(9600);                                 //XBEE Baud Rate     
  LIDAR_Setup();                                    //LIDAR Setup Function Call
  LIDAR_Diff = LIDAR_Left - LIDAR_Right;
  Per_Angle_Change = 90 * (LIDAR_Diff/180);
  Angle_Feedback = 90 - Per_Angle_Change;
  LAST_Angle_Error = Point - Angle_Feedback;
  
  Steering_Wheels.attach(8);                        //ASSIGNING PIN TO WHEELS
  ESC.attach(4);                                    //ASSIGNING PIN TO ESC
  Calibrate_ESC();                                  //CALIBRFATING SPEED CONTROL
}
 
// SPEED CALIBRATION
void Calibrate_ESC()
{
    ESC.write(180);                     // Full Backwards
    delay(startupDelay);
    ESC.write(0);                       // Full Forwards
    delay(startupDelay);
    ESC.write(90);                      // Neutral
    delay(startupDelay);
    ESC.write(90);                      // Reset The ESC To Neutral (non-moving) Value
}

// LIDAR SETUP
void LIDAR_Setup()
{
  Serial.begin(9600);                   // Start Serial Communications
  
  pinMode(10, OUTPUT);                  // Set Pin 10 As Trigger Pin
  pinMode(9, INPUT);                    // Set Pin 9 As Monitor Pin
  digitalWrite(10, LOW);                // Set Trigger LOW For Continuous Read

  pinMode(6, OUTPUT);                   // Set Pin 6 As Trigger Pin
  pinMode(5, INPUT);                    // Set Pin 5 As Monitor Pin
  digitalWrite(6, LOW);                 // Set Trigger LOW For Continuous Read
} 

// LIDAR OUTPUT
void LIDAR()
{
  LIDAR_Right = pulseIn(9, HIGH)/10;    // Count How Long The Pulse Is High In Microseconds
  LIDAR_Left = pulseIn(5, HIGH)/10;     // Count How Long The Pulse Is High In Microseconds
  LIDAR_Right = LIDAR_Right - 30;
  LIDAR_Left = LIDAR_Left - 25;
  LIDAR_L = double(LIDAR_Left);
  LIDAR_R = double(LIDAR_Right);
  LIDAR_Diff = LIDAR_L - LIDAR_R;
  Per_Angle_Change = 90 * (LIDAR_Diff/180);
  Angle_Feedback = 90 - Per_Angle_Change;
}

// ULTRASONIC OUTPUT
void Ultrasonic()
{
  Pulse = pulseIn(ULTRA_PIN, HIGH);
  Inc_Out = Pulse/147;
  Cms_Out = Inc_Out*2.54;
}

// Steering Crawler
void Steer_Crawler()
{
  if (LIDAR_Diff > 90)
  { 
    Steering_Wheels.write(90);
    delay(2);
    ESC.write(60);
  } 

  if (LIDAR_R > 120)
  {
    Steering_Wheels.write(100);
    delay(2);
    ESC.write(60);
    delay(500);
    Steering_Wheels.write(90);
    delay(2);
    ESC.write(60);
  } 
      
  if (abs(OUT_Angle) < 10)
  {
    Steering_Wheels.write(Angle);
    delay(2);
    ESC.write(50);
  }
      
  else 
  if (10 < abs(OUT_Angle) < 20)
  {
    Steering_Wheels.write(Angle);
    delay(2);
    ESC.write(60);
  } 
      
  else 
  {
    Steering_Wheels.write(Angle);
    delay(2);
    ESC.write(70);
  }
}
        
// PID CONTROLLER
void Controller()
{
  delay(delay_time);
  Angle_Error = Point - Angle_Feedback;
  Integral_Val = Integral_Val + (Angle_Error * delay_time);
  Derivative_Val = (Angle_Error - LAST_Angle_Error) / delay_time;
  OUT_Angle = (Kp * Angle_Error) + (Ki * Integral_Val) + (Kd * Derivative_Val);
  LAST_Angle_Error = Angle_Error;
  if ((OUT_Angle >= -0.000001) && (OUT_Angle <= 0.000001))
  { 
    OUT_Angle = 0;
  }
  Angle = 90 + OUT_Angle;
  Steer_Crawler();
}

// MAIN LOOP
void loop()
{
  if(XBee.available())                //Check XBEE Available
  {
    int temp = XBee.read();           //Read From XBEE
    if(temp == 48)
      Trigger = 0;                    //Assign Value To Trigger
    else
    if(temp == 49)
      Trigger = 1;                    //Assign Value To Trigger
  }
  
  if(Trigger == 1)
  {
    Ultrasonic();
    LIDAR();
    if (Cms_Out <= 35)           //Collision Avoidance
    {
      ESC.write(90);
      delay(1);
      Steering_Wheels.write(90);
    }
    else
      Controller();
  }
  else
  if(Trigger == 0)
  {
    ESC.write(90);
    delay(1);
    Steering_Wheels.write(90);
  }
 }


