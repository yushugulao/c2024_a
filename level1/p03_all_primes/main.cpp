///created by whf
#include <iostream>
#include <chrono>
using namespace std;

void IsPrime(bool primes[], int max) {
    int primeslist[max];
    int count = 0;
    for (int i=2; i <= max; i++) {
        if (!primes[i]) {
            primeslist[count++] = i;
        }
        for (int j=0; j<count && i*primeslist[j]<=max;j++) {
            primes[i * primeslist[j]] = true;
            if (!i%primeslist[j]) break;
        }
    }
}

int main() {
    auto start = chrono::high_resolution_clock::now();
    bool primes[1024]={false};
    IsPrime(primes,1000);
    int base=2;
    while(base<1000) {
        if (!primes[base]) {
            cout << base << endl;
        }
        base++;
    }
    auto finish = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(finish - start);
    cout << duration.count() << endl;
    return 0;
}
