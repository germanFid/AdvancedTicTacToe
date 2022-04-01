#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SCORE_SIZE 8
#define CELLS_SIZE 3

struct FIELD // Поле
{
    char fieldCells[CELLS_SIZE][CELLS_SIZE];  // Клеточки внутри поля
    char fieldAxisScore[SCORE_SIZE]; // Счет на "осях" (локальный)

    void (*FUpdateFieldScore)(struct FIELD*);
};

void FUpdateFieldScore(struct FIELD*);

void FInit(struct FIELD* field)
{
    memset(*field->fieldCells, 0, CELLS_SIZE * CELLS_SIZE); // Обнуляем поле
    memset(*field->fieldAxisScore, 0, SCORE_SIZE); // Обнуляем счет

    field->FUpdateFieldScore = FUpdateFieldScore; // Указатель на функцию апдейта счета
}

// ОБЕРНУТЬ В DEFINE

void FUpdateFieldScore(struct FIELD* field) // Обновляет счет всего поля
{
    char* score = field->fieldAxisScore;

    memset(*field->fieldAxisScore, 0, SCORE_SIZE); // Обнуляем счет

    // Обновляем строчки и столбцы
    for (int i = 0; i < CELLS_SIZE; i++) 
    {
        int sum = 0;
        for (int j = 0; j < CELLS_SIZE; j++)
        {
            score[i] += field->fieldCells[i][j]; // Если крестик - плюсуем, иначе - минусуем
            score[6 - j] += field->fieldCells[i][j];
        }
    }

    // Обновляем диагонали
    int j = CELLS_SIZE;
    for (int i = 0; i < CELLS_SIZE; i++)
    {
        score[CELLS_SIZE] += field->fieldCells[i][i];
        score[CELLS_SIZE*CELLS_SIZE - 2] += field->fieldCells[j][j];
        j--;
    }
    
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
    for (int i = 0; i < CELLS_SIZE; i++)
    {
        printf("%d ", field->fieldAxisScore[i]);
    }
    printf("\n");
}

struct MAP // Карта
{
};
