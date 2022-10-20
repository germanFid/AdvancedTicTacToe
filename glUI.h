#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glut.h>
#include <math.h>

#include "glConfig.h"
#include "structure.h"
#include "ai.h"

#define BUTTON_OFFSET 0.1f
#define BUTTON_NUM 5

#define MAP_LINES 8
#define FIELD_LINES 12

enum _screenState {STATE_MAIN_MENU = 100, STATE_PLAY = 0, STATE_PLAY_AI , STATE_LEADERBOARDS, STATE_SETTINGS, STATE_ABOUT} ScreenState;

char menuButtons[][30] = {"Play", "Play with AI","Leaderboards", "Settings", "About"};

#define ABOUT_LEN 200
char* About = "\n \nAdvanced Tic Tac Toe\nAuthor: German Fidarov\n4831001/10003\nSt. Petersburg\nPolytechnic University\n \nInstitute of Cybersecurity \nand Information Protection\n \n2022\n \nPreess ESC to return to main menu";

#define DIFLEN 11
#define DIFNUM 4
char difStrings[DIFNUM][DIFLEN] = {"Easy", "Medium", "Hard", "Super Hard"};

struct GAME game;

struct Point
{
    float x;
    float y;
};

struct MenuButton
{
    struct Point topRight;
    struct Point topLeft;
    struct Point bottomRight;
    struct Point bottomLeft;
};

struct MenuButton MButtons[BUTTON_NUM];

float translateCordinate(float cordMajor, float cordMinor)
{
    float result = -0.9f + (cordMajor) * 0.6f + 0.1f * cordMajor + (cordMinor) * 0.2f;
    return result;
}

