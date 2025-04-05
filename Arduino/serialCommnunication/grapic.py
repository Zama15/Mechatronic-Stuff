# import numpy as np
# import matplotlib.pyplot as plt
# from matplotlib.widgets import Slider, Button
# import threading

# # Define initial values
# ON = 2  # Time (in seconds) the pulse is ON
# OFF = 3  # Time (in seconds) the pulse is OFF
# TIME_CYCLE = 10  # Total time (in seconds) for the PWM cycle
# TIME = 20  # Total time (in seconds) for the PWM signal

# # Create a time array for the PWM cycle
# t = np.linspace(0, TIME, TIME * 1000)

# # Create a figure and axes
# fig, ax = plt.subplots(figsize=(8, 6))
# plt.subplots_adjust(left=0.15, bottom=0.25)

# # Create a square wave signal representing the PWM
# pwm, = ax.plot(t, np.zeros_like(t), lw=2)
# ax.set_xlabel('Time (s)')
# ax.set_ylabel('PWM Signal')
# ax.set_ylim(-0.1, 1.1)
# ax.grid(True)

# # Define a function to update the PWM plot
# def update_pwm(on, off):
#     on_time = on
#     off_time = off
#     duty_cycle = on_time / (on_time + off_time)
#     period = on_time + off_time
#     on_indices = np.arange(0, TIME, period)[:int(TIME / period)] * 1000
#     off_indices = (on_indices + int(on_time * 1000))
#     pwm_signal = np.zeros_like(t)
#     for on_idx, off_idx in zip(on_indices, off_indices):
#         pwm_signal[int(on_idx):int(off_idx)] = 1
#     pwm.set_ydata(pwm_signal)
#     ax.set_title(f'PWM: ON={on_time:.1f}s, OFF={off_time:.1f}s, TIME={TIME_CYCLE:.1f}s, Duty Cycle={duty_cycle*100:.2f}%')
#     fig.canvas.draw_idle()


# def get_input():
#     while True:
#         print("Enter the ON time (in seconds):")
#         ON = float(input())
#         print("Enter the OFF time (in seconds):")
#         OFF = float(input())
#         update_pwm(ON, OFF)


# input_thread = threading.Thread(target=get_input)
# input_thread.daemon = True
# input_thread.start()

# plt.show()
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button

# Define initial values
ON = 2  # Time (in seconds) the pulse is ON
OFF = 3  # Time (in seconds) the pulse is OFF
TIME_CYCLE = 10  # Total time (in seconds) for the PWM cycle
TIME = 20  # Total time (in seconds) for the PWM signal

# Create a time array for the PWM cycle
t = np.linspace(0, TIME, TIME * 1000)

# Create a figure and axes
fig, ax = plt.subplots(figsize=(8, 6))
plt.subplots_adjust(left=0.15, bottom=0.25)

# Create a square wave signal representing the PWM
pwm, = ax.plot(t, np.zeros_like(t), lw=2)
ax.set_xlabel('Time (s)')
ax.set_ylabel('PWM Signal')
ax.set_ylim(-0.1, 1.1)
ax.grid(True)

# Create sliders for ON and OFF times
axcolor = 'lightgoldenrodyellow'
ax_on = plt.axes([0.25, 0.1, 0.65, 0.03], facecolor=axcolor)
ax_off = plt.axes([0.25, 0.15, 0.65, 0.03], facecolor=axcolor)
on_slider = Slider(ax_on, 'ON Time (s)', 0.1, TIME_CYCLE - 0.1, valinit=ON, valstep=0.1)
off_slider = Slider(ax_off, 'OFF Time (s)', 0.1, TIME_CYCLE - 0.1, valinit=OFF, valstep=0.1)

# Define a function to update the PWM plot
def update_pwm(val):
    on_time = on_slider.val
    off_time = off_slider.val
    duty_cycle = on_time / (on_time + off_time)
    period = on_time + off_time
    on_indices = np.arange(0, TIME, period)[:int(TIME / period)] * 1000
    off_indices = (on_indices + int(on_time * 1000))
    pwm_signal = np.zeros_like(t)
    for on_idx, off_idx in zip(on_indices, off_indices):
        pwm_signal[int(on_idx):int(off_idx)] = 1
    pwm.set_ydata(pwm_signal)
    ax.set_title(f'PWM: ON={on_time:.1f}s, OFF={off_time:.1f}s, TIME={TIME_CYCLE:.1f}s, Duty Cycle={duty_cycle*100:.2f}%')
    fig.canvas.draw_idle()

# Call the update function when sliders are moved
on_slider.on_changed(update_pwm)
off_slider.on_changed(update_pwm)

# Create a reset button
reset_ax = plt.axes([0.8, 0.025, 0.1, 0.04])
reset_button = Button(reset_ax, 'Reset', color=axcolor, hovercolor='0.975')

# Define a reset function
def reset(event):
    on_slider.reset()
    off_slider.reset()

# Call the reset function when the button is clicked
reset_button.on_clicked(reset)

# Show the plot
plt.show()
