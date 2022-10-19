#include "ai.h"

#define TEST_N 4

int main()
{
    // init test field
    struct FIELD testField;
    FInit(&testField);

    if (TEST_N == 1)
    {
        testField.fieldCells[0][0] = TURNO;
        testField.fieldCells[0][2] = TURNX;
        testField.fieldCells[1][0] = TURNX;
        testField.fieldCells[2][0] = TURNX;
        testField.fieldCells[2][1] = TURNO;
        testField.fieldCells[2][2] = TURNO;
    }

    else if (TEST_N == 2)
    {
        testField.fieldCells[0][0] = TURNO;
        testField.fieldCells[0][1] = TURNX;
        testField.fieldCells[0][2] = TURNX;
        testField.fieldCells[1][0] = TURNX;
        testField.fieldCells[2][0] = TURNX;
        testField.fieldCells[2][1] = TURNO;
        testField.fieldCells[2][2] = TURNO;
    }

    else if (TEST_N == 3)
    {
        testField.fieldCells[1][1] = TURNX;
    }

    else if (TEST_N == 4)
    {
        testField.fieldCells[0][0] = TURNX;
        testField.fieldCells[2][0] = TURNX;
        testField.fieldCells[1][1] = TURNX;
        // testField.fieldCells[1][2] = TURNO;
    }

    int res = miniMaxField(testField, TURNO, 3, 1, NULL, NULL);

    printf(">> %d\n", res);
}