#include "BigInt.h"
#include <gmpxx.h>
#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
#include <random>
#include <chrono>

using namespace std;

int getRandom(int max_ = INT_MAX, int min_ = 0) {
    static default_random_engine e(std::chrono::system_clock::now().time_since_epoch().count());
    static uniform_int_distribution<> u;
    u.param(decltype(u)::param_type(min_, max_ - 1));
    return u(e);
}

void init(string &s) {
    constexpr int MAX_LEN = 1e5;
    bool negative = getRandom(2);
    if (negative) s.push_back('-');

    int len = getRandom() % MAX_LEN;
    s.push_back(getRandom(9) + '1');  //第一位不能为0
    for (int i = 0; i < len; ++i) {
        s.push_back(getRandom(10) + '0');
    }
}

class TimeCounter {
    chrono::system_clock::time_point start, stop;
    chrono::duration<double> duration;
    bool ready = true;
public:
    void operator ()(const std::string &msg = "");
};

void TimeCounter::operator()(const std::string &msg) {
    if (ready) {
        start = chrono::system_clock::now();
        ready = false;
    } else {
        stop = chrono::system_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        cout <<  msg << ":"
             << double(duration.count()) * chrono::microseconds::period::num / chrono::microseconds::period::den
             << "s" << endl;
        ready = true;
    }
}

namespace {
    BigInt big_a, big_b;
    mpz_class mpz_a, mpz_b;
    TimeCounter timeCounter;
}

bool test_add(const string &a, const string &b) {
    ostringstream big_os, mpz_os;
    timeCounter();
    big_os << big_a + big_b;
    timeCounter("Big Add");
    timeCounter();
    mpz_os << mpz_a + mpz_b;
    timeCounter("GMP Add");
    return big_os.str() == mpz_os.str();
}

bool test_del(const string &a, const string &b) {
    ostringstream big_os, mpz_os;
    timeCounter();
    big_os << big_a - big_b;
    timeCounter("Big Del");
    timeCounter();
    mpz_os << mpz_a - mpz_b;
    timeCounter("GMP Del");
    return big_os.str() == mpz_os.str();
}

bool test_mul(const string &a, const string &b) {
    ostringstream big_os, mpz_os;
    timeCounter();
    big_os << big_a * big_b;
    timeCounter("Big Mul");
    timeCounter();
    mpz_os << mpz_a * mpz_b;
    timeCounter("GMP Mul");
    return big_os.str() == mpz_os.str();
}

bool test() {
    string a, b;
    init(a);
    init(b);
    cout << "a = " << a << "\nb = " << b << endl;
    big_a = a; big_b = b;
    mpz_a = a.c_str(); mpz_b = b.c_str();
    if (test_add(a, b) && test_del(a, b) && test_mul(a, b)) {
        return true;
    } else {
        return false;
    }
}

void work() {
    srand(time(nullptr));
    int T;
    cin >> T;
    while (T--) {
        if (!test()) {
            cout << "error" << "\n";
            break;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);

    BigInt a, b;
    while (cin >> a >> b) {
        cout << "a + b = " << a + b << endl;
        cout << "a - b = " << a - b << endl;
        cout << "a * b = " << a * b << endl;
    }
    //work();

    return 0;
}
