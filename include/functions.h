#include<Arduino.h>
//FUNCIONES USADAS 
//Funcion que calcula los valores de pertencia para cada grafica: 
#include <map>
#include <vector>
#include<string>

#define Trig GPIO_NUM_12 
#define Echo GPIO_NUM_14

using namespace std; 
std::map<int, vector<float>> build_graph(char* Input, vector<Func_meb> &funciones_memb, vector<float> &time, vector<float> &y_values){
    std::map<int, vector<float>> values_to_return;  
   Serial.print("Nombre: "); 
   Serial.println(Input); 
   Serial.print("Funcioens de membre: ");
   Serial.println(funciones_memb[0].k);  
   // Recorrido por todas las funciones de membr, toma sus atributos y calcula el valor de pertenencia de cada uno: 
    for (int i = 0; i < funciones_memb.size(); i++) {
        if (funciones_memb[i].tipo == "exp") {
              Serial.println("EXP"); 
          for (int j = 0; j < time.size(); j++) {
              float cuadrado_x = pow(time[j] - funciones_memb[i].m, 2);
              y_values[j] = exp(- funciones_memb[i].k * cuadrado_x); 
          }
          values_to_return.insert({ i, y_values});  
        }
        else{
            Serial.println("No es exp"); 
        }
    }
    return values_to_return; 
}

///Inicio del vector del tiempo: 
std::vector<float> tiempo(const float init_time, const float finish_time, const float paso)
{
  //Inicio del vector x: Se les dan valores de 0 - 10 con espaciado de 0.001
  std::vector<float> time;
  for (float i = init_time; i < finish_time; i += paso) {
    time.push_back(i);
  }
  
  return time;
}

//Valores del eje x ERROR: 
std::vector<float> error_xval(float init_time, const float finish_time )
{
  //Inicio del vector x: Se les dan valores de 0 - 10 con espaciado de 0.001
  float rango = finish_time - init_time; 
  float j = init_time; 
  std::vector<float> time(rango*100, 0);
  for (float i = 0; i < rango*100; i += 1) {
    time[i] = j;
    j = j + 0.01; 
  }
  
  int32_t length = time.size();
  int capacity = time.capacity();
  return time;
}

//Calculo de la  distancia: 
float calc_dist(int res_prom){
  float dist_prom = 0; 
  for (size_t i = 0; i < res_prom; i++)
  {
    //Calcual la distancia:
    digitalWrite(Trig, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(Trig, LOW); 
    //Medimos la distancia: 
    float tiempo  = pulseIn(Echo, HIGH); 
    //Convertimos la distancia en cm:  
    float distancia = tiempo/59; 
    Serial.print("Distancia: ");
    Serial.println(distancia); 
    delay(60);
    dist_prom += distancia; 
  }
  dist_prom = dist_prom/res_prom; 
  
  return dist_prom; 
}

//Fuzzyfica la entrada: 
//Toma la entrada y busca el valor al que corresponde en 
//los valores fuzzyficados: 
vector<float> fuzzy_input(std::map<int, vector<float>>& func_membr_map, float input, float init_rango, int conts){
  vector<float> fuzzyinputs; 
  for (size_t i = 0; i < func_membr_map.size(); i++)
  {
    auto item = func_membr_map.find(i);
    Serial.println("Valores fuzzyficados: ");   
    Serial.println(item->second[conts*(input - (init_rango))]);
    auto val_fuzzy = item->second[conts*(input - (init_rango))];
    //Lista con el valor de la distancia fuzzyficado   
    fuzzyinputs.push_back(val_fuzzy); 
  }
  return fuzzyinputs; 
}

