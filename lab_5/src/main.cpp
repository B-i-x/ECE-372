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
  turn_off_buzzer
} device_state;
//declare the enum state variable
volatile device_state state_var = wait_for_movement;
int godothing = 0;

int main() {
  sei(); 

  Serial.begin(9600); // using serial port to print values from I2C bus
  delayMs(100); // wait for serial to initialize

  initI2C(); // initialize I2C and set bit rate
  wake_mpu6050(); // wake up MPU6050 from sleep mode

  initSwitchJ1(); 

  spi_init(); // initialize SPI module and set the data rate
  screen_init(); // initialize 8 x 8 LED array (info from MAX7219 datasheet)
  write_happy_face();  
  delayMs(10);  // delay for 1 s to display "HI"
  
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
ISR(PCINT1_vect){
  //if pressed go to debounce state
  Serial.println("Button Pressed");
  if(state_var == buzz){
    state_var = turn_off_buzzer;
    Serial.println("Button Pressed");
  }

}