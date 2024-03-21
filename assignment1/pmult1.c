#include <stdio.h>
#include <stdlib.h>

#define MAX_DEGREE 101

// polynomial의 첫번째 표현 방법은 하나의 구조체로 하나의 식을 나타낸다.
// degree는 식의 차수를 나타내고 coef는 각 항의 계수를 나타낸다.
// 한 배열의 첫 번째 인덱스는 degree이고 나머지 인덱스는 각 항의 계수를 나타낸다.
typedef struct
{
    int degree;
    float coef[MAX_DEGREE];
} polynomial;

// 두 다항식을 곱하는 함수
polynomial multiplyPolynomials(polynomial poly1, polynomial poly2)
{
    polynomial result;
    int i, j;

    // 결과 다항식의 차수는 두 다항식의 차수의 합과 같음
    result.degree = poly1.degree + poly2.degree;

    // 결과 다항식의 각 항을 초기화
    for (i = 0; i <= result.degree; i++)
        result.coef[i] = 0.0;

    // 두 다항식을 곱하여 결과 다항식의 각 항에 더함
    for (i = 0; i <= poly1.degree; i++)
    {
        for (j = 0; j <= poly2.degree; j++)
        {
            result.coef[i + j] += poly1.coef[i] * poly2.coef[j];
        }
    }

    return result;
}

int main()
{
    // 다항식 초기화
    polynomial poly1 = {2, {1.0, 2.0, 4.0}};
    polynomial poly2 = {3, {3.0, 4.0, 5.0, 3.0}};

    // 다항식 곱셈 수행
    polynomial result = multiplyPolynomials(poly1, poly2);

    // 결과 출력
    printf("Result : ");
    for (int i = 0; i <= result.degree; i++)
    {
        if (result.coef[i] == 0)
            continue;
        printf("%.2fx^%d ", result.coef[i], result.degree - i);
        if (i != result.degree)
            printf("+ ");
    }
    printf("\n");

    return 0;
}
