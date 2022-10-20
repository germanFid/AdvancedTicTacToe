#ifndef STRUCT
#define STRUCT

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// #include "ai.h"

#define SCORE_SIZE 8
#define CELLS_SIZE 3
#define MAP_SIZE 3
#define PLAYER_NAME_LEN 10

#define GAME_UNLOCKED 0
#define GAME_LOCKED 1

#define DIRECTION_UP 0
#define DIRECTION_DOWN 1
#define DIRECTION_RIGHT 2
#define DIRECTION_LEFT 3

#define GENERAL_FAIL -1

enum _settingFirst {FIRST_CROSS = 1, FIRST_NOUGHTS = -1} SettingFirst;
enum _settingDifLevel {EASY, MEDIUM, HARD, SUPERHARD} SettingDifLevel;
enum _settingAIFirst {AI_FIRST, AI_SECOND} SettingAIFirst;

enum turns
{
    TURNX = 1,
    TURNO = -1
};

/*
* STRUCT FIELD
*/

struct FIELD // Поле
{
    char fieldCells[CELLS_SIZE][CELLS_SIZE];  // Клеточки внутри поля
    char fieldAxisScore[SCORE_SIZE]; // Счет на "осях" (локальный)

    void (*FUpdateFieldScore)(struct FIELD*);
    int (*FCheckFieldWin)(struct FIELD*);

    int winner;
};

void FUpdateFieldScore (struct FIELD*);
int FCheckFieldWin (struct FIELD*);

void FInit(struct FIELD* field)
{
    memset(*field->fieldCells, 0, CELLS_SIZE * CELLS_SIZE); // Обнуляем поле
    memset(field->fieldAxisScore, 0, SCORE_SIZE); // Обнуляем счет

    field->FUpdateFieldScore = FUpdateFieldScore; // Указатель на функцию апдейта счета
    field->FCheckFieldWin = FCheckFieldWin; // Указатель на функцию проверки победы

    field->winner = 0;
}

void FUpdateFieldScore(struct FIELD* field) // Обновляет счет всего поля
{
    char* score = field->fieldAxisScore;

    memset(field->fieldAxisScore, 0, SCORE_SIZE); // Обнуляем счет

    // Обновляем строчки и столбцы
    for (int i = 0; i < CELLS_SIZE; i++) 
    {
        for (int j = 0; j < CELLS_SIZE; j++)
        {
            score[i] += field->fieldCells[i][j]; // Если крестик - плюсуем, иначе - минусуем
            score[6 - j] += field->fieldCells[i][j];
        }
    }

    // Обновляем диагонали
    int j = CELLS_SIZE - 1;
    for (int i = 0; i < CELLS_SIZE; i++)
    {
        score[CELLS_SIZE] += field->fieldCells[i][i];
        score[CELLS_SIZE*CELLS_SIZE - 2] += field->fieldCells[j][2-j];
        j--;
    }
    
}

int FCheckFieldWin (struct FIELD* field)
{
    for (int i = 0; i < 8; i++)
    {
        // Если нужный счет достигнут
        if (field->fieldAxisScore[i] == MAP_SIZE || field->fieldAxisScore[i] == MAP_SIZE * (-1))
        {
            return field->fieldAxisScore[i];
        }
    }
    
    return 0;
}

int FCheckFieldDraw (struct FIELD* field)
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (field->fieldCells[i][j] == 0)
            {
                return 0;
            }
        }
    }

    return 1;
}

void FDebugOutput(struct FIELD* field) // Выводит счет и поле
{
    // Вывод поля
    for (int i = 0; i < CELLS_SIZE; i++)
    {
        for (int j = 0; j < CELLS_SIZE; j++)
        {
            printf("%d\t", field->fieldCells[i][j]);
        }
        printf("\n");
    }

    // Вывод счета    
    for (int i = 0; i < SCORE_SIZE; i++)
    {
        printf("%d ", field->fieldAxisScore[i]);
    }
    printf("\n");
}

/*
* STRUCT MAP
*/

struct MAP // Карта
{
    struct FIELD FMap[MAP_SIZE][MAP_SIZE];
    char mapAxisScore[SCORE_SIZE];
};

/**
 * @brief Инициализирует карту
 * 
 * @param map Карта
 */
void MInit(struct MAP* map)
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            FInit(&(map->FMap[i][j]));
        }
    }

    memset(map->mapAxisScore, 0, SCORE_SIZE);
}

void MUpdateMapScore(struct MAP* map)
{
    char* score = map->mapAxisScore;
    memset(map->mapAxisScore, 0, SCORE_SIZE);

    for (int i = 0; i < CELLS_SIZE; i++)
    {
        for (int j = 0; j < CELLS_SIZE; j++)
        {
            score[i] += map->FMap[i][j].winner / 3;
            score[6 - j] += map->FMap[i][j].winner / 3;
        }
    }

    int j = CELLS_SIZE - 1;
    for (int i = 0; i < CELLS_SIZE; i++)
    {
        score[CELLS_SIZE] += map->FMap[i][i].winner / 3;
        score[CELLS_SIZE*CELLS_SIZE - 2] += map->FMap[j][2-j].winner / 3;
        j--;
    }
}

