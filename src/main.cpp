#include <Arduino.h>
#include <map>
#include <vector>
#include<string>
#include<map> 

#include<Func_meb.h>
#include<functions.h>

#define motor GPIO_NUM_27 //PWM 
#define Trig GPIO_NUM_12 
#define Echo GPIO_NUM_14

using namespace std;

/// Variables que guardan los valores de pertenecia de las funciones de membr: 
std::map<int, std::vector<float>> salida_values_map; 
std::map<int, std::vector<float>> error_values_map; 
std::map<int, std::vector<float>> dist_values_map; 
std::map<int, std::vector<float>> hecho_values_map; 
std::vector<Func_meb> salida_func_membr;


/// Configuraciones del PWM asignado a el motor: 
const float k = 25; 
const int freq = 1000; //HZ
const int canal0 = 0; 
const int resolucion = 10; //bit
float tiempo_inicio = millis();  
int duty_cycle = 0; 

////
void setup()
{
  Serial.begin(9600);
  
  Serial.println("Cargando funciones de membresia: "); 
  //Input 1 vectores: 
  float init_time = 2.5; 
  float finish_time = 48.5;

  //Error configuraciones: 
  float init_time2 = k - finish_time; 
  float finish_time2 = k - init_time; 

  //Salida configuraciones: 
  float init_salida_val = 4; 
  float finish_salida_val = 9;
  
  //Funcion para automatizar el despliegue de funciones:
  float rango_total = finish_salida_val - init_salida_val; 
  int num_fun = 7; 
  float espacio_entre_func = rango_total/num_fun; 
  vector<float> m; //Valor donde se coloca la funcion de membresia. 
  m.push_back(init_salida_val);
  
  float init_salida = init_salida_val; 
  for (int i = 0; i < num_fun; i++)
  {
    init_salida = init_salida + espacio_entre_func; 
    m.push_back(init_salida); 
  }
  
//Funciones de membresia INPUT1: 
  Func_meb fun1("NB", "exp", init_time, 0.03); 
  Func_meb fun2("NM", "exp", 10.13, 0.04);
  Func_meb fun3("NS", "exp", 17.51, 0.04);
  Func_meb fun4("Z", "exp", k, 0.09);
  Func_meb fun5("PS", "exp", 31.9, 0.05);
  Func_meb fun6("PM", "exp", 30.85, 0.04);
  Func_meb fun7("PB", "exp", finish_time, 0.03);
  Func_meb arreglo[] = { fun1, fun2, fun3, fun4, fun5, fun6, fun7};
  std::vector<Func_meb> funciones_memb;
  for (auto fun : arreglo) {
      funciones_memb.push_back(fun);
      fun.mostrar_valores(); 
  }

  //ERROR func_membr: 
  Func_meb fun8("NB", "exp", init_time2, 0.004);
  Func_meb fun9("NS", "exp", finish_time2, 0.004);
  Func_meb fun18("Z", "exp", 0, 0.1);
  Func_meb error_arreglo[] = { fun8, fun9, fun18};
  std::vector<Func_meb> error_func_membr;
  for (auto fun : error_arreglo) {
      error_func_membr.push_back(fun);
      //fun.mostrar_valores(); 
  }

  //SALIDA funcion membre:  
    Func_meb fun10("NB", "exp", m[0], 6);
    Func_meb fun11("NM", "exp", m[1], 5);
    Func_meb fun12("NS", "exp", m[2], 5);
    Func_meb fun13("Z",  "exp", m[3], 5);
    Func_meb fun14("PS", "exp", m[4], 5);
    Func_meb fun15("PM", "exp", m[5], 5);
    Func_meb fun16("PB", "exp", m[6], 5);
    Func_meb salida_arreglo[] = { fun10, fun11, fun12, fun13, fun14, fun15, fun16 };
    for (auto fun : salida_arreglo) {
        salida_func_membr.push_back(fun);
    }

  //Hecho funciones membr: 
  Func_meb fun17("Hecho", "exp", 25, 0.1);
  Func_meb hecho_arreglo[] = {fun17};
  std::vector<Func_meb> hecho_func_membr;
  for (auto fun : hecho_arreglo) {
      hecho_func_membr.push_back(fun);
  }

  //Inicia el objeto de la clase Grafica y prepara los valores para crear la grafica de 
  //entrada 1.
  vector<float> time = tiempo(init_time, finish_time , 0.01);
  vector<float> y_values(time.size(), 0);
  char Input[10] = "Input1";
  dist_values_map = build_graph(Input, funciones_memb, time, y_values);

  ////Hecho: 
  char Nombre_hecho[10] = "Hecho";
  time.clear(); 
  y_values.clear(); 
  time = tiempo(init_time, finish_time , 0.1); 
  for (int i = 0; i < time.size(); i++)
  {
      y_values.push_back(0); 
  }
  hecho_values_map = build_graph(Nombre_hecho, hecho_func_membr, time, y_values);

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
  vector<float> time2 = error_xval(init_salida_val, finish_salida_val);
  for (int i = 0; i < time.size(); i++)
  {
      y_values.push_back(0);
  }
  char Nombre_salida[10] = "salida";
  salida_values_map = build_graph(Nombre_salida, salida_func_membr, time2, y_values);

  /// PWM resolucion, freq, canal setup. 
  ledcSetup(canal0, freq, resolucion); 
  ledcAttachPin(motor, canal0); 

  // Sensor_Ultrasonico: 
  pinMode(Trig, OUTPUT); 
  pinMode(Echo, INPUT); 
}

