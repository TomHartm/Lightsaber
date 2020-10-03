#include "FastLED.h"
#include "OneButton.h"
#include "WS2812FX.h"

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

  WS2812FX ws2812fx = WS2812FX(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

/////////////////////////////////////////SETUP/////////////////////////////////////////////////////////


void setup() {
  pinMode (switchPin, INPUT);

  delay(1000); // 3 second delay for recovery

  Serial.begin(9600);

  button.attachDoubleClick(change_mode);
  button.attachClick(saber_switch);
  button.attachLongPressStart(change_colour);

  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5,4000); 

  ws2812fx.init();
  ws2812fx.setBrightness(150);
  ws2812fx.setSpeed(300);


  
  
for (int i = NUM_LEDS-1; i>=0; i--)
      {
        leds[i].setRGB(0,0,0);
        }
        FastLED.show();
      
  delay(500);
  saber_switch();
}
/////////////////////////////////////////LOOP/////////////////////////////////////////////////////////
void loop()
{
  button.tick();
  delay(10);
  if(mode != 0 && light_on){
  ws2812fx.service();}
  Serial.println(ws2812fx.getSpeed());
}

/////////////////////////////////////////FUNKTIONEN/////////////////////////////////////////////////////////

void saber_switch(){
  if(!light_on){
    light_on = true;
  for (int i = 0; i<NUM_LEDS; i++)
      {
        leds[i].setRGB(red,green,blue);
        delay(7);
        FastLED.show();
      }        
    }

  else{
    light_on = false;
    for (int i = NUM_LEDS-1; i>=0; i--)
        {
          leds[i].setRGB(0,0,0);
          delay(7);
          FastLED.show();
        }

  }
}
void change_mode(){
  mode++;
  if(mode == 5){mode = 0;}
  Serial.println(mode);
  if(mode == 0){static_mode();}
  else if(mode == 1){smooth();}
  else if(mode == 2){change_color();}
  else if(mode == 3){breath();}
  else if(mode == 4){randomc();}}

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

void refresh(){
    for (int i = 0; i<NUM_LEDS; i++)
      {
        leds[i].setRGB(red,green,blue);
        FastLED.show();
      }}

void static_mode(){refresh();
}

void smooth(){
  ws2812fx.setSpeed(300);
  ws2812fx.setMode(FX_MODE_RAINBOW_CYCLE);
  ws2812fx.start();}
  
void change_color(){
  ws2812fx.setSpeed(300);
  ws2812fx.setMode(FX_MODE_LARSON_SCANNER);
  ws2812fx.start();}

void breath(){
  ws2812fx.setSpeed(300);
  ws2812fx.setMode(FX_MODE_RUNNING_LIGHTS);
  ws2812fx.start();}

void randomc(){
  ws2812fx.setSpeed(18000);
  ws2812fx.setMode(FX_MODE_RAINBOW);
  ws2812fx.start();}



//void sound(){}
