#include <avr/io.h>
#include <Arduino.h>

#include <SevenSegmentDisplay.h>

/* Initialize PORTC
*/
void initDisplay(){

    DDRB |= (1 << DDB4); //this is SER

    DDRH |= ((1 << DDH5) | (1 << DDH6));
    //        RCLK          SRCLK
    //RCLK
    //When this pin is pulled HIGH, the contents of the Shift Register are copied into the Storage/Latch Register, 
    //which eventually appears at the output. So, the latch pin can be seen as the last step before we see our results at the output.
    //setting latch low
    PORTH &= ~(1 << PORTH5);

    //
    //SRCLK
    //is the clock for the shift-register and is positive-edge triggered. 
    //This means that the bits are pushed in on the rising edge of the clock.
    //setting clock low
    PORTH &= ~(1 << PORTH6);
}

void WriteToDisplay(int input) {
    /*
    //0b0000000;
    //msb     lsb
    A = (msb)
    B = 
    C = 
    D = 
    E = 
    F = 
    G = (lsb)
    */
    
    int controlSignal = 0b0000000;

    switch (input){
        case(0):
            controlSignal = 0b1111110;
            break;

        case(1):
            controlSignal = 0b0110000;
            break;

        case(2):
            controlSignal = 0b1101101;
            break;

        case(3):
            controlSignal = 0b1111001;
            break;

        case(4):
            controlSignal = 0b0110011;
            break;

        case(5):
            controlSignal = 0b1011011;
            break;

        case(6):
            controlSignal = 0b1011111;
            break;

        case(7):
            controlSignal = 0b1110000;
            break;
        
        case(8):
            controlSignal = 0b1111111;
            break;

        case(9):
            controlSignal = 0b1111011;
            break;

        default:
            controlSignal = 0b0000000;
            break;
    }
    
    //iterate through every bit in the control signal and write to that port
    for (int i = 0; i < 7; i++) {
        int PortToggle = (controlSignal >> i) & 1;

        // Serial.print("For port and bit: ");
        
        // Serial.print(i);
     
        // Serial.print(" ,value is:");
        
        // Serial.print(PortToggle);
        
        // Serial.print("\n");

        SER((bool)PortToggle);
    }

    

}

void SER(bool toggle) {
    //write to SER
    if (toggle) {
        PORTB |= (1 << PORTB4);
    }
    else {
        PORTB &= ~(1 << PORTB4);
    }

    //open latch
    PORTH |= (1 << PORTH5);

    //make clock go high
    PORTH |= (1 << PORTH6);
    

    

    //make clock go low
    PORTH &= ~(1 << PORTH6);

    //close latch
    PORTH &= ~(1 << PORTH5);


}
