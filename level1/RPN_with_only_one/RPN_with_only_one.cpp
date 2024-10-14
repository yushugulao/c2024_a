#include <iostream>
#include <stack>
#include <string>
#include <locale>

using namespace std;

bool isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/');
}

float performOperation(float operand1, float operand2, char op) {
    switch (op) {
        case '+': return operand1 + operand2;
        case '-': return operand1 - operand2;
        case '*': return operand1 * operand2;
        case '/': return operand1 / operand2;
        default: return 0;
    }
}

// 处理逆波兰表达式
float evaluateRPN(const string& expression) {
    stack<float> st;

    for (char ch : expression) {
        if (isdigit(ch)) {
            st.push(static_cast<float>(ch - '0'));
        } else if (isOperator(ch)) {
            if (st.size() < 2) {
                cout << "逆波兰表达式不合法！运算符 " << ch << " 没有足够的操作数。" << endl;
                return -1;
            }
            float operand2 = st.top(); st.pop();
            float operand1 = st.top(); st.pop();
            float result = performOperation(operand1, operand2, ch);
            st.push(result);
        } else {
            cout << "无效字符：" << ch << endl;
            return -1;
        }
    }

    if (st.size() == 1) {
        return st.top();
    } else {
        cout << "逆波兰表达式不合法！操作数或运算符不匹配。" << endl;
        return -1;
    }
}

int main() {
    system("chcp 65001");
    setlocale(LC_ALL, "zh_CN.UTF-8");
    string rpnExpression;
    cout << "请输入逆波兰表达式（不带空格，数字为1位整数）：";
    cin >> rpnExpression;
    float result = evaluateRPN(rpnExpression);
    if (result != -1) {
        cout << "逆波兰表达式的计算结果是： " << result << endl;
    }
    return 0;
}
