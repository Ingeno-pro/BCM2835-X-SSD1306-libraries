# BCM2835 library for SSD1306 display
## Information 
This library provides a raspberry pi zero library for using the SSD1306 display.  

## Depedencies
In order to use this library, you will have to install the bcm2835lib.  
The installation instruction can be found here : [https://www.airspayce.com/mikem/bcm2835/
](https://www.airspayce.com/mikem/bcm2835/)  


## Use 
In this reposetory you have 4 files : 
+ SSD1306_writer.h
+ SSD1306_writer.c
+ ssd1306.h
+ ssd1306.c

The two last file allow you to : 
+ initialise the display
+ erease the screen
+ draw pixels on the screen
+ update the screen

Indeed, these two files give a higher level programming to control the SSD1306.  
However, if you want to do a specific usage of your display, you can use these files to build your own solution.  

The SSD1306_writer files allow you to easly write text on the screen.  
----  

## Changelog 

24/03/2025 : Create the readme.md
10/12/2024 : Publish the library

Please, if you have any suggest/question contact me on contact@ingeno.fr
question contact me on contact@ingeno.fr
