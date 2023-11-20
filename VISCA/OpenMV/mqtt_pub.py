# MQTT Example.
# This example shows how to use the MQTT library to publish to a topic.
#
# 1) Copy the mqtt.py library to OpenMV storage.
# 2) Run this script on the OpenMV camera.
# 3) Install the mosquitto client on PC and run the following command:
#    mosquitto_sub -h test.mosquitto.org -t "openmv/test" -v
#
# NOTE: If the mosquitto broker is unreachable, try another broker (For example: broker.hivemq.com)
import time, network
from mqtt import MQTTClient
import random

SSID='SII-Cikunir' # Network SSID
KEY='admin.admin'  # Network key
client_id = 'python-mqtt-'+str(random.randint(0, 1000))
broker = '104.248.156.51'
user = 'ali'
pw = '1234' # Init wlan module and connect to network
print("Trying to connect... (may take a while)...")

wlan = network.WLAN(network.STA_IF)
wlan.deinit()
wlan.active(True)
wlan.connect(SSID, KEY, timeout=30000)

# We should have a valid IP now via DHCP
print(wlan.ifconfig())

client = MQTTClient('openmvvggvui', broker, port=1883, user=user, password=pw,keepalive=60)
client.connect()

while (True):
    client.publish("openmv/test", "Hello World!")
    time.sleep_ms(1000)
