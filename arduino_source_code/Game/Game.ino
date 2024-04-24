#include "SerialMP3Player.h"
#include <FastLED.h>
//#include <PCM.h>

//LED stuff 
#define LED_PIN 47
#define NUM_LEDS 50

//pin definitions 
#define TX 11 // but actually connect RX to 11 
#define RX 10 // and TX to 10 
#define RED_BUTTON 31 //24
#define ORANGE_BUTTON 30 //26
#define YELLOW_BUTTON 28 //28
#define GREEN_BUTTON 29 //30
#define BLUE_BUTTON 26 //32
#define PURPLE_BUTTON 27 //34

//mp3 file definitions

#define GREEN_TRACK 1
#define BLUE_TRACK 2
#define RED_TRACK 3 
#define ORANGE_TRACK 4
#define YELLOW_TRACK 5
#define PURPLE_TRACK 6 
#define PROMPT_YELLOW 7 
#define PROMPT_PURPLE 8 
#define PROMPT_ORANGE 9
#define TRY_AGAIN 10 
#define COME_LET_US 11 
#define CRAYON_TO_PAPER 12 
#define COLORS_IN_BOX 13 
#define HI_MARIN_LETS_PLAY 14 
#define PROMPT_GREEN 15 
#define HI_MARIN_DO_YOU_WANT 16 
#define GOOD_JOB 17 
#define PROMPT_RED 18 
#define PROMPT_BLUE 19 

#define RED_START 0
#define ORANGE_START 8
#define YELLOW_START 16 
#define GREEN_START 24
#define BLUE_START 32 
#define PURPLE_START 40


#define STARTUP_TIME 20 //20sec for startup before game starts playing 
#define LETS_PLAY_LENGTH 200 
#define COLOR_SONG_LENGTH 300 //35 sec for color song instrumental 
#define PROMPT_WINDOW 15000

//void red(); void orange(); void yellow(); void green(); void blue(); void purple(); 

SerialMP3Player mp3(RX, TX);

CRGB leds[NUM_LEDS];

CRGB::HTMLColorCode COLORS[6] = {CRGB::Red, CRGB::OrangeRed, CRGB::YellowOrange, CRGB::Green, CRGB::Blue, CRGB::Purple};
int COLOR_TRACKS[6] = {RED_TRACK, ORANGE_TRACK, YELLOW_TRACK, GREEN_TRACK, BLUE_TRACK, PURPLE_TRACK};
int COLOR_BUTTONS[6] = {RED_BUTTON, ORANGE_BUTTON, YELLOW_BUTTON, GREEN_BUTTON, BLUE_BUTTON, PURPLE_BUTTON};
String COLOR_NAMES[6] = {"red", "orange", "yellow", "green", "blue", "purple"}; 
int COLOR_BOUNDS[6] = {RED_START, ORANGE_START, YELLOW_START, GREEN_START, BLUE_START, PURPLE_START}; 
int COLOR_OPPOSITE_LOCS[6] = {(GREEN_START + 4), (BLUE_START + 4), (PURPLE_START + 4), (RED_START + 4), (ORANGE_START + 4), (YELLOW_START + 4)};
typedef void (*ColorFunction)();
int COLOR_PROMPTS[6] = {PROMPT_RED, PROMPT_ORANGE, PROMPT_YELLOW, PROMPT_GREEN, PROMPT_BLUE, PROMPT_PURPLE};
int WELCOME_MESSAGES[2] = {HI_MARIN_LETS_PLAY, HI_MARIN_DO_YOU_WANT};
int COLOR_SONG[3] = {COME_LET_US, CRAYON_TO_PAPER, COLORS_IN_BOX};

char c;  // char from Serial
char cmd = ' ';
char cmd1 = ' ';
static int target_color; 
static int last_prompt; 
static bool prompt_mode; 
int track_num;
static uint8_t timer;
static int turn_off_time; 

