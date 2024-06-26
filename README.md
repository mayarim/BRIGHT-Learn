# BRIGHT-Learn: Building Responsive Interactions for Growth, Healing, and Therapeutic Learning
### Custom-designed toy for a developmentally disabled child in order to facilitate comfort and enjoyment of occupational therapy activities.

![Marin_Poster](https://github.com/mayarim/BRIGHT-Learn/assets/75183224/cfd23567-8a4f-4eb2-91c8-e752d1101683)

## Source code: 
I used FastLED and SerialMP3Player Arduino libraries and wrote a program, Game.ino, integrating these into a game loop. The toy welcomes Marin to teh game, then prompts her to press the button of a random color. If she pressees the correct one, then the toy says "Good job" and a song plays to reward her. If she presses the incorrect one, the lights guide her towards the correct section. Currently, the code is rather disorganized, but future plans include re-factoring to create a "color" struct with various fields and filepaths in order to streamline readability. Additionally, more code will be introduced in later iterations to make the game loop more exciting and engaging. 

## 3D Models: 
The physical casing for the toy was constructed using a modular design of six "box segment" pieces around one "pie core" piece. The central core piece has columns that hold a stand for the speaker to rest on, so that it can be closer to the lid and allow more sound to pass through. All 3D models were created in Solidworks. The lid was laser cut from 1/8" plywood, then painted with a different color for each of six sections. In order to let the 3D strip lights show through from the inside, small "window" slots were cut out, and corresponding "windows" were cut out of 1/8" clear acrylic and press fit inside to make the lights visible. The cad_models folder includes all .stl and .sldprt files of 3D-printed parts, .dxf files for laser-cut parts, and a .sldasm file showing the assembly. 

## Components used: 
* Arduino Mega 2650: https://store.arduino.cc/products/arduino-mega-2560-rev3
  
![image](https://github.com/mayarim/BRIGHT-Learn/assets/75183224/7b88fdf2-e105-4f9c-8de6-7033c2bfd053)


* LM386 Audio Amplifier https://www.ti.com/lit/ds/symlink/lm386.pdf
  
![image](https://github.com/mayarim/BRIGHT-Learn/assets/75183224/ffbdaa67-c67a-4ad1-bfb9-ad368120bcd7)



* HiLetgo GD3300 UART Control Serial MP3 Music Player Module https://github.com/salvadorrueda/SerialMP3Player

![image](https://github.com/mayarim/BRIGHT-Learn/assets/75183224/04ae702c-4c38-41ae-ac97-4a081211e497)


* BTF-LIGHTING WS2812B RGB ECO LED Strip https://github.com/FastLED/FastLED/

![image](https://github.com/mayarim/BRIGHT-Learn/assets/75183224/eac64873-97ef-48cd-b303-dbdf04af9930)


* Arcade-style push-buttons

![image](https://github.com/mayarim/BRIGHT-Learn/assets/75183224/a74fa25e-5498-40fd-a123-5cd2e0f3f5fb)
