#include <avr/io.h>
#include <util/delay.h>

int short_delay = 2;
int long_delay = 100;

#define output(directions,pin) (directions |= pin) // set port direction for output
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set
#define led_delay(delay) _delay_ms(delay) // LED delay

#define red (1 << PA6)
#define green (1 << PB2)
#define blue (1 << PA7)

#define led_port_a PORTA
#define led_direction_a DDRA
#define led_port_b PORTB
#define led_direction_b DDRB


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
   clear(led_port_a, blue );
   output(led_direction_a, blue);
   clear(led_port_a, blue );

   clear(led_port_b, green );
   output(led_direction_b, green);
   clear(led_port_b, green );

   clear(led_port_a, red );
   output(led_direction_a, red);
   clear(led_port_a, red );

   //
   // main loop
   //
   while (1) {
      
      //Splits up time into period millisecond segments
      // The percentage of the time spent on one color determines the perceived color
      int period = 5;
      float frac_blue = .3;
      float frac_red = 0;
      float frac_green = .7;
      int i = 0;
      while(1){
         for (i = 0; i < period; i++){
            set(led_port_a, blue);
            clear(led_port_a, red);
            led_delay(frac_red*period);
            set(led_port_a, red);
            clear(led_port_b, green);
            led_delay(frac_green*period);
            set(led_port_b, green);
            clear(led_port_a, blue);
            led_delay(frac_blue*period);
      }
      }
      }
   }