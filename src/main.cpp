#include <Arduino.h>

float adc2vin(){
  float adc_lect = float(analogRead(A0)); 
  float v_in = (adc_lect*3.3)/1023; 

  // float sum = 0; 
  // for(int i = 0; i < 10; i++){
  //   sum = v_in + sum; 
  // }
  // v_in = sum/10; 

  return v_in;  
}

struct Motor
{
  void encender(){
    digitalWrite(IN1, HIGH); 
    digitalWrite(IN2, LOW);
    analogWrite(PWM, 1023);  
  }; 

  int IN1; 
  int IN2; 
  int PWM;
};

Motor ventilador = {D0, D1, D2}; 
//Sharp
float adc_lect = 0; 
float v_in = 0; 

//Ultrasonico: 
int trig = D3; 
int echo = D8; 
int dist = 0; 
float tiempo_enc = 0; 

void setup() {
  Serial.begin(9600); 
  pinMode(ventilador.IN1, OUTPUT); 
  pinMode(ventilador.IN2, OUTPUT); 
  pinMode(ventilador.PWM, OUTPUT);
  pinMode(trig, OUTPUT); 
  pinMode(echo, INPUT); 
}

void loop() {
  digitalWrite(trig, HIGH); 
  delay(10); 
  digitalWrite(trig,LOW); 
  tiempo_enc = pulseIn(echo, HIGH);
  dist = tiempo_enc /58.2 ; 
  Serial.println(dist);  
  delay(100); 
  // ventilador.encender();
  // v_in = adc2vin(); 
  // Serial.println(v_in);
}