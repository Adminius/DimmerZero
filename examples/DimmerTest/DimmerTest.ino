#include <DimmerZero.h>

//Create an instance for channel1 on pin D11 in inverted mode
DimmerZero channel1(11,true);
//Create an instance for channel2 on pin D13 in inverted mode
DimmerZero channel2(13,true);

void setup() {
    //change frequnecy to 500Hz for both channel, because pins 11 and 13 both on timer2
    channel1.setFrequency(500);
    channel2.setFrequency(500);

    //initialize channel1 and channel2
    channel1.init();
    channel2.init();

    //set channel1 to max value to turn it off, because it inverted 
    channel1.setValue(channel1.getMaxValue());
    //set channel2 to max value to turn it off, because it inverted 
    channel2.setValue(channel2.getMaxValue());
}

void loop() {
    //fade in
    for (int c1 = channel1.getMaxValue(); c1 >= 0;c1-=50){
        channel1.setValue(c1);
        delay(10);
    }
    delay(500);

    //fade out
    for (int c1 = 0; c1 <= channel1.getMaxValue();c1+=50){
        channel1.setValue(c1);
        delay(10);
    }
    delay(500);
    
    //fade in
    for (int c2 = channel2.getMaxValue(); c2 >= 0;c2-=50){
        channel2.setValue(c2);
        delay(10);
    }
    delay(500);

    //fade out
    for (int c2 = 0; c2 <= channel2.getMaxValue();c2+=50){
        channel2.setValue(c2);
        delay(10);
    }
    delay(500);

}