#include <iostream>
using namespace std;
int main() {
	int n, deno, nume = 1;
	float sum = 0;
	cin >> n;
	while (n >= 1) {
		deno = n;
		sum += float(nume) / deno;
		nume++;
		n--;
	}
	cout << sum << endl;
	return 0;
}