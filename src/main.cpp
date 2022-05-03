#include <Arduino.h>
#include <map>
#include <vector>
#include<string>
#include<map> 


#define motor GPIO_NUM_27 //PWM 
#define Trig GPIO_NUM_12 
#define Echo GPIO_NUM_14


using namespace std;

//Clase base de las funciones de membr: 
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

//Funcion que calcula los valores de pertencia para cada grafica: 
std::map<int, vector<float>> build_graph(char* Input, vector<Func_meb> &funciones_memb, vector<float> &time, vector<float> &y_values){
    std::map<int, vector<float>> values_to_return;  
    Serial.print("Nombre: "); 
    Serial.println(Input); 
    Serial.print("Funcioens de membre: ");
    Serial.println(funciones_memb[0].k);  
    //Recorrido por todas las funciones de membr, toma sus atributos y calcula el valor de pertenencia de cada uno: 
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
std::vector<float> tiempo(const float init_time, const float finish_time)
{
  //Inicio del vector x: Se les dan valores de 0 - 10 con espaciado de 0.001
  std::vector<float> time;
  for (float i = init_time; i < finish_time; i += 0.01) {
    time.push_back(i);
  }
  
  return time;
}

//Valores del eje x ERROR: 
std::vector<float> error_xval(float init_time, const float finish_time)
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

vector<float> fuzzy_input(std::map<int, vector<float>>& func_membr_map, float input, float init_rango){
  vector<float> fuzzyinputs; 
  for (size_t i = 0; i < func_membr_map.size(); i++)
  {
    auto item = func_membr_map.find(i);  
    Serial.println(item->second[100*(input - (init_rango))]);
    auto val_fuzzy = item->second[100*(input - (init_rango))];
    //Lista con el valor de la distancia fuzzyficado   
    fuzzyinputs.push_back(val_fuzzy); 
  }
  return fuzzyinputs; 
}

/// Variables que guardan los valores de pertenecia de las funciones de membr: 
std::map<int, std::vector<float>> salida_values_map; 
std::map<int, std::vector<float>> error_values_map; 
std::map<int, std::vector<float>> dist_values_map; 


/// Configuraciones del PWM asignado a el motor: 
const float k = 27; 
const int freq = 1000; //HZ
const int canal0 = 0; 
const int resolucion = 10; //bit
float tiempo_inicio = millis();  
int duty_cycle = 0; 

void setup()
{
  Serial.begin(921600);
  //Inicio del tiempo: 
  float init_time = 2.5; 
  float finish_time = 48.5;

  float init_time2 = k - finish_time; 
  float finish_time2 = k - init_time; 

  float init_salida_val = 7; 
  float finish_salida_val = 11.4;

  vector<float> time = tiempo(init_time, finish_time);
  vector<float> y_values(time.size(), 0);
  
//Funciones de membresia INPUT1: 
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

  //ERROR func_membr: 
  Func_meb fun8("NB", "exp", -21.5, 0.004);
  Func_meb fun9("NS", "exp", 24.5, 0.004);
  Func_meb error_arreglo[] = { fun8, fun9 };

  std::vector<Func_meb> error_func_membr;
  for (auto fun : error_arreglo) {
      error_func_membr.push_back(fun);
      //fun.mostrar_valores(); 
  }

  //SALIDA funcion membre:  
    Func_meb fun10("NB", "exp", 7, 2.4);
    Func_meb fun11("NM", "exp", 7.83, 4);
    Func_meb fun12("NS", "exp", 8.57, 4.3);
    Func_meb fun13("Z", "exp", 9.22, 4.3);
    Func_meb fun14("PS", "exp", 9.96, 4.3);
    Func_meb fun15("PM", "exp", 10.7, 4.3);
    Func_meb fun16("PB", "exp", 11.4, 4.3);
    Func_meb salida_arreglo[] = { fun10, fun11, fun12, fun13, fun14, fun15, fun16 };
    
    std::vector<Func_meb> salida_func_membr;
    for (auto fun : salida_arreglo) {
        salida_func_membr.push_back(fun);
        //fun.mostrar_valores(); 
    }

  //Inicia el objeto de la clase Grafica y prepara los valores para crear la grafica de 
  //entrada 1.
  char Input[10] = "Input1";
  dist_values_map = build_graph(Input, funciones_memb, time, y_values);


  //Graph error 2:  
  time.clear(); 
  y_values.clear(); 
  time = error_xval(init_time2, finish_time2);
  for (int i = 0; i < time.size(); i++)
  {
      y_values.push_back(0); 
  }
  char Nombre[10] = "Error"; 
  error_values_map = build_graph(Nombre, error_func_membr, time, y_values);
  

  /// Salida:
  time.clear();
  y_values.clear();
  time = tiempo(init_salida_val, finish_salida_val);
  for (int i = 0; i < time.size(); i++)
  {
      y_values.push_back(0);
  }

  char Nombre_salida[10] = "salida";
  salida_values_map = build_graph(Nombre, salida_func_membr, time, y_values);

  /// PWM resolucion, freq, canal setup. 
  ledcSetup(canal0, freq, resolucion); 
  ledcAttachPin(motor, canal0); 

  // Sensor_Ultrasonico: 
  pinMode(Trig, OUTPUT); 
  pinMode(Echo, INPUT); 
}

void loop()
{ 
  std::vector<float> fuzzy_val_dist; 
  std::vector<float> fuzzy_val_error;
  std::vector<float> kj; 

  //INPUT: 
  float distancia = calc_dist(1);
  float error = k - distancia; 

  //FUZZIFICAR la distancia:
  //Toma el par de valores en la posicion i y la asigna a item, para acceder a el array de valor de <y> 
  //se usa el el puntero item->second.
  fuzzy_val_dist = fuzzy_input(dist_values_map, distancia, 2.5);  
  fuzzy_val_error = fuzzy_input(error_values_map, error, -21.5); 

  //Calculo de KJ:
  for(auto i: fuzzy_val_dist){
    for(auto j: fuzzy_val_error){
      float kj_local = min(i,j); 
      kj.push_back(kj_local);  
      Serial.print("KJ: "); 
      Serial.println(kj_local); 
    }
  } 


  //Calculo del grado de consistencia 

  //Defuzzificacion:
  float num_defuzzy = 0; 
  float den_defuzzy = 0; 
  vector<float> mult_kj_cj; 
  vector<float> cj = {
    7 , 7,
    7.83, 7,83,
    8.57, 8.57,
    9.22, 9.22, 
    9.96, 9.96, 
    10.7, 10.7,
    11.4, 11.4
  }; 

  for (size_t i = 0; i < kj.size(); i++)
  {
    float mult_num = kj[i] * cj[i]; 
    mult_kj_cj.push_back(mult_num); 
  }
  
  for(auto i: mult_kj_cj){
    num_defuzzy += i; 
  }
  
  //Denomiandor: 
  for(auto i: kj){
    den_defuzzy += i; 
  }


  float v_out = num_defuzzy/den_defuzzy; 
  Serial.println("v_out"); 
  Serial.println(v_out); 
  
  int duty_cycle = 89.639*v_out - 1.473; 

  ledcWrite(canal0, duty_cycle); 
  Serial.println("duty_cycle"); 
  Serial.println(duty_cycle); 

  float tiempo_fijo = millis(); 
  // if(tiempo_fijo > tiempo_inicio + 1000){
  //   tiempo_inicio = millis(); 
  //   digitalWrite(led, LOW); 
  //   Serial.println("Paso 1 seg"); 
  // }
}