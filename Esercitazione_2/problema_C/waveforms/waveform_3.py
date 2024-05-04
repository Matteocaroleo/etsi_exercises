from scipy import signal
import matplotlib.pyplot as plt
import matplotlib.ticker as plticker
import numpy as np

#Creates the signal
samplingFrequency = 10000 #10kHz
samplingRate = 1/samplingFrequency 

#Creates discrete time for when to calculate function
t = np.arange (0.0, 1.0, samplingRate)
frequency = [50, 200, 400, 600, 800]
coeff = [0.9, 0.5, 0.35, 0.2, 0.1]
result = 0
for x in range (5):
    result = result + coeff[x]*np.sin(2*np.pi *frequency[x]*t) 
signal = (result)


plt.figure (figsize = (13, 7))

#time is in milliseconds
plt.xlim (0, 400)

plt.title ("Waveform 3")
plt.xlabel ("time [ms]")
plt.ylabel ("amplitude")
plt.plot (signal)
plt.grid()

#fft of signal
resultDft = np.fft.fft(result)
lenghtOfDft= len(resultDft)
n = np.arange(lenghtOfDft)
T = lenghtOfDft/samplingRate
freq = n / T


plt.figure (figsize = (13,7))
plt.title ('Waveform 3 - FFT')


plt.grid()
plt.stem(freq / 10, np.abs(resultDft), 'b', \
         markerfmt=" ", basefmt="-b")
plt.plot (np.abs(resultDft), linewidth = 2)
plt.xlabel('Freq (Hz)')
plt.ylabel('FFT Amplitude') 
plt.xlim(0, 1000)
plt.xticks (frequency)
plt.show()