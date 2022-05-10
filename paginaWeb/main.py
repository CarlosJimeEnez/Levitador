import json
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

print("Hello World from the begining")
start = 2.5;  stop = 48.5
rango = np.arange(start=start, stop=stop, step=0.001)
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
func_memb = [
    {'nombre': 'Baja', 'tipo': 'exp', 'm': b[0], 'k': 1},
    {'nombre': 'Media', 'tipo': 'exp', 'm': b[1], 'k': 0.1}, 
#     {'nombre': 'Optima', 'tipo': 'exp', 'm': b[2], 'k': Espacio_funciones},
#     {'nombre': 'MediaAlta', 'tipo': 'exp', 'm': b[3], 'k': Espacio_funciones},
#     {'nombre': 'Alta', 'tipo': 'exp', 'm': b[4], 'k': Espacio_funciones},
#    {'nombre': 'SuperAlta', 'tipo': 'exp', 'm': b[5], 'k': Espacio_funciones},
#     {'nombre': 'UltraAlta', 'tipo': 'exp', 'm': b[6], 'k': Espacio_funciones},
]

object = Fis(name = "Input 1", rango = rango, func_membr = func_memb)
humedad_functions = object.build_function()


@app.route('/')
def index():
    return render_template('index.html')

@socketio.on('message')
def handle_subscribe(json_str):
    print("MESSAGE: " + json_str)

@mqtt.on_connect()
def handle_connect(client, userdata, flags, rc): 
    mqtt.subscribe("esp32/test")

@mqtt.on_message()
def handle_mqtt_message(client, userdata, message):
    # data = dict(
    #     topic=message.topic,
    #     payload=message.payload.decode()
    # )
    
    print("Topic:  " + message.topic)
    print(message)
    # socketio.emit('mqtt_message', data=data)
    return 200

@mqtt.on_log()
def handle_logging(client, userdata, level, buf):
    print(level, buf)
   

if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', port=5000, use_reloader=False, debug=True)
