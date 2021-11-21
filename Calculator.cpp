#include <iostream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

struct Num { // 大数
	vector<int> n; // 用数组存储
	void cz(Num& x) { // control zero 控制 0 的数量
		while (x.n.back() == 0 && x.n.size() > 1)
			x.n.pop_back();
	}

	// 构造函数
	Num(string num) {
		for (size_t i = num.size(); i > 0; --i)
			n.push_back(num[i - 1] - '0');
		cz(*this);
	}
	Num(int num) {
		if (!num)
			n.push_back(0);
		while (num) {
			n.push_back(num % 10);
			num /= 10;
		}
	}

	int& operator[](size_t n) { return this->n[n]; } // 重载数组元素的调用
	bool operator<=(Num b) { // 重载 <=
		bool flag = true;
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
	Num operator+(Num b) { // 重载 +
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
		cz(res);
		return res;
	}
	Num operator-(Num b) { // 重载 -
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
		cz(res);
		return res;
	}
	Num operator*(Num b) { // 重载 *
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
		cz(res);
		return res;
	}
	Num operator/(Num b) {
		Num res = 0;
		return res;
	}
	Num operator^(Num n) { // 重载 ^（用异或表示阶乘）
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

ostream& operator<<(ostream& out, Num x) { // 重载输出
	for (size_t i = x.n.size(); i > 0; --i)
		cout << x[i - 1];
	return out;
}

string getNum() { // 获取一段数字的字符串
	string n;
	while (isdigit(expre[p]))
		n += expre[p++];
	return n;
}

void cal() { // 进行一次计算
	Num b = n.top();
	n.pop();
	Num a = n.top();
	n.pop();
	char o = op.top();
	op.pop();
	switch (o) {
	case '+':
		n.push(a + b);
		break;
	case '-':
		n.push(a - b);
		break;
	case '*':
		n.push(a * b);
		break;
	case '/':
		n.push(a * b);
		break;
	case '^':
		n.push(a ^ b);
		break;
	default:
		break;
	}
}

int opa() { // open parenthesis appears，判断是否出现左括号
	int sum = 0;
	while (expre[p] == '(')
		++p, ++sum;
	return sum;
}

void hcp() { // handles close parenthesis，判断是否出现右括号并处理
	while (expre[p] == ')') {
		while (op.top() != '(') {
			cal();
		}
		op.pop();
		++p;
	}
}

int main() {
	// 定义运算符优先级
	pri['+'] = pri['-'] = 1;
	pri['*'] = pri['/'] = 2;
	pri['^'] = 3;

	while (cin >> expre) {
		p = 0;
		int sum = opa();
		while (sum--) // 读取数字之前判断是否出现左括号
			op.push('(');
		n.push(getNum());
		hcp(); // 读取数字之后判断是否出现右括号
		while (p < expre.size()) {
			char o = expre[p++];
			sum = opa(); // 左括号是否出现的标志量，读取数字之前判断是否出现左括号
			Num x = getNum();
			while (!op.empty() && pri[o] <= pri[op.top()]) // o 优先级不大于栈顶操作符优先级就计算
				cal();
			n.push(x);
			op.push(o);
			while (sum--) // 如果出现了左括号就压入，因为前括号是在操作符后出现的，所以在这里压入栈
				op.push('(');
			hcp(); // 读取数字之后判断是否出现右括号
		}
		while (!op.empty()) // 把栈计算干净
			cal();
		cout << n.top() << endl;
		n.pop(); // 垃圾，不要了（bushi
	}
	return 0;
}
// 45489*531-55-(6+8)+((5*3)^(21-15)+83)*8978954421546
// 102276647961163549158