# Home Automation

This is a simple home automation project I am making for my home.
My intent is to build a simple circuit for: 
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
* AC to DC converter (YS-12v450A) -> To power the circuit
* SD card reader -> Store the sensors information 
* Some resistors.

References:
https://github.com/esp8266/Arduino
