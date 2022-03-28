#include <Servo.h>

int LEDGREEN = 10, LEDRED = 9, LEDYELLOW = 8, LEDEP4 = 7;
int temperatura, temperaturaAnterior, diferencaTemperatura;
const int TREF1 = 15, TREF2 = 28, TREF3 = 30;
const int tLimiar1 = 15, tLimiar2 = 25;
Servo servo;
#define timeCode 10000
#define code 1234

int codigo, codigoInt, distance, incomingByte = 0, contadorEP2;
long duration, tempo, t;
const int trigPin = 5;
const int echoPin = 6;
const int buzzer = 4;
const int LEDPinEP2 = 3;
boolean codigoF = false, intrusao = false, imprime = true, imprime2 = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LEDGREEN, OUTPUT);
  pinMode(LEDRED, OUTPUT);
  pinMode(LEDYELLOW, OUTPUT);
  pinMode(LEDEP4, OUTPUT);
  analogWrite(LEDGREEN, 255);
  analogWrite(LEDRED, 255);
  analogWrite(LEDYELLOW, 255);
  analogWrite(LEDEP4, 255);
  servo.attach(11);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  digitalWrite(trigPin, LOW);
  pinMode(buzzer, OUTPUT);
  pinMode(LEDPinEP2, OUTPUT);
  analogWrite(LEDPinEP2, 255);
  tone(buzzer, 880, 500);
  delayMicroseconds(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  int temperatur = analogRead(A0);
  
  float temperatura = ((1/((1/4090)*(log(temperatur/3300)+(1/25))))-273.15);
  
  diferencaTemperatura = temperatura - temperaturaAnterior;

  temperatura = 15;
  if (diferencaTemperatura >= 0.5 || diferencaTemperatura >= -0.5)
  {
    //Serial.println(temperatura);
  }

  if (temperatura <= TREF1)
  {
    analogWrite(LEDGREEN, 255);
    analogWrite(LEDRED, 0);
    analogWrite(LEDYELLOW, 0);
  }
  else if (temperatura > TREF1 && temperatura <= TREF2)
  {
    analogWrite(LEDGREEN, 0);
    analogWrite(LEDRED, 0);
    analogWrite(LEDYELLOW, 0);
  }
  else if (temperatura > TREF2 && temperatura <= TREF3)
  {
    analogWrite(LEDGREEN, 0);
    analogWrite(LEDRED, 0);
    analogWrite(LEDYELLOW, 255);
  }
  else
  {
    analogWrite(LEDGREEN, 0);
    analogWrite(LEDRED, 255);
    analogWrite(LEDYELLOW, 0);
  }

  temperaturaAnterior = temperatura;
  
  int luminosidade;
  int LDRluz = analogRead(A1);

  luminosidade = map(LDRluz, 0, 1023, 0, 255);
  if (millis() > 10000)
  {
    if (luminosidade >= 115)
    {
      luminosidade = luminosidade / 2;
      analogWrite(LEDEP4, luminosidade);
    }
    /*else
      {
      analogWrite(LEDEP4, luminosidade);
      }*/
  }
  else
  {
    if (luminosidade < 200)
    {
      analogWrite(LEDEP4, 0);
    }
    else
    {
      analogWrite(LEDEP4, luminosidade);
    }
  }

  temperatura = 15;
  
  if(temperatura<tLimiar1 && luminosidade<150)
  {
    servo.write(30);
  }
  else if(temperatura>tLimiar2 && luminosidade<150)
  {
    servo.write(120);
  }
  else if(temperatura>=tLimiar1 && temperatura<=tLimiar2)
  {
    servo.write(75);
  }
  else
  {
    servo.write(30);
  }

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58.31;

  int sensorValue = analogRead(A2);

  if (distance < 60 && intrusao == false && imprime2 == false) {
    Serial.print("Distancia: ");
    Serial.println(distance);
    tempo = millis();
    intrusao = true;
    imprime2 = true;
    }

    if (Serial.available() > 0) {
    codigoInt = Serial.parseInt();
    }

    if (code == codigoInt) {
    codigoF = true;
    }
    t = millis();
    t= t - tempo;
    if (intrusao == true && codigoF == false && t > timeCode) {
    tone(buzzer, 880);
    if (imprime == true) {
      Serial.println("INTRUSÃO");
      imprime = false;
    }

    }

    if (codigoF == true) {
    noTone(buzzer);
    intrusao = false;
    codigoInt = 0;
    codigoF = false;
    imprime2 = false;
    imprime = true;
    }

  if (sensorValue < 512) {
    contadorEP2++;
  }

  if (contadorEP2 % 2 == 0) {
    analogWrite(LEDPinEP2, 0);

  } else {
    analogWrite(LEDPinEP2, 255);
  }
}
