///created by whf
///

///#include <stdio.h>
///#include <stdlib.h>
///#include <conio.h>
///#include <windows.h>
///#include <conio.h>
///#include <iostream>
///using namespace std;
///int main() {
   /// char c=10;
    ///short s=100;
    ///short x=s+(short)c;
    ///cout<<x<<endl;
    ///return 0;
///}

#include<stdio.h>
int static stepcount=0;
void hanio(int n, char A, char B, char C) {
    if (1 == n) //防止少写=
    {
        printf("%c--->%c\n", A, C);
        stepcount++;
    }
    else
    {
        hanio(n-1, A, C, B);
        printf("%c--->%c\n", A, C);
        stepcount++;
        hanio(n-1, B, A, C);
    }
}
int main()
{
    int towerlevel = 0;
    printf("Enter the number of towers:\n");
    scanf("%d", &towerlevel);
    hanio(towerlevel, 'A', 'B', 'C');
    printf("all steps needed are  %d\n",stepcount);
    return 0;
}
















