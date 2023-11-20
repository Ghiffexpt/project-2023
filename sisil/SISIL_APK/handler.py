from ComunicateSocket import Server
import threading
sock = Server()


class Events():
    mdl = None

    def __init__(self):
        pass
    def update_ip(self,ip):
        sock.ip_client = ip
    def button_on_event(self):
        print("EVENT BUTTON ON")
        sock.caseEXT = 1
        sock.dev = 1
        t2 = threading.Thread(target=sock.send)
        t2.start()
        return "EVENT BUTTON ON"
    def button_off_event(self):
        print("EVENT BUTTON OFF")
        sock.caseEXT = 0
        sock.dev = 1
        t2 = threading.Thread(target=sock.send)
        t2.start()
        return "EVENT BUTTON OFF"

    def button_connect_event(self):
        print("EVENT BUTTON CONNECT")
        t1 = threading.Thread(target=sock.connect)
        t1.start()
        return "EVENT BUTTON CONNECT"

    def button_disconnect_event(self):
        print("EVENT BUTTON DISCONNECT")
        sock.disconnect()
        return "EVENT BUTTON DISCONNECT"

    def button_POSITION_event(self,scaler):
        sock.valueO = int(scaler)
        sock.dev = 1
        sock.caseEXT = 3
        t2 = threading.Thread(target=sock.send)
        t2.start()
        return "SET POSITION "+scaler

    def button_all_freq_event(self):
        sock.dev = 2
        t2 = threading.Thread(target=sock.send())
        t2.start()
        return "SET ALL FREQ"
    def button_BANDWIDTH_event(self,scaler):
        sock.valueS = int(scaler)
        sock.dev = 1
        sock.caseEXT = 2
        t2 = threading.Thread(target=sock.send)
        t2.start()
        return "SET BANDWIDTH "+scaler
    def button_check_client_event(self):
        print("EVENT CHECK ALL CLIENT")
        sock.caseEXT = 4
        sock.dev = 0
        t2 = threading.Thread(target=sock.send)
        t2.start()
        return "EVENT CHECK ALL CLIENT"

    def button_all_on_event(self):
        print("EVENT BUTTON ALL ON")
        sock.dev = 0
        sock.caseEXT = 1
        t2 = threading.Thread(target=sock.send)
        t2.start()
        return "EVENT BUTTON ALL ON"

    def button_all_off_event(self):
        print("EVENT BUTTON ALL OFF")
        sock.dev = 0
        sock.caseEXT = 0
        t2 = threading.Thread(target=sock.send)
        t2.start()
        return "EVENT BUTTON ALL OFF"


