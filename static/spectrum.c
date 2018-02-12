// spectrum.c 
// communicate with a computer over serial (FTDI cable), to choose the color of an onboard RGB LED on pins 5, 6 and 7. 
// baud 115200 
// set lfuse to 0x5E for 20 MHz xtal
// based on code by Neil Gershenfeld 
//
// (c) Massachusetts Institute of Technology 2010
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose. Copyright is
// retained and must be preserved. The work is provided
// as is; no warranty is provided, and users accept all 
// liability.
//

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#define output(directions,pin) (directions |= pin) // set port direction for output
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set
#define bit_delay_time 8.5 // bit delay for 115200 with overhead
#define bit_delay() _delay_us(bit_delay_time) // RS232 bit delay
#define half_bit_delay() _delay_us(bit_delay_time/2) // RS232 half bit delay
#define char_delay() _delay_ms(10) // char delay

#define serial_port PORTA
#define serial_direction DDRA

#define serial_pins PINA
#define serial_pin_in (1 << PA0)
#define serial_pin_out (1 << PA1)
#define serial_interrupt (1 << PCIE0)
#define serial_interrupt_pin (1 << PCINT0)

#define red (1 << PA6)
#define green (1 << PB2)
#define blue (1 << PA7)

#define led_port_b PORTB
#define led_direction_b DDRB

void get_char(volatile unsigned char *pins, unsigned char pin, char *rxbyte, char wait) {
   //
   // read character into rxbyte on pins pin
   //    assumes line driver (inverts bits)
   //
   *rxbyte = 0;
   if (wait == 1){
   while (pin_test(*pins,pin))
      //
      // wait for start bit
      //
      ;
   }
   //
   // delay to middle of first data bit
   //
   half_bit_delay();
   bit_delay();
   //
   // unrolled loop to read data bits
   //
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 0);
   else
      *rxbyte |= (0 << 0);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 1);
   else
      *rxbyte |= (0 << 1);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 2);
   else
      *rxbyte |= (0 << 2);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 3);
   else
      *rxbyte |= (0 << 3);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 4);
   else
      *rxbyte |= (0 << 4);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 5);
   else
      *rxbyte |= (0 << 5);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 6);
   else
      *rxbyte |= (0 << 6);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 7);
   else
      *rxbyte |= (0 << 7);
   //
   // wait for stop bit
   //
   bit_delay();
   half_bit_delay();
   }

 
// pin change interrupt handler
ISR(PCINT0_vect) {

   static char chr;
   get_char(&serial_pins, serial_pin_in, &chr, 1);
   if (chr == 'r'){
      clear(serial_port, red);
      set(serial_port, blue);
      set(led_port_b, green);
   }
   else if (chr == 'y'){
      clear(serial_port, red);
      set(serial_port, blue);
      clear(led_port_b, green);
   }
   else if (chr == 'g'){
      set(serial_port, red);
      set(serial_port, blue);
      clear(led_port_b, green);
   }
   else if (chr == 'c'){
      set(serial_port, red);
      clear(serial_port, blue);
      clear(led_port_b, green);
   }
   else if (chr == 'b'){
      set(serial_port, red);
      clear(serial_port, blue);
      set(led_port_b, green);
   }
   else if (chr == 'v'){
      clear(serial_port, red);
      clear(serial_port, blue);
      set(led_port_b, green);
   }
   else{  //do white
      clear(serial_port, red);
      clear(serial_port, blue);
      clear(led_port_b, green);
   }
   return;

   }

int main(void) {

   //
   // main
   //
   // set clock divider to /1
   //
   CLKPR = (1 << CLKPCE);
   CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
   //
   // initialize serial output pins
   //
   set(serial_port, serial_pin_out);
   output(serial_direction, serial_pin_out);
   //initialize LED output pins
   set(serial_port, red);
   output(serial_direction, red);
   set(serial_port, blue);
   output(serial_direction, blue);
   set(led_port_b, green);
   output(led_direction_b, green);
   //
   // set up pin change interrupt on Rx pin (pin 13) and enable global interrupts
   //
   set(GIMSK, serial_interrupt);
   set (PCMSK0, serial_interrupt_pin);
   sei();
   
   //
   // main loop
   //
   while (1) {
      //do nothing in particular
      }
   }