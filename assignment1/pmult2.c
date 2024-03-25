#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TERMS 100

typedef struct
{
    float coef;
    int expon;
} polynomial;

polynomial terms[MAX_TERMS];

void multiplyPolynomials(int startA, int finishA, int startB, int finishB, int *startD, int *finishD)
{
    clock_t start_time = clock(); // 작업 시작 시간 측정

    int avail = finishB + 1;
    *startD = finishB + 1;

    for (int i = startA; i <= finishA; i++)
    {
        avail = *startD + i;
        for (int j = startB; j <= finishB; j++)
        {
            int exponent = terms[i].expon + terms[j].expon;
            float coefficient = terms[i].coef * terms[j].coef;

            // 결과 다항식에 항을 추가하거나 계수를 더합니다.
            while (terms[avail].expon > exponent && terms[avail].coef != 0)
            {
                avail++;
            }

            if (terms[avail].expon == exponent)
            {
                terms[avail].coef += coefficient;
            }
            else
            {
                terms[avail].coef = coefficient;
                terms[avail].expon = exponent;
            }
        }
    }

    *finishD = avail;

    clock_t end_time = clock();                                             // 작업 종료 시간 측정
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC; // 작업 시간 계산
    printf("작업 소요 시간: %f 초\n", elapsed_time);                        // 작업 시간 출력
}

int main()
{
    int startA, finishA, startB, finishB, startD, finishD;
    // 다항식 : A: 1.0x^2 + 2.0x^1 + 4.0x^0, B: 3.0x^3 + 4.0x^2 + 5.0x^1 + 3.0x^0
    terms[0].coef = 1.0;
    terms[0].expon = 2;
    terms[1].coef = 2.0;
    terms[1].expon = 1;
    terms[2].coef = 4.0;
    terms[2].expon = 0;
    terms[3].coef = 3.0;
    terms[3].expon = 3;
    terms[4].coef = 4.0;
    terms[4].expon = 2;
    terms[5].coef = 5.0;
    terms[5].expon = 1;
    terms[6].coef = 3.0;
    terms[6].expon = 0;
    startA = 0;
    finishA = 2;
    startB = 3;
    finishB = 6;

    multiplyPolynomials(startA, finishA, startB, finishB, &startD, &finishD);

    printf("Result : ");
    for (int i = startD; i <= finishD; i++)
    {
        if (terms[i].coef == 0)
            continue;
        printf("%.2fx^%d", terms[i].coef, terms[i].expon);
        if (i != finishD)
            printf(" + ");
    }
    printf("\n");
    return 0;
}
