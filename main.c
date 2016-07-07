// Don't be a goober split your code up into multiple files and use multiple functions! Also header files.
// Also please read this or equivalent and don't write ugly code
// http://www.doc.ic.ac.uk/lab/cplus/cstyle.html#N10069
// Brandon is watching...
// Also use header files.

#include "mylib.h"
#include "Title.h"
#include "End.h"
#include "Gameplay.h"
#include "Buzz.h"
#include "Professor.h"
#include "Dawg.h"
#include <stdlib.h>


int main(void)
{
    REG_DISPCTL = MODE3 | BG2_ENABLE;
    drawImage3(0, 0, 240, 160, Title);
    BUZZ buzz;
    int vblankCounter = 0;
    int aPushed = 0;
	int vblankCounter1 = 0;
	BOX box;
	box.box1 = 1;
	box.box2 = 1;
	box.box3 = 1;
    while(1) {
    	waitForVBlank();
    	vblankCounter++;
		if(vblankCounter % 60 == 0) {
			vblankCounter1 = vblankCounter / 60;
			printTimer(vblankCounter1);
			if (vblankCounter1 >= 20) {
	    		drawImage3(0, 0, 240, 160, End);
	    		printLost();
	    		vblankCounter = 0;
	    		while(!(KEY_DOWN_NOW(BUTTON_A)) && !(KEY_DOWN_NOW(BUTTON_SELECT))) {

				}
				reset(&buzz, &box);
			}
		}

    	DAWG dawgs[5]; 
    	DAWG dawgs2[5]; 
    	DAWG dawgs3[5]; //three rows of dawg enemies
    	PROFESSOR professors[5];
    	PROFESSOR professors2[5];
    	PROFESSOR professors3[5]; //three row of professor enemies
    	while(!KEY_DOWN_NOW(BUTTON_A) && (aPushed == 0) ) { //waits till button A is pushed

    	}
    	if(KEY_DOWN_NOW(BUTTON_SELECT)) { //at anypoint in gameplay, if select is pushed, the game ends to title screen
    		drawImage3(0, 0, 240, 160, Title);
    		aPushed = 0;
    		while(!(KEY_DOWN_NOW(BUTTON_A))) { //need to hit A to begin gameplay again
    		}
    	}
    	if(KEY_DOWN_NOW(BUTTON_A) && (aPushed == 0)) { //if A is pushed when game is not in play
			aPushed = 1; //makes sure A does not do anything during gameplay
			vblankCounter = 0; 
    		reset(&buzz, &box);
    		printLives(buzz.lives);
			for (int i = 0; i < 5; i++) { //in this for loop, the dawgs and professors are created in rows
				dawgs[i].row = 90 * i;
				dawgs[i].col = 121;
				dawgs[i].height = DAWG_HEIGHT;
				dawgs[i].width = DAWG_WIDTH;

				dawgs2[i].row = 90 * i;
				dawgs2[i].col = 105;
				dawgs2[i].height = DAWG_HEIGHT;
				dawgs2[i].width = DAWG_WIDTH;

				dawgs3[i].row = 80 * i;
				dawgs3[i].col = 90;
				dawgs3[i].height = DAWG_HEIGHT;
				dawgs3[i].width = DAWG_WIDTH;


				professors[i].row = 70 * i;
				professors[i].col = 50;
				professors[i].height = PROFESSOR_HEIGHT;
				professors[i].width = PROFESSOR_WIDTH;

				professors2[i].row = 90 * i;
				professors2[i].col = 34;
				professors2[i].height = PROFESSOR_HEIGHT;
				professors2[i].width = PROFESSOR_WIDTH;

				professors3[i].row = 90 * i;
				professors3[i].col = 18;
				professors3[i].height = PROFESSOR_HEIGHT;
				professors3[i].width = PROFESSOR_WIDTH;

			}
		}
		if ( (checkCollision(&buzz, &dawgs[0]) == 1) || (checkCollision(&buzz, &dawgs2[0]) == 1) || (checkCollision(&buzz, &dawgs3[0]) == 1) ||
			(checkCollisionProfessor(&buzz, &professors[0]) == 1) || (checkCollisionProfessor(&buzz, &professors2[0]) == 1) || (checkCollisionProfessor(&buzz, &professors3[0]) == 1) ) { //if a collision has occured anywhere
				respawn(&buzz);
				buzz.lives = buzz.lives - 1;
				if (buzz.lives < 0) {
	    			drawImage3(0, 0, 240, 160, End);
	    			printLost();
	    			vblankCounter = 0;
	    			while(!(KEY_DOWN_NOW(BUTTON_A)) && !(KEY_DOWN_NOW(BUTTON_SELECT))) {
					}
					reset(&buzz, &box);
				}
    			else {
    				printLives(buzz.lives);
				}
		}
		hitBoxes(&buzz, &box); //makes sure a box has not been hit
		if(box.box1 == 0 && box.box2 == 0 && box.box3 == 0) { //when all the boxes are hit, the game ends
    		drawImage3(0, 0, 240, 160, End);
    		printWon();
    		vblankCounter = 0;
    		while(!(KEY_DOWN_NOW(BUTTON_A)) && !(KEY_DOWN_NOW(BUTTON_SELECT))) { //waits for button input
			}
				reset(&buzz, &box);
		}
		delay(1); //otherwise objects go too fast
		if (aPushed == 1) { //draws buzz at begining of game
			drawBuzz(&buzz);
		}
		for(int i = 0; i < 5; i++) { //this moves the objects. The if statements loop the objects back around so there is no wraparound error
			if (professors[i].row >= 240) {
				professors[i].row = 0;
			}
			if (professors2[i].row <= 0) {
				professors2[i].row = 240;
			}
			if (professors3[i].row >= 240) {
				professors3[i].row = 0;
			}
			if (dawgs[i].row >= 240) {
				dawgs[i].row = 0;
			}
			if (dawgs2[i].row <= 0) {
				dawgs2[i].row = 240;
			}
			if (dawgs3[i].row >= 240) {
				dawgs3[i].row = 0;
			}

			professors[i].row = professors[i].row + 1; //these statements move the objects accordingly
			professors2[i].row = professors2[i].row - 1;
			professors3[i].row = professors3[i].row + 1;
			dawgs[i].row = dawgs[i].row + 1;
			dawgs2[i].row = dawgs2[i].row - 1;
			dawgs3[i].row = dawgs3[i].row + 1;

			drawDawg(&dawgs[i]); //these statements draw the objects to the screen
			drawDawg(&dawgs2[i]);
			drawDawg(&dawgs3[i]);
			drawProfessor(&professors[i]);
			drawProfessor(&professors2[i]);
			drawProfessor(&professors3[i]);

		}
		//the following 4 if statements are button inputs which move buzz in the direction of push
		if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
			updateBuzz(1, &buzz);
		}
		if(KEY_DOWN_NOW(BUTTON_LEFT)) {
			updateBuzz(2, &buzz);

		}
		if(KEY_DOWN_NOW(BUTTON_UP)) {
			updateBuzz(3, &buzz);

		}
		if(KEY_DOWN_NOW(BUTTON_DOWN)) {
			updateBuzz(4, &buzz);
		}



    }
    return 0;
}
