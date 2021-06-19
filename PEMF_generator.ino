/*
  This is a pemf generator which will step thru frequencies 0-35.
  I'm splitting each integer step into 3 different freqs by choosing random decimal values.
  A 50% duty cycle "square wave" (mostly square) is used.
*/

// runtime: how many minutes program will run
// max_freq: the highest frequency (hz) which will be generated
// num_polarity_flips: change '5' to however many polarity flips are desired
unsigned long runtime = 1;  // don't change type!
int max_freq = 35;
int num_polarity_flips = int(max_freq / 5); // 


unsigned long duration = runtime * 60 * 1000; // convert runtime to milliseconds
unsigned long time_per_freq = round(duration / max_freq / 3);
int pin = 2;  // this gets assigned 2 or 3 to flip polarity
void setup() {
  pinMode(2, OUTPUT);  // polarity pins
  pinMode(3, OUTPUT);
  Serial.begin(9600);
  randomSeed(analogRead(0));  // will shuffle random()
  int polarity_counter = 0;

  for (int i = max_freq; i >= 0; i--) {
    
    // Change the output pin (flip the polarity) based on above params
    polarity_counter += 1;
    if (polarity_counter == num_polarity_flips) {
      polarity_counter = 0; 
      if (pin == 2) {
        pin = 3;
      } else {
          pin = 2;
        }
    }

    // Generate random decimals by choosing num between 1 and 100
    int a = random(1, 100);
    int b = random(1, 100);
    int c = random(1, 100);

    // Divide by 100 to get decimal and add to iterator
    float freq_1 = (a * 0.01) + i;
    float freq_2 = (b * 0.01) + i;
    float freq_3 = (c * 0.01) + i;

    // Convert to milliseconds
    freq_1 = 1 / freq_1 * 1000;
    freq_2 = 1 / freq_2 * 1000;
    freq_3 = 1 / freq_3 * 1000;

    // Set the length that frequencies will run for
    unsigned long start = millis();
    long end_1 = start + time_per_freq;
    long end_2 = end_1 + time_per_freq;
    long end_3 = end_2 + time_per_freq;
    
    while (millis() < end_1) {
      digitalWrite(pin, HIGH);
      delay(freq_1);
      digitalWrite(pin, LOW);
      delay(freq_1);
    }

    while (millis() < end_2) {
      digitalWrite(pin, HIGH);
      delay(freq_2);
      digitalWrite(pin, LOW);
      delay(freq_2);
    }

    while (millis() < end_3) {
      digitalWrite(pin, HIGH);
      delay(freq_3);
      digitalWrite(pin, LOW);
      delay(freq_3);
    }
  }
}

void loop() {
}
