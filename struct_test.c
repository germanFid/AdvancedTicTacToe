#include "structure.h"

int main()
{
    struct FIELD field;

    FInit(&field);
    FDebugOutput(&field);

    field.fieldCells[0][0] = 1;
    FUpdateFieldScore(&field);
    FDebugOutput(&field);
    
    // field.fieldCells[2][2] = -1;
    // FUpdateFieldScore(&field);
    // FDebugOutput(&field);
}