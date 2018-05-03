#include "mylib.h"

u16* videoBuffer = (u16*)0x6000000;
//static vars
static u16 screenWidth = 240;
//static u16 screenHeight = 160;

void setPixel(int row, int col, u16 color)
{
	videoBuffer[OFFSET(row, col, screenWidth)] = color;
}

void drawRect(int row, int col, int width, int height, u16 color)
{
	for (int r = 0; r < height; r++)
	{
        DMA[3].src = &color;
        DMA[3].dst = &videoBuffer[OFFSET((row + r), col, screenWidth)];
        DMA[3].cnt = (width) | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | DMA_ON;
    }
}

void drawImage3(int row, int col, int width, int height, const u16* image) {
    for (int r = 0; r < height; r++) {
        DMA[3].src = &image[r * width];
        DMA[3].dst = &videoBuffer[(row+r) * 240 + col];
        DMA[3].cnt = (width) | DMA_ON;
    }
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void delay(int n)
{
	volatile int size = 0;
	for (int i = 0; i < n * 4000; i++)
	{
		size += 1;
	}
}