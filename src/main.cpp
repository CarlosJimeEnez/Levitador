#include <Arduino.h>
#include <map>
#include <vector>
#include<string>
#include<Func_meb.h>
#include<functions.h>

//Pagina web depandencias: 
#include<WiFi.h>
#include<WebServer.h>
#include<WebSocketsServer.h>
#include<submit.h>
#include<control.h>
// #include <index.h>

#define motor GPIO_NUM_27 //PWM 

using namespace std;

//Declaracion de una tarea que corre paralelamente a el loop(): 
//TaskHandle_t Task1; 

///Credenciales del punto de acceso:
//Parametros de red: 
String ip ;
IPAddress ipStatic(192,168,100,82);
IPAddress ipGateway(192,168,0,1);
IPAddress subnet(255,255,255,0); 
const char* ssid = "Levitador"; //Nombre del punto de acceso.  
char* password;
char* nombre; 
 
WebServer server(80); 
WebSocketsServer webSocket(81); 
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

//Creacion de un punto de acceso: 
void ap_mode(){
  WiFi.softAP(ssid); 
  IPAddress miIP = WiFi.softAPIP(); //
  ip = miIP.toString(); 
  Serial.println("IP APoint"); 
  Serial.println(miIP);
  Serial.println(WiFi.localIP()); 
}

void raiz(){
  String ip_AP = "192.168.4.1"; 
  if(ip == ip_AP){
    server.send(200, "text/html", submit);  
    Serial.println("Submit");
  }else{ 
    server.send(200,"text/html", control);
    Serial.println("Control root"); 
  }
}

//Websockets functions: 
//Manejo de las peticiones de websockets: 
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t welength)
{
  String payloadString = (char *)payload;
  
  if(type == WStype_TEXT) 
  {
    // Que tipo de conexion es: 
    switch(type) {

      // Client has disconnected
      case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;

      // New client has connected
      case WStype_CONNECTED:
        {
          IPAddress ip = webSocket.remoteIP(num);
          Serial.printf("[%u] Connection from ", num);
          Serial.println(ip.toString());
        }
        break;

      // Echo text message back to client
      case WStype_TEXT:
        { 
          Serial.printf("[%u] Text: %s\n", num, payload);      
          char str[30]; //Usado para dividir cadena de entrada del websocket. 
          for (int i = 0; i < payloadString.length(); i++)
          {
            str[i] = {payloadString[i]};
          }  

          nombre = strtok(str, ","); 
          password = strtok(NULL, ""); 

          //Intenta conectarse a la red: 
          Serial.printf("nombre: %s, contraseña: %s",nombre, password); 
          WiFi.config(ipStatic, ipGateway, subnet);
          WiFi.begin(nombre, password); 
          Serial.print("\n\r ....");
          int time_finish = 0; 
          unsigned long int actual_time = millis();
          unsigned long int prev_time = millis();
          while ((WiFi.status() != WL_CONNECTED) && (time_finish != 1)){ 
            actual_time = millis();
            delay(200);
            Serial.println("No connected");
            if(actual_time - prev_time > 13000){
                prev_time = actual_time;
                time_finish = 1;
                Serial.println("Acabo el tiempo"); 
            } 
          }
          //No se pudo conectar: 
          if(WiFi.status() != WL_CONNECTED){
            Serial.println("No se pudo conectar"); 
            WiFi.disconnect(); 
          }
          //Se conecto: 
          else{
            Serial.println("Wifi conectada en la ip 192.168.100.82");  
            ip = "192.168.100.82"; 
          }
        }
        break;

        // For everything else: do nothing
      case WStype_BIN:
      case WStype_ERROR:
      case WStype_FRAGMENT_TEXT_START:
      case WStype_FRAGMENT_BIN_START:
      case WStype_FRAGMENT:
      case WStype_FRAGMENT_FIN:
      default:
      break;  
    }
  }
}


/////////////////////

// void loop2(void *args){
//   while (true)
//   {
//     Serial.println("\t \t \t Se esta corriendo en el nucleo" + String(xPortGetCoreID()));
//     delay(100);  
//   }
//   vTaskDelay(10); 
// }

