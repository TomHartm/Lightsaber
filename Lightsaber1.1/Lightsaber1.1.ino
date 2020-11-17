///SD///
//#include "SD.h"
//#define SD_ChipSelectPin 10
//#include "TMRpcm.h"
//#include "SPI.h"

//Gyro///
//#include "Wire.h"
//const int MPU = 0x68; 
//int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

//TMRpcm tmrpcm;

////ENDE SD////

#include "OneButton.h"
#include "WS2812FX.h"
#include "EEPROM.h"
#include "Arduino.h"
/////////////////////////////////////////INITIALISIERUNG/////////////////////////////////////////////////////////
#define NUM_LEDS    79

#define LED_PIN 11
#define buttonPin 2
#define buttonPin2 3
#define soundPin1 5
#define soundPin2 6
//Sound
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#define busy 9

//sound ende

uint32_t colorwheel[8] ={GREEN, BLUE, YELLOW,RED,CYAN,WHITE,ORANGE,PURPLE};

int mode = 0;
uint32_t colour = GREEN;
int color_count = 0;
bool light_on = false;
int BRIGHTNESS = 130;   //max: 255
bool men = false;
bool soundon = true;
bool mukke;
long zeit= 0.0;
OneButton button(buttonPin, false);
OneButton button2(buttonPin2, false);

  SoftwareSerial mySoftwareSerial(soundPin1,soundPin2); // RX, TX
DFRobotDFPlayerMini Player;

WS2812FX ws2812fx = WS2812FX(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

/////////////////////////////////////////SETUP/////////////////////////////////////////////////////////

void setup() {
      pinMode (busy, INPUT);
  mySoftwareSerial.begin(9600);
  Player.begin(mySoftwareSerial);
  Player.volume(25);
  pinMode (buttonPin, INPUT);
  pinMode (buttonPin2, INPUT);
  BRIGHTNESS = EEPROM.read(0);
 // delay(1000); // 1 second delay for recovery
 Serial.begin(9600);

 // tmrpcm.speakerPin=9;
 // tmrpcm.setVolume(7);

  button.attachDoubleClick(change_mode);
  button.attachClick(saber_switch);
  button.attachLongPressStart(change_colour);

  button2.attachDoubleClick(menue);
  

  ws2812fx.init();
  ws2812fx.setBrightness(BRIGHTNESS);
  ws2812fx.setSpeed(300);
for (int i = 0; i<NUM_LEDS; i++)
      {
          ws2812fx.setPixelColor(i,BLACK);
      }
  ws2812fx.show();      
 //delay(5000);
  ws2812fx.setColor(colour);

  saber_switch();

}
/////////////////////////////////////////LOOP/////////////////////////////////////////////////////////
void loop()
{ 
  button.tick();
  button2.tick();

  ws2812fx.service();
  Serial.println(digitalRead(busy));
  if(digitalRead(busy) == 1 && soundon && light_on && (millis()-zeit > 25)){   Player.play(3); zeit = millis(); }
}

/////////////////////////////////////////FUNKTIONEN/////////////////////////////////////////////////////////
void menue(){
  men = !men;
  if(men){
    button.attachClick(NULL);
    button.attachDoubleClick(NULL);
    button.attachLongPressStart(NULL);
    button.attachDuringLongPress(brightness_up_smooth);
    button2.attachDuringLongPress(brightness_down_smooth);

   // button.attachDoubleClick(brightness_up);
    //button2.attachClick(brightness_down);
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
    button.attachLongPressStart(change_colour);
    button2.attachClick(NULL);
    button2.attachDuringLongPress(NULL);
    EEPROM.write(0, BRIGHTNESS);

    //button2.attachDoubleClick(blablabla);
    ws2812fx.resume();
    //change_mode();
    }
}

void brightness_up_smooth(){
  if(BRIGHTNESS<250){BRIGHTNESS +=2;}
  ws2812fx.setBrightness(BRIGHTNESS);
  for(int i = 0; i < BRIGHTNESS/3; i++){ws2812fx.setPixelColor(i, ws2812fx.getColor());}
    ws2812fx.show();
    delay(30);
  }

void brightness_down_smooth(){
  if(BRIGHTNESS>20){BRIGHTNESS -=2;}
    ws2812fx.setBrightness(BRIGHTNESS);

    for (int i = BRIGHTNESS/3 -1; i<NUM_LEDS; i++){ws2812fx.setPixelColor(i, BLACK);}
    ws2812fx.show();
    delay(30);
  }

void brightness_up(){
    if(BRIGHTNESS<241){BRIGHTNESS +=10;}
    ws2812fx.setBrightness(BRIGHTNESS);

    //for (int i = 0; i<NUM_LEDS; i++){ws2812fx.setPixelColor(i, BLACK);}
    //ws2812fx.show();
    for(int i = 0; i < BRIGHTNESS/3; i++){ws2812fx.setPixelColor(i, ws2812fx.getColor());}
    ws2812fx.show();
    }

void brightness_down(){
    if(BRIGHTNESS>29){BRIGHTNESS -=10;}
    ws2812fx.setBrightness(BRIGHTNESS);

    for (int i = BRIGHTNESS/3 -1; i<NUM_LEDS; i++){ws2812fx.setPixelColor(i, BLACK);}
    ws2812fx.show();
    //for(int i = 0; i<BRIGHTNESS/2.5; i++){ws2812fx.setPixelColor(i, ws2812fx.getColor());}
    //ws2812fx.show();
    }

void saber_switch(){
  if(!light_on){
    light_on = true;
    if(soundon){Player.play(1);}
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
        if(soundon){Player.play(2);}
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
  else if(mode == 2){moving_dot();}
  else if(mode == 3){breath();}
  else if(mode == 4){randomc();}}


void change_colour(){
  color_count++;
  if(color_count == 8){color_count = 0;}
  colour = colorwheel[color_count];
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
  
void moving_dot(){
  ws2812fx.setSpeed(300);
  ws2812fx.setMode(FX_MODE_LARSON_SCANNER);
  ws2812fx.start();}

void breath(){
  ws2812fx.setSpeed(2000);
  ws2812fx.setMode(FX_MODE_RUNNING_LIGHTS);
  ws2812fx.start();}

void randomc(){
  ws2812fx.setSpeed(40000);
  ws2812fx.setMode(FX_MODE_RAINBOW);
  ws2812fx.start();}


//void sound(){}
