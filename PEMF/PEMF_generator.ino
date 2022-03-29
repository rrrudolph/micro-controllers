 /*
  This is a pemf generator which will step thru selected frequencies.
  Im splitting each integer step into 3 different freqs by choosing random decimal values.
  This creates a 50% duty cycle "square wave" (mostly square).
*/

#include <TM1637Display.h>
#include <Encoder.h>

Encoder encoder(3, 2);

int button(11);
int buttonState;
int rotaryValue;

// Default frequency range
int maxFreq = 45;
int minFreq = 0;

// Output pins
int freqSignal = 4;
int relay = 13;
int led = 5;

int hours;
int minutes;
int runTime = 360;  // this will be displayed as 6:00
unsigned long secondsRunTime;
unsigned long millisPerFreq;

float freq_1;
float freq_2;
float freq_3;
unsigned long millisFreq_1;
unsigned long millisFreq_2;
unsigned long millisFreq_3;
unsigned long start;
unsigned long end_1;
unsigned long end_2;
unsigned long end_3;


// Define the display pins:
#define CLK 6
#define DIO 7

// Create display object of type TM1637Display:
TM1637Display display = TM1637Display(CLK, DIO);

// LED brightness based on user selection
int ledBrightness[] = {3, 10, 20, 40, 80, 140, 200, 255};
int brightness; // used for saving the selected value

// "brit" display text
const uint8_t brit[] = {
  SEG_F | SEG_E | SEG_G | SEG_C | SEG_D, // b
  SEG_E | SEG_G, // r
  SEG_E,  // i
  SEG_F | SEG_E | SEG_G | SEG_D, // t
};

// "run" display text
const uint8_t run[] = {
  SEG_E | SEG_G, // r
  SEG_E | SEG_D | SEG_C, // u
  SEG_E | SEG_G | SEG_C,  // n
  0x00, // blank
};

// "HI" display text
const uint8_t HI[] = {
  SEG_F | SEG_E | SEG_G | SEG_B | SEG_C, // H
  SEG_F | SEG_E, // I
  0x00, // blank
  0x00, // blank
};

// "LO" display text
const uint8_t LO[] = {
  SEG_F | SEG_E | SEG_D, // L
  SEG_F | SEG_E | SEG_B | SEG_C | SEG_A | SEG_D, // O
  0x00, // blank
  0x00, // blank
};

// Invoke this function at each new user input screen
void resetDisplay(uint8_t data[], int encVal, bool clear) {
  
  delay(250);
  buttonState = HIGH; // reset or it will remain LOW
  display.setSegments(data);
  delay(1500);
  encoder.write(encVal);
  if (clear == true) {
  display.clear();
  }
}

// Get the millisecond value of a frequency
// (cut in half to more easily use for a 50% duty cycle)
float hzToMillis(float frequency) {
  
  return 1000 / frequency / 2;
}

// Generate the signal for the given frequency and run time
void runFrequency(float millisFreq, unsigned long endTime, int brightness) {
  
  while (millis() < endTime) {
  digitalWrite(freqSignal, HIGH);
  analogWrite(led, brightness);
  delay(millisFreq);
  digitalWrite(freqSignal, LOW);
  digitalWrite(led, LOW);
  delay(millisFreq);
  }
}

