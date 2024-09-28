
#include <iostream>
#include <chrono>
using namespace std;

bool IsPrime(int num) {
    if (num <= 1) return false;
    if (num == 2) return true;
    if (num % 2 == 0) return false;
    for (int i = 3; i * i <= num; i += 2) { // 只检查奇数
        if (num % i == 0) return false;
    }
    return true;
}

int main() {
    auto start = chrono::high_resolution_clock::now();
    int base=2;
    while(base<1000) {
        if (IsPrime(base)) {
            cout << base << endl;
        }
        base++;
    }
    auto finish = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(finish - start);
    cout << duration.count() << endl;
    return 0;
}