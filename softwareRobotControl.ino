#include <SoftwareSerial.h>
#include <Servo.h>

#define LED_R   8
//#define LED_Y   9 //현재는 servo와 겹쳐서 주석처리
#define LED_G   10
#define BUTTON  11
#define VOLUME  A0
#define CDS  A1
//초음파 센서 변수
const int trigPin = 4;
const int echoPin = 5;
float duration, distance;

Servo myservo; 
SoftwareSerial mySerial(2, 3); // RX, TX
bool bStart = false; 
int nVol,cdsVol;
unsigned char cVol;
bool isStand = false;

void setup() {
  pinMode(LED_R,OUTPUT);
  //pinMode(LED_Y,OUTPUT);
  pinMode(LED_G,OUTPUT);
  pinMode(BUTTON,INPUT);
  pinMode(CDS,INPUT);
  myservo.attach(9);
  //입력 용도로 아날로그 핀 A0을 가변저항에 할당
  pinMode(VOLUME,INPUT);
  //초음파 센서 변수
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  //아두이노의 디버깅을 위해 씨리얼 포트 활성화
  Serial.begin(115200);
  Serial.println("----------프로그램 시작----------");
  mySerial.begin(115200);
  Serial.println("----------software Serial 시작----------");
}

void ledCon(int nVol) 
{
  digitalWrite(LED_R,HIGH);
  delay(nVol);
  digitalWrite(LED_R,LOW);
  /*
  digitalWrite(LED_Y,HIGH);
  delay(nVol);
  digitalWrite(LED_Y,LOW);
  */
  digitalWrite(LED_G,HIGH);
  delay(nVol);
  digitalWrite(LED_G,HIGH);
  delay(nVol);
  digitalWrite(LED_G,LOW);
  delay(nVol);
}
void readSensor(){
  cdsVol = analogRead(CDS);
    cVol = nVol = analogRead(VOLUME);
    Serial.print("cdsVol = ");
    Serial.print(cdsVol);
    Serial.print("\t cVol = ");
    Serial.print(cVol);
    Serial.print("\t nVol = ");
    Serial.println(nVol);
    // cVol과 nVol이 차이가 나는 이유 8비트만큼 차이나서 짤리는것
}

bool toggleCheck(){
  for(int i =0; i<100;i++){
    if(digitalRead(BUTTON)){
      return true;
    }
    delay(1);
  }
  return false;
}

void robotCon(int nMotion){ //휴머노이드 제어 하는 방법
  unsigned char exeCmd[15] = {0xff, 0xff, 0x4c, 0x53,//header
                              0x00, 0x00, 0x00, 0x00,//address
                              0x30, 0x0c, //30 0c 는 모션
                              0x03, //파라미터 길이
                              0x01, 0x00, 100, //1번 동작
                              0x54}; //체크
  exeCmd[11] = nMotion;
  exeCmd[14] = 0x00;
  for(int i=6; i<14;i++){
    exeCmd[14]+=exeCmd[i];
  }
  mySerial.write(exeCmd,15);
  delay(50);

}

void ledRobotStat(){
  digitalWrite(LED_G,isStand);
}

void lightReaction(){
  cdsVol = analogRead(CDS);
  if(cdsVol<500){
    if(isStand){
      robotCon(115);
      delay(5000);
      isStand= false;
      myservo.write(0);
      Serial.println("is sitting...");
    }else{
      ledRobotStat();
    }
  }
  if(cdsVol>500){
    if(!isStand){
      robotCon(116);
      delay(5000);
      isStand= true;
      myservo.write(512);
      Serial.println("is standing...");
    }else{
      ledRobotStat();
    }
  }

  if(distance>150){
    if(isStand){
      robotCon(115);
      delay(5000);
      isStand= false;
    }
  }else{
    if(distance>100){
      robotCon(116);
      delay(5000);
      isStand= true;
    }else{
      if(distance>50){
        robotCon(19);
        delay(7000);
      }else{
        if(distance<50){
          robotCon(22);
          delay(5000);
        }
      }
    }
  }
}

void ultrasonic(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  Serial.print("Distance: ");
  Serial.println(distance);
}

void loop() {
  //아날로그 핀 A0의 값을 읽어서 화면에 프린트 하는 것

  ultrasonic();
  lightReaction();
  for(int i =0; i<10;i++){
    if(toggleCheck()){
      bStart = !(bStart);
    }
  }

  if(bStart){
    readSensor();
    //ledCon(nVol);
    robotCon(19);
    delay(7000); //로봇이 동작을 완료할때까지 기다리는 시간
    bStart = false;
  }else{
    delay(10);  //기본 딜레이
    Serial.println("Waiting Button...");
  }
}
