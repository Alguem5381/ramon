#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 12
#define TF 6

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

int hash(char *key);
//Para a opção 2
void showFile(FILE *f);
//Ler o binário e escrever na tabela
int createTable(Table *table, FILE *file);
void destroyTable(Table *table);
void showHash(Table *table);
int searchEmployee(Table *table, char *key);

int main()
{
    FILE *file = fopen("CADASTRO.dat", "rb");
    if(!file)
        printf("\nFile not found!");
    else{
        showFile(file);
    }
    
    fclose(file);
    return 0;
}

int hash(char *key)
{
    int value = 0;
    int len = strlen(key);

    for (int i = 0; i < len; i++)
        value += (int)key[i];

    return value % TF;
}

int createTable(Table *table, FILE *file)
{
    for (int i = 0; i < TF; i++)
        table->head = NULL;

    Employee aux;
    int curr = 0;

    rewind(file);                                                       //Por segurança
    while (fread(&aux, sizeof(Employee), 1, file))
    {
        int index = hash(aux.department);

        Node *node = (Node*)malloc(sizeof(Node));
        if (!node) return 0;

        node->index = curr;
        node->next = table[index].head;
        table[index].head = node;
    }
}

void showFile(FILE *f){
    Employee e;
    while (!feof(f)){
        fread(&e, sizeof(Employee), 1, f);
        printf("Nome: %-13s Departamento: %-13s Salario: %6.2f Status: %c\n", e.name, e.department, e.salary, e.status);
    }
}