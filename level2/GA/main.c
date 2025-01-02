/*#include <stdio.h>
#include <string.h>


int p=0;

int add();
int countString(char str[]);


int main() {
    char input[100]={'\0'};
    gets(input);
    int p=countString(input);
    int s;
    for(int i=0;i<p;i++) {
        s=add();
    }
    printf("%d",s);
    getchar();
    return 0;
}

int add(void) {
    p=p+2;
    return p;
}

int countString(char str[]) {
    int p=strlen(str);
    return p;
}  */

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>

/*int main() {
    int *p;
    p = (int *)malloc(10 * sizeof(int));
    int a[3]={11,22,33};
    scanf("%d",a+1);
    printf("%d",*p);
    printf("%d\n",a[0]);
    printf("%d\n",a[1]);
    system("pause");
    return 0;
}*/

/*
int main(){
int k[3]={2,3,4}; int*p; p=&k; printf("%d",*(++p));
    system("pause");
    return 0;
}
*/


/*
int main() {
    char a[]="b19";
    char b[]="c08";
    printf("%x", a>b);
    system("pause");
    return 0;
}
*/

/*int main() {
    short large_num = 0b001100;
    printf("%d\n", large_num);
    system("pause");
    return 0;
}*/

/*int main() {
    if(1) {
        int x=0;
        x++;
    }
    for (int x=0; x<10; x++) ;
    int k=1;
    switch(1) {
        default:
            break;
        case 1:
            k=0;
            break;
    }
    printf("%d",k);
    system("pause");
    return 0;
}*/

int main() {
    char s[]="baodian",*ps=s;
    printf("%c\n",*ps+2);
    system("pause");
    return 0;
}


