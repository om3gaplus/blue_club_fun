import math
import threading
import time
import serial.tools.list_ports
import serial
import inputs
from inputs import get_gamepad
import serial.tools.list_ports
import re
# import re

# find and show all available COM ports
print("available COM port/s is/are :")
print([comport.device for comport in serial.tools.list_ports.comports()])
#ardport = str(int(input("input the number of arduino port: ")))
com_ports = serial.tools.list_ports.comports()
ardport=17
for port in com_ports:
    try:
        if "CH340" in str(port.description):
            ardport=str(re.findall("COM([0-9]{1,2})",str(port.device))[0])
            print("connected to port : "+ardport)
            break
    except:
        print("device not found!")
lasttime = time.time()

dande = 3
lastc = "500500100/n"
led_vol=0
sat = int(1)
ud_const=0
emergency_t=float(time.time())
arduino = serial.Serial(port='COM' + ardport, baudrate=9600, timeout=0.001)


def en_vib(gamepad=None):
    if not gamepad:
        gamepad = inputs.devices.gamepads[0]
        gamepad.set_vibration(0, 1, 500)
    return


def wasd(x, y,rt):
    if x >= 0.5 and abs(y) < 0.3:
        return '6'
    if x <= -0.5 and abs(y) < 0.3:
        return '4'
    if y >= 0.5 and abs(x) < 0.3:
        return '2'
    if y <= -0.5 and abs(x) < 0.3:
        return '8'
    if x <= -0.3 and y >= 0.3:
        return '1'
    if x >= 0.3 and y >= 0.3:
        return '3'
    if x <= -0.3 and y <= -0.3:
        return '7'
    if x >= 0.3 and y <= -0.3:
        return '9'
    if(abs(x)<0.3 and abs(y)<0.3 and rt!="00"):
        return '2'
    return '5'


#remove write_read
#also there

def con_ud(dx):
    global ud_const
    ud_const +=dx
    return
def c_vib(d, t):
    gamepad = None
    if not gamepad:
        gamepad = inputs.devices.gamepads[0]
    if d == 'r':
        gamepad.set_vibration(0, 1, 400)
    if d == 'l':
        gamepad.set_vibration(1, 0, 400)
    if d == 'b':
        gamepad.set_vibration(1, 1, 1000)
    return


def leaver(dx):
    global dande
    dande += dx
    if dande <= 1:
        dande = 1
    if dande >= 3:
        dande = 3
    if dande == 3:
        c_vib('r', 500)
        print("dande 3")
        return
    if dande == 1:
        c_vib('l', 500)
        print("dande 1")
        return
    if dande == 2:
        c_vib('b', 1000)
        print("dande 2")
        return
def led_func(dx):
    global led_vol
    led_vol += dx
    if led_vol <= 0:
        led_vol = 0
    if led_vol >= 5:
        led_vol = 5
    return

#===========write by manuch\\\\\\\\\\\\\\\\\
def led_command():
    global led_vol
    #global led_str
    return (str(int(mmap(led_vol,0,5,100,199))))
#===========================================
def tra(fl):
    fl = int((fl - (fl % 0.01)) * 100)
    if dande == 1:
        fl = int(fl / 6)
    if dande == 2:
        fl = int(fl * 0.4)
    if fl > 98:
        return "99"
    if fl >= 10:
        return str(fl)
    if fl < 10 and fl != 0:
        return "0" + str(fl)
    if fl == 0:
        return "00"

def up_tra(fl):
    fl=int(fl*100)
    fl+=ud_const
    if abs(fl) <25:
        return "500"
    ret=''
    if fl>0:
        ret+='2'
    elif fl<0:
        ret+='8'
    sp=int(abs(fl)/3)
    if (sp<10):
        ret+='0'
    ret+=str(sp)
    return ret

def make_command(rea):
    global lasttime
    global ud_const
    #print(rea[-6])
    rpm = tra(rea[4])
    dast = "500" + str(up_tra(float(rea[6])))+led_command()
    if int(tra(rea[12])) > 0:
        ud_const=0
        return ('500500'+led_command()+"\n")
    if (float(time.time()) - float(lasttime)) > 0.19:
        if int(rea[13]) == 1:
            leaver(+1)
            lasttime = time.time()
        if int(rea[18]) == 1:
            leaver(-1)
            lasttime = time.time()
        if abs(int(rea[-6]))==1:
            led_func(-1*int(rea[-6]))
            lasttime = time.time()
        if int(rea[9]) == 1:
            con_ud(-9)
            lasttime = time.time()
        if int(rea[10]) == 1:
            con_ud(9)
            lasttime = time.time()
    if wasd(rea[0], rea[1],str(up_tra(float(rea[6])))) is not None and int(rpm) != 0:
        dast = (wasd(rea[0], rea[1],str(up_tra(float(rea[6])))) + rpm + str(up_tra(float(rea[6]))))+led_command()
    return dast + "\n"


