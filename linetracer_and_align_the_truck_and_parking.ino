#include<Servo.h>

// Servo Motor
Servo myservo;
int servoPin=9;

// DC Motor
int DC_F=10; //DC모터
int DC_B=11;
int Speed = 170;

// infrared
int inf_left=5; //적외선 왼쪽
int inf_right=4; //적외선 오른쪽
int left_status;
int right_status;

// potentiometer
int pot = A0;
float trailer_ang;

// Ultrasonic Sensor
int trigPin = 13;
int echoPin = 12;
unsigned long duration;
float distance;

int STEP = 1;

// Initial Setup
void setup() {
  Serial.begin(9600); // Serial Baudrate
  // Servo Setup
  myservo.attach(servoPin);
  myservo.write(90);
  // Infrared Setup
  pinMode(inf_left,INPUT);
  pinMode(inf_right,INPUT);
  //potentiometer
  pinMode(pot, INPUT);
  //Ultrasonic Sensor
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
}

// Main Loop
void loop() {
  infra_red();
  potentio_meter();
  Serial.print("trailer twist: ");
  Serial.println(trailer_ang);
  
  if(STEP == 1)
  {
    while(STEP == 1)
    { 
      infra_red();
      Follow_the_line();
      STEP = Follow_the_line();
    }
    STOP();
    delay(2000);
  }
  else if(STEP == 2)
  {
    while(STEP == 2){
      potentio_meter();
      Align_the_truck();
      STEP = Align_the_truck();
    }
    STOP();
    delay(2000);
  }
  else if(STEP == 3)
  {
    while(STEP == 3)
    {
      ultra_sonic();
      potentio_meter();
      Park_the_truck();
      STEP = Park_the_truck();
    }
  }
  else
  {
    STOP();
  }
}

// STEP 1 : Follow the line
int Follow_the_line(){
    if((left_status==1)&&(right_status==1)){
      myservo.write(91);
      FORWARD();
      return 1;
    }
    else if ((left_status==0)&&(right_status==1))
    {
      myservo.write(146); // right turn
      FORWARD();
      return 1;
    }
    else if ((left_status==1)&&(right_status==0))
    {      
      myservo.write(31); // left turn
      FORWARD();
      return 1;
    }      
    else if((left_status==0)&&(right_status==0))
    {
      myservo.write(91);
      STOP();
      return 2;
    }
}

// STEP 2 : Align the truck
int Align_the_truck(){
  if(trailer_ang > 87 && trailer_ang < 89){
    STOP();
    return 3;
  }
  else{
    FORWARD();
    return 2;
  } 
}

// STEP 3 : Park the truck
int Park_the_truck(){
  Speed = 145;
  if(trailer_ang < 88 && distance > 20.0){
    myservo.write(46);
    BACKWARD();
    return 3;
  }
  else if(trailer_ang > 88 && distance > 20.0){
    myservo.write(136);
    BACKWARD();
    return 3;
  }
  else if(trailer_ang == 88 && distance > 20.0)
  {
    myservo.write(91);
    BACKWARD();
    return 3;
  }
  else
  {
    STOP();
    return 4;
  }
}

// Function of Infrared Sensor
void infra_red(){
  left_status=digitalRead(5);
  right_status=digitalRead(4);
}

// Function of Potentiometer
void potentio_meter(){
  trailer_ang = map(analogRead(pot), 0, 1023, 0, 180);
}

// Function of Ultrasonic Sensor
void ultra_sonic(){
  digitalWrite(trigPin, LOW);
  digitalWrite(echoPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = ((float)(340 * duration) / 10000) / 2;
  if (distance == 0.0)
  {
    distance = 20.1;
  }
  Serial.print(distance);
  Serial.println("cm");
  delay(100);
}

// Function of Motor Mode
void FORWARD(){
  analogWrite(DC_F,Speed);
  analogWrite(DC_B,0);
}
void BACKWARD(){
  analogWrite(DC_F,0);
  analogWrite(DC_B,Speed);
}
void STOP(){
  analogWrite(DC_F,0);
  analogWrite(DC_B,0);
}


