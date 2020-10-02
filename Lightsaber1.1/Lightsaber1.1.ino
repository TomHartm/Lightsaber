#include "FastLED.h"
#include "OneButton.h"

/////////////////////////////////////////INITIALISIERUNG/////////////////////////////////////////////////////////



#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS    100
#define switchPin 5
#define LED_PIN 11

CRGB leds[NUM_LEDS];

#define BRIGHTNESS          200   //alt: 250
#define FRAMES_PER_SECOND  120


const int buttonPin = 2;

int mode = 0;
int colour = 0;

//Startfabe
int green = 250;
int red = 0;
int blue = 0;
 
bool light_on = false;


  OneButton button(buttonPin, false);


/////////////////////////////////////////SETUP/////////////////////////////////////////////////////////


void setup() {
  pinMode (switchPin, INPUT);

  delay(1000); // 3 second delay for recovery

  Serial.begin(9600);

  button.attachDoubleClick(change_colour);
  button.attachClick(saber_switch);
  button.attachLongPressStart(change_colour);

  
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
/////////////////////////////////////////LOOP/////////////////////////////////////////////////////////
void loop()
{
  button.tick();
  delay(10);
}

/////////////////////////////////////////FUNKTIONEN/////////////////////////////////////////////////////////

void saber_switch(){
  if(!light_on){
  
  for (int i = 0; i<NUM_LEDS; i++)
      {
        leds[i].setRGB(red,green,blue);
        delay(7);
        FastLED.show();
      }
      light_on = true;
        
    }

  else{
    for (int i = NUM_LEDS-1; i>=0; i--)
        {
          leds[i].setRGB(0,0,0);
          delay(7);
          FastLED.show();
        }

    
    light_on = false;
  }
}
void change_mode(){
  mode++;
  if(mode == 3){mode = 0;}}

void change_colour(){
  colour++;
  if(colour == 7){colour = 0;}
  if(colour == 0){green = 250;red = 0; blue = 0;}
  
  else if(colour == 1){green = 0;red = 0; blue = 250;}
  else if(colour == 2){green = 0;red = 250; blue = 0;}
  else if(colour == 3){green = 250;red = 250; blue = 0;}
  else if(colour == 4){green = 150;red = 150; blue = 150;}
  else if(colour == 5){green = 250;red = 0; blue = 250;}
  else if(colour == 6){green = 150;red = 250; blue = 0;}



  if(light_on){
    for (int i = 0; i<NUM_LEDS; i++)
      {
        leds[i].setRGB(red,green,blue);
        
      }
    FastLED.show();}
}

void smooth(){
    red = sin8(millis());
    green = sin8(millis());
    blue = sin8(millis());}
