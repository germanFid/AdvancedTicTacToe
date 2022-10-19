#include "structure.h"

#define AI_DEBUG

int miniMaxHandler(struct GAME* game, int turn, int maxDepth)
{
    struct GAME game_copy = *game;
}

int miniMaxField(struct FIELD field, int turn, int maxDepth, int curDepth, int* retX, int* retY) // default curdepth = 1
{
    int result = 0;

    if (curDepth > maxDepth)
    {
        return 0;
    }

    if (field.winner != 0)
    {
        return field.winner;
    }

#ifdef AI_DEBUG
    printf("\n=== minimax on depth %d\nTurn: %d\n\n", curDepth, turn);
#endif

    int peakScore = 0;

    // cord's of the best score
    int peakX = 1;
    int peakY = 1;

    int lastX, lastY;

    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (field.fieldCells[i][j] != 0)
            {
                continue;
            }

            // remember the last empty cell for ai if it is not
            // enough deep for making smart turn
            lastX = i; lastY = j;

#ifdef AI_DEBUG
            printf("=== %d %d ===\n", i, j);
#endif

            struct FIELD tmpField = field; // temporary field for calculating best move
            tmpField.fieldCells[i][j] = turn; // making turn
            tmpField.FUpdateFieldScore(&tmpField);
            tmpField.winner = FCheckFieldWin(&tmpField);

            result = miniMaxField(tmpField, turn * (-1), maxDepth, curDepth + 1, NULL, NULL);

            // find desired score
            if ((turn < 0 && result < 0) || (turn > 0 && result > 0))
            {
                peakScore = result;
                peakX = i; peakY = j;
            }

            else
            {
                continue;
            }

#ifdef AI_DEBUG
            printf("Peak : %d\n", peakScore);
#endif
        }
    }
    
    if (curDepth == 1)
    {
        if (field.fieldCells[peakX][peakY] != 0)
        {
#ifdef AI_DEBUG
            printf("LAST\n");
#endif
            peakX = lastX;
            peakY = lastY;
        }

#ifdef AI_DEBUG
        printf("MOVE: %d %d\n", peakX, peakY);
#endif
        if (retX != NULL && retY != NULL)
        {
            *retX = peakX;
            *retY = peakY;
        }
    }
    
    return peakScore;
}

int AIMakeMove(struct GAME* game)
{
    if (game->locked)
    {
        int retX, retY;
        miniMaxField(game->gameMap.FMap[game->lockedX][game->lockedY], 
                     game->turn, 13, 1, &retX, &retY);
        GMakeMove(game, game->lockedX, game->lockedY, retX, retY, game->turn);
    }

    else
    {
        printf("TBD!\n");
        return EXIT_FAILURE;
    }
}