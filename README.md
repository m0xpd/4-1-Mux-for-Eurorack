# A Dual Channel 4:1 Multiplexer for Eurorack

This project describes a multiplexer (similar to a "sequential switch") for electronic music applications.

<p width=100%, align="center">
<img width=50%, src="https://user-images.githubusercontent.com/3152962/222978851-0f6b30ae-8034-4069-99ba-0095303d9603.jpg">
</p>

The main system, implemented in a 12HP module, has two 4:1 multiplexers, each of which can switch audio or "CV" signals. The switching can be performed manually, under CV control, or synchronously with an external or internal clock. When clocked, the system can either i) follow the CV input requests (switching synchronously with the clock), ii) advance in a repetitive, finite arithmetic sequence through the inputs or iii) 'shuffle' the inputs randomly. When externally clocked, a "clock divide" feature is available and the system can clock on rising or falling edges of the external clock. The system can be set to cycle (sequentially or randomly) through the first 2, 3 or 4 of the available inputs and can be reset to input 1 on application of an external reset pulse.

The main system also features four potentiometers to generate voltages which are normalled to the 'X' channel inputs of the multiplexer, making it easy to (e.g.) generate CV sequences etc..

Note that, unlike some 'sequential switches' (such as the Doepfer A151), the system is unidirectional; it passes signals only from inputs to outputs. 

The system is based upon a conventional  4052 CMOS analog multiplexer, which is operated between -5V and +5V (to allow safe passage of bipolar signals). Signals are first attenuated (by around 10dB) by passive attenuators before entering the multiplexer and subsequently amplified to restore the signal level. This allows full-scale 'Eurorack' signals to be handled and avoids the non-linearity at the extremes of the multiplexer's transfer characteristic. The system is implemented using Surface Mount components (1206) and the multiplexer switching control is achieved by an Arduino Nano. Although the microcontroller's processing speed will never allow this system to be as fast as pure hardware multiplexers, it has been shown capable of clocking the inputs at frequencies over 2kHz using the code in the 'Controller' folder of this repository (this was with an external clock, of course - the internal clock has been programmed such that the 'rate' control delevers clock frequencies between 0.25 Hz and 13.5 Hz).

The expense of precision resistors for the input attenuators was avoided by measuring and selecting from 1% stock. It was found easy to achieve 0.1% equivalent tolerance in system gain.   

The main multiplexer module draws 25mA from the 12V supply (80% of which is for the Arduino) and 4mA from the -12V supply.

# System Expansion for Multi-Channel Operation

A 'Slave' system, also in 12HP, provides another two channels of multiplexing under the control of the main system. 

<p width=100%, align="center">
<img width=50%, src="https://user-images.githubusercontent.com/3152962/222979064-0ddcff23-9f6a-4de9-9029-83d069f950eb.jpg">
</p>


The 'X' channel of the slave unit also has potentiometers offering four voltages to its normalled inputs. 

The slave system is connected to the main system by a ribbon cable, which allows the slave multiplexer to track the switching sequence of the main controller. The slave unit requires an independent eurorack power cable. 

The slave multiplexer module draws 4mA from the 12V supply and 3mA from the -12V supply.

Multiple 'slaves' can be driven by a single main system, allowing 4, 6, 8 ... channel 4:1 mutiplexer systems to be easily constructed - I now run a 6 channel 4:1 mux in my system, using 
a main unit and two slaves...

<p width=100%, align="center">
<img width=50%, src="https://github.com/m0xpd/4-1-Mux-for-Eurorack/assets/3152962/db34bb32-4c8c-4d36-896e-9a7ef4184a69">
</p>




