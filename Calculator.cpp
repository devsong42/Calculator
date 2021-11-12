#include <iostream>
#include <vector>
#include <string>
#include <stack>

using namespace std;

struct Num {
	vector<int> n;
	void clz(Num& x) {
		while (x.n.back() == 0 && x.n.size() > 1)
			x.n.pop_back();
	}
	Num(string num) {
		for (int i = num.size() - 1; i >= 0; --i)
			n.push_back(num[i] - '0');
		clz(*this);
	}
	int operator[] (int n) {
		return this->n[n];
	}
	Num operator+ (Num b) {
		Num res("0");
		int l = max(n.size(), b.n.size());
		while (n.size() < l)
			n.push_back(0);
		while (b.n.size() < l)
			b.n.push_back(0);
		for (int i = 0; i < l; i++) {
			res.n[i] += n[i] + b[i];
			res.n.push_back(res[i] / 10);
			res.n[i] %= 10;
		}
		clz(res);
		return res;
	}
	Num operator* (Num b) {
		Num res("0");
		int l = n.size() + b.n.size();
		while (res.n.size() < l)
			res.n.push_back(0);
		for (int i = 0; i < n.size(); i++)
			for (int j = 0; j < b.n.size(); j++) {
				res.n[i + j] += n[i] * b[j];
				int s = res[i + j] / 10;
				res.n[i + j + 1] += s;
				res.n[i + j] %= 10;
			}
		clz(res);
		return res;
	}
};

string expre;
long long p;
stack<char> op;
stack<Num> n;
int pri[300];

ostream& operator<<(ostream& out, Num x) {
	for (int i = x.n.size() - 1; i >= 0; --i)
		cout << x[i];
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
	switch (o)
	{
		case '+':
			n.push(a + b);
			break;
		case '*':
			n.push(a * b);
			break;
		default:
			break;
	}
}

int main() {
	pri['+'] = pri['-'] = 1;
	pri['*'] = pri['/'] = 2;
	while (1) {
		cin >> expre;
		p = 0;
		n.push(*new Num(getNum()));
		while (p < expre.size()) {
			char o = expre[p++];
			Num x(getNum());
			if (!op.empty() && pri[o] <= pri[op.top()])
				cal();
			n.push(x);
			op.push(o);
		}
		while (!op.empty())
			cal();
		cout << n.top() << endl;
	}
	return 0;
}
//45489*531+556+851*5823*8978954421546
//44494068308783821873