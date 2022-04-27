#include <Arduino.h>
#include <math.h>
#include <map>
#include<Plotter.h>

using namespace std;

#define trig D3 
#define echo D8

#define IN1 D0 
#define IN2 D1 
#define ENA1 D2

double x; 

float ad2volt(){
  float adc_lect = float(analogRead(A0)); 
  float v_in = (adc_lect*3.3)/1023; 
  // float sum = 0; 
  // for(int i = 0; i < 10; i++){
  //   sum = v_in + sum; 
  // }
  // v_in = sum/10; 

  return v_in;  
}

struct Motor{
  void encender(){
    digitalWrite(IN1, HIGH); 
    digitalWrite(IN2, LOW);
    analogWrite(PWM, 1023);  
  }; 

  int PWM;
};

struct Func_meb{
  //Funciones: 
  friend void build_graph(); 
  void get_name(){
    Serial.println(nombre); 
  }

  //Atributos: 
  char nombre[10] = " "; 
  String tipo; 
  float m = 0; 
  float k = 0;  
};

void build_graph(char *nombre, float rango, Func_meb *funciones_meb, int length_fun_memb){ 
    int rango_grafica =  rango/0.01; 
    Serial.println(rango_grafica); 

    //Inicializa el valor del rango del eje Y: 
    float y_values[rango_grafica]; 
    float x_values[rango_grafica]; 

    //Realiza un recorrido por todas las funciones de membresia y comprueba si es Gaussiana, luego 
    //guarda el valor de cada elemento de esta funcion en un array: 
    for (int i = 0; i < length_fun_memb; i++)
    {    
      //Si la funcion es del tipo exp: 
        if (funciones_meb[i].tipo == "exp")
        {
          Serial.println("Exponencial");

          //Construccion de la grafica: 
          for (int j = 0; j < rango_grafica; j++){
            y_values[j] = exp(-funciones_meb[i].k *(pow( x_values[j] - funciones_meb[i].m, 2)));
            Serial.println(y_values[j]); 
          }

        }else{
          Serial.println("IdontNow"); 
        }
    Serial.println("Hi"); 
    }
}

Func_meb fun1 = {"Baja", "exp", 0.001, 1}; 
Func_meb fun2 = {"Media", "exp", 3, 0.4}; 
Func_meb fun3 = {"Optima", "exp", 6, 3.5}; 
Func_meb fun4 = {"MediaAlta", "exp", 7.5, 2}; 
Func_meb fun5 = {"Alta", "exp", 10, 0.9}; 
Func_meb funciones_meb[] = {fun1, fun2, fun3, fun4, fun5};

// Inicializa un objeto de la clase ventilador: 
Motor ventilador = {ENA1}; 

//Sharp
float adc_lect = 0; 
float v_in = 0; 

//Ultrasonico: 
int dist = 0; 
float tiempo_enc = 0; 
Plotter p; 
/// ----------- Setup 
void setup() {
  Serial.begin(115200);
  
  //Tamaño del vector funciones_meb: 
  int length_func_memb_array = 0;
  for(Func_meb funciones: funciones_meb){
    length_func_memb_array += 1; 
  }
 
  //Inicia el objeto de la clase Grafica y prepara los valores para crear la grafica de 
  //entrada 1. 
  build_graph("Input", 10, funciones_meb, length_func_memb_array);
   

  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(ventilador.PWM, OUTPUT);
  pinMode(trig, OUTPUT); 
  pinMode(echo, INPUT);
  
  p.Begin(); 
 
  // //Auto: deduce el tipo de variable entrante: 
  // for(auto nombre: funciones_meb){
  //   nombre.get_name();
  // }   
}

void loop() {
  
  p.Plot(); // usually called within loop()

  // digitalWrite(trig, HIGH); 
  // delay(10); 
  // digitalWrite(trig,LOW); 
  // tiempo_enc = pulseIn(echo, HIGH);
  // dist = tiempo_enc /58.2 ; 
  // Serial.println(dist);  
  // delay(100); 
  // // ventilador.encender();
  // v_in = ad2volt(); 
  // Serial.println(v_in);
}