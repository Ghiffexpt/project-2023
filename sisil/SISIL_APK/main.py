from tkinter import *
import re
class Controller(Tk):
    NUM_CHANNELS = 12
    WIDTH = 980
    HEIGHT = 500
    CENWID = WIDTH / 2
    CENHI = HEIGHT / 2

    SCLEN = 175
    SCMIN = 0
    SCMAX = 100

    ROWLEVEL = 53
    ROW0 = 0
    ROW1 = 20
    ROW2 = ROW1 + ROWLEVEL
    ROW3 = ROW2 + ROWLEVEL
    ROW4 = ROW3 + ROWLEVEL
    ROW5 = ROW4 + ROWLEVEL
    ROW6 = ROW5 + ROWLEVEL
    ROW7 = ROW6 + ROWLEVEL
    ROW8 = ROW7 + ROWLEVEL
    ROW9 = ROW8 + ROWLEVEL
    stat = "disconnect"
    COLSPC = 50
    COLCTRL = 680
    COLC0 = 0
    COLC1 = 680
    COLC2 = COLC1 + COLSPC
    COLC3 = COLC2 + COLSPC
    COLC4 = COLC3 + COLSPC
    COLC5 = COLC4 + COLSPC

    TXT_WID = 21

    options = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]  # dropdown channel

    def __init__(self):
        super().__init__()

    def set_x(self, space):
        return self.X_LEFT + (self.X_SPACE * space)

    def set_y(self, level):
        return self.Y_TOP + (self.Y_LEVEL * level)

    def window(self, channel):
        self.screen_width = self.winfo_screenwidth()
        self.screen_height = self.winfo_screenheight()
        self.geometry('%dx%d+%d+%d'%(self.WIDTH,self.HEIGHT,((self.screen_width/2) - (self.WIDTH/2)), ((self.screen_height/2) - (self.HEIGHT/2))))
        self.title("SISIL DASHBOARD")

    def dropdown_channel(self, channel):
        self.label = Label(self, text="CHANNEL:")
        self.label.place(x = self.COLC1, y = self.ROW1)
        self.var = StringVar(self)
        self.dropdown = OptionMenu(self, self.var, *self.options)
        self.var.set(self.options[channel-1])  # Set default option
        self.dropdown.config(width=17, height=1, background="white")  # Change the width
        self.dropdown.place(x=self.COLC3, y=self.ROW1 - 6)

        def on_value_change(*args):
            print("Selected:", self.var.get())
            controller_update(int(self.var.get()))

        # Attach a trace to the StringVar to listen for changes
        self.var.trace("w", on_value_change)

    def get_selected_channel(self):
        return self.var.get()  # Retrieve the selected channel

    def txt_change_ip(self):
        self.label = Label(self, text="IP Server:")
        self.label.place(x=self.COLC1, y=self.ROW2)

        # Ganti Text dengan Entry
        self.entry_ip = Entry(self, width=self.TXT_WID,justify="center")
        self.entry_ip.insert(END,"172.16.2.72")
        self.entry_ip.place(x=self.COLC3 + 3, y=self.ROW2)
        # Jika Anda ingin mengatur nilai default, misalnya:
        # entry_ip.insert(0, "Nilai Default")

    def txt_status(self, status, power):
        self.label = Label(self, text="STATUS:")
        self.label.place(x = self.COLC1, y = self.ROW3)
        text = Text(height=1, width=self.TXT_WID)
        # text.focus()
        text.insert(END, f"{status} | {power}", "center")
        text.tag_configure("center", justify="center")
        # print(text.get("1.0", END)) #display all the text in the widget into console
        text.place(x = self.COLC3 + 3, y = self.ROW3)

    def txt_ip(self, ip):
        self.label = Label(self, text="IP Client:")
        self.label.place(x = self.COLC1, y = self.ROW4)
        self.text = Entry(self, width=self.TXT_WID, justify="center")
        self.text.insert(END, f"{ip}")

        # Adjust font to increase height

        # Adjust the insertion position to visually center the text
        text_length = len(self.text.get())
        self.text.icursor(text_length)  # Move the insertion cursor to the end

        self.text.place(x=self.COLC3 + 3, y=self.ROW4)

    def txt_freq(self, freq):
        self.label = Label(self, text="FREQUENCY:")
        self.label.place(x = self.COLC1, y = self.ROW5)
        text = Text(height=1, width=self.TXT_WID)
        # text.focus()
        text.insert(END, f"{freq} MHz", "center")
        text.tag_configure("center", justify="center")
        # print(text.get("1.0", END)) #display all the text in the widget into console
        text.place(x = self.COLC3 + 3, y = self.ROW5)

    def BANDWIDTH(self, value):
        self.label = Label(self, text="BANDWIDTH:")
        self.label.place(x = self.COLC1, y = self.ROW6)
        self.scale = Scale(from_=self.SCMIN, to=self.SCMAX, orient=HORIZONTAL, command=self.BANDWIDTH_action, length=self.SCLEN)
        self.scale.place(x = self.COLC3, y = self.ROW6 - 20)
        self.scale.set(value)

    def BANDWIDTH_action(self, scale_value):
        module.txt_communication_display(event.button_BANDWIDTH_event(scaler=scale_value))
        data.write(int(self.var.get()), 'bandwidth', f'{scale_value}')

    def POSITION(self, value):
        self.label = Label(self, text="POSITION:")
        self.label.place(x = self.COLC1, y = self.ROW7)
        self.scale = Scale(from_=self.SCMIN, to=self.SCMAX, orient=HORIZONTAL, command=self.POSITION_action, length=self.SCLEN)
        self.scale.place(x = self.COLC3, y = self.ROW7 - 20)
        self.scale.set(value)

    def POSITION_action(self, scale_value):
        module.txt_communication_display(event.button_POSITION_event(scaler=scale_value))
        data.write(int(self.var.get()), 'position', f'{scale_value}')

    def button_connect(self, state):
        self.button = Button(text="CONNECT", command=self.button_connect_action, width=8)
        self.button.place(x=self.COLC1, y=self.ROW8)
        if state == "disconnect": self.button.config(state=NORMAL)
        else: self.button.config(state=DISABLED)

    def button_connect_action(self):
        sock.ip_server = str(self.entry_ip.get())
        module.txt_communication_display(event.button_connect_event())
        module.button_disconnect(state="connect")
        module.button_connect(state="connect")
        self.stat = "connect"

    def button_disconnect(self, state):
        self.button = Button(text="DISCONNECT", command=self.button_disconnect_action, width=8)
        self.button.place(x=self.COLC1, y=self.ROW9)
        if state == "disconnect": self.button.config(state=DISABLED)
        else: self.button.config(state=NORMAL)

    def button_disconnect_action(self):
        module.txt_communication_display(event.button_disconnect_event())
        module.button_connect(state="disconnect")
        module.button_disconnect(state="disconnect")
        self.stat = "disconnect"

    def button_on(self, state):
        self.button = Button(text="ON", command=self.button_on_action, width=8)
        self.button.place(x=self.COLC3, y=self.ROW8)
        if state == "off": self.button.config(state=NORMAL)
        else: self.button.config(state=DISABLED)

    def button_on_action(self):
        module.txt_communication_display(event.button_on_event())
        # data.write(int(self.var.get()), 'power', 'on')
    def button_off(self, state):
        self.button = Button(text="OFF", command=self.button_off_action, width=8)
        self.button.place(x=self.COLC3, y=self.ROW9)
        if state == "off": self.button.config(state=DISABLED)
        else: self.button.config(state=NORMAL)

    def button_off_action(self):
        module.txt_communication_display(event.button_off_event())
        # data.write(int(self.var.get()), 'power', 'off')

    def button_save_action(self):
        print("Button SAVE")
        data.save()
        self.txt_all_channels(data.df)

    def button_reset(self):
        self.button = Button(text="SAVE", command=self.button_reset_action, width=8)
        self.button.place(x=self.COLC5, y=self.ROW9)

    def button_reset_action(self):
        print("Button RESET")
        controller_update(int(self.var.get()))

    def txt_all_channels(self, data):
        self.label = Label(self, text="DATABASE")
        self.label.place(x=20, y=self.ROW1)
        text = Text(height=13, width=80)
        # text.focus()
        text.insert(END, f"{data}", "center")
        # print(text.get("1.0", END)) #display all the text in the widget into console
        text.place(x=20, y=self.ROW1+20)

    def txt_communication(self, data):
        self.label = Label(self, text="COMMUNICATION")
        self.label.place(x=20, y=self.ROW6)
        text = Text(height=7, width=60)
        # text.focus()
        text.insert(END, f"{data}", "left")
        # print(text.get("1.0", END)) #display all the text in the widget into console
        text.place(x=20, y=self.ROW6+20)
        self.text_comm = text

    def txt_communication_display(self, data):
        self.text_comm.insert(END, "\n", "left")
        self.text_comm.insert(END, f"{data}", "left")

    def button_all_freq(self):
        self.button = Button(text="ALL FREQ", command=self.button_all_freq_action, width=13)
        self.button.place(x=self.COLC1 - (3*self.COLSPC), y=self.ROW7)

    def button_all_freq_action(self):
        module.txt_communication_display(event.button_all_freq_event())

    def button_check_client(self):
        self.button = Button(text="CHECK", command=self.button_check_client_action, width=8)
        self.button.place(x=self.COLC5, y=self.ROW8)

    def button_check_client_action(self):
        module.txt_communication_display(event.button_check_client_event())

    def button_change_ip(self):
        self.button = Button(text="CHANGE IP", command=self.button_change_ip_action, width=13)
        self.button.place(x=self.COLC1 - (3 * self.COLSPC), y=self.ROW6)
    def button_change_ip_action(self):
        data.write(int(self.var.get()), 'ip', f'{self.text.get()}')
        controller_update(int(self.var.get()))
        # Mendapatkan nilai dari input teks
        print(self.entry_ip.get())
    def button_all_on(self):
        self.button = Button(text="ALL ON", command=self.button_all_on_action, width=13)
        self.button.place(x=self.COLC1 - (3 * self.COLSPC), y=self.ROW8)

    def button_all_on_action(self):
        module.txt_communication_display(event.button_all_on_event())

    def button_all_off(self):
        self.button = Button(text="ALL OFF", command=self.button_all_off_action, width=13)
        self.button.place(x=self.COLC1 - (3 * self.COLSPC), y=self.ROW9)

    def button_all_off_action(self):
        module.txt_communication_display(event.button_all_off_event())

    def parse_channel_ip(self,input_string):
        # Pola ekspresi reguler untuk mencocokkan channel dan IP
        pattern = r"Channel: (\d+) \| IP: (\d+\.\d+\.\d+\.\d+)"

        # Mencocokkan pola pada string input
        match = re.match(pattern, input_string)

        # Jika cocok, ekstrak nilai channel dan IP
        if match:
            channel = int(match.group(1))
            ip = match.group(2)
            return channel, ip
        else:
            return None, None



