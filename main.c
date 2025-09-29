#include <stdio.h>
#include <stdlib.h>

#define MAX 12

typedef struct Node
{
    int index;
    struct Node *next;
} Node;

typedef struct
{
    char department[MAX];
    Node *head;
} Table;

typedef struct
{
    char name[MAX];
    char department[MAX];
    float salary;
    char status;
} Employee;

int main()
{


    return 0;
}