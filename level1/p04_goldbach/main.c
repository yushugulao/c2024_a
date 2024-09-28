#include<stdio.h>

int main() {
    int flagofgdbh=1;
    int prime[3];
    short int p=0;
    for(int i=3;i<=100;i++) {
        int canbe=0;
        int j=1;
        int begin=1;
        while(canbe==0) {
            j=begin;
            int assess=2;
            short int flag=1;
            while(j<i) {
                while(assess*assess<=j&&flag==1) {
                    int a;
                    a=j%assess;
                    if(a==0) {
                        flag=0;
                    }
                    assess=assess+1;
                }
                if(flag==1) {
                    prime[p]=j;p++;
                }
                if(p==2) {
                    if(prime[0]+prime[1]+prime[2]==i) {
                        canbe=1;
                        break;
                    }
                    else{
                        p-=1;
                    }
                }
                j++;
            }
        }
    }
    return 0;
}