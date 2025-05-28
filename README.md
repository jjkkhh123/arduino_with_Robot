# arduino_with_Robot
## 아두이노를 이용한 로봇과의 상호작용 및 자동화

로봇 자동화의 핵심 코드는 아래에 있다. <br>
빛의 크기를 나타내는 변수인 cdsVol이고 <br>
초음파를 통한 거리를 나타내는 변수인 distance이다.
```
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
      Serial.println("150cm이내 사물 없음 휴식중...");
    }
  }else{
    if(distance>100){
      robotCon(116);
      delay(5000);
      isStand= true;
      Serial.println("물체 인식 완료 일어납니다.");
    }else{
      if(distance>50){
        robotCon(19);
        delay(7000);
        Serial.println("안녕하세요 이 이상 오면 전투로 간주합니다.");
      }else{
        if(distance<50){
          robotCon(22);
          delay(5000);
          Serial.println("거리 50cm미만 전투 준비");
        }
      }
    }
  }
}
```
<br>
위 코드를 통해 다음의 행동들을 한다. 범위는 초음파 센서의 인식 거리에 따라 나누었다.  <br>     
150cm ~ 아무도 없음, 어두움 : 앉아서 기다림         <br>
100 ~ 150 : 일어나기    <br>
50 ~ 100 : 인사하기    <br>
~ 50 : 전투태세   <br>
