### PEMF Generator
![IMG_6289 (1)](https://user-images.githubusercontent.com/62268115/125211923-9260d980-e26f-11eb-8e3a-fc932c00c923.JPG)
![IMG_6326 (1)](https://user-images.githubusercontent.com/62268115/125211925-93920680-e26f-11eb-8716-7c1ec0afea1b.JPG)


This is a file for the Arduino IDE. It steps through a range of frequencies, choosing 3 random decimals for each integer. The signal gets sent to an amplifier and then passed to a copper coil to create a pulsating magnet field. It's supposed to help with inflammation and other things, which it did.

I'm currently simplifying the circuit as the amplifier, 5v power source, and relay are not needed.  Rather than a relay/amp combo I will simply use a MOSFET to open the 12v circuit to the coil, and reuse the potentiometer that's currently on the amp to control the output. 

This video demonstrates the user inputs:
https://vimeo.com/573739980

For more info on PEMF check this link:
https://www.gildeachiropractic.com/blog/posts/pulsed-electromagnetic-field-therapy
