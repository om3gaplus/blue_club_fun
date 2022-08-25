from inputs import get_gamepad
import math
import threading
import time
import serial
try:
    from inputs import get_gamepad
    sat=int(1)
    arduino = serial.Serial(port='COM15', baudrate=9600, timeout=0.001)
    #--------------------------------------
    def tra(fl):
        fl=int((fl-(fl%0.01))*100)
        if fl>98:
            return "99"
        if fl>=10:
            return str(fl)
        if fl<10 and fl!=0:
            return "0"+str(fl)
        if fl==0:
            return "00"
    #---------------------------------------
    def wasd(x,y):
        if x>=0.5 and abs(y)<0.3:
            return 'd'
        if x<=-0.5 and abs(y)<0.3:
            return 'a'
        if y>=0.5 and abs(x)<0.3:
            return 'w'
        if y<=-0.5 and abs(x)<0.3:
            return 's'
        if x<=-0.3 and y>=0.3:
            return 'q'
        if x>=0.3 and y>=0.3:
            return 'e'
        if x<=-0.3 and y<=-0.3:
            return 'z'
        if x>=0.3 and y<=-0.3:
            return 'c'
        
        return 'x'
    #----------------------------------------
    def updown(x,y):
        if y>=0.5 and abs(x)<0.3:
            return 'u'
        if y<=-0.5 and abs(x)<0.3:
            return 'j'
        return 'i'

    #----------------------------------------
    def write_read(x):
        arduino.write(bytes(x, 'utf-8'))
        return
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

            self._monitor_thread = threading.Thread(target=self._monitor_controller, args=())
            self._monitor_thread.daemon = True
            self._monitor_thread.start()


        def read(self): # return the buttons/triggers that you care about in this methode
            lx = self.LeftJoystickX
            ly = self.LeftJoystickY
            a = self.A
            x = self.X # b=1, x=2
            y=self.Y
            b=self.B
            rb = self.RightBumper
            lb=self.LeftBumper
            ry=self.RightJoystickY
            rx=self.RightJoystickX
            rt=self.RightTrigger
            lt=self.LeftTrigger
            bb=self.Back
            bs=self.Start
            bu=self.UpDPad
            bd=self.DownDPad
            bl=self.LeftDPad
            br=self.RightDPad
            return [lx, ly, a, b, rt,rx,ry]


        def _monitor_controller(self):
            while True:
                events = get_gamepad()
                for event in events:
                    if event.code == 'ABS_Y':
                        self.LeftJoystickY = event.state / XboxController.MAX_JOY_VAL # normalize between -1 and 1
                    elif event.code == 'ABS_X':
                        self.LeftJoystickX = event.state / XboxController.MAX_JOY_VAL # normalize between -1 and 1
                    elif event.code == 'ABS_RY':
                        self.RightJoystickY = event.state / XboxController.MAX_JOY_VAL # normalize between -1 and 1
                    elif event.code == 'ABS_RX':
                        self.RightJoystickX = event.state / XboxController.MAX_JOY_VAL # normalize between -1 and 1
                    elif event.code == 'ABS_Z':
                        self.LeftTrigger = event.state / XboxController.MAX_TRIG_VAL # normalize between 0 and 1
                    elif event.code == 'ABS_RZ':
                        self.RightTrigger = event.state / XboxController.MAX_TRIG_VAL # normalize between 0 and 1
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
                    elif event.code == 'BTN_TRIGGER_HAPPY1':
                        self.LeftDPad = event.state
                    elif event.code == 'BTN_TRIGGER_HAPPY2':
                        self.RightDPad = event.state
                    elif event.code == 'BTN_TRIGGER_HAPPY3':
                        self.UpDPad = event.state
                    elif event.code == 'BTN_TRIGGER_HAPPY4':
                        self.DownDPad = event.state




    if __name__ == '__main__':
        joy = XboxController()
        while True:
            rea=joy.read()
            rpm=tra(rea[4])
            if wasd(rea[0],rea[1])!=None and int(rpm)!=0 or updown(rea[5],rea[6])!='i' :
                print(wasd(rea[0],rea[1])+rpm+updown(rea[5],rea[6])+"00"+">")
                value = write_read(wasd(rea[0],rea[1])+rpm+updown(rea[5],rea[6])+"140"+">")
                sat=2
            if wasd(rea[0],rea[1])=='x' or int(rpm)==0: 
                if sat!=0:
                    value = write_read('x00'+updown(rea[5],rea[6])+"00"+">")
                    sat-=1
                    print("x00"+updown(rea[5],rea[6])+"00"+">")
                    #--------------------------
            
            time.sleep(0.03)

except:
    print("error")
