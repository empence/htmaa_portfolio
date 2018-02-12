#!/usr/bin/env python
# spectrum.py
# Color picker interface written with Tkinter that communicates over serial 
# with microcontroller, to change on-chip RGB LED's color. 
# serial communication code taken from term.py by Neil Gershenfield
# Use: 
# term.py serial_port port_speed 
# we typically use port_speed = 115200. the value of serial_port is 
# /dev/{device_name}, in Linux and macOS, where device name is the 
# microcontroller as connected over an FTDI cable. To find In Linux and macOS, 
# do ls /dev and look for something with usbserial in the name, typically.  
# 
# Neil Gershenfeld
# CBA MIT 7/27/07
#
# (c) Massachusetts Institute of Technology 2007
# This work may be reproduced, modified, distributed,
# performed, and displayed for any purpose. Copyright is
# retained and must be preserved. The work is provided
# as is; no warranty is provided, and users accept all 
# liability.
#

import sys
import time
import serial
from tkinter import *
from tkinter.ttk import *

from select import *

valuelist = [0,10,20,30,40,50,60]
value_to_letter = {0: 'r', 10: 'y', 20:'g' , 30: 'c', 40: 'b',50:'v', 60:'w'}
value_to_hex = {0: '#ff2a00', 10: '#fffb2b', 20: '#3bff0f', 30: '#21f0ff', 
                40: '#0670F2', 50: '#C842E8', 60: '#ffffff'}


class GUI:
  def __init__(self, master):
    self.master = master
    self.master.title("LED Color Picker")
    self.slider = Scale(root, from_=0, to=60, command=self.slider_change)
    self.slider.pack()
    self.widget_quit = Button(root, text="quit",command=self.quit)
    self.widget_quit.pack()

  def quit(self):   
    root.destroy()
    sys.exit()

  def slider_change(self, value):
    newvalue = min(valuelist, key=lambda x:abs(x-float(value)))
    ser.write(value_to_letter[newvalue])
    self.master.configure(bg=value_to_hex[newvalue])
    return


#
#  check command line arguments
#
if (len(sys.argv) != 3):
   print "command line: term.py serial_port speed"
   sys.exit()
port = sys.argv[1]
speed = int(sys.argv[2])
#
# open serial port
#
ser = serial.Serial(port,speed)
ser.setDTR()
#
# flush buffers
#
ser.flushInput()
ser.flushOutput()
#
# set up UI
#
root = Tk()
gui = GUI(root)
root.mainloop()