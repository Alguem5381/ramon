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
void showFile(FILE *f);
int createTable(Table *table, FILE *file);
void showHash(Table *table, FILE *file);
int searchEmployee(Table *table, FILE *file, char *key);
void printDepartment(Node *head, FILE *file);
int printEmployee(int index, FILE *file);
void freeTable(Table *table);

int main()
{
    Table table[TF];
    FILE *file = NULL;
    int opcao;

    do {
        printf("\n------------MENU----------------\n");
        printf("1 - Exibe funcionarios do arq binario CADASTRO\n");
        printf("2 - Leitura do CADASTRO e construcao da tabela HASH por depto\n");
        printf("3 - Exibe tudo - tabela HASH\n");
        printf("4 - Consulta os funcionarios de um unico depto\n");
        printf("5 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // Limpa o \n do buffer

        switch(opcao){
            case 1:
                file = fopen("CADASTRO.dat", "rb");
                if(!file)
                    printf("Arquivo nao encontrado!\n");
                else{
                    showFile(file);
                    fclose(file);
                }
                break;

            case 2:
                file = fopen("CADASTRO.dat", "rb");
                if(!file){
                    printf("Arquivo nao encontrado!\n");
                } else {
                    if(createTable(table, file))
                        printf("Tabela HASH criada com sucesso.\n");
                    else
                        printf("Erro ao criar a tabela HASH.\n");
                    fclose(file);
                }
                break;

            case 3:
                file = fopen("CADASTRO.dat", "rb");
                if(!file){
                    printf("Arquivo nao encontrado!\n");
                } else {
                    showHash(table, file);
                    fclose(file);
                }
                break;

            case 4:
            {
                char dept[MAX];
                printf("Digite o departamento: ");
                fgets(dept, MAX, stdin);
                dept[strcspn(dept, "\n")] = 0;

                file = fopen("CADASTRO.dat", "rb");
                if(!file)
                    printf("Arquivo nao encontrado!\n");
                else{
                    if(!searchEmployee(table, file, dept))
                        printf("Departamento nao encontrado.\n");
                    fclose(file);
                }
                break;
            }

            case 5:
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while(opcao != 6);

    freeTable(table);

    return 0;
}

void showHash(Table *table, FILE *file)
{
    for (int i = 0; i < TF; i++)
    {
        if (!table[i].head)
            continue;

        printf("Posição da tabela: %i, Departamento: %s\n", i, table[i].department);
        printDepartment(table[i].head, file);
    }
}

int printEmployee(int index, FILE *file)
{
    Employee aux;

    fseek(file, index * sizeof(Employee), SEEK_SET);
    if (!fread(&aux, sizeof(Employee), 1, file))
        return 0;

    printf("Pos=%i %s %s %.2f %c\n", index, aux.name, aux.department, aux.salary, aux.status);

    return 1;
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
        table[i].head = NULL;

    Employee aux;
    int curr = 0;

    rewind(file);
    while (fread(&aux, sizeof(Employee), 1, file))
    {
        int index = hash(aux.department);

        int count;
        for (count = 0; count < TF && table[index].head && strcmp(table[index].department, aux.department) != 0; count++, index = (index + 1) % TF);

        if (count == TF) return 0;

        if (!table[index].head)
            strncpy(table[index].department, aux.department, MAX-1), table[index].department[MAX-1] = '\0';

        Node *node = (Node*)malloc(sizeof(Node));
        if (!node) return 0;

        node->index = curr;
        node->next = table[index].head;
        table[index].head = node;

        curr++;
    }

    return 1;
}

void showFile(FILE *f){
    Employee e;
    rewind(f);
    while (fread(&e, sizeof(Employee), 1, f) == 1){
        printf("Nome: %-13s Departamento: %-13s Salario: %6.2f Status: %c\n", e.name, e.department, e.salary, e.status);
    }
}

void printDepartment(Node *head, FILE *file){
    Node *curr = head;
    while (curr)
    {
        printEmployee(curr->index, file);
        curr = curr->next;
    }
}

void showHash(Table *table, FILE *file){
    for (int i = 0; i < TF; i++)
    {
        if (!table[i].head)
            continue;

        printf("Posicao da tabela: %i, Departamento: %s\n", i, table[i].department);
        printDepartment(table[i].head, file);
    }
}

int searchEmployee(Table *table, FILE *file, char *key){
    int index = hash(key);
    int count;
    for (count = 0; count < TF && table[index].head && strcmp(table[index].department, key) != 0; count++, index = (index + 1) % TF);

    if (count == TF) return 0;
    printDepartment(table[index].head, file);
    return 1;
}

void freeTable(Table *table){
    for(int i = 0; i < TF; i++){
        Node *curr = table[i].head;
        while(curr){
            Node *tmp = curr;
            curr = curr->next;
            free(tmp);
        }
        table[i].head = NULL;
    }
}
