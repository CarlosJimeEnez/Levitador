#include <Arduino.h>
#include <math.h>
#include <map>
using namespace std;

#define trig D3 
#define echo D8

#define IN1 D0 
#define IN2 D1 
#define ENA1 D2

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
  friend class Grafica; 
  void get_name(){
    Serial.println(nombre); 
  }

  //Atributos: 
  char nombre[10] = " "; 
  String tipo; 
  float m = 0; 
  float k = 0;  
};

struct Grafica{
  //Funciones
  Grafica(char *nombre, float rango, Func_meb *funciones_meb, int length_fun_memb){
    strcpy(this -> nombre ,nombre);   
    this -> rango = rango;
    this -> length_fun_memb = length_fun_memb;  

    //Tamano del vector del rengo total de la entrada: 
    this->length_vector = rango / 0.001; 

    //Guarda en el atributo: funciones de memb las funciones de memb. 
    for(int i=0;i<length_fun_memb;i++){
      this -> funciones_meb[i] = funciones_meb[i];  
    }

  }
  
  //Devuelve en un mapa de la forma <key - value> los valores de las graficas creadas:
  void build_graph(){
    // int length = this->length_vector; 
    // Inicializa el valor del rango del eje Y: 
    
    // //Realiza un recorrido por todas las funciones de membresia y comprueba si es Gaussiana, luego 
    // //guarda el valor de cada elemento de esta funcion en un array: 
    // for (Func_meb funciones: this->funciones_meb)
    // {    
    //   //Si la funcion es del tipo exp: 
    //     if (funciones.tipo == "exp")
    //     {
    //       Serial.println("Exponencial");
    //       for (int i = 0; i < length_rango_entrada ;i++){
    //         y_values[i] = exp(-funciones.k*(pow(x_values[i]-funciones.m,2)));
            
    //       }
    //     }else{
    //       Serial.println("IdontNow"); 
    //     }
    // }
    
  }

  void get_names(){
    for(Func_meb funciones: funciones_meb){
      funciones.get_name();  
    }
  }

  //Atributos

  int length_vector = 0; 
  int length_fun_memb = 0; 
  char nombre[20] = ""; 
  float rango = 0.0;
  Func_meb funciones_meb[10]; 
  
};


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
  Grafica Input1("Input", 10, funciones_meb, length_func_memb_array);
  Input1.build_graph(); 

  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(ventilador.PWM, OUTPUT);
  pinMode(trig, OUTPUT); 
  pinMode(echo, INPUT);
  
  // //Auto: deduce el tipo de variable entrante: 
  // for(auto nombre: funciones_meb){
  //   nombre.get_name();
  // }   
}

void loop() {
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