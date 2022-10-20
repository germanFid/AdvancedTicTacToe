#include "structure.h"

// #define AI_DEBUG

#define AI_NAIVE 1
#define AI_NOT_NAIVE 0
int AI_NAIVE_MODE = AI_NAIVE; // switches 'naive mode'

#define AI_DEPTH_EASY 3
#define AI_DEPTH_MEDIUM 5
#define AI_DEPTH_HARD 5
#define AI_DEPTH_SUPERHARD 9
int AI_DEPTH = 3;

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
    for (int ts = 1; ts < curDepth; ts++) printf("\t");
    printf("=== minimax on depth %d, Turn: %d\n\n", curDepth, turn);
#endif

    int peakScore = turn * (-3);

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
            for (int ts = 1; ts < curDepth; ts++) printf("\t");
            printf("=== %d %d ===\n", i, j);
#endif

            struct FIELD tmpField = field; // temporary field for calculating best move
            tmpField.fieldCells[i][j] = turn; // making turn
            tmpField.FUpdateFieldScore(&tmpField);
            tmpField.winner = FCheckFieldWin(&tmpField);

            result = miniMaxField(tmpField, turn * (-1), maxDepth, curDepth + 1, NULL, NULL);
            if (result == 0)
            {
                printf("DRAW!\n");
            }
            if (curDepth == 1)
            {
                printf("res (%d %d): %d\n", i, j, result);
            }

            // find desired score
            if ((turn < 0 && result < peakScore) || (turn > 0 && result > peakScore))
            {
                if (result == 0 || result == MAP_SIZE || result == MAP_SIZE * (-1))
                {
                    peakScore = result;
                    peakX = i; peakY = j;
                    if (curDepth == 1)
                        printf("writtern peek: %d %d : %d\n", turn, result, peakScore);
                }
            }

            else
            {
                if (curDepth == 1)
                    printf("nope... peek: %d %d : %d\n", turn, result, peakScore);
                continue;
            }

#ifdef AI_DEBUG
        
            for (int ts = 1; ts < curDepth; ts++) printf("\t");
               printf("Peak : %d\n", peakScore);
#endif
        }
    }

#ifdef AI_DEBUG
        for (int ts = 1; ts < curDepth; ts++) printf("\t");
        printf("Abs.Local Peak : %d\n", peakScore);
#endif
    
    if (curDepth == 1)
    {
        if (field.fieldCells[peakX][peakY] != 0)
        {
// #ifdef AI_DEBUG
            for (int ts = 1; ts < curDepth; ts++) printf("\t");
            printf("%d %d -- LAST\n", peakX, peakY);
// #endif
            peakX = lastX;
            peakY = lastY;
        }

#ifdef AI_DEBUG
        for (int ts = 1; ts < curDepth; ts++) printf("\t");
        printf("MOVE: %d %d\n", peakX, peakY);
#endif
        if (retX != NULL && retY != NULL)
        {
            *retX = peakX;
            *retY = peakY;
        }
    }

    if (!AI_NAIVE_MODE)
    {
        if (FCheckFieldDraw(&field) && FCheckFieldWin(&field) == 0)
        {
            return 0;
        }
    }
    
    return peakScore;
}

int AIMapToField(struct MAP* map, struct FIELD* field)
{
    FInit(field);
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            int winner = map->FMap[i][j].winner;
            if (FCheckFieldDraw(&map->FMap[i][j]))
            {
                winner = SettingFirst;
            }
            if(winner != 0)
            {
                field->fieldCells[i][j] = winner;
            }
        }
    }
    
}

int AIMakeMove(struct GAME* game)
{
    if (game->locked)
    {
        printf("====== GAME IS LOCKED!\n");
        int retX, retY;
        miniMaxField(game->gameMap.FMap[game->lockedX][game->lockedY], 
                     game->turn, AI_DEPTH, 1, &retX, &retY);
        GMakeMove(game, game->lockedX, game->lockedY, retX, retY, game->turn);
    }

    else
    {
        printf("====== GAME IS UNLOCKED!\n");
        struct FIELD fld;
        AIMapToField(&game->gameMap, &fld);

        int mretX, mretY;
        miniMaxField(fld, game->turn, AI_DEPTH, 1, &mretX, &mretY);

        printf("====== MAP: %d %d\n", mretX, mretX);
        int retX, retY;
        miniMaxField(game->gameMap.FMap[mretX][mretY], game->turn, AI_DEPTH, 1, &retX, &retY);
        printf("====== FIELD: %d %d\n", retX, retY);
        GMakeMove(game, mretX, mretY, retX, retY, game->turn);

        return EXIT_FAILURE;
    }
}