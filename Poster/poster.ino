#include <FastLED.h>

#define NUM_LEDS 85
#define NUM_STREAMS 3
#define DATA_PIN 3

// easier like this than struct?
#define STREAM1 0
#define STREAM2 1
#define STREAM3 2

#define MAX_BRIGHTNESS 50
#define TWINKLE_MAX_B 50

// Define the array of leds
CRGB leds[NUM_LEDS];

// led brightness
int current_state[NUM_LEDS];
int target_state[NUM_LEDS];

int min_brightness = 30; 

// led indexes
int stream1_start = 0;
int stream1_end = 19;
int stream2_start = 20;
int stream2_end = 41;
int stream3_start = 42;
int stream3_end = 62;
int splash_start = 63;
int splash_end = 73;
int pond_start = 74;


// flag to make sure the same streams not restarted while running
bool stream1_active;
bool stream2_active;
bool stream3_active;

// this holds the current led thats being iterated on
int stream1_led = 0;
int stream2_led = 20;
int stream3_led = 42;

// brightness percentages of the leds that create the "stream" 
float trail3_brit = 0.35;
float trail2_brit = 0.50;
float trail1_brit = 0.70;
float lead1_brit = 0.60;
float lead2_brit = 0.30;

// when a stream finishes, a random waiting period is selected before that stream will restart
//unsigned long stream1_start_time = millis() + random(2000,20000);
//unsigned long stream2_start_time = millis() + random(2000,20000);
//unsigned long stream3_start_time = millis() + random(2000,20000);

// occasionally one of the streams sends down a twinkling led.
// this will track which stream has it and where it currently is
// int glory_ball_stream = 99;
// int glory_ball_led = 99;

// seems the program cant handle 3 streams... so try only running onee at a time
int last_stream_run = 9;
bool stream_is_active = 9;
int active_stream = 9;
  

// used by incrementor
int _step; 
int color = 0;
int start;
int _end;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


// loop thru all of the leds to compare their current
// state to target state and inc/decrement if needed. 

void the_incrementor() {

  
  // get the start and end points otherwise this things too slow

  // NOTE: when I switch back to multi stream activity I start and _end need to cover all streams
  switch (active_stream) {      
      case STREAM1:
        start = stream1_start;
        _end = stream1_end;
        break;
      case STREAM2:
        start = stream2_start;
        _end = stream2_end;
        break;
      case STREAM3:
        start = stream3_start;
        _end = stream3_end;
        break;
  }

   for (int i = start; i <= _end; i++) {
   
    if (i > stream3_end) {
      color = random(255);
      _step = 13;
    } else {
      _step = 2;
    }
         
    if (target_state[i] > current_state[i]) {
      current_state[i] += _step;
      leds[i] = CHSV(color, 0, current_state[i]);
    }
    
    if (target_state[i] < current_state[i]) {
      current_state[i] -= _step;
      leds[i] = CHSV(color, 0, current_state[i]);
    }
     
    // if this is a pond led it turns off without fading out
    if (i > stream3_end && target_state[i] == current_state[i]) {        
      current_state[i] = 0; 
      leds[i] = CHSV(0, 0, current_state[i]);
//      last_twinkle_time = millis(); // save current time
    }
    
    FastLED.show(); 
   }
}

void(* resetFunc) (void) = 0;
  

// when an led hits its target brightness, this function
// will kick the iterator onto the next led and adjust the targets 
// of the other leds in the stream accordingly. once the iterator 
// reaches the end of the stream all of the target states are set to 0

