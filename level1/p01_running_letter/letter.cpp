///created by whf
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<iostream>
using namespace std;
#define interval 100 //c语言中早期出现 ，不占内存
//int static interval=100 // int换成const代表常量 /static规定了其他文件中无法使用此全局变量 /占内存，但编译器会做出优化

int main() {
    int count=0;
    while(count<100) {
        count++;
        int i=0;
        int j=10;
        while (i<=10)
        {
            system("cls");
            for(int k=0;k<=i;k++){
                cout<<" ";
            }
            cout<<"hello world";
            Sleep(interval);
            i++;
        }
        while(j>=0)
        {
            system("cls");
            for(int k=0;k<=j;k++) {
                cout<<" ";
            }
            cout<<"hello world";
            Sleep(interval);
            j--;
        }
    }
    return 0;
}