void drawstring(float x, float y, char* string)
{
    glRasterPos2f(x, y);
    for (char* c = string; *c != '\0'; c++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
}

void initMenuButton(float x1, float y1, float x2, float y2, struct MenuButton* btn)
{
    btn->topRight.x = x1;
    btn->topRight.y = y1;

    btn->bottomRight.x = x1;
    btn->bottomRight.y = y2;

    btn->topLeft.x = x2; 
    btn->topLeft.y = y1;

    btn->bottomLeft.x = x2;
    btn->bottomLeft.y = y2;
}

void drawMenuButton(struct MenuButton* btn, char* str)
{
    glBegin(GL_QUADS);
        glVertex2f(btn->topRight.x, btn->topRight.y);
        glVertex2f(btn->bottomRight.x, btn->bottomRight.y);
        glVertex2f(btn->bottomLeft.x, btn->bottomLeft.y);
        glVertex2f(btn->topLeft.x, btn->topLeft.y);
	glEnd();

    glColor3f(0.9, 0.7, 1);
    drawstring(btn->bottomLeft.x+0.03f, (btn->topRight.y + btn->bottomLeft.y) / 2 - 0.03, str);
    glColor3f(0.43, 0.17, 0.64);
}

void drawMenu(struct MenuButton* btn)
{
    glColor3f(0.43, 0.17, 0.64);
    drawstring(-0.5f, 1.0f - BUTTON_OFFSET * 2, "Advanced Tic Tac Toe");

    float curHeight = 1.0f - BUTTON_OFFSET * 3.5;
    float btnHeight = (1.0f - BUTTON_OFFSET * BUTTON_NUM) / (BUTTON_NUM / 2) - BUTTON_OFFSET/2;

    for (int i = 0; i < BUTTON_NUM; i++)
    {
        initMenuButton(0.7f, curHeight, -0.7f, curHeight - btnHeight, &btn[i]);
        
        drawMenuButton(&btn[i], menuButtons[i]);
        curHeight -= btnHeight + BUTTON_OFFSET;
    }
    
}

int mainMenuClickHandler(float x, float y)
{
    int resultButton = -1;
    for (int i = 0; i < BUTTON_NUM; i++)
    {
        if (x < MButtons[i].topRight.x && y < MButtons[i].topRight.y)
        {
            if (x > MButtons[i].bottomLeft.x && y > MButtons[i].bottomLeft.y)
            {
                resultButton = i;

                if (resultButton == STATE_PLAY)
                {
                    GInit(&game);
                }
                
                if (resultButton == STATE_PLAY_AI)
                {
                    GInit(&game);
                    if (SettingAIFirst == AI_FIRST)
                    {
                        AIMakeMove(&game);
                    }
                }
                break;
            }
        }
    }

    if (resultButton == -1)
    {
        return GL_NOT_FOUND;
    }

    ScreenState = resultButton;
    printf("Click\t:\t%d\n", ScreenState);

    return SUCCESS;
}

void drawAbout()
{
    char* str = (char*) malloc(ABOUT_LEN);

    strcpy(str, About);
    char* sep = "\n";
    char* istr = strtok(str, sep);

    int i = 0;
    while (istr)
    {
        drawstring(-0.7f, 1.0 - BUTTON_OFFSET*i, istr);
        istr = strtok(NULL, sep);
        i++;
    }

    free(str);
}

struct Point mapLines[MAP_LINES] =  {
                            {-0.35f, 1.0f}, {-0.35f, -1.0f}, 
                            {0.35f, 1.0f}, {0.35f, -1.0f},
                            {-1.0f, 0.35f}, {1.0f, 0.35f},
                            {-1.0f, -0.35f}, {1.0f, -0.35f}
                            };

struct Point fieldLines[FIELD_LINES] = {
                            {0.1f, 1.0f}, {0.1f, -1.0f},
                            {0.6f, 1.0f}, {0.6f, -1.0f},
                            {0.8f, 1.0f}, {0.8f, -1.0f},

                            {-0.1f, 1.0f}, {-0.1f, -1.0f},
                            {-0.6f, 1.0f}, {-0.6f, -1.0f},
                            {-0.8f, 1.0f}, {-0.8f, -1.0f}
                            };

void drawX(float centerX, float centerY, float size)
{
    glBegin(GL_LINES);
    glVertex2f(centerX-size, centerY-size);
    glVertex2f(centerX+size, centerY+size);

    glVertex2f(centerX+size, centerY-size);
    glVertex2f(centerX-size, centerY+size);
    glEnd();
}

void drawO(GLfloat x, GLfloat y, float radius){
	int i;
	int lineAmount = 100; //# of triangles used to draw circle
	
	// GLfloat radius = 0.05f; //radius
	GLfloat twicePi = 2.0f * 3.14159;
	
	glBegin(GL_LINE_LOOP);
		for(i = 0; i <= lineAmount;i++) { 
			glVertex2f(
			    x + (radius * cos(i *  twicePi / lineAmount)), 
			    y + (radius* sin(i * twicePi / lineAmount))
			);
		}
	glEnd();
}

void drawFieldStruct(struct FIELD* field, int majorX, int majorY)
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (field->fieldCells[i][j] == TURNX)
            {
                drawX(translateCordinate(majorX, i), -translateCordinate(majorY, j), 0.05f);
                // printf("Drawing x: %f %f\n", translateCordinate(majorX, i), -translateCordinate(majorY, j));
            }

            else if (field->fieldCells[i][j] == TURNO)
            {
                drawO(translateCordinate(majorX, i), -translateCordinate(majorY, j), 0.05f);
                // printf("Drawing o: %f %f\n", translateCordinate(majorX, i), -translateCordinate(majorY, j));
            }
        }
        
    }
    
}

void drawMap(struct MAP* map)
{
    glBegin(GL_LINES);
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if(map->FMap[i][j].winner != 0)
            {
                glColor3f(0.43, 0.17, 0.64);
                if (map->FMap[i][j].winner == MAP_SIZE)
                {
                    drawX(translateCordinate(i, 1), -translateCordinate(j, 1), 0.2f);
                }

                else
                {
                    drawO(translateCordinate(i, 1), -translateCordinate(j, 1), 0.2f);
                }
            }

            else
            {
                drawFieldStruct(&map->FMap[i][j], i, j);
            }
        }
    }
    glEnd();
}

void drawCursor()
{
    glColor3f(1, 0, 0);
    // printf("%d\n", game.turn);
    // printf("%d %d\t%d %d\n\n", game.mapSelX, game.fieldSelX, game.mapSelY, game.fieldSelY);
    if (game.turn == TURNX)
    {
        drawX(translateCordinate(game.mapSelX, game.fieldSelX), -translateCordinate(game.mapSelY, game.fieldSelY), 0.05f);
    }

    else
    {
        drawO(translateCordinate(game.mapSelX, game.fieldSelX), -translateCordinate(game.mapSelY, game.fieldSelY), 0.05f);
    }
    glColor3f(0, 0, 0);
}

