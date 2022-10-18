
#include "structure.h"

int minimaxHandler(struct GAME* game, int turn, int maxDepth)
{
    struct GAME game_copy = *game;
}

int miniMaxField(struct FIELD field, int turn, int maxDepth, int curDepth) // default curdepth = 1
{
    if (curDepth > maxDepth)
    {
        return 0;
    }
    int maxX = -1;
    int maxY = -1;
    int maxVal = 0;

    int minX = -1;
    int minY = -1;
    int minVal = 999;

    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (field.fieldCells[i][j] == 0)
            {
                struct FIELD newField = field;
                newField.fieldCells[i][j] = turn;
                FUpdateFieldScore(&newField);
                FCheckFieldWin(&newField);

                if (field.winner)
                {
                    return field.winner;
                }

                if (FCheckFieldDraw(&newField))
                {
                    return 0;
                }

                int iResult = miniMaxField(newField, turn * (-1), maxDepth, curDepth + 1);
                if (curDepth % 2 != 0)
                {
                    if (iResult > maxVal)
                    {
                        maxVal = iResult;
                        maxX = i;
                        maxY = j;
                    }
                }

                else
                {
                    if (iResult < minVal)
                    {
                        minVal = iResult;
                        minX = i;
                        minY = j;
                    }
                }
            }
        }
        
    }
    if (curDepth % 2 != 0)
    {
        printf("Max: %d %d\n", maxX, maxY);
        printf("VAL: %d\n", maxVal);
    }

    else
    {
       printf("Min: %d %d\n", minX, minY);
    }
}