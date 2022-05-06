#include<string>
#include<Arduino.h>

struct Func_meb {
  //Funciones: 
  Func_meb(char *nombre, std::string tipo, float m, float k){
    this ->k = k; 
    this ->m = m; 
    this->tipo = tipo; 
    // this->init_time = init_time; 
    // this->finish_time = finish_time; 
    strcpy(this ->nombre, nombre); 
  }

  void mostrar_valores(){
    Serial.println(this->k);
    Serial.println(this->m); 
    Serial.println(this->nombre); 
  }

  //friend map<int, vector<double>> build_graph();

  //Atributos: 
  char nombre[10] = " ";
  std::string tipo = "";
  float m = 0;
  float k = 0;
  // float init_time = 0; 
  // float finish_time = 0; 
};
