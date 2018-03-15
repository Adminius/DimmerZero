### Library for dimmer application with SAMD21G18A (e.g. Arduino Zero/M0).

##### Supported pwm pins (only one pin on the same channel is supported, e.g. 6 or 11):

|timer    | channel  | recommended pin | alternative pins |
|:--------|:---------|:----------------|:-----------------|
|	Timer0	|	0	|	2	|	A3	|
|	Timer0	|	1	|	5	|	A4	|
|	Timer0	|	2	|	6	|	10	|
|	Timer0	|	3	|	7	|	12	|
|	|	|	|
|	Timer1	|	0	|	4	|	1, 8	|
|	Timer1	|	1	|	3	|	0, 9	|
|	|	|	|
|	Timer2	|	0	|	6	|	11	|
|	Timer2	|	1	|	7	|	13	|

Listed pins are for Arduino Zero. On Arduino M0 (Pro) pins 2 and 4 are reversed!

##### How to use

Create an instance:
normal mode (0 to max)

**DimmerSAMD channel1(pinnumber);** 

or inverted mode (max to 0)

**DimmerSAMD channel2(pinnumber, true);**

Initialization instances:
channel1.init();
channel2.init();

##### Functions

**getMaxValue()**: returns max possible value. 

**setValue(int value)**, where value=0..maxValue sets timer to "value" duty-cycle

**setFrequency(int value)**, sets PWM frequency. 250, 500 and 1000 Hz are supported. Default: 1000 Hz. Call this function before init-function.

##### ATTENTION:
if you change frequency or invert output, you should do it for **all** pins on same timer. E.g. if 11 is inverted, 13 must be inverted too!