void setup() {
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.clear();
  FastLED.show();
  
  for(int i = 0; i < 6; i++){
    pinMode(COLOR_BUTTONS[i], INPUT);
  }
  
  mp3.showDebug(1);       // print what we are sending to the mp3 board.

  Serial.begin(9600);     // start serial interface
  mp3.begin(9600);        // start mp3-communication
  delay(100);             // wait for init

  mp3.sendCommand(CMD_SEL_DEV, 0, 2);   //select sd-card
  delay(100);             // wait for init

  randomSeed(analogRead(3));
  
  startup(); 
  
}




// the loop function runs over and over again forever
void loop() {
//  delay(50); 

  timer = millis();

  if(timer > turn_off_time){
    FastLED.clear();
  }

  if(timer > 10000 && timer < 20000 && prompt_mode == false) {
    prompt_mode = true; 
    prompt(3);
  }

  if(timer > 20000 && timer < 30000 && prompt_mode == false) {
    prompt_mode = true; 
    prompt(1);
  }

  if(prompt_mode == false) {
    for(int color_id = 0; color_id < 5; color_id++){
      if(digitalRead(COLOR_BUTTONS[color_id]) == HIGH){
        Serial.print("color pressed non prompt mode - ");
        Serial.println(COLOR_NAMES[color_id]);
        color_func(color_id);
      }
    } 
    if(digitalRead(RED_BUTTON) == HIGH && digitalRead(PURPLE_BUTTON) == HIGH){
      Serial.println("fine okay purple"); 
      color_func(5);
    }
  }
   
 
  if (Serial.available()) {
    c = Serial.read();
    Serial.print(c);
    decode_c(); // Decode c.
    Serial.print("done decoding: ");
    Serial.println(c);
  }
  // Check for the answer.
  if (mp3.available()) {
    Serial.println(mp3.decodeMP3Answer()); // print decoded answers from mp3
  } 



  if(prompt_mode == false) {
    //if not in prompt_mode, normal gameplay 
//    for(int color_id = 0; color_id < 5; color_id++){
//      if(digitalRead(COLOR_BUTTONS[color_id]) == HIGH){
//        Serial.print("time: ");
//        Serial.println(timer);
//        color_func(color_id);
//      }
//    }

    //if it's been a while since we last prompted, prompt 
//    if(timer > last_prompt + PROMPT_WINDOW){
//      prompt_mode = true; 
//      last_prompt = timer; 
//      Serial.print("time to prompt again... prompting at time: ");
//      Serial.println(timer);
//      prompt(random(0,5));
//    } 
  }


  if(prompt_mode == true){
      if(digitalRead(COLOR_BUTTONS[target_color]) == HIGH){
        prompt_mode = false;
        celebrate(target_color); //blocking 
        Serial.println("yay Marin!");
      }
      for(int color_id = 0; color_id < 5; color_id++){
        if(digitalRead(COLOR_BUTTONS[color_id]) == HIGH && color_id != target_color){
          Serial.print("detected color: ");
          Serial.println(COLOR_NAMES[color_id]);
          mp3.play(COLOR_TRACKS[color_id]);
          delay(1000);
          mp3.play(TRY_AGAIN);
          blocking_guide(target_color); //guide to target color 
        }
      }

//      if(timer > last_prompt + PROMPT_WINDOW){
//        //prompt expired 
//        Serial.print("exiting prompt mode at time: ");
//        Serial.println(timer);
//        last_prompt = timer; 
//        prompt_mode = false; 
//      }
  }


 

} // end loop()




void startup() {
  Serial.println("startup!!"); 
  rainbow_all();
  mp3.play(WELCOME_MESSAGES[random(0,1)]);
  target_color = random(0,5);
  delay(2000);
//  blocking_guide(target);
  last_prompt = millis();
  prompt_mode = true; 
  prompt(target_color);
  Serial.println("done startup :)");
  return; 
}

