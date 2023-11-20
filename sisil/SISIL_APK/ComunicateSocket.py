import socket
import threading
import random
import time
import os
import sys
# def status_action(stat):
#     from handler import status_act
#     print(stat)
#     status_act(stat=stat)
class Server:
    clients = []
    ip_server = ''
    data = None
    caseEXT = None
    ip_client = None
    valueS = 0
    valueO = 0
    dev    = None
    cntrl = None
    recont = 0
    status = None
    database = None
    def __init__(self):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        # self.s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        # self.s.setsockopt(socket.SOL_SOCKET, socket.SO_KEEPALIVE, 1)
        # self.s.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPIDLE, 1)
        # self.s.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPINTVL, 1)
        # self.s.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPCNT, 3)
        self.lock = threading.Lock()
        self.shutdown_flag = threading.Event()
        self.running = True
    def connect(self):
        try:
            if self.recont == 1 :
                self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
                self.lock = threading.Lock()
                self.clients = []
                # self.s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
                # self.s.setsockopt(socket.SOL_SOCKET, socket.SO_KEEPALIVE, 1)
                # self.s.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPIDLE, 1)
                # self.s.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPINTVL, 1)
                # self.s.setsockopt(socket.IPPROTO_TCP, socket.TCP_KEEPCNT, 3)
                self.shutdown_flag = threading.Event()
                self.running = True
                self.recont = 0
            print(self.ip_server)
            self.s.bind((self.ip_server, 8234))
            self.status.txt_communication_display(f"IP Server: {self.ip_server} PORT: 8234")
            self.s.listen(5)
            self.condition()

        except OSError as e:
            if e.errno == 98:
                print("Addres already in use. Waiting for reconnect.......")
                # status_action(stat="Addres already in use. Wait for reconnect.......")
                self.status.txt_communication_display("Addres already in use. Wait for reconnect.......")
                time.sleep(5)
                self.connect()
            else:
                print(f"Kesalahan lain terjadi: {str(e)}")

    def reconnect(self):
        # status_action(stat="Wait for reconnect.......")
        self.disconnect()
        time.sleep(0.05)
        self.connect()

    def reconnect_client(self, client_address):
        try:
            pass
            # Buat ulang koneksi ke klien dalam thread terpisah
            # threading.Thread(target=self._reconnect_client, args=(client_address,), daemon=True).start()
        except Exception as e:
            print(f"Kesalahan saat mencoba menyambungkan kembali ke klien {client_address}: {str(e)}")

    def _reconnect_client(self, client_address):
        try:
            new_client_socket = socket.create_connection(client_address)
            with self.lock:
                self.clients.append(new_client_socket)
            print(f"Koneksi ke klien {client_address} berhasil disambungkan kembali.")
        except Exception as e:
            print(f"Kesalahan saat menyambungkan kembali ke klien {client_address}: {str(e)}")

    def accept(self):
        try:
            global c
            c, addr = self.s.accept()
            global ADDR
            (ADDR, PORT) = addr
            self.clients.append(c)
            for i in range(1, 13):
                if (ADDR == self.data.read(channel=i,property='ip')):
                    print('Connected: ',ADDR)
                    print('Channel: ',i)
                    # status_action(stat=f"Connected: {ADDR}, Channel: {i}")
                    self.status.txt_communication_display(f"Connected: {ADDR}, Channel: {i}")
                    self.data.write(int(i), 'status', 'connect')
                    self.status.txt_all_channels(self.data.df)
        except OSError as e:
            print(f"Kesalahan lain di function accept terjadi: {str(e)}")

    def receive(self):
        for i in self.clients:
            def f():
                while not self.shutdown_flag.is_set():
                    try:
                        datarecv = i.recv(1024).decode('utf-8')
                        if len(datarecv) == 0:
                            pass
                        else:
                            try:
                                datarecv = datarecv.strip('{}')
                                # Membagi string berdasarkan koma (",") untuk mendapatkan pasangan key-value
                                pairs = datarecv.split(',')

                                # Membuat kamus (dictionary) untuk menyimpan pasangan key-value
                                result = {}
                                for pair in pairs:
                                    # Membagi setiap pasangan key-value berdasarkan titik dua (":")
                                    key, value = pair.split(':')

                                    # Membersihkan spasi dari key dan value
                                    key = key.strip()
                                    value = value.strip()

                                    # Menambahkan pasangan key-value ke dalam kamus
                                    result[key] = value

                                channel_value = result.get('channel')
                                stat_value = result.get('status')
                                # Sekarang, Anda memiliki data yang diparsing dalam bentuk kamus
                                print("Received data: ", datarecv)
                                print("Channel: ",channel_value)
                                print("Status: ",stat_value)
                                # status_action(stat=f"Channel: {channel_value}, Status: {stat_value}")
                                stat_value = 'off'if stat_value == '' else stat_value
                                self.status.txt_communication_display(f"Channel: {channel_value}, Status: {stat_value}")
                                self.data.write(int(channel_value), 'power', stat_value)
                                self.data.write(int(channel_value), 'status', 'connect')
                                self.status.txt_all_channels(self.data.df)
                                self.status.button_on(self.data.read(channel=int(channel_value), property='power'))
                                self.status.button_off(self.data.read(channel=int(channel_value), property='power'))
                                self.status.txt_status(self.data.read(channel=int(channel_value), property='status'),self.data.read(channel=int(channel_value), property='power'))
                            except ConnectionResetError:
                                self.status.txt_communication_display("Connection reset by peer")
                                # Koneksi di-reset oleh peer
                                self.reconnect()
                            except OSError as e:
                                print(f"an error occurred: {str(e)}")
                                print(f"data recv: {datarecv}")
                    except ConnectionResetError:
                        self.status.txt_communication_display("Connection reset by peer")
                        # Koneksi di-reset oleh peer
                        self.reconnect()
                    except UnicodeDecodeError as e:
                        print(f"UnicodeDecodeError: {str(e)}")

            t1_2_1 = threading.Thread(target=f)
            t1_2_1.start()
            time.sleep(0.01)
    def condition(self):
        while self.running:
            try:
                self.t1_1 = threading.Thread(target=self.accept)
                self.t1_1.daemon = True
                self.t1_1.start()
                self.t1_1.join(1)
                self.t1_2 = threading.Thread(target=self.receive)
                self.t1_2 .daemon = True
                self.t1_2 .start()
                self.t1_2 .join(1)
            except OSError as e:
                print(f"Kesalahan thread terjadi: {str(e)}")

    def send(self):
        global hex_data
        svr = ""
        vs = format(self.valueS, 'x')
        vo = format(self.valueO, 'x')
        if self.caseEXT == 0:
            hex_data = "02140001"
            svr = "OFF"
        elif self.caseEXT == 1:
            hex_data = "02140100"
            svr = "ON"
        elif self.caseEXT == 2:
            if self.valueS < 16:
                vs = "0" + vs
            hex_data = "0105" + vs + "01"
            svr = "Control Scaller"
        elif self.caseEXT == 3:
            if self.valueO < 16:
                vo = "0" + vo
            hex_data = "0105" + vo + "02"
            svr = "Control Ofset"
        elif self.caseEXT == 4:
            for i in range(1, 13):
                self.data.write(int(i), 'status', 'disconnect')
                self.data.write(int(i), 'power', 'off')
            valueC = random.randint(16, 100)
            vc = format(valueC, 'x')
            hex_data = "030415" + vc
            svr = "Check client"
        respond = "Server: {}".format(str(svr))
        try:
            with self.lock:
                for i in self.clients:
                    try:
                        if i.fileno() != -1 and i.getsockopt(socket.SOL_SOCKET, socket.SO_ERROR) == 0:
                            if self.dev == 0:
                                sendHex(i)
                            elif self.dev == 1:
                                client_address = i.getpeername()[0]
                                if client_address == self.ip_client:
                                    sendHex(i)

                            elif self.dev == 2:
                                client_address = i.getpeername()[0]
                                for a in range(1, 13):
                                    if (client_address == self.data.read(channel=a, property='ip')):
                                        print('ip: ', ADDR)

                                        bandwidth = self.data.read(channel=a, property='bandwidth')
                                        position = self.data.read(channel=a,property='position')
                                        print('BANDWIDTH: ', bandwidth)
                                        print('POSITION: ', position)

                                        if int(bandwidth) < 16:
                                            bandwidth = format(int(bandwidth), 'x')
                                            bandwidth = "0" + bandwidth
                                        else:
                                            bandwidth = format(int(bandwidth), 'x')
                                        hexa_bandwidth = "0105" + bandwidth + "01"
                                        binary_bandwidth = bytes.fromhex(hexa_bandwidth)

                                        if int(position) < 16:
                                            position = format(int(position), 'x')
                                            position = "0" + position
                                        else:
                                            position = format(int(position), 'x')
                                        hexa_position = "0105" + position + "02"
                                        binary_position = bytes.fromhex(hexa_position)
                                        i.sendall(binary_bandwidth)
                                        time.sleep(0.01)
                                        i.sendall(binary_position)

                        else:
                            print("Socket tidak terhubung: ", i)
                            # status_action(stat=f"Socket not connected: {i.getpeername()[0]}")
                            self.status.txt_communication_display(f"Socket not connected: {i.getpeername()[0]}")
                            # Handle broken pipe error here
                            self.disconnect_client(i)
                            print('Waiting for reconnect 5 seconds')
                            self.status.txt_communication_display('Waiting for reconnect 5 seconds')
                            time.sleep(5)
                            client_address = i.getpeername()
                            self.reconnect_client(client_address)
                    except BrokenPipeError:
                        print("Broken pipe error. Disconnecting client.")
                        self.status.txt_communication_display("Broken pipe error. Disconnecting client.")

                        # status_action(stat="Broken pipe error. Disconnecting client.")
                        # Handle broken pipe error here
                        self.reconnect()
                    time.sleep(0.01)
        except OSError as e:
            print(f"Kesalahan lain di function send terjadi: {str(e)}")

    def disconnect(self):
        self.running = False
        self.shutdown_flag.set()  # Set flag untuk mematikan thread
        self.s.shutdown(socket.SHUT_RDWR)
        self.s.close()
        for i in self.clients:
            i.shutdown(socket.SHUT_RDWR)
            i.close()

        for i in range(1,13):
            self.data.write(int(i), 'status', 'disconnect')
            self.data.write(int(i), 'power', 'off')
            self.status.txt_all_channels(self.data.df)

        self.recont = 1

    def disconnect_client(self, client_socket):
        try:
            with self.lock:
                if client_socket in self.clients:
                    # Tutup socket klien tanpa menggunakan shutdown
                    client_socket.close()
                    # Hapus socket dari daftar klien
                    self.clients.remove(client_socket)
                    self.shutdown_flag.set()  # Set flag untuk mematikan thread
        except OSError as e:
            print(f"Kesalahan saat memutuskan koneksi klien: {str(e)}")

    def resetValue(self):
        for i in range(1,13):
            self.data.write(int(i), 'status', 'disconnect')
            self.data.write(int(i), 'power', 'off')


    def restart_program(self):
        python = sys.executable
        os.execl(python, python, *sys.argv)

def sendHex(i):
    binary_data = bytes.fromhex(hex_data)
    i.sendall(binary_data)


# def ON(s1):
#     t2 = threading.Thread(target=s1.send)
#     t2.start()
