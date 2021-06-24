//
// Created by edward on 2021/6/23.
//

#include "BigInt.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>

BigInt BigInt::operator-() const {
    BigInt ret = *this;
    ret.negative = !ret.negative;
    return ret;
}

BigInt & BigInt::setOppo() {
    negative = !negative;
    return *this;
}

bool BigInt::isNegative() const {
    return negative;
}

void BigInt::num2big(std::vector<int> &bit, ll num) {
    ll x;
    //必须使用do while，循环至少应该执行一遍，处理num是0的情况
    do {
        x = num % BASE;
        bit.push_back(static_cast<int>(x));
        num /= BASE;
    } while (num);
}

BigInt::BigInt(ll num):negative(false) {
    if (num < 0) {
        num = -num;
        negative = true;
    }
    num2big(this->bit, num);
}

void BigInt::str2big(std::vector<int> &bit, std::string &s) {
    int len = (s.size() - 1) / WIDTH + 1;       //ceil得到len
    int start, end;
    for (int i = 0; i < len; ++i) {
        end = s.size() - i * WIDTH;
        start = std::max(0, end - WIDTH);
        bit.push_back(std::stoi(s.substr(start, end - start)));
    }
}

BigInt::BigInt(std::string s):negative(false) {
    trim(s);
    if (s[0] == '-') {
        negative = true;
        s.erase(s.begin());
    }
    str2big(this->bit, s);
}

void BigInt::trim(std::string &s) {
    auto ltrim = [](std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char c) -> bool {
            //找到第一个非空字符
            return !std::isspace(c);
        }));
    };
    auto rtrim = [](std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char c) -> bool {
            //找到最后一个非空字符
            return !std::isspace(c);
        }).base(), s.end());
    };
    ltrim(s);
    rtrim(s);
}

BigInt &BigInt::operator=(ll num) {
    bit.clear();
    negative = false;
    if (num < 0) {
        num = -num;
        negative = true;
    }
    num2big(this->bit, num);
    return *this;
}

BigInt &BigInt::operator=(std::string s) {
    bit.clear();
    negative = false;

    trim(s);
    if (s[0] == '-') {
        negative = true;
        s.erase(s.begin());
    }
    str2big(this->bit, s);
    return *this;
}

std::ostream &operator << (std::ostream &os, const BigInt &bigInt) {
    if (bigInt.negative) {
        //输出负号
        os << "-";
    }
    auto &bit = bigInt.bit;
    //以大端字节序输出
    os << bit.back();
    //除了最后一位，其他的如果有前导0不能忽略
    os << std::setfill('0');
    for (int i = bit.size() - 2; i >= 0; --i) {
        os << std::setw(BigInt::WIDTH) << bit[i];
    }
    os << std::setfill(' ');
    return os;
}

std::istream &operator >> (std::istream &is, BigInt &bigInt) {
    std::string s;
    if (is >> s) {
        //必须判断是否读入成功
        bigInt = s;
    }
    return is;
}

BigInt & BigInt::operator+=(const BigInt &rhs) {
    if (!negative && rhs.negative) {
        BigInt tmp = rhs;
        return *this = (tmp -= this->setOppo());
    } else if (negative && !rhs.negative) {
        return *this -= -rhs;
    }
    auto &rbit = rhs.bit;
    int max_len = std::max(bit.size(), rbit.size());
    int min_len = std::min(bit.size(), rbit.size());
    int g = 0;      //进位
    for (int i = 0; i < min_len; ++i) {
        bit[i] += rbit[i] + g;
        g = bit[i] / BASE;
        bit[i] %= BASE;
    }
    if (bit.size() < rbit.size()) {
        for (int i = min_len; i < max_len; ++i) {
            bit.push_back(g + rbit[i]);
            g = bit.back() / BASE;
            bit.back() %= BASE;
        }
    } else {
        for (int i = min_len; i < max_len; ++i) {
            bit[i] += g;
            g = bit[i] / BASE;
            bit[i] %= BASE;
            if (!g) break;
        }
    }
    if (g) {
        //加法的进位最多为1
        bit.push_back(g);
    }
    return *this;
}


BigInt operator + (const BigInt &lhs, const BigInt &rhs) {
    BigInt ret = lhs;
    ret += rhs;
    return std::move(ret);
}

