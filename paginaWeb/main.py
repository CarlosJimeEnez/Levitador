import json
from flask import Flask, render_template
from flask_mqtt import Mqtt
from flask_socketio import SocketIO, send, emit
from matplotlib.font_manager import json_load


app = Flask(__name__)
app.config['MQTT_BROKER_URL'] = 'broker.emqx.io'  # use the free broker from HIVEMQ
app.config['MQTT_BROKER_PORT'] = 1883  # default port for non-tls connection
app.config['MQTT_KEEPALIVE'] = 5  # set the time interval for sending a ping to the broker to 5 seconds
app.config['MQTT_TLS_ENABLED'] = False  # set TLS to disabled for testing purposes


mqtt = Mqtt(app)
socketio = SocketIO(app)


@app.route('/')
def index():
    return render_template('index.html')


@socketio.on('publish')
def handle_publish(json_str):
    #data = json.loads(json_str)
    #mqtt.publish(data['topic'], data['message'])
    pass


@socketio.on('message')
def handle_subscribe(json_str):
    print("MESSAGE: " + json_str)


@socketio.on('unsubscribe_all')
def handle_unsubscribe_all():
    mqtt.unsubscribe_all()

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