void setup() {

  pinMode(led, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(freqSignal, OUTPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);

  display.clear();
  display.setBrightness(2);
  buttonState = HIGH; // not pressed
  randomSeed(analogRead(0));  // will shuffle random()
  
  // Now for the user inputs of:
  // brightness, run time, max frequency, min frequency 

  // Set the brightness
  resetDisplay(brit, 0, false);
  while (buttonState == HIGH) {
    
    buttonState = digitalRead(11);
    rotaryValue = encoder.read();

    if (rotaryValue < 0) {
      rotaryValue = 0;
      encoder.write(0);
    }
    if (rotaryValue > 28) {
      rotaryValue = 28;
      encoder.write(28);
    }

    rotaryValue /= 4;
    brightness = ledBrightness[rotaryValue];
    analogWrite(led, brightness);
    display.showNumberDec(rotaryValue);
    display.setBrightness(rotaryValue);
  }
  
  digitalWrite(led, LOW);
  
  
  // Set run time
  resetDisplay(run, runTime, true);
  display.showNumberDecEx(runTime, 0b11100000, false, 4, 0);
  while (buttonState == HIGH) { 
    
    buttonState = digitalRead(11);
    rotaryValue = encoder.read();

    // Don't let the runtime go negative
    if (rotaryValue < 0) {
      rotaryValue = 0;
      encoder.write(0);
    }

    // Handle the rotaryVal to time conversion
    hours = rotaryValue / 60;
    if (hours != 0) {
      minutes = rotaryValue - hours * 60;
    } else {
      minutes = rotaryValue;
    }

    // Make adjustments to display correctly
    runTime = hours * 100 + minutes;
    display.showNumberDecEx(runTime, 0b11100000, false, 4, 0);
  }
    
  // Convert the displayed runTime value into actual minutes
  if (runTime > 99) {
    hours = runTime / 100;
    minutes = runTime - hours * 100;
  } else {
    hours = 0;
    minutes = runTime;
  }

  // Convert total minutes to seconds
  secondsRunTime = (hours * 60 + minutes) * 60;

  // Set the max frequency
  resetDisplay(HI, maxFreq * 4, true);

  while (buttonState == HIGH) { 
    
    buttonState = digitalRead(11);
    rotaryValue = encoder.read() / 4;

    if (rotaryValue < 1) {
      rotaryValue = 1;
      encoder.write(1);
    }

    maxFreq = rotaryValue;
    display.showNumberDec(maxFreq);
  }

  // Set the min frequency (must be lower than maxFreq)
  resetDisplay(LO, minFreq * 4, true);

  while (buttonState == HIGH) { // not pressed
    buttonState = digitalRead(11);
    rotaryValue = encoder.read();

    if (rotaryValue < 0) {
      rotaryValue = 0;
      encoder.write(0);
    }

    if (rotaryValue >= maxFreq * 4) {  // * 4 because I'm not dividing by 4 yet
      rotaryValue = maxFreq * 4 - 1;
      encoder.write(maxFreq * 4 - 1);
    }

    minFreq = rotaryValue / 4;
    display.showNumberDec(minFreq);
  }
  
  display.clear();
  
  // Based on the range of frequencies selected, get the run time per frequency
  // remember each frequency integer is split into 3 floats
  millisPerFreq = (secondsRunTime / (maxFreq - minFreq) / 3) * 1000;
  digitalWrite(relay, HIGH);

  // Calculate the exact frequencies and set their end times
  for (int i = maxFreq; i >= minFreq; i--) {  
    
    // Generate random decimals
    freq_1 = i + (random(1, 100) * 0.01);
    freq_2 = i + (random(1, 100) * 0.01);
    freq_3 = i + (random(1, 100) * 0.01);

    // Convert to milliseconds
    millisFreq_1 = hzToMillis(freq_1);
    millisFreq_2 = hzToMillis(freq_2);
    millisFreq_3 = hzToMillis(freq_3);

    // Set the duration that the frequencies will run for
    start = millis();
    end_1 = start + millisPerFreq;
    end_2 = end_1 + millisPerFreq;
    end_3 = end_2 + millisPerFreq;
  
    // Generate this batch of frequencies
    display.showNumberDec(i);
    runFrequency(millisFreq_1, end_1, brightness);
    runFrequency(millisFreq_1, end_2, brightness);
    runFrequency(millisFreq_1, end_3, brightness);
    display.clear();
  }
  
  digitalWrite(relay, LOW);
  digitalWrite(led, LOW);
}

void loop() {
}
