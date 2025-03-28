import threading
import serial
import numpy as np
from matplotlib import pyplot as plt, use as mpl_use
from tkinter import Tk, Label, Button, Frame, NW
from time import sleep as Sleep

mpl_use("TkAgg")

####### Constants #######

MAIN_COLOR = "#FFD700"
SECONDARY_COLOR = "#FFA500"

FONT = "Arial 20"
FONT_BOLD = "Arial 20 bold"

WINDOW_WIDTH = 500
WINDOW_HEIGHT = 500
WINDOW_TITLE = "Serial Communication"

FRAME_WIDTH = 350
FRAME_HEIGHT = 100

PORT = "/dev/ttyACM0"
BAUD_RATE = 9600

PWM_TOTAL_TIME = 10

####### Class #######


class PWMGraphThread(threading.Thread):
  def __init__(self):
    super().__init__()
    self.on = 0
    self.off = 0
    self.cycle_time = 0
    self.event = threading.Event()
    self.__setupGraph()
    
  def __setupGraph(self):
    self.t = np.linspace(0, PWM_TOTAL_TIME, PWM_TOTAL_TIME * 1000)

    self.fig, self.ax = plt.subplots(figsize=(8, 6))
    plt.subplots_adjust(left=0.15, bottom=0.25)

    self.pwm, = self.ax.plot(self.t, np.zeros_like(self.t), lw=2)
    self.ax.set_xlabel('Time (s)')
    self.ax.set_ylabel('PWM Signal')
    self.ax.set_ylim(-0.1, 1.1)
    self.ax.grid(True)

  def __updatePlot(self):
    on_time = self.on
    off_time = self.off
    duty_cycle = on_time / (on_time + off_time)
    period = on_time + off_time
    on_indices = np.arange(0, self.cycle_time, period)[:int(self.cycle_time / period)] * 1000
    off_indices = (on_indices + int(on_time * 1000))
    pwm_signal = np.zeros_like(self.t)
    for on_idx, off_idx in zip(on_indices, off_indices):
        pwm_signal[int(on_idx):int(off_idx)] = 1
    self.pwm.set_ydata(pwm_signal)
    self.ax.set_title(f'PWM: ON={on_time:.1f}s, OFF={off_time:.1f}s, TIME={self.cycle_time:.1f}s, Duty Cycle={duty_cycle*100:.2f}%')
    self.fig.canvas.draw_idle()

  def updatePWM(self, on, off, cicle_time):
    self.on = on
    self.off = off
    self.cycle_time = cicle_time
    self.event.set()

  def run(self):
    while True:
      self.event.wait()
      self.event.clear()
      self.__updatePlot()


####### Class #######

class SerialThread(threading.Thread):
  def __init__(self, serial, label, graph):
    super().__init__()
    self.serial = serial
    self.label = label
    self.graph = graph

  def read(self):
    while self.serial.isOpen():
      if self.serial.isOpen() and self.serial.in_waiting > 0:
        data = self.serial.readline().decode().strip()
        on, off, time, output = data.split("/")
        dataInLabel = f"Data Received: {output}"
        self.label.config(text=dataInLabel)

        self.graph.updatePWM(float(on), float(off), float(time))

        print(f"Data Received: {data}")

  def close(self):
    self.serial.close()
    self.label.config(text="Port Closed")

  def run(self):
    self.thread = threading.Thread(target=self.read)
    self.thread.daemon = True
    self.thread.start()

  def stop(self):
    self.thread = threading.Thread(target=self.close)
    self.thread.daemon = True
    self.thread.start()


####### Class #######


class SerialCommunication:
  def __init__(self):
    self._window = Tk()
    self.serial = serial.Serial(PORT, BAUD_RATE)
    self.__setupWindow()
    self.__setupFrame()
    self.__createInputArea()
    self.__createOutputArea()


  def __setupWindow(self):
    self._window.title(WINDOW_TITLE)
    self._window.geometry(f"{WINDOW_WIDTH}x{WINDOW_HEIGHT}")
    self._window.configure(bg=MAIN_COLOR)
    self._window.resizable(False, False)

  def __setupFrame(self):
    self.inputFrame = Frame(self._window, width=FRAME_WIDTH, height=FRAME_HEIGHT, bg=SECONDARY_COLOR)
    self.inputFrame.grid(row=0, column=0, pady=10, padx=10)
    self.inputFrame.columnconfigure(0, weight=1)
    self.inputFrame.columnconfigure(1, weight=1)

    self.outputFrame = Frame(self._window, width=FRAME_WIDTH, height=FRAME_HEIGHT, bg=SECONDARY_COLOR)
    self.outputFrame.grid(row=1, column=0, pady=10, padx=10)
    self.outputFrame.columnconfigure(0, weight=1)
    self.outputFrame.columnconfigure(1, weight=1)

  def __createInputArea(self):
    self.inputLabel = Label(self.inputFrame, text="No data Receive Yet", font=FONT, bg=SECONDARY_COLOR)
    self.inputLabel.grid(row=0, column=0, padx=10, pady=10, sticky=NW)


    self.pwmGraph = PWMGraphThread()
    self.pwmGraph.start()
    self.thread = SerialThread(self.serial, self.inputLabel, self.pwmGraph)

    self.inputButtonStart = Button(self.inputFrame, text="start", font=FONT, bg=SECONDARY_COLOR, command=self.thread.run)
    self.inputButtonStart.grid(row=1, column=0, padx=10, pady=10, sticky=NW)

    self.inputButtonStop = Button(self.inputFrame, text="Stop", font=FONT, bg=SECONDARY_COLOR, command=self.thread.stop)
    self.inputButtonStop.grid(row=1, column=1, padx=10, pady=10, sticky=NW)
  
  def __createOutputArea(self):
    self.outputLabel = Label(self.outputFrame, text="LED Status: OFF", font=FONT_BOLD, bg=SECONDARY_COLOR)
    self.outputLabel.grid(row=0, column=0, padx=10, pady=10, sticky=NW)

    self.outputButton = Button(self.outputFrame, text="ON", font=FONT, bg=SECONDARY_COLOR, command=self.__sendOutputSerial)
    self.outputButton.grid(row=1, column=0, padx=10, pady=10, sticky=NW)

  def __sendOutputSerial(self):
    text = self.outputButton.cget("text")
    self.outputLabel.config(text=f"LED Status: {text}")
    
    buttonText = "ON" if text == "OFF" else "OFF"
    self.outputButton.config(text=buttonText)
    
    output = "0" if text == "OFF" else "1"

    print(f"Sending data: {output}")
    
    self.serial.write(output.encode())

    Sleep(0.1)

  @staticmethod
  def run():
    self = SerialCommunication()
    self._window.mainloop()


####### Main #######


if __name__ == "__main__":
  SerialCommunication.run()
    



    