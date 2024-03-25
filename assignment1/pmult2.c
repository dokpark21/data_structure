#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TERMS 100

typedef struct
{
    float coef;
    int expon;
} polynomial;

void multiplyPolynomials(int startA, int finishA, int startB, int finishB, int *startD, int *finishD, polynomial terms[MAX_TERMS])
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

void printPolynomial(int start, int finish, polynomial terms[MAX_TERMS])
{
    for (int i = start; i <= finish; i++)
    {
        if (terms[i].coef == 0)
            continue;
        printf("%.2fx^%d", terms[i].coef, terms[i].expon);
        if (i != finish)
            printf(" + ");
    }
    printf("\n");
}

int main()
{
    polynomial terms[MAX_TERMS] = {0};
    polynomial terms2[MAX_TERMS] = {0};

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

    int startA2, finishA2, startB2, finishB2, startD2, finishD2;
    // 다항식 : A: 1.0x^5 + 2.0x^4 + 3.0x^3 + 4.0x^2 + 5.0x^1 + 6.0x^0, B: 1.0x^4 + 2.0x^3 + 3.0x^2 + 4.0x^1 + 5.0x^0
    terms2[0].coef = 1.0;
    terms2[0].expon = 5;
    terms2[1].coef = 2.0;
    terms2[1].expon = 4;
    terms2[2].coef = 3.0;
    terms2[2].expon = 3;
    terms2[3].coef = 4.0;
    terms2[3].expon = 2;
    terms2[4].coef = 5.0;
    terms2[4].expon = 1;
    terms2[5].coef = 6.0;
    terms2[5].expon = 0;
    terms2[6].coef = 1.0;
    terms2[6].expon = 4;
    terms2[7].coef = 2.0;
    terms2[7].expon = 3;
    terms2[8].coef = 3.0;
    terms2[8].expon = 2;
    terms2[9].coef = 4.0;
    terms2[9].expon = 1;
    terms2[10].coef = 5.0;
    terms2[10].expon = 0;
    startA2 = 0;
    finishA2 = 5;
    startB2 = 6;
    finishB2 = 10;

    multiplyPolynomials(startA, finishA, startB, finishB, &startD, &finishD, terms);
    multiplyPolynomials(startA2, finishA2, startB2, finishB2, &startD2, &finishD2, terms2);

    printPolynomial(startD, finishD, terms);
    printPolynomial(startD2, finishD2, terms2);
    return 0;
}