int MCheckMapWin(struct MAP* map)
{
    for (int i = 0; i < SCORE_SIZE; i++)
    {
        printf("%d ", map->mapAxisScore[i]);
        if (map->mapAxisScore[i] == CELLS_SIZE || map->mapAxisScore[i] == CELLS_SIZE * (-1))
        {
            return map->mapAxisScore[i];
        }
        printf("\n");
    }
    
    return 0;
}

void MDebugOutput(struct MAP* map)
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            FDebugOutput(&map->FMap[i][j]);
        }
    }
}

/*
* STRUCT GAME
*/

struct GAME
{
    // info
    char turn; // 0 - o ; 1 - x

    // fields
    struct MAP gameMap;

    unsigned char locked :1; // 0 - unlocked, 1 - locked
    int lockedX;
    int lockedY;

    int winner;

    int mapSelX;
    int mapSelY;
    int fieldSelX;
    int fieldSelY;

    // functions
    int (*GMakeMove)(struct GAME*, int, int, int, int, int);
};

void GameSel(struct GAME* game, int direction)
{
    switch (direction)
    {
    case DIRECTION_UP:
        if (game->fieldSelY == 0)
        {
            if (game->mapSelY != 0)
            {
                game->mapSelY--;
                game->fieldSelY = MAP_SIZE - 1;
                printf("map up\n");
            }
        }

        else
        {
            game->fieldSelY--;
            printf("field up\n");
        }

        break;

    case DIRECTION_DOWN:
        if (game->fieldSelY == MAP_SIZE - 1)
        {
            if (game->mapSelY != MAP_SIZE - 1)
            {
                game->mapSelY++;
                game->fieldSelY = 0;
                printf("map down\n");
            }
        }

        else
        {
            game->fieldSelY++;
            printf("field down\n");
        }

        break;

    case DIRECTION_RIGHT:
        if (game->fieldSelX == MAP_SIZE - 1)
        {
            if (game->mapSelX != MAP_SIZE - 1)
            {
                game->mapSelX++;
                game->fieldSelX = 0;
                printf("map right\n");
            }
        }

        else
        {
            game->fieldSelX++;
            printf("field right\n");
        }

        break;

    case DIRECTION_LEFT:
        if (game->fieldSelX == 0)
        {
            if (game->mapSelX != 0)
            {
                game->mapSelX--;
                game->fieldSelX = MAP_SIZE - 1;
                printf("map left\n");
            }
        }

        else
        {
            game->fieldSelX--;
            printf("field left\n");
        }
    
    default:
        break;
    }
}

int GMakeMove(struct GAME*, int, int, int, int, int);

/**
 * @brief Инициализирует игру
 * 
 * @param game
 */
void GInit(struct GAME* game)
{
    MInit(&(game->gameMap));

    game->turn = SettingFirst;
    game->GMakeMove = GMakeMove;

    game->locked = 0;
    game->winner = 0;

    game->mapSelX = 0;
    game->mapSelY = 0;
    game->fieldSelX = 0;
    game->fieldSelY = 0;
}

/**
 * @brief Делает ход
 * 
 * @param game Игра
 * @param MapX MAP [MapX][]
 * @param MapY MAP [][MapY]
 * @param FieldX FIELD [FieldX][]
 * @param FieldY FIELD [][FieldY]
 * @param turn Определяет чей ход
 */
int GMakeMove(struct GAME* game, int MapX, int MapY, int FieldX, int FieldY, int turn)
{
    if (game->gameMap.FMap[MapX][MapY].winner != 0)
    {
        return 1;
    }

    if (FCheckFieldDraw(&game->gameMap.FMap[MapX][MapY]))
    {
        return 1;
    }

    if(game->locked)
    {
        if (game->lockedX != MapX || game->lockedY != MapY)
        {
            return 1;
        }
    }

    else
    {
        // lock'аем карту
        game->locked = GAME_LOCKED;
        game->lockedX = MapX;
        game->lockedY = MapY;
    }

    if (game->gameMap.FMap[MapX][MapY].fieldCells[FieldX][FieldY] == 0)
    {
        game->gameMap.FMap[MapX][MapY].fieldCells[FieldX][FieldY] = game->turn;
        game->gameMap.FMap[MapX][MapY].FUpdateFieldScore(&(game->gameMap.FMap[MapX][MapY]));

        game->gameMap.FMap[MapX][MapY].winner = FCheckFieldWin(&game->gameMap.FMap[MapX][MapY]);
        if (FCheckFieldDraw(&game->gameMap.FMap[MapX][MapY]))
        {
            game->locked = GAME_UNLOCKED;
        }

        if (game->gameMap.FMap[MapX][MapY].winner == MAP_SIZE || game->gameMap.FMap[MapX][MapY].winner == MAP_SIZE * (-1))
        {
            printf("WINNER: %d", game->gameMap.FMap[MapX][MapY].winner);
            game->locked = GAME_UNLOCKED;
            MUpdateMapScore(&game->gameMap);
            game->winner = MCheckMapWin(&game->gameMap);

            if (game->winner != 0)
            {
                return game->winner;
            }
        }

        game->turn = game->turn * (-1);
        // miniMaxField(game->gameMap.FMap[MapX][MapY], game->turn, 12, 1);
    }
    
    else
        return 1;

    return 0;
}

#endif