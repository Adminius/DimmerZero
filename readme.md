### Library for dimmer application with SAMD21G18A (e.g. Arduino Zero/M0).

##### Supported pwm pins (only one pin on the same channel is supported, e.g. 6 or 11):

|timer    | channel  | recommended pin | alternative pins |
|:--------|:---------|:----------------|:-----------------|
|	Timer0	|	0	|	2 (PA14) |	A3	(PA04) |
|	Timer0	|	1	|	5	(PA15) |	A4	(PA05) |
|	Timer0	|	2	|	6	(PA20) |	10	(PA18) |
|	Timer0	|	3	|	7	(PA21) |	12	(PA19) |
|	|	|	|
|	Timer1	|	0	|	4	(PA08) |	1 (PA11), 8	(PA06) |
|	Timer1	|	1	|	3	(PA09) |	0 (PA10), 9	(PA07) |
|	|	|	|
|	Timer2	|	0	|	11 (PA16) |	6	(PA20) |
|	Timer2	|	1	|	13 (PA17) |	7	(PA21) |

Listed pins are for Arduino Zero. On Arduino M0 (Pro) pins 2 and 4 are reversed!

##### How to use

Create an instance:
normal mode (0 to max)

**DimmerZero channel1(pinnumber);** 

or inverted mode (max to 0)

**DimmerZero channel2(pinnumber, true);**

Initialization instances:
channel1.init();
channel2.init();

##### Functions

**getMaxValue()**: returns max possible value. 

**setValue(int value)**, where value=0..maxValue sets timer to "value" duty-cycle

**setFrequency(int value)**, sets PWM frequency. 250, 500 and 1000 Hz are supported. Default: 1000 Hz. Call this function before init-function.

##### ATTENTION:
if you change frequency or invert output, you should do it for **all** pins on same timer. E.g. if 11 is inverted, 13 must be inverted too!
