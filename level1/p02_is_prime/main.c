#include <stdio.h>
int main() {
    long num;
    scanf("%i",&num);
    int assess=2;
    short int flag=1;
    while(assess*assess<=num&&flag==1) {
        int a;
        a=num%assess;
        if(a==0) {
            flag=0;
        }
        assess=assess+1;
    }
    if (flag==1) {
        printf("YES\n");
    }
    else {
        printf("NO\n");
    }
    return 0;
}