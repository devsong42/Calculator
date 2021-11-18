#include <iostream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

struct Num {
    vector<int> n;
    void clz(Num &x) {
        while (x.n.back() == 0 && x.n.size() > 1)
            x.n.pop_back();
    }
    Num(string num) {
        for (size_t i = num.size(); i > 0; --i)
            n.push_back(num[i - 1] - '0');
        clz(*this);
    }
    Num(int num) {
        if (!num)
            n.push_back(0);
        while (num) {
            n.push_back(num % 10);
            num /= 10;
        }
    }
    int &operator[](size_t n) { return this->n[n]; }
    bool operator<=(Num b) {
        bool flag = 1;
        Num a = *this;
        if (a.n.size() != b.n.size())
            flag = a.n.size() < b.n.size();
        else
            for (size_t i = a.n.size(); i > 0; i--)
                if (a[i - 1] == b[i - 1])
                    continue;
                else {
                    flag = a[i - 1] < b[i - 1];
                    break;
                }
        return flag;
    }
    Num operator+(Num b) {
        Num res = 0, a = *this;
        size_t l = max(a.n.size(), b.n.size());
        while (a.n.size() < l)
            a.n.push_back(0);
        while (b.n.size() < l)
            b.n.push_back(0);
        for (size_t i = 0; i < l; i++) {
            res[i] += a[i] + b[i];
            res.n.push_back(res[i] / 10);
            res[i] %= 10;
        }
        clz(res);
        return res;
    }
    Num operator-(Num b) {
        Num res = 0, a = *this;
        size_t l = max(a.n.size(), b.n.size());
        while (a.n.size() < l)
            a.n.push_back(0);
        while (b.n.size() < l)
            b.n.push_back(0);
        for (size_t i = 0; i < l; i++) {
            res[i] += a[i] - b[i];
            res.n.push_back(0);
            if (res[i] < 0) {
                res[i] += 10, --res[i + 1];
            }
        }
        clz(res);
        return res;
    }
    Num operator*(Num b) {
        Num res = 0, a = *this;
        size_t l = a.n.size() + b.n.size();
        while (res.n.size() < l)
            res.n.push_back(0);
        for (size_t i = 0; i < a.n.size(); i++)
            for (size_t j = 0; j < b.n.size(); j++) {
                res[i + j] += a[i] * b[j];
                int s = res[i + j] / 10;
                res[i + j + 1] += s;
                res[i + j] %= 10;
            }
        clz(res);
        return res;
    }
    Num operator/(Num b) {
        Num res = 0;
        return res;
    }
    Num operator^(Num n) {
        Num res = *this;
        for (Num i = 2; i <= n; i = i + 1)
            res = res * *this;
        return res;
    }
};

string expre;
size_t p;
stack<char> op;
stack<Num> n;
int pri[300];

ostream &operator<<(ostream &out, Num x) {
    for (size_t i = x.n.size(); i > 0; --i)
        cout << x[i - 1];
    return out;
}

string getNum() {
    string n;
    while (isdigit(expre[p]))
        n += expre[p++];
    return n;
}

void cal() {
    char o;
    Num a = n.top();
    n.pop();
    Num b = n.top();
    n.pop();
    o = op.top();
    op.pop();
    switch (o) {
    case '+':
        n.push(b + a);
        break;
    case '-':
        n.push(b - a);
        break;
    case '*':
        n.push(b * a);
        break;
    case '/':
        n.push(b * a);
        break;
    case '^':
        n.push(b ^ a);
        break;
    default:
        break;
    }
}

int main() {
    pri['+'] = pri['-'] = 1;
    pri['*'] = pri['/'] = 2;
    pri['^'] = 3;
    while (1) {
        cin >> expre;
        p = 0;
        n.push(getNum());
        while (p < expre.size()) {
            char o = expre[p++];
            Num x = getNum();
            while (!op.empty() && pri[o] <= pri[op.top()])
                cal();
            n.push(x);
            op.push(o);
        }
        while (!op.empty())
            cal();
        cout << n.top() << endl;
        n.pop();
    }
    return 0;
}
// 45489*531-556+851*3^25+583*8978954421546
// 5955773058551414