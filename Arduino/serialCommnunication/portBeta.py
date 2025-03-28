from matplotlib.pyplot import axes as Axes
from matplotlib.pyplot import show as Show
from matplotlib.pyplot import subplots as Subplots
from matplotlib.pyplot import subplots_adjust as SubplotsAdjust
from matplotlib.widgets import Button, TextBox
from numpy import arange as Arange
from numpy import linspace as Linspace
from numpy import zeros_like as ZerosLike
from serial import Serial
from time import sleep as Sleep
import threading

"""CONSTANTS"""

PORT = "/dev/ttyACM0"
BAUD_RATE = 9600

PWM_TOTAL_TIME = 50

"""PWM GRAPH CLASS"""

class PWMGraph():
  def __init__(self):
    self.on = 0
    self.off = 0
    self.cycle_time = 0

    self.__setupGraph()
    self.__setupInputButton()

  def updatePlot(self):
    on_time = self.on
    off_time = self.off
    duty_cycle = on_time / (on_time + off_time)
    period = on_time + off_time
    on_indices = Arange(0, PWM_TOTAL_TIME, period)[:int(PWM_TOTAL_TIME / period)] * 1000
    off_indices = (on_indices + int(on_time * 1000))
    pwm_signal = ZerosLike(self.__time_array)
    for on_idx, off_idx in zip(on_indices, off_indices):
        pwm_signal[int(on_idx):int(off_idx)] = 1
    self.__pwm.set_ydata(pwm_signal)
    self.__ax.set_title(f'PWM: ON={on_time:.1f}s, OFF={off_time:.1f}s, TIME={self.cycle_time:.1f}s, Duty Cycle={duty_cycle*100:.2f}%')
    self.__fig.canvas.draw_idle()
    
  def __setupGraph(self):
    self.__time_array = Linspace(0, PWM_TOTAL_TIME, PWM_TOTAL_TIME * 1000)

    self.__fig, self.__ax = Subplots(figsize=(8, 6))
    SubplotsAdjust(left=0.15, bottom=0.25)

    self.__pwm, = self.__ax.plot(self.__time_array, ZerosLike(self.__time_array), lw=2)
    self.__ax.set_xlabel('Time (s)')
    self.__ax.set_ylabel('PWM Signal')
    self.__ax.set_ylim(-0.1, 1.1)
    self.__ax.grid(True)

  def __setupInputButton(self):
    button_ax = Axes([0.8, 0.05, 0.1, 0.075])
    self.input_button = Button(button_ax, 'LED ON', color='#EAEAEA', hovercolor='0.975')

  def run(self):
    Show()

if __name__ == "__main__":
  pwm_graph = PWMGraph()

  try:
    serial = Serial(PORT, BAUD_RATE)
  except Exception as e:
    print(f"Error: {e}")
    exit()

  def get_input(event):
    try:
      button_text = pwm_graph.input_button.label.get_text()
      _, status = button_text.split(" ")

      new_status = "OFF" if status == "ON" else "ON"
      pwm_graph.input_button.label.set_text(f"LED {new_status}")

      action = "0" if new_status == "OFF" else "1"

      serial.write(action.encode())

      print(f"Data Sent: {action}")
    except Exception as e:
      print("Serial port is closed")

  def get_output():
    while serial.is_open:
      data = serial.readline().decode().strip()
      
      try:
        on, off, time, output = data.split("/")

        pwm_graph.on = float(on)
        pwm_graph.off = float(off)
        pwm_graph.cycle_time = float(time)
        
        pwm_graph.updatePlot()
      except:
        output = data

      print(f"Data Received: {data} | Potentiometer: {output}")

      

  input_thread = threading.Thread(target=get_output)
  input_thread.daemon = True
  input_thread.start()

  pwm_graph.input_button.on_clicked(get_input)

  pwm_graph.run()