def mmap(value, leftMin, leftMax, rightMin, rightMax):
    # Figure out how 'wide' each range is
    leftSpan = leftMax - leftMin
    rightSpan = rightMax - rightMin

    # Convert the left range into a 0-1 range (float)
    valueScaled = float(value - leftMin) / float(leftSpan)

    # Convert the 0-1 range into a value in the right range.
    return rightMin + (valueScaled * rightSpan)


def write_read(x):
    global emergency_t
    arduino.write(bytes(x, 'utf-8'))
    print(x) #second
    emergency_t=float(time.time())
    return None


class XboxController(object):
    MAX_TRIG_VAL = math.pow(2, 8)
    MAX_JOY_VAL = math.pow(2, 15)

    def __init__(self):

        self.LeftJoystickY = 0
        self.LeftJoystickX = 0
        self.RightJoystickY = 0
        self.RightJoystickX = 0
        self.LeftTrigger = 0
        self.RightTrigger = 0
        self.LeftBumper = 0
        self.RightBumper = 0
        self.A = 0
        self.X = 0
        self.Y = 0
        self.B = 0
        self.LeftThumb = 0
        self.RightThumb = 0
        self.Back = 0
        self.Start = 0
        self.LeftDPad = 0
        self.RightDPad = 0
        self.UpDPad = 0
        self.DownDPad = 0
        self.PressL = 0
        self.PressR = 0
        self._monitor_thread = threading.Thread(target=self._monitor_controller, args=())
        self._monitor_thread.daemon = True
        self._monitor_thread.start()

    def read(self):  # return the buttons/triggers that you care about in this methode
        lx = self.LeftJoystickX
        ly = self.LeftJoystickY
        a = self.A
        x = self.X  # b=1, x=2
        y = self.Y
        b = self.B
        rb = self.RightBumper
        lb = self.LeftBumper
        ry = self.RightJoystickY
        rx = self.RightJoystickX
        rt = self.RightTrigger
        lt = self.LeftTrigger
        bb = self.Back
        bs = self.Start
        bu = self.UpDPad
        bd = self.DownDPad
        bl = self.LeftDPad
        br = self.RightDPad
        pr = self.RightThumb
        pl = self.LeftThumb
        # print(lt)
        return [lx, ly, a, b, rt, rx, ry, y, b, a, x, bb, lt, rb, bl, bu, pl, pl, lb, bs, pr]

    def _monitor_controller(self):
        while True:
            try:
                events = get_gamepad()
            except:
                print('controller not connected!')
                write_read("500500100\n")
                time.sleep(1)

            for event in events:
                if event.code == 'ABS_Y':
                    self.LeftJoystickY = event.state / XboxController.MAX_JOY_VAL  # normalize between -1 and 1
                elif event.code == 'ABS_X':
                    self.LeftJoystickX = event.state / XboxController.MAX_JOY_VAL  # normalize between -1 and 1
                elif event.code == 'ABS_RY':
                    self.RightJoystickY = event.state / XboxController.MAX_JOY_VAL  # normalize between -1 and 1
                elif event.code == 'ABS_RX':
                    self.RightJoystickX = event.state / XboxController.MAX_JOY_VAL  # normalize between -1 and 1
                elif event.code == 'ABS_Z':
                    self.LeftTrigger = event.state / XboxController.MAX_TRIG_VAL  # normalize between 0 and 1
                elif event.code == 'ABS_RZ':
                    self.RightTrigger = event.state / XboxController.MAX_TRIG_VAL  # normalize between 0 and 1
                elif event.code == 'BTN_TL':
                    self.LeftBumper = event.state
                elif event.code == 'BTN_TR':
                    self.RightBumper = event.state
                elif event.code == 'BTN_SOUTH':
                    self.A = event.state
                elif event.code == 'BTN_NORTH':
                    self.X = event.state
                elif event.code == 'BTN_WEST':
                    self.Y = event.state
                elif event.code == 'BTN_EAST':
                    self.B = event.state
                elif event.code == 'BTN_THUMBL':
                    self.LeftThumb = event.state
                elif event.code == 'BTN_THUMBR':
                    self.RightThumb = event.state
                elif event.code == 'BTN_SELECT':
                    self.Back = event.state
                elif event.code == 'BTN_START':
                    self.Start = event.state
                elif event.code == 'ABS_HAT0X':
                    self.LeftDPad = event.state
                elif event.code == 'BTN_TRIGGER_HAPPY2':
                    self.RightDPad = event.state
                elif event.code == 'ABS_HAT0Y':
                    self.UpDPad = event.state
                elif event.code == 'BTN_TRIGGER_HAPPY4':
                    self.DownDPad = event.state


if __name__ == '__main__':
    joy = XboxController()
    while True:
        rea = joy.read()
        cc = make_command(rea)
        if (cc != lastc):
            write_read(cc)
            lastc = cc
        if float(time.time())-float(emergency_t)>8:
            write_read(lastc)
        time.sleep(0.03)
