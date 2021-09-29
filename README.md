# Project-Snake-For-MCU
A hobby project to make a snake which can run on an inexpensive mcu.
It is based on the classic game "snake" and is programmed in mostly barebones C. 
This is merely a development build and it is not yet ready for real implementation, however functions as expected in VisualStudio during debugmode.
It uses some functions (sleep(delay)) and system("clc") which must have been replaced for MCU implementation amoungst others.

The project thus uses 

<lo>
  <li> #include <stdlib.h> </li>
  <li> #include <stdio.h>  </li>
  <li> #include <windows.h></li>
  <li> #include <conio.h>  </li>
</lo>

It uses a [x][y] matrix gamefiled system and takes user input to control a snake.
