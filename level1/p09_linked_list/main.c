#include <stdio.h>
#include <stdlib.h>

typedef struct Slist {
    int data;
    struct Slist *next;
} Slist;

void create_node(int data);
void add(int padd, int data);
void del(int pdel);
void print_list();

Slist *head = NULL;
int capacity = 0;

int main() {
    int token;
    printf("Please input the first number:\n");
    scanf("%d", &token);
    create_node(token);

    int ptoken;
    while (1) {
        printf("Please input the type of operation\n");
        printf("1. Add\n2. Delete\n3. Search\n4. Exit\n");
        scanf("%d", &token);
        switch (token) {
            case 1:
                printf("Please input the number:\n");
                scanf("%d", &token);
                printf("Please input the index:\n");
                scanf("%d", &ptoken);
                add(ptoken, token);
                break;
            case 2:
                printf("Please input the index you want to delete:\n");
                scanf("%d", &ptoken);
                del(ptoken);
                break;
            case 4:
                goto BREAK;
            default:
                printf("I can't understand you\n");
        }
    }

BREAK:
    print_list();
    return 0;
}

void create_node(int data) {
    Slist *node = (Slist *)malloc(sizeof(Slist));
    node->data = data;
    node->next = NULL;
    head = node;
    capacity++;
}

void add(int padd, int data) {
    if (padd < 0 || padd > capacity) {
        printf("Out of range\n");
        return;
    }

    Slist *node = (Slist *)malloc(sizeof(Slist));
    node->data = data;

    if (padd == 0) {
        node->next = head;
        head = node;
    } else {
        Slist *p = head;
        int count = 0;
        while (count < padd - 1 && p != NULL) {
            p = p->next;
            count++;
        }
        node->next = p->next;
        p->next = node;
    }
    capacity++;
}

void del(int pdel) {
    if (capacity == 0) {
        printf("The list is empty\n");
        return;
    }

    if (pdel < 0 || pdel >= capacity) {
        printf("Out of range\n");
        return;
    }

    if (pdel == 0) {
        Slist *temp = head;
        head = head->next;
        free(temp);
    } else {
        Slist *p = head;
        int count = 0;
        while (count < pdel - 1 && p->next != NULL) {
            p = p->next;
            count++;
        }
        Slist *temp = p->next;
        p->next = temp->next;
        free(temp);
    }
    capacity--;
}

void print_list() {
    Slist *p = head;
    if (capacity == 0) {
        printf("The list is empty\n");
        return;
    }

    while (p != NULL) {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");
}