void _update_stream(int this_stream, int i, int stream_start, int stream_end) {
  
    // Serial.print("\ni: ");
    // Serial.print(i);
    
  // conditions:
  // 1. current state == target state 
  //   a. if i == stream end: shut down
  //   b. if i < stream end: i++ and update target states of stream leds
  // 2. current state < target state
  //   a. update target states of stream leds
  
  // check for shut down condition
  if (i == stream_end) {
    if (current_state[i] == target_state[i]) {
      // Serial.print("\ninside stream shut down condition. i==stream end:");
      // Serial.print(i==stream_end);
      // delay(2000);
      
        target_state[i-4] = 0; 
        target_state[i-3] = 0;  
        target_state[i-2] = 0;  
        target_state[i-1] = 0;  
        target_state[i] = 0;
        
        // set splash 
//        switch(active_stream) {
//          case STREAM1:
//            target_state[73] = 20;
//            target_state[72] = 10;
//            target_state[71] = 10;
//            break;
//          case STREAM2:
//            target_state[67] = 10;
//            target_state[68] = 20;
//            target_state[69] = 10;
//            break;
//          case STREAM3:
//            target_state[63] = 20;
//            target_state[64] = 10;
//            target_state[65] = 10;
//            break;
//        }
        
        // for some reason i always get caught here so im gonn atry calling the iterator here till it turns off
        while (current_state[i] > 0 || current_state[i-1] > 0 || current_state[i-2] > 0) the_incrementor();
  
        
        // after the target state of 0 is hit, I can set the flags for the next stream
//        if (current_state[i] == 0) {
//           Serial.print("\this_stream stream: ");
//           Serial.print(active_stream);
//           Serial.print("\ncurrent state of last led has reached 0 ");
          last_stream_run = this_stream;
//           Serial.print("\nthis_stream stream: ");
//           Serial.print(active_stream);
          active_stream = 99;
//           Serial.print("\nlast stream run: ");
//           Serial.print(last_stream_run);
        // turn off splash zone
//        for (int x = splash_start; x <= splash_end; x++) target_state[x] = 0; 
            resetFunc(); //call reset
          return;
//        }
    
    }
  }
  
  // otherwise proceed as usual
  
  // move iterator to next led
  else if (current_state[i] == MAX_BRIGHTNESS) {
    
    i++;
    
    // if the glory ball is active on this stream, update its state too
    // if (this_stream == glory_ball_stream) {
        
    //   glory_ball_led = i; 
    //   target_state[i] = twinkle_MAX_BRIGHTNESS;
    // }
    
    // update the new i
    switch (this_stream) {
      
      case STREAM1:
        stream1_led = i;
        // Serial.print("\nupdating stream 1");
        break;
      case STREAM2:
        stream2_led = i;
        // Serial.print("\nupdating stream 2");
        break;
      case STREAM3:
        stream3_led = i;
        // Serial.print("\nupdating stream 3");
        break;
    }
  
  // last condition is if current state hasnt reached target state. in that case just need to 
  // update stream targets
  } else {
      
    // whether i got moved or not, update the targets of surrounding leds
    // based on the current state of i
    if (i - 4 >= stream_start) {
      target_state[i-4] = 0;
    }
    if (i - 3 >= stream_start) {
      target_state[i-3] = (current_state[i] * trail3_brit);
    }
    
    if (i - 2 >= stream_start) {
      target_state[i-2] = current_state[i] * trail2_brit;
    }
    
    if (i - 1 >= stream_start) {
      target_state[i-1] = current_state[i] * trail1_brit;
    } 
    
    target_state[i] = MAX_BRIGHTNESS;
    
    if (i + 1 <= stream_end) {
      target_state[i+1] = current_state[i] * lead1_brit;
    } 
    
    if (i + 2 <= stream_end) {
      target_state[i+2] = current_state[i] * lead2_brit;
    }
  }
}

// this new test version is going to try to only run one strream at a
// time but still utilize incrementor to try n throw twinkles in. plus
// the incrementor was the only way I succesfully got a good stream
void streams_handler() {
  
  int choice;
  
  // > 3 means inactive
  if (active_stream > STREAM3) {  
    
//    Serial.print("\no streams active");  
    choice = random(3);
  } else {
//    Serial.print("\nelse choice is active stream"); 
    choice = active_stream;
  }
  
  switch (choice) {
      
      // don't allow the same stream to run twice in a row
    case STREAM1:
    
//         Serial.print("\nin stream stream 1");
       if (last_stream_run == STREAM1) break;
        active_stream = STREAM1;
        _update_stream(STREAM1, stream1_led, stream1_start, stream1_end);
//         Serial.print("\nupdated stream 1");
        break;
    case STREAM2:
//         Serial.print("\nin stream stream 2");
       if (last_stream_run == STREAM2) break;
        active_stream = STREAM2;
        _update_stream(STREAM2, stream2_led, stream2_start, stream2_end);
//         Serial.print("\nupdated stream 2");
        break;
    case STREAM3:
//         Serial.print("\nin stream stream 3");
       if (last_stream_run == STREAM3) break;
        active_stream = STREAM3;
        _update_stream(STREAM3, stream3_led, stream3_start, stream3_end);
//         Serial.print("\nupdated stream 3");
        break;
  }
}

// will randomly assign a pond led to twinkle (the_incrementor knows
// which leds are in the pond so the colors are selected there rather
// than saved globaly)
void twinkle() {

  // if this runs too much i can add a top level line like this
   if (random(18) != 1) return;

  int _step = 5;
  
  int i = random(pond_start, NUM_LEDS + 1);
  for (int j = 0; j < 40; j++) {
    leds[i] = CHSV(random(255), random(255), random(25,100));
    FastLED.show();
  }
  for (int j = 40; j >= 0; j -= _step) {
    leds[i] = CHSV(random(255), random(255), j);
    FastLED.show();
  }  
}



void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
    
  randomSeed(analogRead(0));
//  Serial.begin(9600); // open the serial port at 9600 bps:
}

void loop() { 
  
  
   twinkle();
  // Serial.print("stream 1 led ");
  // Serial.print(stream1_led);
  // Serial.print(".\n");
  // activate streams or update target states if already active
  streams_handler();
  
//        active_stream = STREAM1;
//        _update_stream(STREAM1, stream1_led, stream1_start, stream1_end);
  // Serial.print("stream 1 led ");
  // Serial.print(stream1_led);
  // Serial.print(".\n");
  // randomly select leds to twinkle with a 0 to n seconds delay between events
  // Serial.print("after pond_twinkle_handler");
  
  // move all leds a single step towards their target brightness
  the_incrementor();
  // Serial.print("after the_incrementor");
  
  
}