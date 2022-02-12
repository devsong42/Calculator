#include <iostream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

struct Num { // 大数
	vector<int> n; // 用数组存储
	bool sign = 1; // 符号，1 正，0 负
	size_t sum = 0; // 小数位数
	void trim() { // 修剪 0 的数量
		while (n[0] == 0 && sum > 0) { // 去除小数末尾无效的 0
			n.erase(n.begin());
			sum--;
		}

		while (n.size() > sum + 1 && n.back() == 0) // 去除前导 0
			n.pop_back();

		while (sum >= n.size()) // 补齐 0
			n.push_back(0);
	}

	// 构造函数
	Num(string num) {
		if (num[0] == '+' || num[0] == '-') {
			if (num[0] == '-')
				sign = !sign;
			num.erase(num.begin());
		}
		for (size_t i = num.size(); i > 0; --i) {
			if (num[i - 1] == '.') {
				sum = n.size();
				continue;
			}
			n.push_back(num[i - 1] - '0');
		}
		trim();
	}
	Num(int num) {
		if (num < 0)
			sign = 0, num = -num;
		if (!num)
			n.push_back(0);
		while (num) {
			n.push_back(num % 10);
			num /= 10;
		}
	}

	//string ToString() { // 字面
	//	string str = "";
	//	if (!sign)
	//		str += '-';
	//	for (size_t i = n.size(); i > 0; --i) {
	//		if (i == sum)
	//			str += '.';
	//		str += n[i - 1] + '0';
	//	}
	//	return str;
	//}

	int& operator[](size_t n) { return this->n[n]; } // 重载数组元素的调用
	bool operator>=(Num b) { // 重载 >=
		bool flag = true;
		Num a = *this;
		if (!(a.sign ^ b.sign)) { // 如果符号相同
			if (!a.sign)
				flag = false;
			if (a.n.size() != b.n.size())
				flag = a.n.size() > b.n.size();
			else
				for (size_t i = a.n.size(); i > 0; i--)
					if (a[i - 1] == b[i - 1])
						continue;
					else {
						bool tmp = a[i - 1] > b[i - 1];
						flag = flag ? tmp : !tmp;
						break;
					}
		}
		else
			flag = a.sign ? true : false;
		return flag;
	}
	bool operator<(Num b) { // 重载 >=
		return !(*this >= b);
	}
	Num operator+(Num b) { // 重载 +
		Num res = 0, a = *this;
		if (!(a.sign ^ b.sign)) { // 如果符号相同
			if (!a.sign)
				res.sign = 0/*, a.sign = 1, b.sign = 1*/; // 负加负=负的正加正 (-a)+(-b)=-a-b=-(a+b)

		// 对齐数位
			while (a.sum > b.sum) {
				b.n.insert(b.n.begin(), 0);
				b.sum++;
			}
			while (b.sum > a.sum) {
				a.n.insert(a.n.begin(), 0);
				a.sum++;
			}
			res.sum = a.sum;

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
			res.trim();
		}
		else {
			if (a.sign)
				b.sign = 1, res = a - b; // 正加负=正减正
			else
				a.sign = 1, res = b - a; // 负（a）加正（b）=正（b）减正（-a）
		}
		return res;
	}
	Num operator-(Num b) { // 重载 -
		Num res = 0, a = *this;
		if (!(a.sign ^ b.sign)) { // 如果符号相同
			if (!a.sign)
				b.sign = 1, res = a + b; // 负减负=负加正
			else {
				if (a < b) { // 小减大=负的大减小
					res.sign = 0;
					Num t = a;
					a = b;
					b = t;
				}

				// 对齐数位
				while (a.sum > b.sum) {
					b.n.insert(b.n.begin(), 0);
					b.sum++;
				}
				while (b.sum > a.sum) {
					a.n.insert(a.n.begin(), 0);
					a.sum++;
				}
				res.sum = a.sum;

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
				res.trim();
			}
		}
		else {
			if (a.sign)
				b.sign = 1, res = a + b; // 正减负=正加正
			else
				b.sign = 0, res = a + b; // 负减正=负加负
		}
		return res;
	}
	Num operator*(Num b) { // 重载 *
		Num res = 0, a = *this;
		res.sum = a.sum + b.sum;
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
		res.trim();
		if (a.sign ^ b.sign) // 如果符号不同
			res.sign = 0;
		return res;
	}
	Num operator/(Num b) {
		Num res = 0;
		return res;
	}
	Num operator^(Num n) { // 重载 ^（用异或表示阶乘）
		Num res = *this;
		for (Num i = 1; i < n; i = i + 1)
			res = res * *this;
		return res;
	}

	void print() { // 输出自身
		if (!sign)
			putchar('-');
		for (size_t i = n.size(); i > 0; --i) {
			if (i == sum)
				putchar('.');
			putchar(n[i - 1] + '0');
		}
	}

	void println() { // 输出并换行
		print();
		cout << endl;
	}
};

string expre;
size_t p;
stack<char> op;
stack<Num> n;
int pri[300];

string getNum() { // 获取一段数字的字符串
	string n = "+";
	while (expre[p] == '+' || expre[p] == '-') {
		if (expre[p] == '-')
			n[0] = n[0] == '+' ? '-' : '+';
		++p;
	}
	while (isdigit(expre[p]) || expre[p] == '.')
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
		n.top().println();
		n.pop(); // 垃圾，不要了（bushi
	}
	return 0;
}
// 45484.9*-531.3-55-(6+8)+((5*3)^(21-15)+83)*(897841523.45-95465442145.46646)
// -1077191924970174063.42368