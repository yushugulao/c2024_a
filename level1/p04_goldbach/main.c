#include <stdio.h>
#include <stdbool.h>

// 判断一个数是否是素数
bool isPrime(int num) {
    if (num <= 1) return false;
    for (int i=2; i*i<= num; i++) {
        if (num%i==0) return false;
    }
    return true;
}

int main() {
    int num;
    bool found;
    bool yes[48];
    for (int i=0; i<48; i++) {yes[i]=false;}
    for (num = 4; num <= 100; num += 2) {
        found = false;
        for (int i = 2; i <= num / 2; i++) {
            if (isPrime(i) && isPrime(num - i)) {
                printf("%d = %d + %d\n", num, i, num - i);
                found = true;
                yes[(num-4)/2]=true;
                break;
            }
        }
        if (found==false) {
            printf("not found of%d \n", num);
        }
    }
    int scan=0;bool gdbh_is_ok=true;
    while (scan<48) {
        if(yes[scan]==false) {
            gdbh_is_ok=false;
            break;
        }
        scan++;
    }
    if (gdbh_is_ok==true) {printf("gdbh is right\n");}
    else {printf("gdbh is not right\n");}
    return 0;
}
