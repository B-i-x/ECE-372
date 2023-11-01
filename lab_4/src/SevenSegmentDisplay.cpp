#include <avr/io.h>
#include <Arduino.h>

#include <SevenSegmentDisplay.h>

/* Initialize PORTC
*/
void initDisplay(){

    DDRC |= (1<<DDC0) | (1<<DDC1) | (1<<DDC2) | (1<<DDC3) | (1<<DDC4) | (1<<DDC5) | (1<<DDC6);
}

void WriteToDisplay(int input) {
    /*
    num = bit_index AND portC index
    A = 6 (MSB)
    B = 5
    C = 4
    D = 3
    E = 2
    F = 1
    G = 0 (LSB)
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
            controlSignal = 0b1111110;
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

        numToPort(i, (bool)PortToggle);
    }
}

void numToPort(int portNum, bool toggle) {

    if (toggle) {
        PORTC |= (1 << portNum);
    }
    else {
        PORTC &= ~(1 << portNum);
    }

}
