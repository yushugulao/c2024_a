#include <stdio.h>
#include <iostream>
#include <string>
using namespace std;
typedef struct {
    string *data;
    int top;
    int size;
} Stack;

string RPN;
char compute_sign[4]={'+','-','*','/'};

Stack *createStack(int size) ;
int isEmpty(Stack *stack);
int isFull(Stack *stack);
void push(Stack *stack, string data);
string pop(Stack *stack);
void display(Stack *stack);
int RPNloading(void);
int WhetherIN(char c,char *compute_sign);
int ComputeResult(int number1,int number2,char sign);

int main() {
    int size=RPNloading();
    Stack *stack=createStack(size);
    int i=0;
    while(i<size) {
        if(RPN[i]>='0' && RPN[i]<='9') {
            char charnumber[]={RPN[i]};
            int charnumberSize=1;
            int j=1;
            while(RPN[i]>='0' && RPN[i]<='9') {
                if (i>=size) {
                    cout<<"Invalid Input"<<endl;
                    return 0;
                }
                charnumber[j]=RPN[i];
                charnumberSize++;
                i++;
                j++;
            }
            int number=charnumber[0]-'0';
            if(charnumberSize!=1) {
                for(int j=0;j<charnumberSize-1;j++) {
                    number=number*10+(RPN[j]-'0');
                }
            }
            push(stack,);
        }
        else if(WhetherIN(RPN[i],compute_sign)!=-1) {
            if(int number1=pop(stack) != "-1") {
                while(pop(stack)) {
                    number1
                }
            }
            else {
                cout<<"Invalid Input"<<endl;
                return 0;
            }
            if(int number1=pop(stack)) {
                number1=number1;
            }
            else {
                cout<<"Invalid Input"<<endl;
                return 0;
            }
            push(stack,ComputeResult(number1,number2,RPN[i]));
        }
        i++;
    }
    return 0;
}


Stack *createStack(int size) {
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->top = -1;
    stack->size = size;
    stack->data = (string *)malloc(size * sizeof(int));
    return stack;
}

int isEmpty(Stack *stack) {
    return stack->top == -1;
}

int isFull(Stack *stack) {
    return stack->top == stack->size - 1;
}

void push(Stack *stack, string value) {
    if (isFull(stack)) {
        // cout << "Stack is full" << endl;
        return;
    }
    else {
        stack->data[++stack->top] = value;
    }
}

string pop(Stack *stack) {
    if (isEmpty(stack)) {
        // cout << "Stack is empty" << endl;
        return "-1";
    }
    else {
        return stack->data[stack->top--];
    }
}

int RPNloading(void) {
    cin>>RPN;
    return RPN.length();
}

int WhetherIN(char c,char *compute_sign) {
    int index=-1;
    for(int i=0;i<4;i++) {
        if(compute_sign[i] == c) {
            index=i;
        }
    }
    return index;
}

