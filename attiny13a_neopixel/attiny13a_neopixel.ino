/*
 * ATTINY13A neopixel
 *  Ref: https://github.com/fbiego/attiny13a_neopixel_ir.git
 *  WS2812 >> PIN 0
 */

#define F_CPU 9600000 // Must stay at this speed

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "light_ws2812.c"

#define  PIXEL_NUM  (8)     //number of ws2812 leds
#define  BRIGHTNESS (10)    //brightness 0-255
#define  DELAY_TIME (400)   //ms

struct pixel {
  uint8_t g;
  uint8_t r;
  uint8_t b;
};

struct pixel p[PIXEL_NUM];

void my_delay(uint16_t ms) {
  while (0 < ms)
  {
    _delay_ms(1); //must be fixed parameter in compile time.
    --ms;
  }
}

void set_all_off(void) {
  for (int n=0; n<PIXEL_NUM; n++) {
     p[n].r = 0;
     p[n].g = 0;
     p[n].b = 0;
  }
  ws2812_setleds((struct cRGB *)&p, PIXEL_NUM);
  my_delay(DELAY_TIME);
}

void setleds_act(uint8_t r, uint8_t g, uint8_t b) {

  for (int n=0; n<PIXEL_NUM; n=n+2) {
     p[n].r = r;
     p[n].g = g;
     p[n].b = b;
  }
  ws2812_setleds((struct cRGB *)&p, PIXEL_NUM);
  my_delay(DELAY_TIME);

  for (int n=0; n<PIXEL_NUM; n=n+2) {
     p[n].r = g;
     p[n].g = b;
     p[n].b = r;
  }
  ws2812_setleds((struct cRGB *)&p, PIXEL_NUM);
  my_delay(DELAY_TIME);

}

void setleds(uint8_t r, uint8_t g, uint8_t b) {

  set_all_off();

  //sequencial lighting
  for (int n=0; n<PIXEL_NUM; n++) {
     p[n].r = r;
     p[n].g = g;
     p[n].b = b;
     ws2812_setleds((struct cRGB *)&p, PIXEL_NUM);
     my_delay(DELAY_TIME);
  }

  //pattern lighting
  for (int n=0; n<3; n++) {
    setleds_act(r, g, b);
  }
}

int main(void) {
  while(1){
    setleds(BRIGHTNESS, 0, 0);
    my_delay(DELAY_TIME);

    setleds(0, BRIGHTNESS, 0);
    my_delay(DELAY_TIME);

    setleds(0, 0, BRIGHTNESS);
    my_delay(DELAY_TIME);
  }
}
