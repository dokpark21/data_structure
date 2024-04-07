#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 100

typedef struct
{
    int top;
    char items[MAX_SIZE][MAX_SIZE]; // 스택 아이템을 문자열로 변경
} Stack;

void push(Stack *s, char *value)
{
    if (s->top < MAX_SIZE - 1)
    {
        strcpy(s->items[++(s->top)], value);
    }
    else
    {
        printf("Stack full\n");
    }
}

char *pop(Stack *s)
{
    if (s->top > -1)
    {
        return s->items[(s->top)--];
    }
    else
    {
        return ""; // 스택이 비었음을 나타내는 빈 문자열 반환
    }
}

int is_empty(Stack *s)
{
    return (s->top == -1);
}

int precedence(char *op)
{
    if (strcmp(op, "+") == 0 && strlen(op) == 1 || strcmp(op, "-") == 0 && strlen(op) == 1)
        return 14;
    if (strcmp(op, "/") == 0)
        return 13;
    if (strcmp(op, "%") == 0)
        return 13;
    if (strcmp(op, "*") == 0)
        return 13; // multiplicative
    if (strcmp(op, "+") == 0)
        return 12;
    if (strcmp(op, "-") == 0)
        return 12; // binary plus and minus
    if (strcmp(op, "<<") == 0)
        return 11;
    if (strcmp(op, ">>") == 0)
        return 11;
    if (strcmp(op, "<=") == 0)
        return 10;
    if (strcmp(op, ">=") == 0)
        return 10;
    if (strcmp(op, "<") == 0)
        return 10;
    if (strcmp(op, ">") == 0)
        return 10;
    if (strcmp(op, "==") == 0)
        return 9;
    if (strcmp(op, "!=") == 0)
        return 9;
    if (strcmp(op, "&&") == 0)
        return 5;
    if (strcmp(op, "||") == 0)
        return 4;

    return -1; // not a valid operator
}

void postfix_new(char *infix)
{
    Stack s;
    s.top = -1;
    char output[MAX_SIZE][MAX_SIZE]; // 후위 표현식을 저장할 배열
    int j = 0, k = 0;

    char token[MAX_SIZE];
    char *pt = strtok(infix, " "); // 공백으로 토큰화

    while (pt != NULL)
    {
        if (isalnum(pt[0]))
        { // 피연산자인 경우
            strcpy(output[j++], pt);
        }
        else if (strcmp(pt, "(") == 0)
        {
            push(&s, pt);
        }
        else if (strcmp(pt, ")") == 0)
        {
            while (strcmp(s.items[s.top], "(") != 0)
            {
                strcpy(output[j++], pop(&s));
            }
            pop(&s); // '(' 제거
        }
        else
        { // 연산자인 경우
            while (!is_empty(&s) && precedence(pt) <= precedence(s.items[s.top]))
            {
                strcpy(output[j++], pop(&s));
            }
            push(&s, pt);
        }
        pt = strtok(NULL, " ");
    }

    while (!is_empty(&s))
    {
        strcpy(output[j++], pop(&s));
    }

    for (k = 0; k < j; k++)
    {
        printf("%s ", output[k]);
    }
}

// 수식에 포함된 복잡한 연산자도 고려하여 공백으로 구분하는 함수
void tokenizeExpression(char *expr, char *tokenized)
{
    int len = strlen(expr);
    int i = 0, j = 0;

    while (i < len)
    {
        if (expr[i] == '+' || expr == '-' || expr[i] == '*' || expr[i] == '/' || expr[i] == '%')
        {
            if (j > 0 && tokenized[j - 1] != ' ')
                tokenized[j++] = ' ';
            tokenized[j++] = expr[i];
            tokenized[j++] = ' ';
        }
        // 현재 문자가 연산자의 일부인지 확인하고, 다음 문자와 함께 복잡한 연산자를 구성하는지 검사
        else if (expr[i] == '<' || expr[i] == '>' || expr[i] == '!' || expr[i] == '=' || expr[i] == '&' || expr[i] == '|')
        {
            // 복잡한 연산자 처리 (&&, ||, <=, >=, ==, !=)
            if ((i + 1 < len) && (expr[i + 1] == '=' || expr[i + 1] == '&' || expr[i + 1] == '|'))
            {
                // 이전 문자가 공백이 아니라면 공백 추가
                if (j > 0 && tokenized[j - 1] != ' ')
                    tokenized[j++] = ' ';
                tokenized[j++] = expr[i];   // 현재 문자 추가
                tokenized[j++] = expr[++i]; // 다음 문자 추가
                tokenized[j++] = ' ';       // 연산자 뒤에 공백 추가
            }
            else
            { // 단일 연산자 처리 (+, -, *, /, %, <, >)
                if (j > 0 && tokenized[j - 1] != ' ')
                    tokenized[j++] = ' ';
                tokenized[j++] = expr[i];
                tokenized[j++] = ' ';
            }
        }
        else if (expr[i] == '(' || expr[i] == ')')
        { // 괄호 처리
            if (j > 0 && tokenized[j - 1] != ' ')
                tokenized[j++] = ' ';
            tokenized[j++] = expr[i];
            tokenized[j++] = ' ';
        }
        else
        { // 피연산자 처리
            tokenized[j++] = expr[i];
        }
        i++;
    }
    tokenized[j] = '\0'; // NULL 문자로 문자열 종료
}

int main()
{
    char infix[MAX_SIZE];
    char tokenized[MAX_SIZE];

    printf("Enter an infix expression: ");
    gets(infix);

    tokenizeExpression(infix, tokenized);
    printf("Tokenized expression: %s\n", tokenized);

    printf("Postfix expression: ");
    postfix_new(tokenized);

    return 0;
}
