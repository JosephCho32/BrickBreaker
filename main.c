#include "mylib.h"
#include "text.h"
#include "brick.h"
#include "down.h"
#include "up.h"
#include "redbrick.h"

int main()
{
	REG_DISPCNT = MODE_3 | BG2_ENABLE;

	//paddle stuff
	int row = 150;
	int col = 100;
	int rdelta = 1;
	int cdelta = 1;
	int oldrow = row;
	int oldcol = col;
	//ball stuff
	int brow = 139;
	int bcol = 115;
	int launched = 0;
	int canlaunch = 1;
	int oldbrow = brow;
	int oldbcol = bcol;
	int brdelta = -1;
	int bcdelta = 1;

	//hud stuff
	char lives = '3';
	int score = 0;

	char buffer[3];

	static enum GBAState state = DRAW_START;

	while (1)
	{
		waitForVblank();
		switch(state)
		{
			//draw title bg
			case DRAW_START:
				drawImage3(0, 0, 240, 160, brick);
				state = START;
				break;
			//title screen
			case START:
				drawString(60, 90, "Brick Break", YELLOW);
				drawString(80, 90, "Press Start", YELLOW);
				if (KEY_DOWN_NOW(BUTTON_START))
				{
					state = DRAW_MAIN;
					break;

				}
				break;
			//create game assets
			case DRAW_MAIN:
				//paddle stuff
				row = 150;
				col = 100;
				rdelta = 1;
				cdelta = 1;
				oldrow = row;
				oldcol = col;

				//ball stuff
				brow = 139;
				bcol = 115;
				launched = 0;
				canlaunch = 1;
				oldbrow = brow;
				oldbcol = bcol;
				brdelta = -1;
				bcdelta = 1;
				drawRect(0, 0, 240, 59, BLACK);
				drawRect(oldrow, oldcol, 40, 5, BLACK);
				drawRect(oldbrow, oldbcol, 11, 11, BLACK);
				drawRect(0, 0, 240, 160, BLACK);

				//draw paddle and ball
				drawRect(row, col, 40, 5, WHITE);
				drawRect(row - 11, col + 15, 10, 10, BLUE);
				lives = '3';
				score = 0;

				//brick stuff
				u16 colors[] = {
					RED, GREEN, YELLOW, MAGENTA, CYAN, RED, GREEN, YELLOW, MAGENTA, CYAN,
					RED, GREEN, YELLOW, MAGENTA, CYAN, RED, GREEN, YELLOW, MAGENTA, CYAN,
					RED, GREEN, YELLOW, MAGENTA, CYAN, RED, GREEN, YELLOW, MAGENTA, CYAN,
					RED, GREEN, YELLOW, MAGENTA, CYAN, RED, GREEN, YELLOW, MAGENTA, CYAN
				};
				BRICK bricks[40];
				int brickIsOn[] = {
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1
				};
				for (int r = 0; r < 10; r++)
				{
					for (int c = 0; c < 4; c++)
					{
						int b = c * 10 + r;
						bricks[b].col = r * 24;
						bricks[b].row = 15 + c * 8;
						bricks[b].color = colors[b];
					}
				}
				for (int r = 0; r < 10; r++)
				{
					for (int c = 0; c < 4; c++)
					{
						int b = c * 10 + r;
						if (brickIsOn[b] == 1)
						{
							drawRect(bricks[b].row, bricks[b].col, 24, 5, bricks[b].color);
						}
					}
				}

				//top hud
				drawRect(0, 0, 240, 10, GREY);
				sprintf(buffer, "Score:%d", score);
				drawString(1, 1, buffer, YELLOW);
				drawString(1, 195, "Lives: ", YELLOW);
				drawChar(1, 195 + 36, lives, YELLOW);

				state = MAIN;
				break;
			//game
			case MAIN:
				//win condition
				if (score == 40)
				{
					state = DRAW_WIN;
				}
				//movement controls
				if (KEY_DOWN_NOW(BUTTON_LEFT))
				{
					if (!(col <= 0))
					{
						col -= cdelta;
					}

				}
				if (KEY_DOWN_NOW(BUTTON_RIGHT))
				{
					if (!(col >= 200))
					{
						col += cdelta;
					}

				}
				if (KEY_DOWN_NOW(BUTTON_UP))
				{
					if (row > 140)
					{
						row -= rdelta;
					}
				}
				if (KEY_DOWN_NOW(BUTTON_DOWN))
				{
					if (row < 155)
					{
						row += rdelta;
					}
				}

				//launch ball
				if (KEY_DOWN_NOW(BUTTON_A) && canlaunch)
				{
					launched = 1;
					canlaunch = 0;
					brow = row - 11;
					bcol = col + 15;
				}

				//go to title screen
				if (KEY_DOWN_NOW(BUTTON_SELECT))
				{
					state = DRAW_START;
				}

				//changes ball's state
				if (launched)
				{
					if (bcol == 0)
					{
						bcdelta = 1;
					}
					if (bcol >= 230)
					{
						bcdelta = -1;
					}
					if (brow == 10)
					{
						brdelta = 1;
					}
					if ((brow + 10 >= row) && (brow + 10 <= row + 5) && (bcol + 10 >= col) && (bcol <= col + 40))
					{
						brdelta = -1;
					}

					if (brow >= 150) {
						//lose condition
						if (lives == '1')
						{
							//go to game over screen
							state = DRAW_GAMEOVER;
						}

						//removes a life and resets ball to unlaunched state
						lives -= 1;
						launched = 0;
						canlaunch = 1;
						brdelta = -1;
						drawRect(oldbrow, oldbcol, 10, 10, BLACK);
						brow = row - 11;
						bcol = col + 15;
						drawRect(brow, bcol, 10, 10, BLUE);
						drawRect(0, 0, 240, 10, GREY);
						drawString(1, 195, "Lives: ", YELLOW);
						drawChar(1, 195 + 36, lives, YELLOW);
						sprintf(buffer, "Score:%d", score);
						drawString(1, 1, buffer, YELLOW);
						delay(30);
					}
					brow += brdelta;
					bcol += bcdelta; 
				}


				//delete
				drawRect(oldrow, oldcol, 40, 5, BLACK);
				drawRect(oldbrow, oldbcol, 10, 10, BLACK);
				for (int i = 0; i < 40; i++)
				{
					if ((brickIsOn[i]) && (brow <= bricks[i].row + 5) && (brow >= bricks[i].row) && (bcol + 10 >= bricks[i].col) && (bcol <= bricks[i].col + 24))
					{
						score += 1;
						brdelta = 1;
						brickIsOn[i] = 0;
						drawRect(bricks[i].row, bricks[i].col, 24, 5, BLACK);
						drawRect(0, 0, 180, 10, GREY);
						sprintf(buffer, "Score:%d", score);
						drawString(1, 1, buffer, YELLOW);
					}
				}
				//draw new
				drawImage3(66, 96, 48, 48, redbrick);
				for (int r = 0; r < 10; r++)
				{
					for (int c = 0; c < 4; c++)
					{
						int b = c * 10 + r;
						//check if brick is active, redraw only if active
						if (brickIsOn[b] == 1)
						{
							drawRect(bricks[b].row, bricks[b].col, 24, 5, bricks[b].color);
						}
					}
				}
				drawRect(row, col, 40, 5, WHITE);
				drawRect(brow, bcol, 10, 10, BLUE);

				//fix positions of ball and paddle
				oldrow = row;
				oldcol = col;
				oldbrow = brow;
				oldbcol = bcol;

				//keep ball attached to paddle if unlaunched
				if (!(launched))
				{
					brow = row - 11;
					bcol = col + 15;
				}
				break;

			//draw gameover bg
			case DRAW_GAMEOVER:
				drawImage3(0, 0, 240, 160, down);
				state = GAMEOVER;
				break;

			//gameover screen
			case GAMEOVER:
				drawString(60, 80, "G A M E O V E R", BLACK);
				sprintf(buffer, "YOUR SCORE WAS %d", score);
				drawString(80, 76, buffer, BLACK);
				drawString(100, 70, "Press Start to Play", BLACK);
				if (KEY_DOWN_NOW(BUTTON_SELECT))
				{
					state = DRAW_START;
				}
				if (KEY_DOWN_NOW(BUTTON_START))
				{
					state = DRAW_MAIN;
				}
				break;

			//draw win bg
			case DRAW_WIN:
				drawImage3(0, 0, 240, 160, up);
				state = WIN;
				break;

			//win screen
			case WIN:
				drawString(60, 80, "Y O U W I N ! !", BLACK);
				sprintf(buffer, "YOUR SCORE WAS %d", score);
				drawString(80, 76, buffer, BLACK);
				if (KEY_DOWN_NOW(BUTTON_SELECT))
				{
					state = DRAW_START;
				}
				if (KEY_DOWN_NOW(BUTTON_START))
				{
					state = DRAW_MAIN;
				}
				break;

		}
	}
	return 0;
}