void web_setup(){
  WiFi.mode(WIFI_AP_STA); 
  WiFi.softAP(ssid); 
  ap_mode();
  //Manejo de las peticiones: 
  server.on("/", raiz); 
  server.begin(); 
  webSocket.begin();
  //Se asigna la funcion de llegada de datos de webserber:  
  webSocket.onEvent(webSocketEvent); 
}

void setup() //////// Setup ////////////
{
  // //Inicio de una tarea en paralelo: 
  // xTaskCreatePinnedToCore(
  //   loop2,
  //   "Task2", 
  //   1000,
  //   NULL, 
  //   1, //Prioridad 
  //   &Task1, 
  //   0
  // );

  Serial.begin(9600);
  web_setup(); 
//   Serial.println("Cargando funciones de membresia: "); 
//   //Inicio del tiempo: 
//   float init_time = 2.5; 
//   float finish_time = 48.5;

//   float init_time2 = k - finish_time; 
//   float finish_time2 = k - init_time; 

//   //Salida configuraciones: 
//   float init_salida_val = 4; 
//   float finish_salida_val = 10;

//   //Funcion para automatizar el desplieuge de las graficas: 
//   vector<float> time = tiempo(init_time, finish_time , 0.01);
//   vector<float> y_values(time.size(), 0);
  
//   float rango_total = finish_salida_val - init_salida_val; 
//   int num_fun = 7; 
//   float Espacio_funciones = rango_total/num_fun; 
//   vector<float> m; 
//   m.push_back(init_salida_val);
//   float init_salida = init_salida_val; 
  
//   for (int i = 0; i < num_fun; i++)
//   {
//     init_salida = init_salida + Espacio_funciones; 
//     m.push_back(init_salida); 
//   }
  

// //Funciones de membresia INPUT1: 
//   Func_meb fun1("NB", "exp", 2.5, 0.03); 
//   Func_meb fun2("NM", "exp", 10.13, 0.04);
//   Func_meb fun3("NS", "exp", 19.71, 0.04);
//   Func_meb fun4("Z", "exp", 27, 0.09);
//   Func_meb fun5("PS", "exp", 33.6, 0.05);
//   Func_meb fun6("PM", "exp", 40.87, 0.04);
//   Func_meb fun7("PB", "exp", 48.5, 0.03);
//   Func_meb arreglo[] = { fun1, fun2, fun3, fun4, fun5, fun6, fun7};

//   std::vector<Func_meb> funciones_memb;
//   for (auto fun : arreglo) {
//       funciones_memb.push_back(fun);
//       fun.mostrar_valores(); 
//   }

//   //ERROR func_membr: 
//   Func_meb fun8("NB", "exp", -21.5, 0.004);
//   Func_meb fun9("NS", "exp", 24.5, 0.004);
//   Func_meb error_arreglo[] = { fun8, fun9 };

//   std::vector<Func_meb> error_func_membr;
//   for (auto fun : error_arreglo) {
//       error_func_membr.push_back(fun);
//       //fun.mostrar_valores(); 
//   }

//   //SALIDA funcion membre:  
//     Func_meb fun10("NB", "exp", m[0], 4);
//     Func_meb fun11("NM", "exp", m[1], 4);
//     Func_meb fun12("NS", "exp", m[2], 4);
//     Func_meb fun13("Z", "exp", m[3], 4);
//     Func_meb fun14("PS", "exp", m[4], 4);
//     Func_meb fun15("PM", "exp", m[5], 4);
//     Func_meb fun16("PB", "exp", m[6], 4);
//     Func_meb salida_arreglo[] = { fun10, fun11, fun12, fun13, fun14, fun15, fun16 };
    
//     for (auto fun : salida_arreglo) {
//         salida_func_membr.push_back(fun);
//         //fun.mostrar_valores(); 
//     }

//   //Inicia el objeto de la clase Grafica y prepara los valores para crear la grafica de 
//   //entrada 1.
//   char Input[10] = "Input1";
//   dist_values_map = build_graph(Input, funciones_memb, time, y_values);


//   /////---- Graph error 2:  
//   time.clear(); 
//   y_values.clear(); 
//   time = error_xval(init_time2, finish_time2);
//   for (int i = 0; i < time.size(); i++)
//   {
//       y_values.push_back(0); 
//   }
//   char Nombre[10] = "Error"; 
//   error_values_map = build_graph(Nombre, error_func_membr, time, y_values);
  

//   ////// -----  Salida:
//   time.clear();
//   y_values.clear(); 
//   vector<float> time2 = error_xval(init_salida_val, finish_salida_val);
//   for (int i = 0; i < time.size(); i++)
//   {
//       y_values.push_back(0);
//   }

//   char Nombre_salida[10] = "salida";
//   salida_values_map = build_graph(Nombre_salida, salida_func_membr, time2, y_values);

//   /// PWM resolucion, freq, canal setup. 
//   ledcSetup(canal0, freq, resolucion); 
//   ledcAttachPin(motor, canal0); 

//   // Sensor_Ultrasonico: 
//   pinMode(Trig, OUTPUT); 
//   pinMode(Echo, INPUT); 
}

