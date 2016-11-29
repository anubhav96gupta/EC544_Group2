
const int PWPin = 5;
long pulse, sensor , cms;

void setup () {
  Serial.begin(9600);
  pinMode(PWPin, INPUT);
}

void read_sensor()
{
  pulse = pulseIn(PWPin, HIGH);
  sensor = pulse/147;
  cms = sensor*2.54;
  Serial.println(cms) ;
}

}

void loop () {
  read_sensor();
  delay(1000);
}

