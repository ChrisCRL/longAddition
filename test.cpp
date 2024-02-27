#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    vector<int> num1(32, 0);
    vector<int> num2(32, 0);
    vector<int> result(32, 0);

    string s1 = "12345678901234567890123456789012";
    string s2 = "98765432109876543210987654321098";

    for (int i = 0; i < 32; i++) {
        num1[i] = s1[i] - '0';
        num2[i] = s2[i] - '0';
    }

    int carry = 0;

    for (int i = 31; i >= 0; i--) {
        int sum = num1[i] + num2[i] + carry;
        result[i] = sum % 10;
        carry = sum / 10;
    }

    if (carry > 0) {
        result.insert(result.begin(), carry);
    }

    for (int i = 0; i < result.size(); i++) {
        cout << result[i];
    }
    cout << endl;

    return 0;
}