void drawField()
{
    glBegin(GL_LINES);
        for (int i = 0; i < MAP_LINES; i++)
        {
            glVertex2f(mapLines[i].x, mapLines[i].y);
        }

        glColor3f(0.64, 0.64, 0.64);

        for (int i = 0; i < FIELD_LINES; i++) // drawing vertical field lines
        {
            glVertex2f(fieldLines[i].x, fieldLines[i].y);
        }

        for (int i = 0; i < FIELD_LINES; i++) // drawing vertical field lines
        {
            glVertex2f(fieldLines[i].y, fieldLines[i].x);
        }

        // drawX(translateCordinate(0, 0), -translateCordinate(0, 0));
    glEnd();

    // printf("%f %f\n", translateCordinate(0, 0), -translateCordinate(1, 1));
    drawMap(&game.gameMap);
    drawCursor();
}

void drawGame(int ai)
{
    drawField();

    if (game.winner != 0)
    {
        glBegin(GL_QUADS);
        glColor3f(0.43, 0.17, 0.64);
        glVertex2f(0.5f, 0.5f);
        glVertex2f(-0.5f, 0.5f);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glColor3f(1, 1, 1);
        glEnd();

        if (game.winner == MAP_SIZE)
        {
            drawstring(-0.4f, 0.1f, "Cross Wins!");
            drawstring(-0.4f, -0.1f, "Press ESC to exit!");
        }

        else
        {
            drawstring(-0.4f, 0.1f, "Nought Wins!");
            drawstring(-0.4f, -0.1f, "Press ESC to exit!");
        }
    }
}

struct MenuButton moveBtn;
struct MenuButton difBtn;
struct MenuButton firstBtn;

void DrawSettings()
{
    glColor3f(0.43, 0.17, 0.64);
    drawstring(-0.9, 0.9, "First Moves:");
    drawMenuButton(&moveBtn, ((SettingFirst == FIRST_CROSS) ? "X" : "0"));

    drawstring(-0.9, 0.7, "Difficulty:");
    drawMenuButton(&difBtn, difStrings[SettingDifLevel]);

    drawstring(-0.9, 0.5, "AI FIRST?:"); // ((SettingAIFirst == AI_FIRST) ? "AI FIRST" : "AI SECOND")
    drawMenuButton(&firstBtn, ((SettingAIFirst == AI_FIRST) ? "AI FIRST" : "AI SECOND"));
}

void settingsClickHandler(float x, float y)
{
    if (x < moveBtn.topRight.x && y < moveBtn.topRight.y)
    {
        if (x > moveBtn.bottomLeft.x && y > moveBtn.bottomLeft.y)
        {
            SettingFirst = SettingFirst * (-1);
        }
    }

    if (x < difBtn.topRight.x && y < difBtn.topRight.y)
    {
        if (x > difBtn.bottomLeft.x && y > difBtn.bottomLeft.y)
        {
            SettingDifLevel++;
            if (SettingDifLevel > SUPERHARD)
            {
                SettingDifLevel = EASY;
            }

            switch (SettingDifLevel)
            {
            case EASY:
                AI_NAIVE_MODE = AI_NAIVE;
                AI_DEPTH = AI_DEPTH_EASY;
                break;

            case MEDIUM:
                AI_NAIVE_MODE = AI_NAIVE;
                AI_DEPTH = AI_DEPTH_MEDIUM;
                break;

            case HARD:
                AI_NAIVE_MODE = AI_NOT_NAIVE;
                AI_DEPTH = AI_DEPTH_HARD;
                break;

            case SUPERHARD:
                AI_NAIVE_MODE = AI_NOT_NAIVE;
                AI_DEPTH = AI_DEPTH_SUPERHARD;
                break;
            
            default:
                break;
            }

            printf("Difficulity: %d\n NAIVE_MODE: %d\n", AI_DEPTH, AI_NAIVE_MODE);
        }
    }

    if (x < firstBtn.topRight.x && y < firstBtn.topRight.y)
    {
        if (x > firstBtn.bottomLeft.x && y > firstBtn.bottomLeft.y)
        {
            SettingAIFirst++;
            if(SettingAIFirst > AI_SECOND)
            {
                SettingAIFirst = AI_FIRST;
            }
        }
    }
}