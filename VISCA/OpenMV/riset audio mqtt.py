import image, audio, time, network
from mqtt import MQTTClient
import random


SSID='SII-Cikunir' # Network SSID
KEY='admin.admin'  # Network key
client_id = 'python-mqtt-'+str(random.randint(0, 1000))
broker = '104.248.156.51'
user = 'ali'
pw = '1234' # Init wlan module and connect to network
audio.init(channels=1, frequency=16000, gain_db=20, highpass=0.9883)

print("Trying to connect... (may take a while)...")

wlan = network.WLAN(network.STA_IF)
wlan.deinit()
wlan.active(True)
wlan.connect(SSID, KEY, timeout=30000)

# We should have a valid IP now via DHCP
print(wlan.ifconfig())

client = MQTTClient(client_id, broker, port=1883, user=user, password=pw,keepalive=60)
client.connect()

def audio_callback(buf):
    try:
        global fout
        b= b'Hello World'
        my_byte_array = b.decode('utf-8')
        print(my_byte_array)
        #client.publish("byte/array", my_byte_array)
    except Exception as e:
        print("exption:", e)
# Start audio streaming
audio.start_streaming(audio_callback)
# do something else
print("recording audio...")
start = time.ticks_ms()
while ((time.ticks_ms() - start) < 5000):
    time.sleep_ms(100)

# Stop streaming
audio.stop_streaming()
