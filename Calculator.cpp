#include <iostream>
#include <vector>
#include <string>

using namespace std;
string expre;

struct Int {
	vector<int> n;
	Int(int num) {
		if (num) {
			int tmp = 0;
			while (num)
				tmp = tmp * 10 + num % 10, num /= 10;
			while (tmp) {
				n.push_back(tmp % 10);
				tmp /= 10;
			}
		} else
			n.push_back(0);
	}
	int operator[] (int n) {
		return this->n[n];
	}
	Int operator+ (Int b) {
		Int res(0);
		int l = max(n.size(), b.n.size()), i;
		while (n.size() < l)
			n.push_back(0);
		while (b.n.size() < l)
			b.n.push_back(0);
		for (i = 0; i < l; i++) {
			res.n[i] = n[i] + b[i];
			res.n.push_back(res[i] / 10);
			res.n[i] %= 10;
		}
		while (res[i--] == 0)
			res.n.pop_back();
		return res;
	}
};

ostream& operator<<(ostream& out, Int x) {
	for (int i = x.n.size() - 1; i >= 0; --i)
		cout << x[i];
	return out;
}

int main() {
	while (1) {
		//getline(cin, expre);
		int a, b;
		cin >> a >> b;
		Int x(a), y(b);
		cout << x + y << endl;
	}
	return 0;
}