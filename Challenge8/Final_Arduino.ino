#include <math.h>
#include <Wire.h>
#include <Servo.h>
#include <SoftwareSerial.h>
#include <XBee.h>
#include  <SharpIR.h>

// GLOBAL FLAGS
int POWER = 0;
int SWITCH = 1;
int PAUSE = 0;
char COMMAND = 'Q';
char TURN = 'Q';
int CORNER = 1;


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


// IR SENSOR VARIABLES
int IR_PIN = 2;
int IR_Distance;

// LIDAR VARIABLES
unsigned long LIDAR_Left, LIDAR_Right;
double LIDAR_L, LIDAR_R;
double LIDAR_Diff, Per_Angle_Change, LAST_Angle_Error, Angle_Error, OUT_Angle, Angle_Feedback;
double Integral_Val = 0, Derivative_Val;
double Angle;
double save_error;

//SETPOINT FOR CRAWLER
const double Point = 90;

double Kp = 1.2;
double Ki = 0;
double Kd = 0.1;

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
  LIDAR();
  SharpIR sharp(IR_PIN, 20150);
  IR_Distance = sharp.distance();
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
  Serial.begin(9600);                   // Start Serial Communications;
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

// OSCILLATE LEFT
void oscillate_L()
{
  
  ESC.write(90);
  delay(400);
  
  switch(CORNER)
  {
    case 1:
              ESC.write(120);
              Steering_Wheels.write(90);
              delay(500);
              ESC.write(60);
              Steering_Wheels.write(165);
              delay(2100); 
              Steering_Wheels.write(90);
              CORNER++;
              save_error = LAST_Angle_Error;
              break;
              
    case 2:
              ESC.write(90);
              CORNER++;
              Steering_Wheels.write(65);
              ESC.write(60);
              delay(700);
              Steering_Wheels.write(85);
              delay(2200);
              LAST_Angle_Error = save_error;
              break;
                  
    case 3:      
              Steering_Wheels.write(80);    
              ESC.write(110);
              delay(1400);
              ESC.write(65);
              Steering_Wheels.write(165);
              delay(2200);
              Steering_Wheels.write(80);
              CORNER++; 
              break;
    
    case 4:
              ESC.write(120);
              Steering_Wheels.write(90);
              delay(600);
              ESC.write(60);
              Steering_Wheels.write(165);
              delay(2500);
              Steering_Wheels.write(90);
              CORNER++; 
              break;
    
    case 5:
              ESC.write(110);
              delay(1000);
              ESC.write(65);
              Steering_Wheels.write(165);
              delay(2300);
              Steering_Wheels.write(90);
              CORNER = 1; 
              break;
  }
  ESC.write(90);
}


// Steering Crawler
void Steer_Crawler()
{
  if (LIDAR_Diff > 90)
  { 
    Steering_Wheels.write(90);
    delay(2);
    ESC.write(50);
  } 

  // LEFT FUNTIONALITY
  if(LIDAR_L > 250)
  {
    oscillate_L();
    //delay(100);
  }
      
  if (abs(OUT_Angle) < 10)
  {
    Steering_Wheels.write(Angle);
    delay(2);
    ESC.write(55);
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

// FUNCTION TO RECEIVE DATA FROM XBEE
int RecievePacket()
{
 xbee.readPacket(50); 
 xbee.getResponse().getZBRxResponse(rx);
 if(xbee.getResponse().isAvailable())
  return rx.getData(0);
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
      TURN = 'Q';
      break;

    case 66:
      COMMAND = 'B';
      TURN = 'Q';
      break;

    case 82:
      TURN = 'R';
      break;
      
    case 76:
      TURN = 'L';
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
    if(SWITCH)
    {
      if(!PAUSE)
      {
        switch(COMMAND)
        {
          case 'F':
            if(TURN == 'Q')
            {
               Steering_Wheels.write(90);
               delay(2);
               ESC.write(75);
               break;
            }
            else
            if(TURN == 'R')
            {
              Steering_Wheels.write(70);
              delay(2);
              ESC.write(75);
              break;
            }
            else
            if(TURN == 'L')
            {
              Steering_Wheels.write(120);
              delay(2);
              ESC.write(75);
              break;
            }
            
          case 'B':
            if(TURN == 'Q')
            {
              Steering_Wheels.write(90);
              delay(2);
              ESC.write(105);
              break;
            }
            else
            if(TURN == 'R')
            {
              Steering_Wheels.write(70);
              delay(2);
              ESC.write(105);
              break;
            }
            else
            if(TURN == 'L')
            {
              Steering_Wheels.write(120);
              delay(2);
              ESC.write(105);
              break;
            }

          default:
            Steering_Wheels.write(90);
            delay(2);
            ESC.write(90);
            break;
        }
      }
      else
      if(PAUSE)
      {
         ESC.write(90);
         delay(10);
         //Steering_Wheels.write(90); 
      }
    }
    else
    if(SWITCH == 0)
    {
      SharpIR sharp(IR_PIN, 20150);
      IR_Distance = sharp.distance();
      LIDAR();
      if (IR_Distance > 30)           //Collision Avoidance
        Controller();
      else
      {
        //ESC.write(90);
        Steering_Wheels.write(90);
        delay(100);
        ESC.write(105);
        delay(3000);
        if(LIDAR_R > LIDAR_L)
        {
          Steering_Wheels.write(50);
          delay(100);
          ESC.write(75);
          delay(1500);
        }
        else
        if(LIDAR_L > LIDAR_R)
        {
          Steering_Wheels.write(140);
          delay(100);
          ESC.write(75);
          delay(1500);
        }
      }
    }
  }
  else
  if(POWER == 0)
  {
    int POWER = 0;
    int SWITCH = 1;
    int PAUSE = 0;
    COMMAND = 'Q';
    TURN = 'Q';
    ESC.write(90);
    delay(10);
    Steering_Wheels.write(90);
  }
}


