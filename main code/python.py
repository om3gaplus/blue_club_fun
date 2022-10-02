from inputs import get_gamepad
import math
import threading
import time
import serial.tools.list_ports;
import serial
import inputs
print ("available COM port/s is/are :")
print([comport.device for comport in serial.tools.list_ports.comports()])
ardport=str(int(input("input the number of arduino port: ")))
lasttime=time.time()
lastc="."
check=False
cruse =False
sport=False
uspeed=0
led_volume=0
cruse_s=0
crues_d='i'
dande=False
def make_command(rea):
  sat=0
  dast="x00"+cru(uspeed)
  rpm=tra(rea[4])
  urpm=str(tra(rea[12]))
  global cruse
  global check
  global cruse_d
  global cruse_s
  global lasttime
  sig=motor_sport(float(rea[5]),float(rea[6]))
  if wasd(rea[0],rea[1])!=None and int(rpm)!=0 :
    dast=(wasd(rea[0],rea[1])+rpm+cru(uspeed))
    sat=2
  if wasd(rea[0],rea[1])=='x' or int(rpm)==0 or int(rea[7])!=0 or int(rea[8])!=0 or int(rea[9])!=0 or int(rea[10])!=0:
    if sat!=0 or int(rea[7])!=0 or int(rea[8])!=0 or int(rea[9])!=0 or int(rea[10])!=0:
      dast=('x00'+cru(uspeed))
      sat-=1
  if (float(time.time())-float(lasttime))>0.19:
    if int(rea[7])==1:
      sig="n"
      print("press x")
    if int(rea[8])==1:
      sig="r"
    if int(rea[9])==1:
      updn(-9)
    if int(rea[10])==1:
      updn(9)
    if int(rea[11])==1:
      gcommand()
    if int(rea[15])==1 or int(rea[15])==-1 :
      sig=led_power(int(rea[15]))
    if int(rea[13])==1:
      leaver()
    if int(rea[16])==1:
        tog_sport()
        if sport==True:
          print("sport mode is activated!")
        if sport==False:
          print("sport mode is deactivated!")
    if int(rea[17])==1:
        print("press left joy")
    lasttime=time.time()

  else:
    sig=motor_sport(float(rea[5]),float(rea[6]))
  return (dast+sig+">")
def gcommand():
  gc=1
  while gc==1:
    co=str(input("input command: "))
    if co=="esc":
      return
    else:
      value = write_read(co+">")
def change_crues():
  global cruse
  if cruse==True:
    cruse=False
    print("cruse des")
    return
  if cruse==False:
    cruse=True
    print("cruse en")
    en_vib()
    return
from inputs import get_gamepad
sat=int(1)
arduino = serial.Serial(port='COM'+ardport, baudrate=9600, timeout=0.001)
#--------------------------------------
def tra(fl):
  fl=int((fl-(fl%0.01))*100)
  if dande:
    fl=int(fl/3)
  if fl>98:
    return "99"
  if fl>=10:
    return str(fl)
  if fl<10 and fl!=0:
    return "0"+str(fl)
  if fl==0:
    return "00"
#---------------------------------------
def tog_sport():
  global sport
  if sport==False:
    sport=True
    return
  if sport == True:
    sport=False
    return
#=======================================
def motor_sport(x,y):
  #print(y)
  if y>=0.5 and abs(x)<0.5:
    return 'w'
  if y<=-0.5 and abs(x)<0.3:
    return 's'
  return 'n'
#=======================================
def leaver():
  global dande
  if dande:
    dande=False
    c_vib('r',500)
    print("dande 2")
    return
  if dande!=True:
    dande=True
    c_vib('l',500)
    print("dande 1")
    return
#---------------------------------------
def led_power(x):
  global led_volume
  dx=int(x)
  if led_volume >0 and led_volume<9:
    led_volume-=dx
  if led_volume==0 and dx <0:
    led_volume-=dx
  if led_volume==9 and dx>0:
    led_volume-=dx
  if (led_volume ==0 or led_volume==9):
    en_vib()
  return str(led_volume)
#---------------------------------------
def en_vib(gamepad=None):
    if not gamepad:
      gamepad = inputs.devices.gamepads[0]
      gamepad.set_vibration(0, 1, 500)
    return
#=======================================
def mvib():
  if __name__ == "__main__":
    en_vib()
#=======================================
def c_vib(d,t):
    gamepad=None
    if not gamepad:
      gamepad = inputs.devices.gamepads[0]
    if d=='r':
      gamepad.set_vibration(0, 1, 400)
    if d=='l':
      gamepad.set_vibration(1, 0, 400)
    return
#---------------------------------------
def wasd(x,y):
  global sport
  if sport==False:
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
  if sport==True:
    if x>=0.5 and abs(y)<0.3:
      return 'l'
    if x<=-0.5 and abs(y)<0.3:
      return 'j'
    if y>=0.5 and abs(x)<0.3:
      return 'i'
    if y<=-0.5 and abs(x)<0.3:
      return 'k'
    if x<=-0.3 and y>=0.3:
      return 'u'
    if x>=0.3 and y>=0.3:
      return 'o'
    if x<=-0.3 and y<=-0.3:
      return 'n'
    if x>=0.3 and y<=-0.3:
      return 'm'
    return 'p'
#----------------------------------------
def cru(ispeed):
  global sport
  if sport==False:
    if(ispeed>0):
      if ispeed<10:
        return "u0"+str(ispeed)
      return "u"+str(ispeed)
    if(ispeed<0):
      if ispeed >-10:
        return "j0"+str(abs(ispeed))
      return "j"+str(abs(ispeed))
    if (ispeed==0):
      return "i00"
  if sport==True:
    if(ispeed>0):
      if ispeed<10:
        return "w0"+str(ispeed)
      return "w"+str(ispeed)
    if(ispeed<0):
      if ispeed >-10:
        return "s0"+str(abs(ispeed))
      return "s"+str(abs(ispeed))
    if (ispeed==0):
      return "x00"
#----------------------------------------
def updn(dx):
  global uspeed
  if(abs(uspeed)<=90):
    uspeed+=dx
    return
  if(uspeed==99 and dx<0):
    uspeed+=dx
    return
  if(uspeed==-99 and dx>0):
    uspeed+=dx
    return
  if uspeed==0 or uspeed==99 or uspeed==-99:
    en_vib()
#----------------------------------------
def updown(x,y,check):
  global cruse_d
  if y>=0.5 and abs(x)<0.3 and check!=True:
    return 'u'
  if y<=-0.5 and abs(x)<0.3 and check!=True:
    return 'j'
  if(check!=True):
    return 'i'
  if(cruse==True):
    return cruse_d

#----------------------------------------
def write_read(x):
  arduino.write(bytes(x, 'utf-8'))
  try:
    return arduino.readline().decode()
  except:
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
    self.PressL=0
    self.PressR=0
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
    pr=self.RightThumb
    pl=self.LeftThumb
    #print(ry)
    return [lx, ly, a, b, rt,rx,ry,y,b,a,x,bb,lt,rb,bl,bu,pr,pl]


  def _monitor_controller(self):
    while True:
      try:
          events = get_gamepad()
      except:
          print('controller not connected!')
          time.sleep(1)

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
    rea=joy.read()
    cc=make_command(rea)
    if(cc!=lastc):
      value = write_read(cc)
      if "x" in cc or (cc[2]=="0" and cc[1]=="0"):
        value = write_read(lastc)
        time.sleep(0.05)        
        value = write_read(cc)
        time.sleep(0.05)
      print(cc)
      lastc=cc
    time.sleep(0.03)
