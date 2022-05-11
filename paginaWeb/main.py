from base64 import decode
from calendar import c
import json
from pickle import GLOBAL, TRUE
from unicodedata import name
from flask import Flask, render_template
from flask_mqtt import Mqtt
from flask_socketio import SocketIO, send, emit
from matplotlib.font_manager import json_load
import numpy as np 
from Fis_mamdami import Fis

app = Flask(__name__)
app.config['MQTT_BROKER_URL'] = 'broker.emqx.io'  # use the free broker from HIVEMQ
app.config['MQTT_BROKER_PORT'] = 1883  # default port for non-tls connection
app.config['MQTT_KEEPALIVE'] = 5  # set the time interval for sending a ping to the broker to 5 seconds
app.config['MQTT_TLS_ENABLED'] = False  # set TLS to disabled for testing purposes


mqtt = Mqtt(app)
socketio = SocketIO(app)

init = False
payload = 0 


@app.route('/')
def index():
    return render_template('index.html')

@socketio.on('message')
def handle_subscribe(json_str):
    print("MESSAGE: " + json_str)

@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc): 
    print("Conectado a esp32")
    mqtt.subscribe("esp32/test")

@mqtt.on_message()
def handle_mqtt_message(client, userdata, message):
    global init
    global payload
    global distancia 
    global error
    global stop
    k = 25

    data = dict(
        topic=message.topic,
        payload=message.payload.decode()
    )   

    while(init == False): 
        print("Hello World from the begining")
        start = 2.5;  stop = 48.5
        rango = np.arange(start=start, stop=stop, step=0.001)

        #Funcion para automatizar despliegue de Func
        rango_total = stop - start 
        num_fun = 7
        Espacio_funciones = rango_total/num_fun 
        b = []
        b.append(start)
        for i in range(0, num_fun): 
            start = start + Espacio_funciones
            print(start)
            b.append(start)
            
        #Funciones de mebresias: 
        disancia_func_membr = [
            {'nombre': 'NB', 'tipo': 'exp', 'm': 2.5, 'k': 0.03},
            {'nombre': 'NM', 'tipo': 'exp', 'm': 10.13, 'k': 0.04}, 
            {'nombre': 'NS', 'tipo': 'exp', 'm': 17.51, 'k': 0.04},
            {'nombre': 'Z', 'tipo': 'exp', 'm': k, 'k': 0.09},
            {'nombre': 'PS', 'tipo': 'exp', 'm': 31.9, 'k': 0.05},
           {'nombre': 'PM', 'tipo': 'exp', 'm': 39, 'k': 0.04},
            {'nombre': 'PB', 'tipo': 'exp', 'm': 48.5, 'k': 0.03},
        ]

        distancia = Fis(name = "Input 1", rango = rango, func_membr = disancia_func_membr)
        distancia_dicc = distancia.build_function()

        # ERROR Funciones Membr: 
        start = k - stop;  stop = k - 2.5
        rango2 = np.arange(start=start, stop=stop, step=0.001)

        error_func_membr = [
            {'nombre': 'NB', 'tipo': 'exp', 'm': start, 'k': 0.004},
            {'nombre': 'NS', 'tipo': 'exp', 'm': stop, 'k': 0.004}, 
            {'nombre': 'Z', 'tipo': 'exp', 'm': 0, 'k': 0.1},
        ]
        error = Fis(name = "Error", rango = rango2, func_membr = error_func_membr)
        error_dic = error.build_function() 

        # SALIDA Funciones Membr: 
        start = 4;  stop = 10
        rango3 = np.arange(start=start, stop=stop, step=0.001)

        salida_func_membr = [
            {'nombre': 'NB', 'tipo': 'exp', 'm': b[0], 'k': 6},
            {'nombre': 'NM', 'tipo': 'exp', 'm': b[1], 'k': 5}, 
            {'nombre': 'NS', 'tipo': 'exp', 'm': b[2], 'k': 5},
            {'nombre': 'Z', 'tipo': 'exp', 'm': b[3], 'k': 5},
            {'nombre': 'PS', 'tipo': 'exp', 'm': b[4], 'k': 5},
            {'nombre': 'PM', 'tipo': 'exp', 'm': b[5], 'k': 5},
            {'nombre': 'PB', 'tipo': 'exp', 'm': b[6], 'k': 5},
        ]
        salida = Fis(name = "salida", rango = rango3, func_membr = salida_func_membr)
        saldia_dic = salida.build_function() 

        #Centroides cj: 
        cj = [
            b[0], b[0], b[0],
            b[0], b[0], b[0],  
            b[2], b[2], b[0],
            b[3], b[3], b[3],
            b[4], b[4], b[4],
            b[5], b[5], b[5],
            b[6], b[6], b[6]
        ]
        print("Here")
        init = True

    

    print(f'Payload: {float(data["payload"])}')
    payload_float = float(data['payload'])
    # -- Inputs: 
    if payload_float > 2.5 and payload_float < 48.5:
        print("Payload: " + f'{payload_float}') 
        distancia_input = payload_float
        error_intput = k - distancia_input 
        #Calculos de los valores de pertenencia: 
        fuzzy_input = distancia.belonging_value(2.5, distancia_input)
        fuzzy_error = error.belonging_value(k - stop, error_intput)
        print(f'Valores de pertenencia distancia: {fuzzy_input}')
        print(f'Valores de pertenencia error_intput: {fuzzy_error}')

        #Calculos de las KJ:
        kj = [] 
        for x1 in fuzzy_input: 
            for x2 in error_intput: 
                kj.append(np.minimum(x1, x2)) 
        print(f'Tamano de kj: {len(kj)}')

        #Mult de los centroides con las kj:
        mult_kj = [] 
        for i, j in zip(kj, cj): 
            mult_kj.append(i * j)
        print(f'Tamano de mult_kj: {len(mult_kj)}')

        #Sumatoria de la multiplicacion de las kj y cj: 
        numerador = sum(mult_kj)
        #Sumatoriad de las kj: 
        sum_kj = sum(kj)

        Resultado = numerador/sum_kj
        print(f'Resultado: {Resultado}')
        
    # socketio.emit('mqtt_message', data=data)


@mqtt.on_log()
def handle_logging(client, userdata, level, buf):
    print(level, buf)
    

if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', port=5000, use_reloader=False, debug=True)
