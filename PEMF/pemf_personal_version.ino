 /*
  This is a pemf generator which will step thru selected frequencies.
  Im splitting each integer step into 3 different freqs by choosing random decimal values.
*/

int brightness = 60;  // LED

String selectedProgram = "descend"; // default value

// unsigned long thirtyFiveMinsAsMillis = 2100000L;
unsigned long _40minsAsMillis = 2400000L;
unsigned long _20minsAsMillis = 1200000L;
unsigned long startCoolDown = millis() + _40minsAsMillis;

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

unsigned long totalTime;
unsigned long timePerFrequency;
int numFreqs;


// Get the millisecond value of a frequency. 
// Will need to divide by 2 later for ON and OFF cycle
float hzToMillis(float frequency) {
  return 1000 / frequency;
}

// Generate the signal for the given frequency and run time
void runFrequency(float frequencyAsMillis, unsigned long endTime) {
  
  // change the duty cycle to fight heat
  float onTime = frequencyAsMillis * 0.2; // 20%
  float offTime = frequencyAsMillis * 0.8;
  
  // Using port manipulation to trigger the driver fet for speed and so
  // the nano get's a little bit of a buffer on the potential current drawn
  while (millis() < endTime) {
    PORTB = B11111100; // 8-13 HIGH
    delay(onTime);
    PORTB = B00000000; // 8-13 LOW
    delay(offTime);
  }
}

// See if it's time to let the coils cool down. This had to be added
// because my coils are highly insulated. If they weren't it probably
// wouldn't be necessary
void checkForCoolDown() {
  
  if (millis() > startCoolDown) {
    
    // sleep and reset the next cool down point
    delay(coolDown);
    
    startCoolDown = millis() + _40minsAsMillis;
  }
}

// This is the only top level function that will be called. It will determine the time per
// frequency based on the range and run time passed, and will generate the frequencies in 
// either ascending or descending order.
int playFrequencyRange(int startFreq, int stopFreq, unsigned long minutes) {
  
  // Get run time in milliseconds
  totalTime = minutes * 60 * 1000;
  // Serial.print("\nminutes ");
  // Serial.print(minutes);
  // Serial.print("\ntotalTime ");
  // Serial.print(totalTime);
    

  // I split each frequency integer into 3 parts to add random 
  // decimals so that there is increased variety
  numFreqs = abs(startFreq - stopFreq);
  timePerFrequency = totalTime / numFreqs / 3; 
  // Serial.print("\ntimePerFreq " + timePerFreq);
  
  // Ascend or descend based on the order of frequencies passed
  if (startFreq < stopFreq) {
    
    // Ascend
    for (int i = startFreq; i <= stopFreq; i++) {
    
      // Generate random decimals to append to i for variety
      freq_1 = i + random(0, 33) * 0.01;
      freq_2 = i + random(33, 66) * 0.01;
      freq_3 = i + random(66, 99) * 0.01;
  
      // Convert to milliseconds
      millisFreq_1 = hzToMillis(freq_1);
      millisFreq_2 = hzToMillis(freq_2);
      millisFreq_3 = hzToMillis(freq_3);
    
      // Generate this batch of frequencies
      runFrequency(millisFreq_1, millis() + timePerFrequency);
      checkForCoolDown();
      runFrequency(millisFreq_2, millis() + timePerFrequency);
      checkForCoolDown();
      runFrequency(millisFreq_3, millis() + timePerFrequency);
      checkForCoolDown();
    }
    
  } else {
    
    // Descend
    for (int i = startFreq; i >= stopFreq; i--) {
    
      // Generate random decimals to append to i for variety
      freq_1 = i + random(66, 99) * 0.01;
      freq_2 = i + random(33, 66) * 0.01;
      freq_3 = i + random(0, 33) * 0.01;
  
      // Convert to milliseconds
      millisFreq_1 = hzToMillis(freq_1);
      millisFreq_2 = hzToMillis(freq_2);
      millisFreq_3 = hzToMillis(freq_3);
    
      // Generate this batch of frequencies
      runFrequency(millisFreq_1, millis() + timePerFrequency);
      checkForCoolDown();
      runFrequency(millisFreq_2, millis() + timePerFrequency);
      checkForCoolDown();
      runFrequency(millisFreq_3, millis() + timePerFrequency);
      checkForCoolDown();
    }
  }
}


void setup() {
  // Serial.begin(9600);
  randomSeed(analogRead(0)); 
  DDRB = B11111100;  // sets pins 8 to 13 as outputs

  if (selectedProgram == "descend") {
    
              // start, stop, minutes
    playFrequencyRange(16, 4, 200); 
   
  } else {
    // playFrequencyRange(25, 1, 30);  // beta -> delta
    // playFrequencyRange(1, 6, 45);   // delta -> theta
    // playFrequencyRange(6, 15, 30);  // theta -> beta
    // playFrequencyRange(12, 1, 30);  // beta -> delta
    // playFrequencyRange(1, 20, 30);  // delta -> beta
    // playFrequencyRange(30, 40, 5);  // beta -> gamma
    // playFrequencyRange(20, 4, 30);  // beta -> theta
    // playFrequencyRange(1, 22, 30);  // delta -> beta
    // playFrequencyRange(40, 20, 5);  // gamma -> beta
    // playFrequencyRange(20, 7, 30);  // beta -> theta
    // playFrequencyRange(8, 22, 30);  // alpha -> beta
    // playFrequencyRange(40, 12, 5);  // gamma -> alpha
    // playFrequencyRange(12, 40, 5);  // alpha -> gamma
    // playFrequencyRange(22, 12, 30);  // beta -> alpha
    // playFrequencyRange(10, 22, 30);  // alpha -> beta
  }
}

void loop() {}
