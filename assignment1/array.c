#include <stdio.h>
#include <stdlib.h>

// 배열을 생성하고 순차적인 정수 값으로 채우는 함수
void fillArray(int **array, int rows, int cols)
{
    int count = 1;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            array[i][j] = count++;
        }
    }
}

// 배열의 주소와 저장된 데이터를 출력하는 함수
void printArray(int **array, int rows, int cols)
{
    printf("Array:\n");
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("[%p]: %d\t", (void *)&array[i][j], array[i][j]);
        }
        printf("\n");
    }
}

int main()
{
    // array1(5x3)
    int r1 = 5, c1 = 3;
    int **array1 = (int **)malloc(r1 * sizeof(int *));

    // 각 행에 대한 연속된 메모리 블록 할당
    array1[0] = (int *)malloc(r1 * c1 * sizeof(int));
    for (int i = 1; i < r1; i++)
    {
        array1[i] = array1[0] + i * c1;
    }

    fillArray(array1, r1, c1);
    printArray(array1, r1, c1);

    // array1 해제
    free(array1);

    // array2
    // 행, 열 받아서 메모리 동작 할당(2차원 배열)
    int r2, c2;
    printf("Enter the number of rows for array2: ");
    scanf("%d", &r2);
    printf("Enter the number of columns for array2: ");
    scanf("%d", &c2);

    int **array2 = (int **)malloc(r2 * sizeof(int *));

    // 각 행에 대한 연속된 메모리 블록 할당
    array2[0] = (int *)malloc(r2 * c2 * sizeof(int));
    for (int i = 1; i < r2; i++)
    {
        array2[i] = array2[0] + i * c2;
    }
    fillArray(array2, r2, c2);
    printArray(array2, r2, c2);

    // array2 메모리 해제
    free(array2);

    return 0;
}
