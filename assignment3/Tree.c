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
        Joint *temp = parent;
        while (temp->sibling != NULL)
        {
            temp = temp->sibling;
        }
        temp->sibling = child;
    }
}

void traverseAndPrint(Joint *joint, float parentX, float parentY)
{
    if (joint == NULL)
        return;

    float currentX = parentX + joint->offsetX;
    float currentY = parentY + joint->offsetY;

    printf("%s: (%.2f, %.2f)\n", joint->name, currentX, currentY);

    traverseAndPrint(joint->child, currentX, currentY);

    traverseAndPrint(joint->sibling, parentX, parentY);
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
    traverseAndPrint(hips, 0, 0);

    return 0;
}
