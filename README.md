# 6800snoop
Teensy Test to snoop a 6800 bus on pinball machines. purpose to capture lamps and dmd

I used a Teensy 3.5 because of the 5volt tolerant option at 120MHz and profile "fastest"
Also pins from the inside of the board are used. 

It's still in alpha phase.
Current tests provide:
- read/write capturing ADDRESS DATA CTRL
- RAM dump
- PIA (6821) tracing
- Lamp and switch status
