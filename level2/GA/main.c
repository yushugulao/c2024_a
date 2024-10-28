#include <stdio.h>
int main() {
    char x,m,n;
    scanf("%c %c %c",&x,&m,&n);
    printf("%c %c %c\n",x,m,n);
    if(x==m-1 && m==n-1) {
        printf("yes");
    } else {
        printf("no");
    }
    return 0;
}