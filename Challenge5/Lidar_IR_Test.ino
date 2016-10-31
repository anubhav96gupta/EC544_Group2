#include <math.h>
#include <Wire.h>
#include <Servo.h>
#include <PID_v1.h>

// Servo for turning the car and servo for reference.
Servo steering_wheels;
Servo ESC;

int startupDelay = 1000;
//double maxSpeedOffset = 45; // maximum speed magnitude, in servo 'degrees'
//double maxWheelOffset = 85; // maximum wheel turn magnitude, in servo 'degrees'

int IRpin = 1;                                    // analog pin for reading the IR sensor
const float pi = 3.14159;

//LIDAR Variables
unsigned long LIDAR_Left, LIDAR_Right;
//Pulse Width
/*unsigned long pulse_width1, pulse_width2 ;

// PID requisites
double input_angle;
double output_angle;
double reference_angle;

double distanceIn1;
double distanceOut1;
double reference_distance1 = 90;  //Error of 45, Setpoint of 50cm

double distanceIn2;
double distanceOut2;
double reference_distance2 = 90;  //Error of 45, Setpoint of 50cm

double speed_in;
double speed_out;
double reference_speed;

// angle control
double aKp = 1;
double aKi = 0;
double aKd = 0;

//  distance control
double dKp = 1;
  double dKi = 0;
double dKd = 0;

// speed control
double sKp = 40;
double sKi = 0;
double sKd = 0;


double positional_error;*/


// We'll need 3 PID's. Angle from wall, distance from wall, and speed

/*PID angle_PID(&input_angle, &output_angle, &reference_angle, aKp, aKi, aKd, DIRECT);
PID control_PID1(&distanceIn1, &distanceOut1, &reference_distance1, dKp, dKi, dKd, DIRECT);
PID control_PID2(&distanceIn2, &distanceOut2, &reference_distance2, dKp, dKi, dKd, DIRECT);
PID speed_PID(&speed_in, &speed_out, &reference_speed, sKp, sKi, sKd, DIRECT);*/

void setup(){
  // Controls steering
  LIDAR_setup();
  steering_wheels.attach(8);

  // Controls speed
  ESC.attach(9);

  // Calibrate speed control
  calibrateESC();

 /* angle_PID.SetMode(AUTOMATIC);
  control_PID1.SetMode(AUTOMATIC);
  control_PID2.SetMode(AUTOMATIC);
  speed_PID.SetMode(AUTOMATIC);*/

}

void calibrateESC(){
    ESC.write(180); // full backwards
    delay(startupDelay);
    ESC.write(0); // full forwards
    delay(startupDelay);
    ESC.write(90); // neutral
    delay(startupDelay);
    ESC.write(90); // reset the ESC to neutral (non-moving) value
}

/* Convert degree value to radians */
double degToRad(double degrees){
  return (degrees * 71) / 4068;
}

/* Convert radian value to degrees */
double radToDeg(double radians){
  return (radians * 4068) / 71;
}
  

void LIDAR_setup(){
  Serial.begin(9600); // Start serial communications
  pinMode(2, OUTPUT); // Set pin 2 as trigger pin
  pinMode(3, INPUT); // Set pin 3 as monitor pin
  digitalWrite(2, LOW); // Set trigger LOW for continuous read

  pinMode(6, OUTPUT); // Set pin 2 as trigger pin
  pinMode(5, INPUT); // Set pin 3 as monitor pin
  digitalWrite(6, LOW); // Set trigger LOW for continuous read
}

/*void oscillate(){
  for (int i =0; i < 180; i++)
  {
    double rad = degToRad(i);
    //double speedOffset = sin(rad) * maxSpeedOffset;
    double wheelOffset = sin(rad) * maxWheelOffset;
    ESC.write(45);
    steering_wheels.write(90 + wheelOffset);
    delay(50);
    LIDAR_reading();
    control_PID1.Compute();
  }
}*/

void LIDAR()
{
  LIDAR_Left = pulseIn(5, HIGH)/10;
}

void loop()
{
  float volts = analogRead(IRpin)*0.00322265625;   // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
  float distance = 65*pow(volts, -1.10);          // worked out from graph 65 = theretical distance / (1/Volts)S
  LIDAR();
  Serial.println("IR Reading:\n");
  Serial.println(distance);
  Serial.println("LIDAR Readings: \n");
  Serial.println("Right Lidar:\t");
  Serial.println(LIDAR_Right);
  Serial.println("\n Left Lidar: \t");
  Serial.println(LIDAR_Left);
  
  
  /*control_PID1.Compute();
  control_PID2.Compute();
  if(Output!=0)
  {
    Oscillate();
  }       
  Serial.println(distance);
  if(distance > 35)
    oscillate();
  else{
     ESC.write(90);
    steering_wheels.write(90);
  }*/
  delay(2000); //Delay so we don't overload the serial port
}
