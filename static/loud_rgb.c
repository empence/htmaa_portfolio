//based off of http://academy.cba.mit.edu/classes/embedded_programming/hello.arduino.328P.blink.c
//blinks the led on my hello world board

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <math.h>


#define output(directions,pin) (directions |= pin) // set port direction for output
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set
#define led_delay(mill) _delay_ms(mill) // LED delay

#define port_a PORTA
#define direction_a DDRA
#define port_b PORTB
#define direction_b DDRB
#define r1 (1 << PA0)
#define g1 (1 << PA1)
#define b1 (1 << PA2)


#define NPTS 400 // points in buffer


int clear_pin(pin){
   clear(port_a, pin);
   return 0;
}
int set_pin(pin){
   set(port_a, pin);
   
   return 0; 
}
int output_pin(pin){
   output(direction_a, pin);
   return 0; 
}
int initialize_output_pin(pin){
   clear_pin(pin);
   output_pin(pin);
   set_pin(pin);
   return 0;
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
   // initialize LED pin
   //

   ADMUX =  (1 << REFS1) | (0 << REFS0) | (0 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0); // ADC3
   ADCSRA = (1 << ADEN) // enable ADC. 
      | (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0); // prescaler /64

   initialize_output_pin(r1);
   initialize_output_pin(g1);
   initialize_output_pin(b1);

   //
   // main loop
   //
   int16_t adc_out = 0;
   double ema = 0;
   double alpha = .6;
   double emvar = 20;
   double delta = 0;
   double emstd = 0;

   while (1) {

         ADCSRA |= (1 << ADSC);
         //
         // wait for completion
         //
         while (ADCSRA & (1 << ADSC))
            ;
         //
         // save result
         //
         adc_out = ADCL + 256 * ADCH;

         delta = (double)adc_out - ema;
         ema = ema + alpha * delta;
         emvar = (1 - alpha) * (emvar + (alpha * delta * delta));
         emstd = sqrt(emvar);

         if (ema <= 750){
            clear_pin(r1);
            clear_pin(g1);
            clear_pin(b1);
         }
         else if (ema <= 800){
            set_pin(r1);
            clear_pin(g1);
            clear_pin(b1);
         }
         
         else if (ema <= 850){
            set_pin(r1);
            set_pin(g1);
            clear_pin(b1);
         }
         
         else if ( ema <= 900){
            clear_pin(r1);
            set_pin(g1);
            clear_pin(b1);}

         else if (ema <= 950){
            set_pin(g1);
            clear_pin(r1);
            set_pin(b1);
         }
         else if (ema <= 1000) {
            set_pin (b1);
            clear_pin(g1);
            clear_pin(r1);
         }

         else {
            set_pin(b1);
            set_pin(r1);
            clear_pin(g1);
         }
           

/*      if (rms <= 0){
         clear(port_b, r2);
         set(port_b, g2);
      }
      else if (rms <= 50){
         clear(port_b, r2);
         clear(port_b, g2);
      }
      else if (rms <= 1000){
         set(port_b, g2);
         set(port_b, r2);
      }
*/       
   }

   }      
