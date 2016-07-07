#include "mylib.h"
u16 *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int row, int col, u16 color) {
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRect(int row, int col, int height, int width, unsigned short color) {
	for(int r=0; r<height; r++) {
		for(int c = 0; c<width; c++) {
			setPixel(row+r, col+c, color);
		}
	}
}
void delay(int n) {
	volatile int x = 0;
	for(int i=0; i<n*10000; i++) {
		x++;
	}
}

void waitForVBlank() {
	while (SCALINE_COUNTER > 160);
	while (SCALINE_COUNTER < 160);
}

void drawImage3(int x, int y, int w, int h, const u16* image) {
	for (int i = 0; i < h; i++) {
		DMA[3].src = image + OFFSET(i, 0, w);
		DMA[3].dst = videoBuffer + OFFSET(i + x, y, 240);
		DMA[3].cnt = DMA_ON | w;
	}
}

void fillScreen(volatile u16 color) {
	DMA[3].src = &color;
	DMA[3].dst = videoBuffer;
	DMA[3].cnt = 38400 | DMA_ON | DMA_SOURCE_FIXED; 
}
void fillPartScreen(int x, int y, int w, int h, volatile u16 color) {
	for (int i = 0; i < h; i++) {
		DMA[3].src = &color;
		DMA[3].dst = videoBuffer + OFFSET(i + x, y, 240);
		DMA[3].cnt = DMA_ON | DMA_SOURCE_FIXED | w;
	}
}

void drawDawg(DAWG *d) {
	fillPartScreen(d->oldcol,d->oldrow, d->height, d->width, BLACK); //changed from green		
	drawImage3(d->col,d->row,d->width,d->height, Dawg);
	d->oldrow = d->row;
	d->oldcol = d->col;
}

void drawProfessor(PROFESSOR *p) {
	fillPartScreen(p->oldcol,p->oldrow, p->height, p->width, BLACK); //changed from blue		
	drawImage3(p->col,p->row,p->width,p->height, Professor);
	p->oldrow = p->row;
	p->oldcol = p->col;
}
void updateBuzz(int dir, BUZZ *b) {
	fillPartScreen(b->oldcol,b->oldrow, BUZZ_HEIGHT, BUZZ_WIDTH, BLACK);
	if(dir == 1) {
		b->row = b->row + 3;
		drawImage3(b->col,b->row,BUZZ_WIDTH, BUZZ_HEIGHT, Buzz);
		b->oldrow = b->row;
		b->oldcol = b->col;

	}
	if(dir == 2) {
		b->row = b->row - 3;
		drawImage3(b->col,b->row,BUZZ_WIDTH, BUZZ_HEIGHT, Buzz);
		b->oldrow = b->row;
		b->oldcol = b->col;
	}
	if(dir == 3) {
		b->col = b->col - 3;
		drawImage3(b->col,b->row,BUZZ_WIDTH, BUZZ_HEIGHT, Buzz);
		b->oldrow = b->row;
		b->oldcol = b->col;
	}
	if(dir == 4) {
		b->col = b->col + 3;
		drawImage3(b->col,b->row,BUZZ_WIDTH, BUZZ_HEIGHT, Buzz);
		b->oldrow = b->row;
		b->oldcol = b->col;
	}

}
void drawBuzz(BUZZ *b) {
	drawImage3(b->col,b->row,BUZZ_WIDTH, BUZZ_HEIGHT, Buzz);
}
void reset(BUZZ *b, BOX *box) {
	b->row = 119;
	b->col = 140;
	b->lives = 3;
	fillScreen(BLACK);
	updateBuzz(1, b);
	box->box1 = 1;
	box->box2 = 1;
	box->box3 = 1;
	printBoxes(box);
	printLives(b->lives);
}
void respawn(BUZZ *b) {
	b->row = 119;
	b->col = 140;
	updateBuzz(1, b);

}

void drawChar(int row, int col, char ch, u16 color) {
	for(int r=0; r<8; r++) {
		for(int c=0; c<6; c++) {
			if(fontdata_6x8[OFFSET(r, c, 6) + ch*48]) {
				setPixel(row+r, col+c, color);
			}
		}
	}
}

void drawString(int row, int col, char *str, u16 color) {
	while(*str) {
		drawChar(row, col, *str++, color);
		col += 6;
	}
}
void printLives(int l) {
	drawRect(140, 0, 19, 50, BLACK);
	if(l == 3){
		drawString(140, 0, "Lives: 3",GREEN);
	} else if(l == 2){
		drawString(140, 0, "Lives: 2",GREEN);
	} else if (l == 1){
		drawString(140,0,"Lives: 1",GREEN);
	}
	else {
		drawString(140,0,"Lives: 0",GREEN);
	}
}
void printLost() {
	drawString(150, 0, "Well.. At least you don't go to uGA", WHITE);
}
void printWon() {
	drawString(150, 30, "SIC EMMM! #WeCanDoThat", WHITE);

}
void printBoxes(BOX *box) {
	if(box->box1 == 1) {
		drawRect(0, 60, 15, 20, RED);
	}
	if(box->box2 == 1) {
		drawRect(0, 120, 15, 20, RED);
	}
	if(box->box3 == 1) {
		drawRect(0, 180, 15, 20, RED);
	}
}

int checkCollision(BUZZ *b, DAWG *d) {
	for (int x = 0; x < 5; x++) {
		if (b->col < ((*d).col + (*d).width) && (b->col + b->width) > (*d).col &&
    		b->row < ((*d).row + (*d).height) && (b->row + b->height) > (*d).row) {
			return 1;
		}
		d++;
	}
	return 0;
}

int checkCollisionProfessor(BUZZ *b, PROFESSOR *p) {
	for (int x = 0; x < 5; x++) {
		if (b->col < ((*p).col + (*p).width) && (b->col + b->width) > (*p).col &&
    		b->row < ((*p).row + (*p).height) && (b->row + b->height) > (*p).row) {
			return 1;
		}
		p++;
	}
	return 0;
}
void printTimer(int second) {
	char str1[15];
	second = 20 - second;
	sprintf(str1, "%d", second);
	drawRect(150, 180, 20, 20, BLACK);
	drawString(150, 180, str1, WHITE);
}
void hitBoxes(BUZZ *b, BOX *box) {
	if (box->box1 == 1 && b->row >= 50 && b->row >= 70 && b->col >= 0 && b->col <= 18) {
		drawRect(0, 60, 15, 20, BLACK);
		box->box1 = 0;
		respawn(b);
	}
	if (box->box2 == 1 && b->row >= 110 && b->row >= 130 && b->col >= 0 && b->col <= 18) {
		drawRect(0, 120, 15, 20, BLACK);
		box->box2 = 0;
		respawn(b);
	}
	if (box->box3 == 1 && b->row >= 150 && b->row >= 170 && b->col >= 0 && b->col <= 18) {
		drawRect(0, 180, 15, 20, BLACK);
		box->box3 = 0;
		respawn(b);
	}
}