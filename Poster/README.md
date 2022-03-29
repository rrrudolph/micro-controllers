
I made this backlit poster for my wife's birthday. It's meant to look like the waterfall has streams of light coming down and random twinkles throughtout the pond. It's really hard to get the lighting right in images or videos (the LEDs either come out too strong or too faint), but you'll get the idea hopefully. There's a link to a video at the bottom.
<!-- ![IMG_7669](https://user-images.githubusercontent.com/62268115/160714854-5aadd9a8-ec81-44d0-97cd-299af55dc829.JPG) -->
![IMG_7669 (1)](https://user-images.githubusercontent.com/62268115/160715069-0bdbd9e8-ca1e-4502-b364-ab24daa109bd.JPG)

It was a fun program to write as I needed to make everything async so that multilple things could appear to happen at once.  I did this by relying on two primary functions in the event loop. 

The first function sets an LEDs `target state` (brightness) based on certain environment conditions.  These conditions could be the `stream` that is currently active, how far the `stream` has progressed, or if the target state is equal to an LEDs `current state` (ie, the LED has reached max brightness, reset the target to min brightness). In other words, it checks a lot of global flags that are continually updated by sub functions and resets the LEDs `target states` to match the global environment. 

The second function actually increments the brightness one step towards the target. This happens for every LED, each one being incremented by one unit of brightness at a time per event loop.  

At a high level this is how the program operates: 
```
> Global environment flags set on start
> LED `target states` set 
> LED `current states` incrementally update, leading to 
> Global environment flags being updated 
> LED `target states` updated 
> ...
```
However, it either needs refactoring or a faster micro controller because overall it's too slow.  The arduino nano only has a 16mhz processor, so first step will just be to upgrade to something in the 40mhz range and see how it goes. 
<!-- ![IMG_8205](https://user-images.githubusercontent.com/62268115/160714895-6f6c1f19-63ac-4c38-97b4-10957c51bcf1.JPG) -->
<!-- ![IMG_7710](https://user-images.githubusercontent.com/62268115/160714900-6e3ce033-6913-45fe-8f5a-0bbb35d60feb.jpg) -->
![IMG_7710 (1)](https://user-images.githubusercontent.com/62268115/160715613-780f285b-3791-40fb-94da-faa04e61d059.jpg)
![IMG_8205 (1)](https://user-images.githubusercontent.com/62268115/160715625-69baa1c4-626d-482e-a7a3-58dc75c78b83.JPG)

Attempt at filming it in action:
https://drive.google.com/file/d/17R2RlwGfaN6Vwl7flB3HLcWuIN2OMDza/view?usp=sharing
