#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LEVEL 16
#define P 0.5

typedef struct Node {
    int key;
    struct Node **forward;
} Node;

typedef struct SkipList {
    int level;
    Node *header;
} SkipList;

Node* create_node(int key, int level) {
    Node *n = (Node*)malloc(sizeof(Node));
    n->key = key;
    n->forward = (Node**)malloc(sizeof(Node*) * (level + 1));
    for(int i = 0; i <= level; i++) {
        n->forward[i] = NULL;
    }
    return n;
}

SkipList* create_skiplist() {
    SkipList *sl = (SkipList*)malloc(sizeof(SkipList));
    sl->level = 0;
    sl->header = create_node(-1, MAX_LEVEL);
    for(int i = 0; i <= MAX_LEVEL; i++) {
        sl->header->forward[i] = NULL;
    }
    srand(time(NULL));
    return sl;
}

int random_level() {
    int level = 0;
    while (((double)rand() / RAND_MAX) < P && level < MAX_LEVEL)
        level++;
    return level;
}

void insert_skiplist(SkipList *sl, int key) {
    Node *update[MAX_LEVEL + 1];
    Node *current = sl->header;
    for(int i = sl->level; i >= 0; i--) {
        while(current->forward[i] != NULL && current->forward[i]->key < key)
            current = current->forward[i];
        update[i] = current;
    }
    current = current->forward[0];
    if(current == NULL || current->key != key) {
        int rlevel = random_level();
        if(rlevel > sl->level) {
            for(int i = sl->level + 1; i <= rlevel; i++)
                update[i] = sl->header;
            sl->level = rlevel;
        }
        Node *n = create_node(key, rlevel);
        for(int i = 0; i <= rlevel; i++) {
            n->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = n;
        }
        printf("Inserted key %d with level %d\n", key, rlevel);
    }
}

Node* search_skiplist(SkipList *sl, int key) {
    Node *current = sl->header;
    for(int i = sl->level; i >= 0; i--) {
        while(current->forward[i] != NULL && current->forward[i]->key < key)
            current = current->forward[i];
    }
    current = current->forward[0];
    if(current != NULL && current->key == key) {
        printf("Found key %d\n", key);
        return current;
    }
    printf("Key %d not found\n", key);
    return NULL;
}

int delete_skiplist(SkipList *sl, int key) {
    Node *update[MAX_LEVEL + 1];
    Node *current = sl->header;
    for(int i = sl->level; i >= 0; i--) {
        while(current->forward[i] != NULL && current->forward[i]->key < key)
            current = current->forward[i];
        update[i] = current;
    }
    current = current->forward[0];
    if(current != NULL && current->key == key) {
        for(int i = 0; i <= sl->level; i++) {
            if(update[i]->forward[i] != current)
                break;
            update[i]->forward[i] = current->forward[i];
        }
        free(current->forward);
        free(current);
        while(sl->level > 0 && sl->header->forward[sl->level] == NULL)
            sl->level--;
        printf("Deleted key %d\n", key);
        return 1;
    }
    printf("Key %d not found for deletion\n", key);
    return 0;
}

void display_skiplist(SkipList *sl) {
    for(int i = 0; i <= sl->level; i++) {
        Node *node = sl->header->forward[i];
        printf("Level %d: ", i);
        while(node != NULL) {
            printf("%d ", node->key);
            node = node->forward[i];
        }
        printf("\n");
    }
}

int main() {
    SkipList *sl = create_skiplist();
    insert_skiplist(sl, 3);
    insert_skiplist(sl, 6);
    insert_skiplist(sl, 7);
    insert_skiplist(sl, 9);
    insert_skiplist(sl, 12);
    insert_skiplist(sl, 19);
    insert_skiplist(sl, 17);
    insert_skiplist(sl, 26);
    insert_skiplist(sl, 21);
    insert_skiplist(sl, 25);
    display_skiplist(sl);
    search_skiplist(sl, 19);
    search_skiplist(sl, 15);
    delete_skiplist(sl, 19);
    delete_skiplist(sl, 15);
    display_skiplist(sl);
    Node *current = sl->header->forward[0];
    while(current != NULL) {
        Node *next = current->forward[0];
        free(current->forward);
        free(current);
        current = next;
    }
    free(sl->header->forward);
    free(sl->header);
    free(sl);
    return 0;
}

