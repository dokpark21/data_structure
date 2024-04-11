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
    if (strcmp(op, "p") == 0 || strcmp(op, "m") == 0)
        return 15;
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

void postfix_new(char *infix, char *postfix)
{
    Stack s;
    s.top = -1;
    int j = 0;

    char temp[MAX_SIZE];
    char *pt = strtok(infix, " ");

    while (pt != NULL)
    {
        if (isdigit(pt[0]))
        {
            strcat(postfix, pt);
            strcat(postfix, " ");
        }
        else if (strcmp(pt, "(") == 0)
        {
            push(&s, pt);
        }
        else if (strcmp(pt, ")") == 0)
        {
            while (strcmp(s.items[s.top], "(") != 0)
            {
                strcpy(temp, pop(&s));
                strcat(postfix, temp);
                strcat(postfix, " ");
            }
            pop(&s); // '(' 제거
        }
        else
        {
            while (!is_empty(&s) && precedence(pt) <= precedence(s.items[s.top]))
            {
                strcpy(temp, pop(&s));
                strcat(postfix, temp);
                strcat(postfix, " ");
            }
            push(&s, pt);
        }
        pt = strtok(NULL, " ");
    }

    while (!is_empty(&s))
    {
        strcpy(temp, pop(&s));
        strcat(postfix, temp);
        strcat(postfix, " ");
    }
}

// 수식에 포함된 복잡한 연산자도 고려하여 공백으로 구분하는 함수
void tokenizeExpression(char *expr, char *tokenized)
{
    int len = strlen(expr);
    int i = 0, j = 0;

    while (i < len)
    {
        if (expr[i] == '+' && i == 0 || expr[i] == '+' && !isalnum(expr[i - 1]) && !isalnum(expr[i + 1]))
        {
            tokenized[j++] = 'p';
            tokenized[j++] = ' ';
            i++;
            continue;
        }

        if (expr[i] == '-' && i == 0 || expr[i] == '-' && !isalnum(expr[i - 1]) && !isalnum(expr[i + 1]))
        {
            tokenized[j++] = 'm';
            tokenized[j++] = ' ';
            i++;
            continue;
        }

        if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/' || expr[i] == '%')
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

int eval(char *postfix)
{
    Stack s;
    s.top = -1;
    char *token;
    char numStr[10];
    int op1, op2;
    int result;

    // strtok를 사용하여 토큰화
    token = strtok(postfix, " ");
    while (token != NULL)
    {
        if (isdigit(token[0]))
        {
            // 숫자인 경우 스택에 push
            push(&s, token);
        }
        else
        {
            if (strcmp(token, "p") == 0 || strcmp(token, "m") == 0)
            {
                op1 = atoi(pop(&s));
            }
            else
            {
                // 연산자인 경우 두 개의 숫자를 스택에서 pop하여 계산
                op2 = atoi(pop(&s)); // 먼저 pop한 것이 op2
                op1 = atoi(pop(&s)); // 그 다음 pop한 것이 op1
            }
            switch (token[0])
            { // 연산자를 처리
            case 'p':
                snprintf(numStr, 10, "%d", op1);
                break;
            case 'm':
                snprintf(numStr, 10, "%d", -op1);
                break;
            case '+':
                snprintf(numStr, 10, "%d", op1 + op2);
                break;
            case '-':
                snprintf(numStr, 10, "%d", op1 - op2);
                break;
            case '*':
                snprintf(numStr, 10, "%d", op1 * op2);
                break;
            case '/':
                if (op2 == 0)
                {
                    fprintf(stderr, "Division by zero error.\n");
                    exit(EXIT_FAILURE);
                }
                snprintf(numStr, 10, "%d", op1 / op2);
                break;
            case '%':
                if (op2 == 0)
                {
                    fprintf(stderr, "Division by zero error.\n");
                    exit(EXIT_FAILURE);
                }
                snprintf(numStr, 10, "%d", op1 % op2);
                break;
            default:
                fprintf(stderr, "Invalid operator: %s\n", token);
                exit(EXIT_FAILURE);
            }
            push(&s, numStr); // 계산 결과를 스택에 push
        }
        token = strtok(NULL, " "); // 다음 토큰을 가져옴
    }
    // 단항 연산자가 많아서 스택에 마지막에 2개 이상의 숫자가 남은경우
    if (s.top >= 1)
    {
        while (s.top > 0)
        {
            op1 = atoi(pop(&s));
            op2 = atoi(pop(&s));
            sprintf(numStr, "%d", op1 + op2);
            push(&s, numStr);
        }
        result = atoi(pop(&s));
        return result;
    }
    result = atoi(pop(&s));
    return result; // 마지막 결과 반환
}

int main()
{
    char infix[MAX_SIZE] = {0};
    char tokenized[MAX_SIZE * 2] = {0}; // 공백 추가를 위해 크기를 늘림
    char postfix[MAX_SIZE * 2] = {0};   // 후위 표현식 저장을 위해 충분한 크기 할당
    int result;

    printf("Enter an infix expression: ");
    fgets(infix, MAX_SIZE, stdin);
    infix[strcspn(infix, "\n")] = 0; // fgets로 인한 개행 문자 제거

    tokenizeExpression(infix, tokenized);
    printf("Tokenized expression: ");
    for (int i = 0; i < strlen(tokenized); i++)
    {
        if (tokenized[i] == 'p')
            printf("+");
        else if (tokenized[i] == 'm')
            printf("-");
        else
            printf("%c", tokenized[i]);
    }

    postfix_new(tokenized, postfix);
    printf("\nPostfix expression: ");
    for (int i = 0; i < strlen(postfix); i++)
    {
        if (postfix[i] == 'p')
            printf("+");
        else if (postfix[i] == 'm')
            printf("-");
        else
            printf("%c", postfix[i]);
    }

    result = eval(postfix);
    printf("\nResult: %d\n", result);

    return 0;
}