//////////  --- LOOP --- //// ///////////// --------
void loop()
{ 

  std::vector<float> fuzzy_val_dist; 
  std::vector<float> fuzzy_val_error;
  std::vector<float> fuzzy_val_hecho;
  std::vector<float> kj; 
  std::vector<float> alpha_j; 

  //INPUT: 
  float distancia = calc_dist(1);
  float error = k - distancia; 

  //FUZZIFICAR la distancia:
  //Toma el par de valores en la posicion i y la asigna a item, para acceder a el array de valor de <y> 
  //se usa el el puntero item->second.
  fuzzy_val_dist = fuzzy_input(dist_values_map, distancia, 2.5, 100);  
  fuzzy_val_error = fuzzy_input(error_values_map, error, -21.5, 100); 
  fuzzy_val_hecho = fuzzy_input(hecho_values_map, distancia, 2.5, 10); 

  //Calculo de KJ:
  for(auto i: fuzzy_val_dist){
    for(auto j: fuzzy_val_error){
      float kj_local = min(i,j); 
      kj.push_back(kj_local);   
    }
  } 

  Serial.println("Here"); 
  // //Calculo del grado de consistencia 
  // for(auto i: kj){
  //   alpha_j.push_back(max(min(fuzzy_val_hecho[0], i))); 
  // }

  //Defuzzificacion:
  float num_defuzzy = 0; 
  float den_defuzzy = 0; 
  vector<float> mult_kj_cj;
  //Los centroides estan colocados de la forma: 
  // NB and NB -> NB, NB and PB -> NB, NB and Z -> NB,  
  // NM and NB -> NB, NM and PB -> NB , NM and Z -> NB
  vector<float> cj = {
    //ERROR: NB                     //NM                    //Z
    salida_func_membr[0].m , salida_func_membr[0].m, salida_func_membr[0].m,
    salida_func_membr[0].m, salida_func_membr[0].m,  salida_func_membr[0].m,
    salida_func_membr[2].m, salida_func_membr[2].m,  salida_func_membr[0].m,
    salida_func_membr[3].m, salida_func_membr[3].m,  salida_func_membr[3].m,
    salida_func_membr[4].m, salida_func_membr[4].m,  salida_func_membr[4].m,
    salida_func_membr[5].m, salida_func_membr[5].m, salida_func_membr[5].m,
    salida_func_membr[6].m, salida_func_membr[6].m,  salida_func_membr[6].m,
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
  //Conversion del volt salida en el duty cycle del pwm.  
  int duty_cycle = 83.697 * v_out + 62.775 ; 

  ledcWrite(canal0, duty_cycle); 
  Serial.println("duty_cycle"); 
  Serial.println(duty_cycle); 
}