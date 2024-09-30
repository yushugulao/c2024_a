#include <iostream>
#include <vector>
#include <numeric>
using namespace std;
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
int main() {
    vector<int> two_char;
    two_char = Char_to_Two('7');
    for (int i = 0; i < two_char.size(); i++) {
        cout << two_char[i] << endl;
    }
    return 0;
}