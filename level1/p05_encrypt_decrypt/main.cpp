///backpack 背包加密
///输入限制：必须是几个不带空格的数字，字母或可用ascll码表示的特殊字符
///明文为长度为四的倍数的一串数字
#include <iostream>
#include <vector>
#include <numeric>
#include <string>
#include <stdlib.h>
using namespace std;

// 用于生成质数
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

// 求逆元
int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    return -1;
}

// 超递增序列(也就是私钥)生成
vector<int> generateSuperIncreasingSequence(int n) {
    vector<int> sequence(n);
    sequence[0]=rand() % 10 + 1;  // 生成第一个元素
    for (int i=1; i<n; i++) {
        sequence[i] = accumulate(sequence.begin(), sequence.end(), 0) + (rand() % 10 + 1);
    }
    return sequence;
}

// 把二进制下的ascll码转为要输出的字符
char Two_to_Char(const vector<int>&decryptedMessage) {
    int sum=0;
    for (int i = 0; i < decryptedMessage.size(); i++) {
        sum=sum*2+decryptedMessage[i];
    }
    char str=static_cast<char>(sum);
    return str;
}

//把输入的字符转为可操作的比特串
vector<int> Char_to_Two(char str) {
    int ascii=static_cast<int>(str);
    vector<int> sequence;
    while(ascii>0) {
        sequence.push_back(0);
        for (int i = sequence.size()-1; i>=1 ; i--) {
            sequence[i] = sequence[i-1];
        }
        sequence[0]=ascii%2;
        ascii=ascii/2;
    }
    while (sequence.size()<8){
        sequence.push_back(0);
        for (int i = sequence.size()-1; i>=1 ; i--) {
            sequence[i] = sequence[i-1];
        }
        sequence[0]=0;
    }
    return sequence;
}
// 公钥生成
vector<int> generatePublicKey(const vector<int>& privateKey, int m, int w) {
    vector<int> publicKey;
    for (int i = 0; i < privateKey.size(); i++) {
        publicKey.push_back((privateKey[i] * w) % m);
    }
    return publicKey;
}

// 加密过程
int encrypt(const vector<int>& publicKey, const vector<int>& message) {
    int cipher = 0;
    for (int i = 0; i < publicKey.size(); i++) {
        cipher += publicKey[i] * message[i];
    }
    return cipher;
}

// 解密过程
vector<int> decrypt(int cipher, const vector<int>& privateKey, int m, int w) {
    int wInverse = modInverse(w, m);
    int cPrime = (cipher * wInverse) % m;

    vector<int> message(privateKey.size());
    for (int i = privateKey.size() - 1; i >= 0; i--) {
        if (cPrime >= privateKey[i]) {
            message[i] = 1;
            cPrime -= privateKey[i];
        } else {
            message[i] = 0;
        }
    }
    return message;
}

//输出字符串的长度
/*int Length(char in) {
    string str = to_string(in);
    int len = str.length();
    return len;
}*/

int main() {
    // 超递增序列大小
    int n=8;

    // 生成私钥（超递增序列）
    vector<int> privateKey = generateSuperIncreasingSequence(n);

    // 模数和乘数
    int m=accumulate(privateKey.begin(), privateKey.end(), 0) + 10;  // 模数m要大于私钥的和
    int w;  // 随机选择一个与m互质的数
    bool wwhetherisprime=false;
    int mPrime;
    bool primes[1024]={false};
    IsPrime(primes,1000);
    while (!wwhetherisprime) {
        mPrime=rand()%1000;
        if(primes[mPrime]){
            wwhetherisprime=true;
            w=mPrime;
        }
    }


    // 生成公钥
    vector<int> publicKey = generatePublicKey(privateKey, m, w);

    // 输入要加密的消息然后转成比特串组
    string input;
    printf("Please enter the message to encrypt:\n");
    cin>>input;
    char inputs[input.length()];
    int Messagelen=input.length();
    for (int i = 0; i < Messagelen; i++) inputs[i] = input[i];
    vector<vector<int>> messageGroup;
    // 加密
    string Encrypts="";
    for (int i = 0; i < Messagelen; i++) {
        vector<int> message = Char_to_Two(inputs[i]);
        int cipher = encrypt(publicKey, message);
        string strcipher=to_string(cipher);
        if (strcipher.length()%4) {
            strcipher='0'+strcipher;
        }
        Encrypts+=strcipher;
    }
    cout << "The encryptedMessage is:" << Encrypts << endl;

    // 解密
    int EncryptsLen=Messagelen*4;//模拟读取了明文的长度，明文的长度一定是密文的四倍
    string output="";
    for (int i = 0; i < EncryptsLen; i+=4) {
        char strcipher[]="";
        for (int j = 0; j < 4; j++) {
            strcipher[j]=Encrypts[i+j];
        };
        int cipher=atoi(strcipher);
        vector<int> decryptedMessage = decrypt(cipher, privateKey, m, w);
        output+=Two_to_Char(decryptedMessage);
    }
    printf("The decryptedMessage is:\n");
    for (int i = 0; i < EncryptsLen/4; i++) {
        printf("%c",output[i]);
    }
    return 0;
}
