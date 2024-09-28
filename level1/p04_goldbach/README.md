### 功能要求：

1. 在100范围内验证哥德巴赫猜想的正确性


#include <stdio.h>
#include <stdbool.h>

// 判断一个数是否是素数
bool isPrime(int num) {
if (num <= 1) return false;
for (int i = 2; i * i <= num; i++) {
if (num % i == 0) return false;
}
return true;
}

int main() {
int num;
bool found;
    for (num = 1; num <= 100; num++) {
        found = false;
        for (int i = 2; i <= num / 2; i++) {
            if (isPrime(i) && isPrime(num - i)) {
                printf("%d = %d + %d\n", num, i, num - i);
                found = true;
                break; 
            }
        }
        if (!found) {
            printf("%d 不能表示为两个素数的和\n", num);
        }
    }

    return 0;
}
