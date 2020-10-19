#include <Arduino.h>
#include <HCSR04.h>
#include <FastLED.h>

HCSR04 hc(2,3);//initialisation class HCSR04 (trig pin , echo pin)

#define LED_PIN     4
#define NUM_LEDS    10
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];


bool too_close = false;
float alpha = 0.8;
float distance = 0;
float threshold_l = 100;
float threshold_h = 120;
unsigned long time_threshold = 1000;
unsigned long last_far;
unsigned long last_close; 

void setup()
{ Serial.begin(9600); 
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
}

void loop()
{ 
  distance = alpha * distance + (1 - alpha) * hc.dist();
  if (distance <= threshold_l)
  {
    last_far = millis();
  }

  if (distance >= threshold_h)
  {
    last_close = millis();
  }

  if ((millis() - last_close) > time_threshold/2.00)
  {
      too_close = true;
  }

  if ((millis() - last_far) > time_threshold)
  {
    too_close = false;
  }

  if (too_close){
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Red;
    }
  }  
  else
  {
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Green;
    }
    
  }
  FastLED.show();

  Serial.print( distance); 
  Serial.print("   ");
  Serial.println(too_close);
  delay(50);
  }//return curent distance in serial
  