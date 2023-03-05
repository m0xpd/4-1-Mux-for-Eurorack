# 4-1-Mux-for-Eurorack
A Dual Channel 4:1 Multiplexer for Eurorack Synthesizers

This project describes a multiplexer (similar to a "sequential switch") for use in electronic music applications. 
![MUX Front Panel](https://user-images.githubusercontent.com/3152962/222978851-0f6b30ae-8034-4069-99ba-0095303d9603.jpg)

The main system has two 4:1 multiplexing channels, each of which can switch audio or "CV" signals. The switching can be performed manually, under CV control or synchronously with an external or local clock. When clocked, the system will advance in a simple sequential cycle through the inputs or can 'shuffle' the inputs randomly. When under external clock, a clock divide feature is available and the system can clock on rising or falling edges of the external clock. The system can be set to cycle through 2, 3 or 4 of the available inputs and can be reset to input 1 on application of an external reset pulse.

The main system also features potentiometers to generate voltages which are normalled to the 'X' channel inputs of the multiplexer, making it easy to (e.g.) generate CV sequences etc..

The system is implemented using a conventional CMOS analog multiplexer, which is operated between -5V and +5V (to allow the passage of bipolar signals). Signals are first attenuated (by ~10dB) by precision passive attenuators before entering the multiplexer and subsequently amplified to restore the signal level. This allows full-scale 'Eurorack' signals to be handled and avoids the non-linearity at the upper end of the multiplexer's transfer characteristic. The system is implemented using Surface Mount components (1206) and the control is achieved by an Arduino Nano. 

The main system occupies 12HP.

System Expansion for Multi-Channel Opration

A 'Slave' system, also in 12HP, offers another two channels of multiplexing under the control of the main system. The 'X' channel of the slave unit also has potentiometers offering voltages to its normalled inputs. The slave system is connected to the main syatem by a ribbon cable but requires an independant eurorack power cable. 

Multiple 'slaves' can be driven by a single main system, allowing 4, 6, 8 ... channel mutiplexer systems to be constructed.