from database import Database
data = Database('db_channels.csv')



from handler import *

event = Events()
sock.data = data
event.data = data
module = Controller()
event.mdl = module
sock.status=module
def controller_channel(channel):
    module.window(channel)
    module.dropdown_channel(channel)
    controller_update(channel)

def controller_update(channel):
    module.txt_status(data.read(channel=channel, property='status'), data.read(channel=channel, property='power'))
    module.txt_ip(data.read(channel=channel, property='ip'))
    module.txt_change_ip()
    module.txt_freq(data.read(channel=channel, property='frequency'))
    module.BANDWIDTH(data.read(channel=channel, property='bandwidth'))
    module.POSITION(data.read(channel=channel, property='position'))
    module.button_connect(state=module.stat)
    module.button_disconnect(state=module.stat)
    module.button_on(data.read(channel=channel, property='power'))
    module.button_off(data.read(channel=channel, property='power'))
    module.button_reset()
    module.button_change_ip()
    module.button_all_freq()
    module.button_save_action()
    module.txt_all_channels(data.df)
    module.txt_communication("COMMUNICATIONS")
    module.button_check_client()
    module.button_all_on()
    module.button_all_off()
    event.update_ip(ip=data.read(channel=channel, property='ip'))


sock.resetValue()
controller_channel(1)
module.mainloop()

'''
'''