BigInt & BigInt::operator-=(const BigInt &rhs) {
    if (!negative && !rhs.negative) {
        if (*this >= rhs) {

        } else {
            BigInt tmp = rhs;
            tmp -= *this;
            return *this = tmp.setOppo();
        }
    } else if (!negative && rhs.negative) {
        this->setOppo();
        *this += rhs;
        this->setOppo();
        return *this;
    } else if (negative && !rhs.negative) {
        return *this += -rhs;
    } else {
        BigInt tmp = -rhs;
        this->setOppo();
        return *this = (tmp -= *this);
    }
    auto &rbit = rhs.bit;
    for (int i = 0; i < rbit.size(); ++i) {
        if (bit[i] < rbit[i]) {
            bit[i] += BASE;
            bit[i + 1] -= 1;
        }
        bit[i] -= rbit[i];
    }
    for (int i = rbit.size(); i < bit.size(); ++i) {
        if (bit[i] >= 0) {
            break;
        }
        bit[i] += BASE;
        bit[i + 1] -= 1;
    }
    //删除前导0
    for (int i = bit.size() - 1; i > 0; --i) {
        if (!bit[i]) bit.pop_back();
    }
    return *this;
}


BigInt operator - (const BigInt &lhs, const BigInt &rhs) {
    BigInt res = lhs;
    res -= rhs;
    return std::move(res);
}

BigInt & BigInt::operator*=(const BigInt &rhs) {
    //负负得正
    if (negative && rhs.negative || !negative && !rhs.negative) {
        negative = false;
    } else {
        negative = true;
    }
    auto &rbit = rhs.bit;
    constexpr ll LBASE = BASE;
    std::vector<ll> c(bit.size() + rbit.size(), 0);
    for (int i = 0; i < bit.size(); ++i) {
        for (int j = 0; j < rbit.size(); ++j) {
            c[i + j] += static_cast<ll>(bit[i]) * static_cast<ll>(rbit[j]);
            //在这里处理进位防止溢出
            if (c[i + j] >= LBASE) {
                //有必要再进行除法，毕竟除法比较慢
                c[i + j + 1] += c[i + j] / LBASE;
                c[i + j] %= LBASE;
            }
        }
    }
    //处理进位
    for (int i = 0; i < c.size(); ++i) {
        if (c[i] >= LBASE) {
            //有必要再进行除法，毕竟除法比较慢
            c[i + 1] += c[i] / LBASE;
            c[i] %= LBASE;
        }
    }
    //删除前导0
    for (int i = c.size() - 1; i > 0; --i) {    //至少留一位
        if (!c[i]) c.pop_back();
        else break;
    }
    bit.resize(c.size());
    for (int i = 0; i < c.size(); ++i) {
        bit[i] = static_cast<int>(c[i]);
    }
    return *this;
}

BigInt operator * (const BigInt &lhs, const BigInt &rhs) {
    BigInt res = lhs;
    res *= rhs;
    return std::move(res);
}

std::string BigInt::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

bool BigInt::less(const BigInt &lhs, const BigInt &rhs) {
    if (lhs.bit.size() != rhs.bit.size()) return lhs.bit.size() < rhs.bit.size();
    for (int i = lhs.bit.size() - 1; i >= 0; --i) {
        if (lhs.bit[i] != rhs.bit[i]) return lhs.bit[i] < rhs.bit[i];
    }
    //相等
    return false;
}

bool operator < (const BigInt &lhs, const BigInt &rhs) {
    if (!lhs.negative && !rhs.negative) {
        return BigInt::less(lhs, rhs);
    } else if (lhs.negative && !rhs.negative) {
        return true;
    } else if (!lhs.negative && rhs.negative) {
        return false;
    } else if (lhs.negative && rhs.negative) {
        //都是负数
        if (BigInt::less(lhs, rhs)) {
            return false;
        } else if (BigInt::less(rhs, lhs)) {
            return true;
        } else {
            return false;
        }
    }
}

bool operator > (const BigInt &lhs, const BigInt &rhs) {
    return rhs < lhs;
}

bool operator == (const BigInt &lhs, const BigInt &rhs) {
    return !(lhs < rhs) && !(rhs < lhs);
}
bool operator != (const BigInt &lhs, const BigInt &rhs) {
    return (lhs < rhs) || (rhs < lhs);
}

bool operator <= (const BigInt &lhs, const BigInt &rhs) {
    return !(rhs < lhs);
}

bool operator >= (const BigInt &lhs, const BigInt &rhs) {
    return !(lhs < rhs);
}
