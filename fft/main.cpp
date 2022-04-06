/*
 * Created by Grigoriy Khlytin, April 2022
 */

#include <iostream>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

typedef long long ll;

# define PI 3.14159265358979323846

struct Num {
    double re = 0;
    double im = 0;

    Num() = default;

    Num(double re, double im) : re(re), im(im) {
    }

    Num operator+(const Num &y) const {
        return {re + y.re, im + y.im};
    }

    Num operator-(const Num &y) const {
        return {re - y.re, im - y.im};
    }

    Num operator*(const Num &y) const {
        return {re * y.re - im * y.im, im * y.re + re * y.im};
    }

    Num operator/(const double &y) const {
        return {re / y, im / y};
    }
};

vector<Num> toNum(const vector<ll> &vec, ll size) {
    vector<Num> res(size);
    for (ll i = 0; i < vec.size(); i++) {
        res[i] = Num((double) vec[i], 0);
    }
    return res;
}

vector<ll> toLL(const vector<Num> &vec, ll size) {
    vector<ll> res(size);
    for (ll i = 0; i < size; i++) {
        res[i] = (ll) round(vec[i].re);
    }
    return res;
}

ll round_up_to_power_of_two(ll size) {
    ll n = 1;
    while (n < size) {
        n = n << 1;
    }
    n = n << 1;
    return n;
}

void normalize(vector<ll> &vec, ll BASE_SIZE) {
    ll base = 1;
    for (ll i = 0; i < BASE_SIZE; i++) {
        base *= 10;
    }
    ll carry = 0;
    for (ll &x: vec) {
        x += carry;
        carry = x / base;
        x %= base;
    }
    while (!vec.empty() && vec[vec.size() - 1] == 0) {
        vec.pop_back();
    }
}

vector<ll> numToPoly(const string &s, bool sign, ll BASE_SIZE) {
    vector<ll> res;
    ll pos = (ll) s.length() - 1;
    while (pos >= sign) {
        ll num = 0;
        ll base = 1;
        for (ll j = 0; (pos >= sign) && (j < BASE_SIZE); j++) {
            num += (s[pos] - '0') * base;
            base *= 10;
            pos--;
        }
        res.push_back(num);
    }
    return res;
}

string polyToNum(const vector<ll> &vec, ll BASE_SIZE) {
    string res;
    for (ll i = (ll) vec.size() - 1; i >= 0; i--) {
        ll val = vec[i];
        vector<ll> digits;
        for (ll j = 0; j < BASE_SIZE; j++) {
            digits.push_back(val % 10);
            val /= 10;
        }
        if (i == vec.size() - 1) {
            while (!digits.empty() && digits[digits.size() - 1] == 0) {
                digits.pop_back();
            }
            if (digits.empty()) {
                res.push_back('0');
            }
        }
        for (ll j = (ll) digits.size() - 1; j >= 0; j--) {
            res.push_back((char) ('0' + digits[j]));
        }
    }
    return res;
}

void fft(vector<Num> &a, bool invert) {
    ll n = (ll) a.size();
    if (n <= 1) {
        return;
    }

    vector<Num> even(n / 2);
    vector<Num> odd(n / 2);

    for (ll i = 0; i < n / 2; i++) {
        even[i] = a[2 * i];
        odd[i] = a[2 * i + 1];
    }

    fft(even, invert);
    fft(odd, invert);

    double alpha = (invert ? -1 : 1) * 2 * PI / (double) n;

    Num x(1, 0);
    Num step(cos(alpha), sin(alpha));

    for (ll i = 0; i < n / 2; i++) {
        Num t = x * odd[i];
        a[i] = even[i] + t;
        a[i + n / 2] = even[i] - t;
        x = x * step;

        if (invert) {
            a[i] = a[i] / 2;
            a[i + n / 2] = a[i + n / 2] / 2;
        }
    }
}

vector<ll> multiply_polynomials(const vector<ll> &a, const vector<ll> &b) {
    ll n = round_up_to_power_of_two((ll) max(a.size(), b.size()));

    vector<Num> fa = toNum(a, n);
    vector<Num> fb = toNum(b, n);

    fft(fa, false);
    fft(fb, false);

    for (ll i = 0; i < n; i++) {
        fa[i] = fa[i] * fb[i];
    }

    fft(fa, true);

    return toLL(fa, n);
}

int main() {
    ll BASE_SIZE = 2;

    string s1, s2;
    cin >> s1 >> s2;

    bool sign_a = (s1[0] == '-');
    bool sign_b = (s2[0] == '-');

    vector<ll> a = numToPoly(s1, sign_a, BASE_SIZE);
    vector<ll> b = numToPoly(s2, sign_b, BASE_SIZE);

    vector<ll> answer = multiply_polynomials(a, b);

    normalize(answer, BASE_SIZE);

    if (answer.empty()) {
        cout << 0;
    } else {
        if (sign_a ^ sign_b) {
            cout << "-";
        }
        cout << polyToNum(answer, BASE_SIZE);
    }

    return 0;
}
