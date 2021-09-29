#Project-Snake-For-MCU
This is a hobby project to make a snake game that can run on an inexpensive MCU.
It is based on the classic game "snake" and is programmed in mostly barebones C. 
This is merely a development build, and it is not yet ready for actual MCU implementation. However, it functions as expected in VisualStudio during debug mode.

The game field is based on a [x][y] matrix and takes user input to control a snake, and uses random functions to spawn the snake and the enemies (food). It also has logic to determine whether the spawn is legal/fun to enhance the game experience. Lastly, it has some switch/case menus and functionality supporting sleep mode and shutting down the MCU.

It uses some functions (sleep(delay)) and system("clc"), which must be replaced for MCU implementation. Sleep should be replaced by interruptions, allowing the MCU to process the game logic between accepting user input (such as with a timer).

The project thus uses 

<lo>
  <li> stdlib.h </li>
  <li> stdio.h  </li>
  <li> windows.h</li>
  <li> conio.h  </li>
</lo>


<h1> In-game control </h1>

<lo>
  <li> W - Forward </li>
  <li> A - Left </li>
  <li> D - Right </li>
  <li> S - Backwards</li>
  <li> 1 - Reset snake speed</li>
  <li> 2 - Decrease snake speed</li>
  <li> 3 - Increase snake speed</li>
  <li> 4 - Restart</li>
</lo>

<h1> Menu control </h1>

<lo>
  <li> W - Up </li>
  <li> A - Previous menu </li>
  <li> D - Next menu </li>
  <li> S - Down</li>
  <li> 1 - New game (on death screen)</li>
  <li> 2 - Sleep mode (on death screen)</li>
  <li> 3 - Shut down (on deat screen)</li>
  <li> 4 - Not in use</li>
</lo>
