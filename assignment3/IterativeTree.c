#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Joint
{
    char name[20];
    float offsetX, offsetY;
    struct Joint *child;
    struct Joint *sibling;
} Joint;

typedef struct StackNode
{
    Joint *joint;
    float parentX, parentY;
    struct StackNode *next;
} StackNode;

StackNode *createStackNode(Joint *joint, float parentX, float parentY)
{
    StackNode *node = (StackNode *)malloc(sizeof(StackNode));
    node->joint = joint;
    node->parentX = parentX;
    node->parentY = parentY;
    node->next = NULL;
    return node;
}

void push(StackNode **top, Joint *joint, float parentX, float parentY)
{
    StackNode *node = createStackNode(joint, parentX, parentY);
    node->next = *top;
    *top = node;
}

StackNode *pop(StackNode **top)
{
    if (*top == NULL)
        return NULL;
    StackNode *node = *top;
    *top = (*top)->next;
    return node;
}

int isEmpty(StackNode *top)
{
    return top == NULL;
}

Joint *createJoint(char *name, float offsetX, float offsetY)
{
    Joint *newJoint = (Joint *)malloc(sizeof(Joint));
    strcpy(newJoint->name, name);
    newJoint->offsetX = offsetX;
    newJoint->offsetY = offsetY;
    newJoint->child = NULL;
    newJoint->sibling = NULL;
    return newJoint;
}

void addChild(Joint *parent, Joint *child)
{
    if (parent->child == NULL)
    {
        parent->child = child;
    }
    else
    {
        Joint *temp = parent->child;
        while (temp->sibling != NULL)
        {
            temp = temp->sibling;
        }
        temp->sibling = child;
    }
}

void traverseAndPrint(Joint *root)
{
    if (root == NULL)
        return;

    StackNode *stack = NULL;
    push(&stack, root, 0, 0); // root 노드를 스택에 푸시합니다.

    while (!isEmpty(stack))
    {
        StackNode *node = pop(&stack); // 스택에서 노드를 꺼냅니다.
        Joint *joint = node->joint;
        float currentX = node->parentX + joint->offsetX; // 현재 노드의 x 좌표 계산
        float currentY = node->parentY + joint->offsetY; // 현재 노드의 y 좌표 계산

        printf("%s: (%.2f, %.2f)\n", joint->name, currentX, currentY);

        while (joint->sibling)
        {
            push(&stack, joint->sibling, node->parentX, node->parentY);
            joint = joint->sibling;
        }

        if (joint->child)
        {
            push(&stack, joint->child, currentX, currentY);
        }

        free(node); // 사용한 노드 메모리 해제
    }
}

int main()
{
    Joint *hips = createJoint("Hips", 0.0, 0.0);
    Joint *chest = createJoint("Chest", 0.0, 5.21);
    addChild(hips, chest);

    Joint *neck = createJoint("Neck", 0.0, 18.65);
    addChild(chest, neck);

    Joint *head = createJoint("Head", 0.0, 5.45);
    addChild(neck, head);

    Joint *leftCollar = createJoint("LeftCollar", 1.12, 16.23);
    Joint *rightCollar = createJoint("RightCollar", -1.12, 16.23);
    addChild(neck, leftCollar);
    addChild(neck, rightCollar);

    Joint *leftUpArm = createJoint("LeftUpArm", 5.54, 0.0);
    addChild(leftCollar, leftUpArm);

    Joint *leftLowArm = createJoint("LeftLowArm", 0.0, -11.96);
    addChild(leftUpArm, leftLowArm);

    Joint *leftHand = createJoint("LeftHand", 0.0, -9.93);
    addChild(leftLowArm, leftHand);

    Joint *rightUpArm = createJoint("RightUpArm", -6.07, 0.0);
    addChild(rightCollar, rightUpArm);

    Joint *rightLowArm = createJoint("RightLowArm", 0.0, -11.82);
    addChild(rightUpArm, rightLowArm);

    Joint *rightHand = createJoint("RightHand", 0.0, -10.65);
    addChild(rightLowArm, rightHand);

    Joint *leftUpLeg = createJoint("LeftUpLeg", 3.91, 0.0);
    addChild(hips, leftUpLeg);

    Joint *leftLowLeg = createJoint("LeftLowLeg", 0.0, -18.34);
    addChild(leftUpLeg, leftLowLeg);

    Joint *leftFoot = createJoint("LeftFoot", 0.0, -17.37);
    addChild(leftLowLeg, leftFoot);

    Joint *rightUpLeg = createJoint("RightUpLeg", -3.91, 0.0);
    addChild(leftUpLeg, rightUpLeg);

    Joint *rightLowLeg = createJoint("RightLowLeg", 0.0, -17.63);
    addChild(rightUpLeg, rightLowLeg);

    Joint *rightFoot = createJoint("RightFoot", 0.0, -17.14);
    addChild(rightLowLeg, rightFoot);

    printf("Humanoid Character Model Coordinates:\n");
    traverseAndPrint(hips);

    return 0;
}
