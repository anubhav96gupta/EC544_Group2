#include <math.h>
#include <Wire.h>
#include <Servo.h>
#include <PID_v1.h>


#define LIDAR_ADDRESS 0x62
#define LIDAR_CONTROL_ADDRESS 0x00
#define LIDAR_CONTROL_VALUE 0x04
#define LIDAR_RANGE_ADDRESS 0x8f


// Servo for turning the car and servo for reference.
Servo steering_wheels;
Servo ESC;

const float pi = 3.14159;

// PID requisites
double input_angle;
double output_angle;
double reference_angle;

double distanceIn;
double distanceOut;
double reference_distance;

double speed_in;
double speed_out;
double reference_speed;

// angle control
double aKp = 1;
double aKi = 0;
double aKd = 0;

// distance control
double dKp = 1;
double dKi = 0;
double dKd = 0;

// speed control
double sKp = 40;
double sKi = 0;
double sKd = 0;


double positional_error;


// We'll need 3 PID's. Angle from wall, distance from wall, and speed

PID angle_PID(&input_angle, &output_angle, &reference_angle, aKp, aKi, aKd, DIRECT);
PID control_PID(&distanceIn, &distanceOut, &reference_distance, dKp, dKi, dKd, DIRECT);
PID speed_PID(&speed_in, &speed_out, &reference_speed, sKp, sKi, sKd, DIRECT);

void setup(){
  // Controls steering
  steering_wheels.attach(D1);

  // Controls speed
  ESC.attach(D2);

  // Calibrate speed control
  calibrateESC();

  Wire.begin();
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);


  angle_PID.SetMode(AUTOMATIC);
  control_PID.SetMode(AUTOMATIC);
  speed_PID.SetMode(AUTOMATIC);

  angle_PID.SetOutputLimits(-45,45);
  control_PID.SetOutputLimits(-45,45);
  speed_PID.SetOutputLimits(-60,60);

}


void loop() {

}


void pid_steering(void) {
  positional_error = distanceIn - reference_distance;
  control_PID.SetTunings(dKp, dKi, dKd);
  control_PID.Compute();
  steering_wheels.write(1);
}
