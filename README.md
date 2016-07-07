# Buzz Hop
BuzzHop is a Game Boy Advance game coded in C and compiled with a VBA. The game can be compiled using a vba compiler software and the game can be demoed with a GBA emulator such as [this one](http://www.nogba.com/) and running the GBA file using it.
## Information
Version 1.0
April 2016

## Objective
BuzzHop is a platform game with the object that the character Buzz collects all the red square boxes on the other side of the screen while avoiding all Dawgs and Professors. If Buzz hits a Dawg or a Professor, he loses a life (of which he has 3). If Buzz does not get all three boxes in time (20 seconds) or loses all 3 lives, he loses the game.
To start the game, you hit the Button A. At any point after the game has started, you can push the Select Button to end the game and then hit A to start it over from there.

## Future Implementations
I would like a background to this game so it is not just black.
	
## Bugs
Timer is not exactly 1 second long (can be slightly longer due to timer being implemented with waitForVBlank()
