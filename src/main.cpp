#include <Arduino.h>
#include <map>
#include <vector>
#include<string>
#include<Func_meb.h>
#include<functions.h>

//Pagina web depandencias: 
#include<WiFi.h>
#include<PubSubClient.h>

#define motor GPIO_NUM_27 //PWM 

using namespace std;

//Declaracion de una tarea que corre paralelamente a el loop(): 
//TaskHandle_t Task1; 

//Parametros de red: 
const char *ssid = "RS_NETWORK_2_2.4GHZ"; //Nombre Red 
const char *password = "rsautomation2017";  //Contraseña
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "esp32/test";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

/// Variables que guardan los valores de pertenecia de las funciones de membr: 
std::map<int, std::vector<float>> salida_values_map; 
std::map<int, std::vector<float>> error_values_map; 
std::map<int, std::vector<float>> dist_values_map; 
std::vector<Func_meb> salida_func_membr;

/// Configuraciones del PWM asignado a el motor: 
const float k = 27; 
const int freq = 1000; //HZ
const int canal0 = 0; 
const int resolucion = 10; //bit
float tiempo_inicio = millis();  
int duty_cycle = 0; 

void callback(char *topic, byte *payload, unsigned int length) {
 Serial.print("Message arrived in topic: ");
 Serial.println(topic);
 Serial.print("Message:");
 for (int i = 0; i < length; i++) {
     Serial.print((char) payload[i]);
 }
 Serial.println();
 Serial.println("-----------------------");
}

void setup() //////// Setup ////////////
{
  Serial.begin(115200);
  //WIFI begin: 
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500); 
    Serial.print("Connecting to Wifi: ");
    Serial.println(ssid);
  }
  
  //CONNECION A EL BROKER: 
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
        Serial.println("Public emqx mqtt broker connected");
    } else {
        Serial.print("failed with state ");
        Serial.print(client.state());
        delay(2000);
    }
  }
  
  //PUB and Sub: 
  client.publish(topic, "Message from ESP32"); 
  client.subscribe(topic); 

  Serial.println("Cargando funciones de membresia: "); 
  //Inicio del tiempo: 
  float init_time = 2.5; 
  float finish_time = 48.5;

  float init_time2 = k - finish_time; 
  float finish_time2 = k - init_time; 

  //Salida configuraciones: 
  float init_salida_val = 4; 
  float finish_salida_val = 10;

  //Funcion para automatizar el desplieuge de las graficas: 
  vector<float> time = tiempo(init_time, finish_time , 0.01);
  vector<float> y_values(time.size(), 0);
  
  float rango_total = finish_salida_val - init_salida_val; 
  int num_fun = 7; 
  float Espacio_funciones = rango_total/num_fun; 
  vector<float> m; 
  m.push_back(init_salida_val);
  float init_salida = init_salida_val; 
  
  for (int i = 0; i < num_fun; i++)
  {
    init_salida = init_salida + Espacio_funciones; 
    m.push_back(init_salida); 
  }
  

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
    Func_meb fun10("NB", "exp", m[0], 4);
    Func_meb fun11("NM", "exp", m[1], 4);
    Func_meb fun12("NS", "exp", m[2], 4);
    Func_meb fun13("Z", "exp", m[3], 4);
    Func_meb fun14("PS", "exp", m[4], 4);
    Func_meb fun15("PM", "exp", m[5], 4);
    Func_meb fun16("PB", "exp", m[6], 4);
    Func_meb salida_arreglo[] = { fun10, fun11, fun12, fun13, fun14, fun15, fun16 };
    
    for (auto fun : salida_arreglo) {
        salida_func_membr.push_back(fun);
        //fun.mostrar_valores(); 
    }

  //Inicia el objeto de la clase Grafica y prepara los valores para crear la grafica de 
  //entrada 1.
  char Input[10] = "Input1";
  dist_values_map = build_graph(Input, funciones_memb, time, y_values);


  /////---- Graph error 2:  
  time.clear(); 
  y_values.clear(); 
  time = error_xval(init_time2, finish_time2);
  for (int i = 0; i < time.size(); i++)
  {
      y_values.push_back(0); 
  }
  char Nombre[10] = "Error"; 
  error_values_map = build_graph(Nombre, error_func_membr, time, y_values);
  

  ////// -----  Salida:
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

//////  --- LOOP --- //// ///////////// --------
void loop()
{ 
  client.loop();  
  
  std::vector<float> fuzzy_val_dist; 
  std::vector<float> fuzzy_val_error;
  std::vector<float> kj; 

  //INPUT: 
  float distancia = calc_dist(5);
  float error = k - distancia; 

  //FUZZIFICAR la distancia:
  //Toma el par de valores en la posicion i y la asigna a item, para acceder a el array de valor de <y> 
  //se usa el el puntero item->second.
  fuzzy_val_dist = fuzzy_input(dist_values_map, distancia, 2.5);  
  fuzzy_val_error = fuzzy_input(error_values_map, error, -21.5); 

  //Calculo de KJ:
  for(auto i: fuzzy_val_dist){
    for(auto j: fuzzy_val_error){
      kj.push_back(min(i,j));   
    }
  } 


  //Calculo del grado de consistencia 

  //Defuzzificacion:
  float num_defuzzy = 0; 
  float den_defuzzy = 0; 
  vector<float> mult_kj_cj;
  //Los centroides estan colocados de la forma: 
  // NB and NB -> NB, NB and PB -> NB 
  // NM and NB -> NM, NM and PB -> NM 
  // NS and NB -> NS, NS and PB -> NS 
  vector<float> cj = {
    salida_func_membr[0].m , salida_func_membr[0].m,
    salida_func_membr[0].m, salida_func_membr[0].m,
    salida_func_membr[3].m, salida_func_membr[3].m,
    salida_func_membr[3].m, salida_func_membr[3].m, 
    salida_func_membr[4].m, salida_func_membr[4].m, 
    salida_func_membr[5].m, salida_func_membr[5].m,
    salida_func_membr[6].m, salida_func_membr[6].m
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

  float tiempo_fijo = millis(); 
}