void blocking_guide(int color_id) {
  CRGB::HTMLColorCode color = COLORS[color_id];
  int start_loc = COLOR_OPPOSITE_LOCS[color_id];
  FastLED.clear();
  FastLED.show();
  int delay_time = 10; 
  int right_led = start_loc+1;
  int left_led = start_loc;  
  
  if(left_led > right_led){
    Serial.println("uh oh");
  } else Serial.println("yay ");
  
  for(int i = 0; i <= (NUM_LEDS/2); i++){
    leds[left_led] = CRGB(color);
    leds[right_led] = CRGB(color);
    right_led ++;
    left_led --;
    if(left_led < 0){
      left_led = NUM_LEDS;
    } 
    if(right_led > NUM_LEDS){
      right_led = 0;
    }
    FastLED.show();
    delay(100);
    
  }
  
}

void prompt(int target_color){
  Serial.println(target_color);
  mp3.play(COLOR_PROMPTS[target_color]);
  all_color(target_color);
  delay(1000);
  FastLED.clear();
  FastLED.show();
}

void blink(int color_id) {
  CRGB::HTMLColorCode color = COLORS[color_id];
  static uint8_t wait = 50;
  static unsigned long lastUpdate = 0;
  static int currentLed = 0; 
 
  unsigned long now = millis();
  if (now > lastUpdate+delay) {
    leds[currentLed] = color;
    FastLED.show();   
    currentLed = currentLed>NUM_LEDS ? 0 : currentLed+1;
    lastUpdate = now;
  }
}

void celebrate(int color_id){
  all_color(color_id);
  mp3.play(GOOD_JOB);
  delay(1000);
  mp3.play(COLOR_TRACKS[color_id]);
  delay(500);
  mp3.play(COLOR_SONG[0]);
  delay(4000);
  mp3.play(COLOR_TRACKS[color_id]);
  delay(200);
  mp3.play(COLOR_SONG[1]);
  delay(11000);
  mp3.play(COLOR_TRACKS[color_id]);
  delay(200);
  mp3.play(COLOR_SONG[2]);
  delay(5000);
  FastLED.clear();
  FastLED.show();
  return; 
}


void rainbow_all(){
  for(int i = 0; i< NUM_LEDS; i++){
    if( i >= RED_START && i < ORANGE_START){
       leds[i] = CRGB(255,0,0);
    } else if(i >= ORANGE_START && i < YELLOW_START){
//      leds[i] = CRGB(255,140,0); 
       leds[i] = CRGB::Orange;
    } else if(i >= YELLOW_START && i < GREEN_START) {
       leds[i] = CRGB(255,255,0);
    } else if(i >= GREEN_START && i < BLUE_START) {
       leds[i] = CRGB(0,255,0);
    } else if(i >= BLUE_START && i < PURPLE_START) {
       leds[i] = CRGB(0,0,255);
    } else if(i >= PURPLE_START && i < NUM_LEDS) {
       leds[i] = CRGB(200,0,200);
    }  
    FastLED.show();
  }
}

void all_color(int color_id){
  CRGB::HTMLColorCode color_code = COLORS[color_id];
  FastLED.clear();
  FastLED.setBrightness(100);
  for(int i = 0; i< NUM_LEDS; i++){
    leds[i] = color_code; 
    FastLED.show();
  }
  return; 
}

void color_func(int color_id){
    Serial.print("prompt mode? ");
    Serial.print(prompt_mode);
    Serial.print("  color: ");
    Serial.println(COLOR_NAMES[color_id]);
    all_color(color_id);
    mp3.play(COLOR_TRACKS[color_id]);
    return;
}


