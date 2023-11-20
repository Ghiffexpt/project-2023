import socket,network,audio,time

SSID='Workshop-Cikunir'      # Network SSID
KEY='admin.admin'       # Network key
HOST ='172.16.3.64'     # Use first available interface
PORT = 8080  # Arbitrary non-privileged port
CHANNELS = 1
SIZE = 256//(2*CHANNELS)

audio.init(channels=1, frequency=16000, gain_db=50, highpass=0.9883)

raw_buf = None
# Init sensor

# Init wlan module and connect to network
print("Trying to connect... (This may take a while)...")
wlan = network.WLAN(network.STA_IF)
wlan.deinit()
wlan.active(True)
wlan.connect(SSID, KEY, timeout=30000)

# We should have a valid IP now via DHCP
print("WiFi Connected ", wlan.ifconfig())
# create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# bind the socket to a specific IP address and port
addr = socket.getaddrinfo(HOST, PORT)[0][4]
print(addr)
sock.connect(addr)
sock.settimeout(3.0)

# listen for incoming connections


def millis():
    return int(round(time.time() * 1000))

def audio_callback(buf):
    try:
        global fout
        start_time = millis()
        while millis() - start_time < 2:
            sock.sendall(bytearray(buf))
            print (buf)
    except Exception as e:
        print("exption:", e)


try:
    audio.start_streaming(audio_callback)
except OSError as e:
    print("socket error: ", e)
    audio.stop_streaming()
