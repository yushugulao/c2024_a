//
// Created by HONOR on 24-9-30.
//

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;
int static head=0;
int static tail=0;
vector<int> static queue;
void append(int NewElement) {
    queue.push_back(NewElement);
    tail+=1;
}
int pop() {
    int output = queue[head];
    if(head==tail) {
        printf("Queue is empty\n");
    }
    else {
        head+=1;
    }
    return output;
}\
int capacity(){
    return queue.size();
}
bool is_empty() {
    if(head==tail) {
        return true;
    }
    else {
        return false;
    }
}

int main() {

}