#include <stdio.h>
#include <stdlib.h>

// 다항식 리스트의 노드 구조를 구조체로 정의
typedef struct ListNode
{
    float coef;
    int expo;
    struct ListNode *link;
} ListNode;

// 다항식 리스트의 head 노드를 구조체로 정의
typedef struct LinkedList
{
    ListNode *head;
} LinkedList;

// 공백 다항식 리스트를 생성하는 함수
LinkedList *create_linked_list()
{
    LinkedList *L = (LinkedList *)malloc(sizeof(LinkedList));
    L->head = NULL;
    return L;
}

void append_term(LinkedList *L, float coef, int expo)
{
    ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));
    new_node->coef = coef;
    new_node->expo = expo;
    new_node->link = NULL;

    if (L->head == NULL)
    {
        L->head = new_node;
        return;
    }

    ListNode *current = L->head;
    while (current->link != NULL)
    {
        current = current->link;
    }
    current->link = new_node;
}

LinkedList *mul_poly(LinkedList *A, LinkedList *B)
{
    LinkedList *C = create_linked_list();
    ListNode *pA = A->head;
    while (pA)
    {
        ListNode *pB = B->head;
        while (pB)
        {
            float sum = pA->coef * pB->coef;
            int exposum = pA->expo + pB->expo;

            // 결과 다항식에 이미 같은 차수의 항이 있는지 확인하고,
            // 있다면 해당 항의 계수에 현재 계산된 값을 더해줍니다.
            ListNode *pC = C->head;
            ListNode *prev = NULL;
            while (pC && pC->expo > exposum)
            {
                prev = pC;
                pC = pC->link;
            }

            // 만약 동일한 차수의 항이 이미 존재하는 경우
            if (pC && pC->expo == exposum)
            {
                pC->coef += sum;
            }
            else
            {
                // 동일한 차수의 항이 없는 경우 새로운 항을 추가합니다.
                ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));
                new_node->coef = sum;
                new_node->expo = exposum;
                new_node->link = pC;

                // 새 항을 연결 리스트에 삽입합니다.
                if (prev == NULL)
                {
                    C->head = new_node;
                }
                else
                {
                    prev->link = new_node;
                }
            }
            pB = pB->link;
        }
        pA = pA->link;
    }
    return C;
}

// 다항식 리스트를 출력하는 함수
void print_poly(LinkedList *L)
{
    ListNode *current = L->head;
    while (current)
    {
        printf("%.0fx^%d", current->coef, current->expo);
        if (current->link != NULL)
        {
            printf(" + ");
        }
        current = current->link;
    }
    printf("\n");
}

// 주 함수
int main()
{
    LinkedList *A, *B, *C;

    // 공백 다항식 리스트 A, B, C 생성
    A = create_linked_list();
    B = create_linked_list();
    C = create_linked_list();

    // 다항식 : A: 1.0x^2 + 2.0x^1 + 4.0x^0, B: 3.0x^3 + 4.0x^2 + 5.0x^1 + 3.0x^0
    // 다항식 리스트 A에 노드 추가
    append_term(A, 1, 2);
    append_term(A, 2, 1);
    append_term(A, 4, 0);
    printf("A(x) = ");
    print_poly(A);

    // 다항식 리스트 B에 노드 추가
    append_term(B, 3, 3);
    append_term(B, 4, 2);
    append_term(B, 5, 1);
    append_term(B, 3, 0);
    printf("B(x) = ");
    print_poly(B);

    // 다항식 곱셈 연산 수행
    C = mul_poly(A, B);
    printf("C(x) = ");
    print_poly(C);

    return 0;
}
