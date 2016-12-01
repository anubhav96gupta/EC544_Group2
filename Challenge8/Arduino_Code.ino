#include <math.h>
#include <Wire.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <XBee.h>

// GLOBAL FLAGS
int POWER = 0;
int SWITCH = 0;
int PAUSE = 0;
char COMMAND;

// TURNING MAX SPEED AND MAX WHEEL ANGLE
double maxWheelOffset = 85;
double maxSpeedOffset = 70;

// XBEE VARIABLES
XBee xbee = XBee();
XBeeResponse response = XBeeResponse(); 
ZBRxResponse rx = ZBRxResponse();
ZBTxStatusResponse txStatus = ZBTxStatusResponse();
SoftwareSerial xbeeSerial(2,3);

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
  xbeeSerial.begin(9600);
  xbee.setSerial(xbeeSerial); 
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
  Cms_Out = 0;
  int i=5;
  while(i>0)
  {
    Pulse = pulseIn(ULTRA_PIN, HIGH);
    Inc_Out = Pulse/147;
    Cms_Out += Inc_Out*2.54;
  i--;
  delay(10);
  }
  Cms_Out = Cms_Out/5;
}

double degToRad(double degrees){
  return (degrees * 71) / 4068;
}


// OSCILLATE RIGHT
/*void oscillate_R(){
  for (int i =210; i < 220; i++){
    double rad = degToRad(i);
    double speedOffset = sin(rad) * maxSpeedOffset;
    double wheelOffset = sin(rad) * maxWheelOffset;
    ESC.write(90 + speedOffset);
    Steering_Wheels.write(90 + wheelOffset);
    delay(70);
  }
  Steering_Wheels.write(90);
}*/

// OSCILLATE LEFT
void oscillate_L(){
  for (int i =60; i < 80; i++){
    double rad = degToRad(i);
    double speedOffset = sin(rad) * maxSpeedOffset;
    double wheelOffset = sin(rad) * maxWheelOffset;
    ESC.write(0 + speedOffset);
    Steering_Wheels.write(90 + wheelOffset);
    delay(70);
  }
  Steering_Wheels.write(90);
}


// Steering Crawler
void Steer_Crawler()
{
/*  if (LIDAR_R > 180)
  {
    oscillate_R();
    Steering_Wheels.write(100);
    delay(2);
    ESC.write(60);
    delay(500);
    Steering_Wheels.write(90);
    delay(2);
    ESC.write(60);
  } */

  if (LIDAR_L > 180)
  {
    oscillate_L();
    /*Steering_Wheels.write(80);
    delay(2);
    ESC.write(60);
    delay(500);
    Steering_Wheels.write(90);
    delay(2);
    ESC.write(60);*/
  }

   if (LIDAR_Diff > 90)
  { 
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

int RecievePacket()
{
 xbee.readPacket(50); 
 if (xbee.getResponse().isAvailable())
  {
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
    {
      xbee.getResponse().getZBRxResponse(rx);
      return rx.getData(0);
    }
  }
  else
    return 0;
}

// MAIN LOOP
void loop()
{
   int receive = RecievePacket();
   switch(receive)
   {
    case 48:
      POWER = 0;
      break;
      
    case 49:
      POWER = 1;
      break;
      
    case 77:
      SWITCH = 1;
      break;

    case 65:
      SWITCH = 0;
      break;

    case 70:
      COMMAND = 'F';
      break;

    case 66:
      COMMAND = 'B';
      break;

    case 82:
      COMMAND = 'R';
      break;

    case 76:
      COMMAND = 'L';
      break;

    case 80:
      PAUSE = 1;
      break;

    case 67:
      PAUSE = 0;
      break;

    default:
      break;
   }
  if(POWER)
  {
    Serial.println("INSIDE POWER ON");
    if(SWITCH)
    {
      Serial.println("INSIDE MANUAL");
      if(!PAUSE)
      {
        Serial.println("INSIDE CONTINUE");
        switch(COMMAND)
        {
          case 'F':
            Steering_Wheels.write(90);
            delay(2);
            ESC.write(60);
            Serial.println("INSIDE FORWARD");       
            break;
            
          case 'B':
            Steering_Wheels.write(90);
            delay(2);
            ESC.write(120);
            break;
            
          case 'R':
            Steering_Wheels.write(70);
            ESC.write(70);
            delay(1500);
            ESC.write(90);         
            break;
            
          case 'L':
            Steering_Wheels.write(110);
            ESC.write(70);
            delay(1500);
            ESC.write(90);         
            break;
        }
      }
      else
      if(PAUSE)
      {
         Serial.println("INSIDE PAUSE");
         ESC.write(90);
         delay(10);
         Steering_Wheels.write(90); 
      }
    }
    else
    if(SWITCH == 0)
    {
      Serial.println("INSIDE AUTO");
      Ultrasonic();
      LIDAR();
      if (Cms_Out <= 80)           //Collision Avoidance
      {
        ESC.write(90);
        delay(1);
        Steering_Wheels.write(90);
      }
      else
        Controller();
    }
  }
  else
  if(POWER == 0)
  {
    int POWER = 0;
    int SWITCH = 0;
    int PAUSE = 0;
    Serial.println("INSIDE POWER OFF");
    ESC.write(90);
    delay(10);
    Steering_Wheels.write(90);
  }
}


