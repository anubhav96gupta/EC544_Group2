
const int PWPin = 5;
long pulse1, sensor1 , cms;

void setup () {
  Serial.begin(9600);
  pinMode(PWPin, INPUT);
}

void read_sensor(){
  pulse = pulseIn(PWPin, HIGH);
  sensor = pulse1/147;
  cms = sensor*2.54;
}

void printall(){         
  Serial.print("S1");
  Serial.print(" ");
  Serial.print(cms);
  Serial.println(" ");
}

void loop () {
  read_sensor();
  printall();
  delay(1000);
}

