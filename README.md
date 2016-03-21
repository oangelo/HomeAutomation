# Intellect Box

The objective of this project is to add sensor and actuators to a house, in
order to be possible to control it, but not just this, the system should gather
some data in order to make possible for the user to take decisions driven by
informations about his environment. Knowing the ambient noise, temperature, humidity and
other quantities, all day long, it is possible to take smart decisions to make
home a better place.

My intent is to build a circuit for: 
* turn on/off the lights of two lampshades, 
* to record the room temperature and humidity, 
* to detect presence using some PIRs, 
* to measure the ambient noise with a microphone,
* and to measure the ambient light with a LDR.

All this controlled wirelessly.

I have these components for this task:
* Arduino Nano -> Brain
* esp2866 (esp-01) -> Wireless
* DHT-22 -> Temperature and humidity. 
* LDR -> Ambient Light intensity 
* PIR -> Movement detection
* Small Arduino microphone -> Room noise 
* RELAY -> To turn lamps on/off
* AC to DC converter (YS-12v450A) (110-220v to 5v) -> To power the circuit
* Step down power module (5V to 3.3V) -> Arduino can't power the esp8266 directly 
* SD card reader -> Store the sensors information 
* Some resistors.

References:
https://github.com/esp8266/Arduino
