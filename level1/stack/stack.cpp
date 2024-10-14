#include <stdio.h>
#include <iostream>
using namespace std;
typedef struct {
    int *data;
    int top;
    int size;
} Stack;

Stack *createStack(int size) ;
int isEmpty(Stack *stack);
int isFull(Stack *stack);
void push(Stack *stack, int data);
int pop(Stack *stack);
void display(Stack *stack);

int main() {
    Stack *stack=createStack(10);
    push(stack,100);
    push(stack,200);
    push(stack,300);
    cout<<pop(stack)<<endl;
    cout<<pop(stack)<<endl;
    cout<<pop(stack)<<endl;
    cout<<pop(stack)<<endl;
    cout<<isEmpty(stack)<<endl;
}


Stack *createStack(int size) {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->top = -1;
    stack->size = size;
    stack->data = (int *)malloc(size * sizeof(int));
    return stack;
}

int isEmpty(Stack *stack) {
    return stack->top == -1;
}

int isFull(Stack *stack) {
    return stack->top == stack->size - 1;
}

void push(Stack *stack, int value) {
    if (isFull(stack)) {
        cout << "Stack is full" << endl;
        return;
    }
    else {
        stack->data[++stack->top] = value;
    }
}

int pop(Stack *stack) {
    if (isEmpty(stack)) {
        cout << "Stack is empty" << endl;
        return -1;
    }
    else {
        return stack->data[stack->top--];
    }
}
