#include <avr/io.h>
#include <Arduino.h>

#include "i2c.h"
#include "pwm.h"
#include "timer.h"
#include "switch.h"
#include "spi.h"
#include "mpu6050.h"


//create states
typedef enum {
  wait_for_movement, 
  buzz, 
  turn_off_buzzer} device_state;
//declare the enum state variable
volatile device_state state_var = wait_for_movement;
int godothing = 0;

int main() {

  Serial.begin(9600); // using serial port to print values from I2C bus
  sei(); 
  initI2C(); 
  initSwitchPD2(); // initialize I2C and set bit rate
  SPI_MASTER_Init(); // initialize SPI module and set the data rate
  _delay_ms(100);  // delay for 1 s to display "HI"
  // initialize 8 x 8 LED array (info from MAX7219 datasheet)
  write_execute(0x0A, 0x03);  // brightness control
  write_execute(0x0B, 0x07); // scanning all rows and columns
  write_execute(0x0C, 0x01); // set shutdown register to normal operation (0x01)
  write_execute(0x0F, 0x00); // display test register - set to normal operation (0x01)
  write_happy_face();

    
  wake_mpu6050();

  int i = 1000;

  initPWMTimer3();
  while (1) {

    delayMs(100);

    mpu6050_data mpu_data = read_mpu6050_data(true);

    switch (state_var) {
      case wait_for_movement:       

        if(!is_sensor_laying_down(mpu_data)){
          Serial.println("Sensor Moved!");
          write_sad_face();
          state_var = buzz; 
        }


        break;

      case buzz:
        Serial.println("Buzzing");
        changer(i);
        i = i+1000;
        if(i > 4000){
          i = 1000;
        }

        break;

      case (turn_off_buzzer):
        Serial.println("Turning off buzzer");
        turnOff();
        write_happy_face();
        state_var = wait_for_movement;
        break;

      }
    
    }
  return 0;
}

//Interrupt state machine
ISR(INT1_vect){
  //if pressed go to debounce state
  if(state_var == buzz){
    state_var = turn_off_buzzer;
    Serial.println("Button Pressed");
  }

}