#include "FastLED.h"
#include "OneButton.h"


#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define LED_PIN 11


#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    100

#define switchPin 5

CRGB leds[NUM_LEDS];

#define BRIGHTNESS          100   //alt: 250
#define FRAMES_PER_SECOND  120


const int buttonPin = 2;

int buttonState = LOW;
int mode = 0;
int buttonpressed = 0;
int entprellzeit = 200;
unsigned long buttontime = 0;

int green = 250;
int red = 0;
int blue = 0;
 
bool light_on = false;

void saber_switch();

  OneButton button(buttonPin, false);




void setup() {
  pinMode (switchPin, INPUT);

  delay(1000); // 3 second delay for recovery

  Serial.begin(9600);

  button.attachDoubleClick(change_colour);
  button.attachClick(saber_switch);

  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5,3000); 

for (int i = NUM_LEDS-1; i>=0; i--)
      {
        leds[i].setRGB(0,0,0);
        }
        FastLED.show();
      
  delay(1000);
  saber_switch();
}

uint8_t gHue = 150; // rotating "base color" used by many of the patterns
  
void loop()
{
  button.tick();
  delay(10);
  /*buttonState = digitalRead(buttonPin);
  Serial.print(mode);

  if(buttonState == HIGH)
    {
      buttontime = millis();
      buttonpressed = 1;    
     }
   if((millis() - buttontime > entprellzeit) && buttonpressed == 1)
    {
      mode++;
      buttonpressed = 0;
      if(mode == 3) {mode = 0;}
     }
    
   if((millis() - buttontime > entprellzeit) && buttonpressed == 1)
    {
      saber_switch(); 
      buttonpressed = 0;
    }


    if(mode == 0){green = 250;red = 0; blue = 0;}
    else if(mode == 1){green = 0;red = 0; blue = 250;}
    else if(mode == 2){green = 0;red = 250; blue = 0;}
    if(light_on){change_colour();}*/
}
void change_colour(){
  mode++;
  if(mode ==3){mode = 0;}
  if(mode == 0){green = 250;red = 0; blue = 0;}
    else if(mode == 1){green = 0;red = 0; blue = 250;}
    else if(mode == 2){green = 0;red = 250; blue = 0;}
    if(light_on){
  for (int i = 0; i<NUM_LEDS; i++)
      {
        leds[i].setRGB(red,green,blue);
        
      }
    FastLED.show();}
}
void saber_switch(){
  if(!light_on){
  
  for (int i = 0; i<NUM_LEDS; i++)
      {
        leds[i].setRGB(red,green,blue);
        delay(10);
        FastLED.show();
      }
      light_on = true;
        
    }

  else{
    for (int i = NUM_LEDS-1; i>=0; i--)
        {
          leds[i].setRGB(0,0,0);
          delay(10);
          FastLED.show();
         // red=0; green=0; blue=0;
        }

    
    light_on = false;
  }
}
  