void menu(char op, int nval) {
  // Menu
  switch (op) {
    case '\n':
      Serial.println("new line");
      break;
    case '?':
    case 'h':
      Serial.println("SerialMP3Player Basic Commands:");
      Serial.println(" ? - Display Menu options. ");
      Serial.println(" P01 - Play 01 file");
      Serial.println(" F01 - Play 01 folder");
      Serial.println(" S01 - Play 01 file in loop");
      Serial.println(" V01 - Play 01 file, volume 30");
      Serial.println(" p - Play");
      Serial.println(" a - pause");
      Serial.println(" s - stop ");
      Serial.println(" > - Next");
      Serial.println(" < - Previous");
      Serial.println(" + - Volume UP");
      Serial.println(" - - Volume DOWN");
      Serial.println(" v15 - Set Volume to 15");
      Serial.println(" c - Query current file");
      Serial.println(" q - Query status");
      Serial.println(" x - Query folder count");
      Serial.println(" t - Query total file count");
      Serial.println(" r - Reset");
      Serial.println(" e - Sleep");
      Serial.println(" w - Wake up");
      break;

    case 'p':
//      Serial.println("play fr");
//      mp3.play();
      prompt_mode = true;
      target_color = random(0,5);
      prompt(target_color);
      break;

    case 'e': 
      prompt_mode = false;
      break;

    case 'a':
      Serial.println("Pause");
      mp3.pause();
      break;

    case 's':
      Serial.println("Stop");
      mp3.stop();
      break;


    case 'r': 
      color_func(0);
//      red
      break;
      Serial.println("red");
      mp3.play(RED_TRACK);
      break;

    case 'o': 
//      orange();
      color_func(1);
      break;
      Serial.println("orange from Serial");
      mp3.play(ORANGE_TRACK);
      Serial.println("done orange");
      break;

    case 'y': 
      color_func(2);
//      yellow();
      break; 

    case 'g':
      color_func(3); 
//      green();
      break;
      Serial.println("green"); 
      mp3.play(GREEN_TRACK);
      break; 

    case 'b':
      color_func(4);
//      blue();
      break;
      Serial.println("blue");
      mp3.play(BLUE_TRACK);
      break;

    case 'u': 
      color_func(5);
//      purple();
      break;
      Serial.println("purple"); 
      mp3.play(PURPLE_TRACK);
      break; 

    case 'c':
      FastLED.clear();
      break;

    case 'l':
      celebrate(random(0,5));
      break;
  }
  return;
}

void decode_c() {
  // Decode c looking for a specific command or a digit

  // if c is a 'v', 'P', 'F', 'S' or 'V' wait for the number XX
  if (c == 'v' || c == 'P' || c == 'F' || c == 'S' || c == 'V') {
    cmd = c;
  } else {
    // maybe c is part of XX number
    if (c >= '0' && c <= '9') {
      // if c is a digit
      if (cmd1 == ' ') {
        // if cmd1 is empty then c is the first digit
        cmd1 = c;
      } else {
        // if cmd1 is not empty c is the second digit
        menu(cmd, ((cmd1 - '0') * 10) + (c - '0'));
        cmd = ' ';
        cmd1 = ' ';
      }
    } else {
      // c is not a digit nor 'v', 'P', 'F' or 'S' so just call menu(c, nval);
      menu(c, 0);
    }
  }
}

/*if(digitalRead(RED_BUTTON) == HIGH){
    Serial.println("red button");
    red(); 
  }

  if(digitalRead(ORANGE_BUTTON) == HIGH){
    Serial.println("orange button");
    orange();
  } 

  if(digitalRead(YELLOW_BUTTON) == HIGH){
    Serial.println("yellow button");
    yellow();
  }

  if(digitalRead(GREEN_BUTTON) == HIGH){
    Serial.println("green button");
    green();
  }

  if(digitalRead(BLUE_BUTTON) == HIGH){
    Serial.println("blue button");
    blue(); 
  }
  
  if(digitalRead(PURPLE_BUTTON) == HIGH){
    Serial.println("purple button");
    purple(); 
  }

  if(Serial.available()){
    int old_track_num = track_num;
    track_num = (int)Serial.read() - 64;
    if(track_num >= 0 && track_num < 26){
        Serial.println(track_num); 
        mp3.play(track_num);
    } 
    if(track_num == (110 - 64)) {
      old_track_num++;
      Serial.println(old_track_num); 
      mp3.play(old_track_num);
    }

  }
  */
