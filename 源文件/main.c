#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "game.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) 
{
	char choose, game;
	init();
		menu();
			while (1)
			{
				game = gameboard();
				switch (game)
				{
				case '1':
					BallFlatthrow();
					break;
				case '2':
					JumpBall();
					break;
				case '3':
					RotateSnake();
					break;
				case '4':
					MovingRectangle();
					break;
				case '5':
					ThrowPins();
					break;
				case '6':
					BeatifulCircle();
					break;
				case '7':
					Snake();
					break;
				case '8':
					TenStep();
					break;
				case 'q':
					system("cls");
					return 0;
				default:
					error();
					break;
				}
			}
	return 0;
}
