#include <iostream>
#include <string>
#include <ctime>
#include <cstdio>
#include <chrono>
#include <memory>
#include <vector>
#include <windows.h>
#include <psapi.h>
enum PA{P1=13591409,P2=108,P3=72,P4=46,P5=5,P6=10939058860032000,P7=3,P8=545140134,P9=426880};

using namespace std;
struct Wint:vector<int>
{
    Wint(int n=0)//默认初始化为0，但0的保存形式为空
    {
        push_back(n);
        check();
    }
    Wint& check()
    {
        while(!empty()&&!back())pop_back();//去除最高位可能存在的0
        if(empty())return *this;
        for(int i=1; i<size(); ++i)
        {
            (*this)[i]+=(*this)[i-1]/10;
            (*this)[i-1]%=10;
        }
        while(back()>=10)
        {
            push_back(back()/10);
            (*this)[size()-2]%=10;
        }
        return *this;//为使用方便，将进位后的自身返回引用
    }
};
//输入输出
istream& operator>>(istream &is,Wint &n)
{
    string s;
    is>>s;
    n.clear();
    for(int i=s.size()-1; i>=0; --i)n.push_back(s[i]-'0');
    return is;
}
ostream& operator<<(ostream &os,const Wint &n)
{
    if(n.empty())os<<0;
    for(int i=n.size()-1; i>=0; --i)os<<n[i];
    return os;
}
//比较，只需要写两个，其他的直接代入即可
//常量引用当参数，避免拷贝更高效
bool operator!=(const Wint &a,const Wint &b)
{
    if(a.size()!=b.size())return 1;
    for(int i=a.size()-1; i>=0; --i)
        if(a[i]!=b[i])return 1;
    return 0;
}
bool operator==(const Wint &a,const Wint &b)
{
    return !(a!=b);
}
bool operator<(const Wint &a,const Wint &b)
{
    if(a.size()!=b.size())return a.size()<b.size();
    for(int i=a.size()-1; i>=0; --i)
        if(a[i]!=b[i])return a[i]<b[i];
    return 0;
}
bool operator>(const Wint &a,const Wint &b)
{
    return b<a;
}
bool operator<=(const Wint &a,const Wint &b)
{
    return !(a>b);
}
bool operator>=(const Wint &a,const Wint &b)
{
    return !(a<b);
}

Wint& operator+=(Wint &a,const Wint &b)
{
    if(a.size()<b.size()) {
        a.resize(b.size());
    }
    for(int i=0; i!=b.size(); ++i)a[i]+=b[i];
    return a.check();
}
Wint operator+(Wint a,const Wint &b)
{
    return a+=b;
}

Wint& operator-=(Wint &a,Wint b)
{
    if(a<b)swap(a,b);
    for(int i=0; i!=b.size(); a[i]-=b[i],++i)
        if(a[i]<b[i])//需要借位
        {
            int j=i+1;
            while(!a[j])++j;
            while(j>i)
            {
                --a[j];
                a[--j]+=10;
            }
        }
    return a.check();
}
Wint operator-(Wint a,const Wint &b)
{
    return a-=b;
}

Wint operator*(const Wint &a,const Wint &b)
{
    Wint n;
    n.assign(a.size()+b.size()-1,0);
    for(int i=0; i!=a.size(); ++i)
        for(int j=0; j!=b.size(); ++j)
            n[i+j]+=a[i]*b[j];
    return n.check();
}
Wint& operator*=(Wint &a,const Wint &b)
{
    return a=a*b;
}
//除法和取模先实现一个带余除法函数
Wint divmod(Wint &a,const Wint &b)
{
    Wint ans;
    for(int t=a.size()-b.size(); a>=b; --t)
    {
        Wint d;
        d.assign(t+1,0);
        d.back()=1;
        Wint c=b*d;
        while(a>=c)
        {
            a-=c;
            ans+=d;
        }
    }
    return ans;
}
Wint operator/(Wint a,const Wint &b)
{
    return divmod(a,b);
}
Wint& operator/=(Wint &a,const Wint &b)
{
    return a=a/b;
}
Wint& operator%=(Wint &a,const Wint &b)
{
    divmod(a,b);
    return a;
}
Wint operator%(Wint a,const Wint &b)
{
    return a%=b;
}
Wint pow(const Wint &n,const Wint &k)
{
    if(k.empty())return 1;
    if(k==2)return n*n;
    if(k.front()%2)return n*pow(n,k-1);
    return pow(pow(n,k/2),2);
}

Wint Sqrt10005(Wint n) {
    Wint n1=0;
    Wint c=100024;
    c*=100000;
    c+=99687;
    Wint mc=8; Wint m=mc;
    Wint f1=1;
    for (int i = 1; i <= mc; i++) {
        f1*=10;
    }
    Wint f2=f1*f1;
    Wint a=f2*10005-c*c;
    Wint b,d;
    while (mc<n){
        a*=f2;
        b=c*2*f1;
        d=a/b;
        c=c*f1+d;
        a-=d*(b+d);
        mc+=m;
        if (mc*2>n) {
            m=n-mc;
        }
        else {
            m=mc;
        }
        for (int i = 1; i <= m; i++) {
            f1*=10;
        }
        f2=f1*f1;
        n1+=1;
    }
    return c;
}

Wint abs(Wint A) {
    if(A<0) {
        return 0-A;
    }
    return A;
}

int main(){
    //获取开始时间
    auto start=chrono::system_clock::now();
    time_t start_time = chrono::system_clock::to_time_t(start);
    //初始化最大内存
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    auto MaxMemoryUsage=pmc.WorkingSetSize/1024;
    cout<<"please enter the accuracy you want.";
    Wint N;
    cin>>N;
    Wint times=1;Wint base=pow(10,N);Wint c1,c2,c3,c4;
    Wint A=P1*base; Wint B=A;
    c3=P1;
    while (abs(A)>P5) {
        c1=((P2-P3*times)*times-P4)*times+P5;
        c2=P6*pow(times,3);
        c4=c3; c3+=P8;
        times+=1;
        A=A*c1*c3/(c2*c4);
        B+=A;
        //GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
        //auto nowUse=pmc.WorkingSetSize/1024;
        //if ((int)nowUse>(int)MaxMemoryUsage) {
            //MaxMemoryUsage=nowUse;
        //}
    }
    Wint p=P9*base*Sqrt10005(N)/B/100;
    //获取结束时间
    auto end=std::chrono::system_clock::now();
    time_t end_time = std::chrono::system_clock::to_time_t(end);

    chrono::duration<double> diff=end-start;
    cout<< "all the time needed is:" << diff.count()<<"seconds"<<endl;
    cout<< "the max memoryUsage is:" << to_string(MaxMemoryUsage)<<"KB"<<endl;
    cout<<p<<endl;
    return 0;
}