typedef unsigned short u16;
typedef unsigned int u32;
#include "Dawg.h"
#include "Buzz.h"
#include "Professor.h"
#include <stdio.h>
#include <stdlib.h>
#define MAX_WIDTH 240;
#define MAX_HEIGHT 160;
#define SCALINE_COUNTER *(volatile unsigned short*) 0x4000006
#define REG_DISPCTL *(unsigned short *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)

extern u16 *videoBuffer;
extern const unsigned char fontdata_6x8[12288];

#define COLOR(r, g, b)   ((r) | (g)<<5 | (b)<<10)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0, 31, 0)
#define BLUE COLOR(0,0,31)
#define WHITE COLOR(31,31,31)
#define YELLOW COLOR(31,31,0)
#define BLACK 0

#define OFFSET(r, c, rowlen) ((r)*(rowlen) + (c))

// Buttons

#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)
#define BUTTONS *(volatile unsigned int *)0x4000130


/* DMA */

#define REG_DMA0SAD         *(const volatile u32*)0x40000B0 // source address
#define REG_DMA0DAD         *(volatile u32*)0x40000B4       // destination address
#define REG_DMA0CNT         *(volatile u32*)0x40000B8       // control register

// DMA channel 1 register definitions
#define REG_DMA1SAD         *(const volatile u32*)0x40000BC // source address
#define REG_DMA1DAD         *(volatile u32*)0x40000C0       // destination address
#define REG_DMA1CNT         *(volatile u32*)0x40000C4       // control register

// DMA channel 2 register definitions
#define REG_DMA2SAD         *(const volatile u32*)0x40000C8 // source address
#define REG_DMA2DAD         *(volatile u32*)0x40000CC       // destination address
#define REG_DMA2CNT         *(volatile u32*)0x40000D0       // control register

// DMA channel 3 register definitions
#define REG_DMA3SAD         *(const volatile u32*)0x40000D4 // source address
#define REG_DMA3DAD         *(volatile u32*)0x40000D8       // destination address
#define REG_DMA3CNT         *(volatile u32*)0x40000DC       // control register

typedef struct
{
	const volatile void *src;
	const volatile void *dst;
	unsigned int cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

typedef struct {
	int width;
	int height;
	int col;
	int row;
	int oldrow;
	int oldcol;
	int lives;
} BUZZ; //BUZZ is strct of character Buzz
typedef struct {
	int width;
	int height;
	int col;
	int row;
	int oldrow;
	int oldcol;
} DAWG; //DAWG is a struct of enemy character Dawg
typedef struct {
	int width;
	int height;
	int col;
	int row;
	int oldrow;
	int oldcol;
} PROFESSOR; //PROFESSOR is a struct of enemy character Professor
typedef struct {
	int box1;
	int box2;
	int box3;
} BOX; //BOX is a struct of a box object

//functions
void setPixel(int row, int col, u16 color); //sets the specific pixel to the color
void drawRect(int row, int col, int height, int width, u16 color); //draws a rectangle of specific location, dimensions, and color
void delay(int n); //delays the VBA from doing an action
void waitForVBlank(); //waits for the SCALINE_COUNTER
void drawImage3(int x, int y, int w, int h, const u16* image); //draws a given image to the screen
void fillScreen(volatile u16 color); //fills the screen with a specific color
void fillPartScreen(int x, int y, int w, int h, volatile u16 color); //fills part of the screen with a specific color
void drawDawg(DAWG *d); //draws the dawg struct
void drawProfessor(PROFESSOR *p); //draws the professor struct
void drawBuzz(BUZZ *b); //draws buzz struct
void updateBuzz(int dir, BUZZ *buzz); //updates Buzz's location based on key pushed
void reset(BUZZ *b, BOX *bo); //resets the screen for a new game
void drawString(int row, int col, char *s, u16 color); //draws a specific string to the VBA
void drawChar(int row, int col, char ch, u16 color); //draws a specific char to the VBA. Used to make drawString
void printLives(int l); //prints the number of lives left
void printBoxes(BOX *box); //prints the boxes
int checkCollision(BUZZ *b, DAWG *d); //checks for a collision with the Dawg structs
int checkCollisionProfessor(BUZZ *b, PROFESSOR *p); //checks for a collision with the Professor structs
void respawn(BUZZ *b); //respawns buzz when he dies or gets a box
void printLost(); //draws the end screen when the game is lost
void printWon(); //draws the end screen when the game is won
void printTimer(int second); //prints the time left till the game is lost
void hitBoxes(BUZZ *b, BOX *box); //checks if Buzz is on an open box