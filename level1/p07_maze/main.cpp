#include<iostream>
#include<windows.h>
using namespace std;
int main() {
    int m = 5;
    int count = 0;
    int rev = 1;
    while (count<=m)
    {
        cout << 'b' << endl;
        Sleep(500);
        system("cls");
        int step = 0;
        while (step <= count)
        {
            cout << ' ';
            step++;
        }
        count += rev;
        if (count == m or rev==-1 and count==-1)
        {
            rev *= -1;
            //cout << count << endl;
        }
    }
}