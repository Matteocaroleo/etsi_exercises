from scipy import signal
import matplotlib.pyplot as plt
import matplotlib.ticker as plticker
import numpy as np

#Creates the signal
samplingFrequency = 10000 #10kHz
samplingRate = 1/samplingFrequency 

#Creates discrete time for when to calculate function
t = np.arange (0.0, 1.0, samplingRate)

#It's a G3 dom13th chord
frequency = [196.00, 246.94 , 349.23 , 493.88 , 659.25]
coeff = [0.9, 0.5, 0.7, 0.55, 0.65]
result = 0
for x in range (5):
    result = result + coeff[x]*np.sin(2*np.pi *frequency[x]*t) 
signal = (result)


plt.figure (figsize = (13, 7))

#time is in milliseconds
plt.xlim (0, 500)

plt.title ("Waveform 3 (G3dom13th)")
plt.xlabel ("Samples")
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
plt.xlim(0, frequency [4] + 100)
plt.xticks (frequency)
plt.show()