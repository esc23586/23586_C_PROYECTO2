# Import standard python modules.
import sys
import time
import serial

# This example uses the MQTTClient instead of the REST client
from Adafruit_IO import MQTTClient
#from Adafruit_IO import Client, Feed

# holds the count for the feed
#run_count = 0

# Set to your Adafruit IO username and key.
# Remember, your key is a secret,
# so make sure not to publish it when you publish this code!
ADAFRUIT_IO_USERNAME = "Grett2026"
ADAFRUIT_IO_KEY = "aio_dJAC71qUNsYRzX3P7ot2eKS14Z3W"

#modificaciones===============
# =====================================
# SERIAL
# =====================================

ser = serial.Serial('COM3', 9600)

time.sleep(2)
#=======================
# Set to the ID of the feed to subscribe to for updates.
#FEED_ID_receive = 'Counter_TX'
#FEED_ID_Send = 'Counter_RX'

# Define "callback" functions which will be called when certain events 
# happen (connected, disconnected, message arrived).
def connected(client):
    """Connected function will be called when the client is connected to
    Adafruit IO.This is a good place to subscribe to feed changes. The client
    parameter passed to this function is the Adafruit IO MQTT client so you
    can make calls against it easily.
    """
    # Subscribe to changes on a feed named Counter.
    #print('Subscribing to Feed {0}'.format(FEED_ID_receive))
    #client.subscribe(FEED_ID_receive)
    #print('Waiting for feed data...')
    print("Conectado a Adafruit IO")

    client.subscribe("ojos")
    client.subscribe("ojo-d")

    client.subscribe("base-ojo-iz")
    client.subscribe("base-ojo-d")

    client.subscribe("parpado-iz")
    client.subscribe("parpado-d")

def disconnected(client):
    """Disconnected function will be called when the client disconnects."""
    print("Desconectado")
    sys.exit(1)

def message(client, feed_id, payload):
    """Message function will be called when a subscribed feed has a new value.
    The feed_id parameter identifies the feed, and the payload parameter has
    the new value.
    """
    #print('Feed {0} received new value: {1}'.format(feed_id, payload))
    # Publish or "send" message to corresponding feed
    #print('Sendind data back: {0}'.format(payload))
    #client.publish(FEED_ID_Send, payload)
    print("Feed:", feed_id)
    print("Valor:", payload)

    comando = ""

    # =====================================
    # CONVERTIR FEEDS A COMANDOS UART
    # =====================================
    if feed_id == "ojos":
        comando = f"I2;{payload};"

    elif feed_id == "ojo-d":
        comando = f"D2;{payload};"

    elif feed_id == "base-ojo-iz":
        comando = f"I1;{payload};"

    elif feed_id == "base-ojo-d":
        comando = f"D1;{payload};"

    elif feed_id == "parpado-iz":
        comando = f"M1;{payload};"

    elif feed_id == "parpado-d":
        comando = f"M2;{payload};"    

    # =====================================
    # ENVIAR UART
    # =====================================

    if comando != "":

        print("Enviando:", comando)
        print(repr(comando))# me permite ver qué es lo que ando mandando
        ser.write(comando.encode())


# Create an MQTT client instance.
client = MQTTClient(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY) #modificaaaaaaar con mi clave y usuario

# Setup the callback functions defined above.
client.on_connect = connected
client.on_disconnect = disconnected
client.on_message = message

# Connect to the Adafruit IO server.
client.connect()

# The first option is to run a thread in the background so you can continue
# doing things in your program.
client.loop_background()

while True:
    """ 
    # Uncomment the next 3 lines if you want to constantly send data
    # Adafruit IO is rate-limited for publishing
    # so we'll need a delay for calls to aio.send_data()
    run_count += 1
    print('sending count: ', run_count)
    client.publish(FEED_ID_Send, run_count)
    """
    #print('Running "main loop" ')

    time.sleep(1)
