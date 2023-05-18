#include "glUI.h"

void resize(int width, int height)
{
    glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
}

void renderScene(void)
{
    glClearColor(1, 1, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glColor3f(0.43, 0.17, 0.64);

	switch (ScreenState)
	{
		case STATE_MAIN_MENU:
			drawMenu(MButtons);
			break;

		case STATE_ABOUT:
			drawAbout();
			break;

		case STATE_PLAY:
			drawGame(GAME_2P);
			break;

		case STATE_PLAY_AI:
			drawGame(GAME_AI);
			break;

		case STATE_SETTINGS:
			DrawSettings();
			break;

		default:
			break;
	}

	if (ScreenState == STATE_MAIN_MENU)
	{
		drawMenu(MButtons);
	}

    glutSwapBuffers();
}

void idle() 
{
	glutPostRedisplay(); 
}

void mousePressed(int button, int state, int x, int y)
{
	float xClip = ((x + 0.5f) / WINDOW_WIDTH) * 2.0f - 1.0f;
	float yClip = 1.0f - ((y + 0.5f) / WINDOW_HEIGHT) * 2.0f;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		printf("%f %f\n", xClip, yClip);

		switch (ScreenState)
		{
		case STATE_MAIN_MENU:
			mainMenuClickHandler(xClip, yClip);
			break;

		case STATE_SETTINGS:
			settingsClickHandler(xClip, yClip);
			break;

		default:
			break;
		}
	}
	glutIdleFunc(idle);
	glutMainLoop();
}

void keyboardPressed(unsigned char key, int x, int y)
{
	if (key == 27) // if ESC
	{
		ScreenState = STATE_MAIN_MENU;
	}

	printf("KEY: %d\n", key);

	if (key == 32) // if spacebar
	{
		if (ScreenState == GAME_2P)
		{
			GMakeMove(&game, game.mapSelX, game.mapSelY, game.fieldSelX, game.fieldSelY, game.turn);
		}

		else if (ScreenState == GAME_AI)
		{
			if ((game.turn == SettingFirst && SettingAIFirst == AI_SECOND) || (game.turn  != SettingFirst && SettingAIFirst == AI_FIRST)) // don't let the player to make turn if it's AI turn
			{
				GMakeMove(&game, game.mapSelX, game.mapSelY, game.fieldSelX, game.fieldSelY, game.turn);
				if ((game.turn == SettingFirst && SettingAIFirst == AI_FIRST) || (game.turn != SettingFirst && SettingAIFirst == AI_SECOND))
				{
					AIMakeMove(&game);
				}
			}

			else
			{
				printf("NOPE!\n");
			}
		}

		glutIdleFunc(idle);
		glutMainLoop(); 
	}
}

void SpecialInput(int key, int x, int y)
{
	if (game.winner == 0)
	{
		switch(key)
		{
			case GLUT_KEY_UP:
				printf("UP\n");
				GameSel(&game, DIRECTION_UP);
			break;

			case GLUT_KEY_DOWN:
				printf("DOWN\n");
				GameSel(&game, DIRECTION_DOWN);
			break;

			case GLUT_KEY_LEFT:
				printf("LEFT\n");
				GameSel(&game, DIRECTION_LEFT);
				break;

			case GLUT_KEY_RIGHT:
				printf("RIGHT\n");
				GameSel(&game, DIRECTION_RIGHT);
				break;

			default:
				break;
		}
	}
}

int main(int argc, char **argv) 
{
	ScreenState = STATE_MAIN_MENU;

	SettingFirst = FIRST_CROSS;
	SettingDifLevel = MEDIUM;
	SettingAIFirst = AI_SECOND;
	AI_DEPTH = AI_DEPTH_MEDIUM;
	AI_NAIVE_MODE = AI_NAIVE;
	
	initMenuButton(0.6, 0.95, 0.0, 0.85, &moveBtn);
	initMenuButton(0.6, 0.8, 0.0, 0.7, &difBtn);
	initMenuButton(0.6, 0.65, 0.0, 0.55, &firstBtn);

	// инициализация
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Advanced Tic Tac Toe");
	glutIdleFunc(idle);

	glutMouseFunc(mousePressed);
	glutKeyboardFunc(keyboardPressed);
	glutSpecialFunc(SpecialInput);

    glutReshapeFunc(resize);
	glutDisplayFunc(renderScene);
 
	printf("Start!\n");
	glutMainLoop();
 
	return 0;
}
