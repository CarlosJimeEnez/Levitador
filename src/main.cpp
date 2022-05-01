#include <Arduino.h>
#include <map>
#include <vector>
#include<string>
#include<map>
using namespace std; 
struct Func_meb {
  //Funciones: 
  Func_meb(char *nombre, std::string tipo, double m, double k){
    this ->k = k; 
    this ->m = m; 
    this->tipo = tipo; 
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
  double m = 0;
  double k = 0;
};
//Clase para hacer arreglos de arreglos: 
struct Arreglo_arreglos
{
    //Constructor: 
    Arreglo_arreglos(vector<float> arreglo) {
        for (auto i : arreglo) {
            this->arreglo.push_back(i); 
        }
    }

    //atributos: 
    vector<float> arreglo; 
};

std::map<int, vector<float>> build_graph(char* Input, vector<Func_meb> &funciones_memb, vector<float> &time, vector<float> &y_values){
    std::map<int, vector<float>> values_to_return;  
    
    //Recorrido por todas las funciones de membr, toma sus atributos y calcula el valor de pertenencia de cada uno: 
    for (int i = 0; i < funciones_memb.size(); i++) {
        if (funciones_memb[i].tipo == "exp") {
          Serial.println("EXP"); 
          for (int j = 0; j < time.size(); j++) {
              double cuadrado_x = pow(time[j] - funciones_memb[i].m, 2);
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
std::vector<float> tiempo(const float init_time, const float finish_time)
{
  //Inicio del vector x: Se les dan valores de 0 - 10 con espaciado de 0.001
  std::vector<float> time;
  int rango = finish_time - init_time; 
  for (float i = init_time; i < finish_time; i += 0.01) {
    time.push_back(i);
  }
  
  int32_t length = time.size();
  int capacity = time.capacity();
  return time;
}

///
void setup()
{
  Serial.begin(115200);
  //Inicializamos el eje y y x: 
  vector<float> time = tiempo(2.5, 48.5); 
  vector<float> y_values(time.size(), 0); 

  // for (auto &&i : time)
  // {
  //   Serial.println(i); 
  // }

//  Funciones de membresia: 
  Func_meb fun1("NB", "exp", 2.5, 0.03); 
  Func_meb fun2("NM", "exp", 10.13, 0.04);
  Func_meb fun3("NS", "exp", 19.71, 0.04);
  Func_meb fun4("Z", "exp", 27, 0.09);
  Func_meb fun5("PS", "exp", 33.6, 0.05);
  Func_meb fun6("PM", "exp", 40.87, 0.04);
  Func_meb fun7("PB", "exp", 48.5, 0.03);
  Func_meb arreglo[] = { fun1, fun2, fun3, fun4, fun5, fun6, fun7};

  std::vector<Func_meb> funciones_memb;
  for (auto fun : arreglo) {
      funciones_memb.push_back(fun);
      fun.mostrar_valores(); 
  }

  //Inicia el objeto de la clase Grafica y prepara los valores para crear la grafica de 
  //entrada 1.
  char Input[10] = "Input1";
  auto y_values_map = build_graph(Input, funciones_memb, time, y_values);
  Serial.println(y_values.size());   
  //Obtien los valores retornados en y_values_map y los organiza en y_values
  vector<float> contendor_temp; 
  vector<Arreglo_arreglos> y_values_arreglo; 
  for (auto i = 0; i < funciones_memb.size(); i++) {
    auto item = y_values_map.find(i);
    Serial.println(item->second[100*(48.5 - 2.5)]);   
  }
}

void loop()
{

}