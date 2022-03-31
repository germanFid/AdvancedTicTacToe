#include <stdlib.h>
#include <stdio.h>

struct FIELD // Поле
{
    char fieldCells[3][3];  // Клеточки внутри поля
    char fieldAxisScore[8]; // Счет на "осях" (локальный)

    // void (*UpdateFieldScore)(const struct FIELD*);
};

void FInit(struct FIELD* field)
{
    for (int i = 0; i < 9; i++) // Обнуляем поле
        (*field->fieldCells)[i] = 0;

    for (int i = 0; i < 8; i++) // Обнуляем счет
        field->fieldAxisScore[i] = 0;
}

void FUpdateFieldScore(struct FIELD* field) // Обновляет счет всего поля
{
    // char* cells = field->fieldCells;
    char* score = field->fieldAxisScore;

    for (int i = 0; i < 8; i++) // Обнуляем счет
        score[i] = 0;

    // Обновляем строчки и столбцы
    for (int i = 0; i < 3; i++) 
    {
        int sum = 0;
        for (int j = 0; j < 3; j++)
        {
            score[i] += field->fieldCells[i][j]; // Если крестик - плюсуем, иначе - минусуем
            score[6 - j] += field->fieldCells[i][j];
        }
    }

    // Обновляем диагонали
    int j = 3;
    for (int i = 0; i < 3; i++)
    {
        score[3] += field->fieldCells[i][i];
        score[7] += field->fieldCells[j][j];
        j--;
    }
    
}

void FDebugOutput(struct FIELD* field) // Выводит счет и поле
{
    // Вывод поля
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%d\t", field->fieldCells[i][j]);
        }
        printf("\n");
    }

    // Вывод счета    
    for (int i = 0; i < 8; i++)
    {
        printf("%d ", field->fieldAxisScore[i]);
    }
    printf("\n");
}

struct MAP // Карта
{
};
