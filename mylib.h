#include <stdio.h>

typedef unsigned int u32;
typedef unsigned short u16;

#define REG_DISPCNT *(unsigned short *)0x4000000
#define MODE_3 3

#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006
#define BG2_ENABLE (1 << 10)

//DMA
typedef struct
{
	const volatile void *src;
	const volatile void *dst;
	u32                  cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

//colors
#define COLOR(r, g, b) (r | (g << 5) | (b << 10))
#define RED COLOR(31, 0, 0)
#define GREEN COLOR(0, 31, 0)
#define BLUE COLOR(0, 0, 31)
#define YELLOW COLOR(31, 31, 0)
#define WHITE COLOR(31, 31, 31)
#define BLACK 0
#define GREY COLOR(15, 15, 15)
#define MAGENTA COLOR(31, 0, 31)
#define CYAN COLOR(0, 31, 31)

#define OFFSET(row, col, rowlen) ((row * rowlen) + col)

//buttons
#define BUTTON_A (1 << 0)
#define BUTTON_B (1 << 1)
#define BUTTON_SELECT (1 << 2)
#define BUTTON_START (1 << 3)
#define BUTTON_RIGHT (1 << 4)
#define BUTTON_LEFT (1 << 5)
#define BUTTON_UP (1 << 6)
#define BUTTON_DOWN (1 << 7)
//#define BUTTON_R (1 << 8)
//#define BUTTON_L (1 << 9)

#define KEY_DOWN_NOW(key) (~(BUTTONS) & key)
#define BUTTONS *(volatile unsigned int *)0x4000130

//prototypes
void setPixel(int row, int col, u16 color);
void drawRect(int row, int col, int height, int width, u16 color);
void drawImage3(int row, int col, int height, int width, const u16* image);
void delay(int n);
void waitForVblank();

//brick struct 
typedef struct
{
	int row;
	int col;
	u16 color;
} BRICK;

//gba states
enum GBAState {
	DRAW_START,
	START,
	DRAW_MAIN,
	MAIN,
	DRAW_GAMEOVER,
	GAMEOVER,
	DRAW_WIN,
	WIN,
	// TODO add more states here!
};