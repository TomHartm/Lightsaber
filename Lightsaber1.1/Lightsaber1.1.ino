#include "OneButton.h"
#include "WS2812FX.h"

/////////////////////////////////////////INITIALISIERUNG/////////////////////////////////////////////////////////

//#define LED_TYPE    WS2812
//#define COLOR_ORDER GRB
#define NUM_LEDS    100
//#define switchPin 5
#define LED_PIN 11

//#define FRAMES_PER_SECOND  60

#define buttonPin 2
#define buttonPin2 3

int mode = 0;
uint32_t colour = GREEN;
int color_count = 0;
bool light_on = false;
int BRIGHTNESS = 150;   //alt: 250

//Startfabe
//byte green = 250;
//byte red = 0;
//byte blue = 0;

bool men = false;

OneButton button(buttonPin, false);
OneButton button2(buttonPin2, false);

WS2812FX ws2812fx = WS2812FX(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

/////////////////////////////////////////SETUP/////////////////////////////////////////////////////////


void setup() {
  pinMode (buttonPin, INPUT);
  pinMode (buttonPin2, INPUT);


 // delay(1000); // 1 second delay for recovery
 // Serial.begin(9600);

  button.attachDoubleClick(change_mode);
  button.attachClick(saber_switch);
  button.attachLongPressStart(change_colour);

  button2.attachLongPressStart(menue);
  button2.attachClick(brightness_up);

  ws2812fx.init();
  ws2812fx.setBrightness(BRIGHTNESS);
  ws2812fx.setSpeed(300);
  
for (int i = 0; i<NUM_LEDS; i++)
      {
          ws2812fx.setPixelColor(i,BLACK);
      }
  ws2812fx.show();      
 // delay(500);
  ws2812fx.setColor(colour);
  saber_switch();
}
/////////////////////////////////////////LOOP/////////////////////////////////////////////////////////
void loop()
{
  button.tick();
  button2.tick();

  ws2812fx.service();  
}

/////////////////////////////////////////FUNKTIONEN/////////////////////////////////////////////////////////
void menue(){ men = !men;
  if(men){
    button.attachClick(brightness_up);
    button.attachDoubleClick(brightness_up);
    button2.attachClick(brightness_down);
    //button2.attachDoubleClick(blalbabla);
    ws2812fx.pause();
    for (int i = 0; i<NUM_LEDS; i++){ws2812fx.setPixelColor(i,BLACK);}
    ws2812fx.show();
    for(int i = 0; i<BRIGHTNESS/2.5; i++){ws2812fx.setPixelColor(i, colour);}
    ws2812fx.show();
    }
  else{
    button.attachClick(saber_switch);
    button.attachDoubleClick(change_mode);
    //button2.attachDoubleClick(blablabla);
    ws2812fx.resume();
    //change_mode();
    }
}

void brightness_up(){
    if(BRIGHTNESS<240){BRIGHTNESS +=10;}
    ws2812fx.setBrightness(BRIGHTNESS);

    //for (int i = 0; i<NUM_LEDS; i++){ws2812fx.setPixelColor(i, BLACK);}
    //ws2812fx.show();
    for(int i = 0; i<BRIGHTNESS/2.5; i++){ws2812fx.setPixelColor(i, ws2812fx.getColor());}
    ws2812fx.show();
    }

void brightness_down(){
    if(BRIGHTNESS>30){BRIGHTNESS -=10;}
    ws2812fx.setBrightness(BRIGHTNESS);

    for (int i = BRIGHTNESS/2.5 -1; i<NUM_LEDS; i++){ws2812fx.setPixelColor(i, BLACK);}
    ws2812fx.show();
    //for(int i = 0; i<BRIGHTNESS/2.5; i++){ws2812fx.setPixelColor(i, ws2812fx.getColor());}
    //ws2812fx.show();
    }

void saber_switch(){
  if(!light_on){
    light_on = true;
    ws2812fx.setColor(colour);
  for (int i = 0; i<NUM_LEDS; i++)
      {
        ws2812fx.setPixelColor(i, colour);
        delay(7);
        ws2812fx.show();  
      }            
    }

  else{
    light_on = false;
    colour = ws2812fx.getColor();
    ws2812fx.setColor(BLACK);
    ws2812fx.pause();
    for(int i = NUM_LEDS-1; i>=0; i--){
        
        ws2812fx.setPixelColor(i, BLACK);
        delay(7);
        ws2812fx.show();
    }
  }
}

void change_mode(){
  mode++;
  if(mode == 5){mode = 0;}
  //Serial.println(mode);
  if(mode == 0){static_mode();}
  else if(mode == 1){smooth(); light_on = true;}
  else if(mode == 2){change_color();}
  else if(mode == 3){breath();}
  else if(mode == 4){randomc();}}

void change_colour(){
  color_count++;
  if(color_count == 8){color_count = 0;}
  if(color_count == 0){colour = GREEN;} 
  else if(color_count == 1){colour = BLUE;}
  else if(color_count == 2){colour = YELLOW;}
  else if(color_count == 3){colour = RED;}
  else if(color_count == 4){colour = CYAN;}
  else if(color_count == 5){colour = WHITE;}
  else if(color_count == 6){colour = ORANGE;}
  else if(color_count == 7){colour = RED;}
  ws2812fx.setColor(colour);
  ws2812fx.start();
  ws2812fx.show();
  }

void static_mode(){
      ws2812fx.setMode(FX_MODE_STATIC);
      ws2812fx.start();}

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