//////  --- LOOP --- //// ///////////// --------
void loop()
{ 
  webSocket.loop(); 
  server.handleClient(); 

  // std::vector<float> fuzzy_val_dist; 
  // std::vector<float> fuzzy_val_error;
  // std::vector<float> kj; 

  // //INPUT: 
  // float distancia = calc_dist(5);
  // float error = k - distancia; 

  // //FUZZIFICAR la distancia:
  // //Toma el par de valores en la posicion i y la asigna a item, para acceder a el array de valor de <y> 
  // //se usa el el puntero item->second.
  // fuzzy_val_dist = fuzzy_input(dist_values_map, distancia, 2.5);  
  // fuzzy_val_error = fuzzy_input(error_values_map, error, -21.5); 

  // //Calculo de KJ:
  // for(auto i: fuzzy_val_dist){
  //   for(auto j: fuzzy_val_error){
  //     kj.push_back(min(i,j));   
  //   }
  // } 


  // //Calculo del grado de consistencia 

  // //Defuzzificacion:
  // float num_defuzzy = 0; 
  // float den_defuzzy = 0; 
  // vector<float> mult_kj_cj;
  // //Los centroides estan colocados de la forma: 
  // // NB and NB -> NB, NB and PB -> NB 
  // // NM and NB -> NM, NM and PB -> NM 
  // // NS and NB -> NS, NS and PB -> NS 
  // vector<float> cj = {
  //   salida_func_membr[0].m , salida_func_membr[0].m,
  //   salida_func_membr[0].m, salida_func_membr[0].m,
  //   salida_func_membr[3].m, salida_func_membr[3].m,
  //   salida_func_membr[3].m, salida_func_membr[3].m, 
  //   salida_func_membr[4].m, salida_func_membr[4].m, 
  //   salida_func_membr[5].m, salida_func_membr[5].m,
  //   salida_func_membr[6].m, salida_func_membr[6].m
  // }; 

  // for (size_t i = 0; i < kj.size(); i++)
  // {
  //   float mult_num = kj[i] * cj[i]; 
  //   mult_kj_cj.push_back(mult_num); 
  // }
  
  // for(auto i: mult_kj_cj){
  //   num_defuzzy += i; 
  // }
  
  // //Denomiandor: 
  // for(auto i: kj){
  //   den_defuzzy += i; 
  // }


  // float v_out = num_defuzzy/den_defuzzy; 
  // Serial.println("v_out"); 
  // Serial.println(v_out); 
  // //Conversion del volt salida en el duty cycle del pwm.  
  // int duty_cycle = 83.697 * v_out + 62.775 ; 

  // ledcWrite(canal0, duty_cycle); 
  // Serial.println("duty_cycle"); 
  // Serial.println(duty_cycle); 

  // float tiempo_fijo = millis(); 
}
