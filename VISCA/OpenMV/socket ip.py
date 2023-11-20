import socket,network,audio,time,pyb

SSID='Workshop-Cikunir'      # Network SSID
KEY='admin.admin'       # Network key
HOST ='172.16.1.192'     # Use first available interface
PORT = 1883  # Arbitrary non-privileged port
CHANNELS = 1
SIZE = 256//(2*CHANNELS)

audio.init(channels=1, frequency=16000, gain_db=20, highpass=0.9883)

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
server_address = (HOST, 8000)
sock.bind(server_address)


# listen for incoming connections

sock.listen(1)
sock.setblocking(True)

def millis():
    return int(round(time.time() * 1000))
def audio_callback(buf):
        try:
            global raw_buf
            raw_buf = buf
        except Exception as e:
            print("exption:", e)

def play(sock):
    # wait for a connection
    print('waiting for a connection...')
    audio.start_streaming(audio_callback)
    connection, client_address = sock.accept()
    print('connection from', client_address)
    while (True):
            connection.sendall(bytearray(raw_buf))
            time.sleep_ms(23)


while (True):
    try:
        play(sock)
    except OSError as e:
        print("socket error: ", e)
        audio.stop_streaming()
