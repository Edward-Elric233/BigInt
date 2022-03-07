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
    ret.negative_ = !ret.negative_;
    return ret;
}

BigInt & BigInt::setOppo() {
    negative_ = !negative_;
    return *this;
}

bool BigInt::isNegative() const {
    return negative_;
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

BigInt::BigInt(ll num):negative_(false) {
    if (num < 0) {
        num = -num;
        negative_ = true;
    }
    num2big(bit_, num);
}

void BigInt::str2big(std::vector<int> &bit, const std::string &s, std::pair<int, int> pr) {
    int len = (pr.second - pr.first - 1) / WIDTH + 1;       //ceil得到len
    int start, end = pr.second;
    for (int i = 1; i < len; ++i) {
        start = end - WIDTH;
        bit.push_back(std::stoi(s.substr(start, end - start)));
        end -= WIDTH;
    }
    start = pr.first;
    bit.push_back(std::stoi(s.substr(start, end - start)));
}

BigInt::BigInt(const std::string &s):negative_(false) {
    auto pr = trim(s);
    if (s[0] == '-') {
        negative_ = true;
        ++pr.first;
    }
    str2big(bit_, s, pr);
}

std::pair<int, int> BigInt::trim(const std::string &s) {
    auto ltrim = [](const std::string &s)-> int {
        return std::find_if(s.begin(), s.end(), [](char c) -> bool {
            //找到第一个非空字符
            return !std::isspace(c);
        }) - s.begin();
    };
    auto rtrim = [](const std::string &s) -> int {
        return std::find_if(s.rbegin(), s.rend(), [](char c) -> bool {
            //找到最后一个非空字符
            return !std::isspace(c);
        }).base() - s.begin();
    };
    return {ltrim(s), rtrim(s)};
}

BigInt &BigInt::operator=(ll num) {
    bit_.clear();
    negative_ = false;
    if (num < 0) {
        num = -num;
        negative_ = true;
    }
    num2big(bit_, num);
    return *this;
}

BigInt &BigInt::operator=(const std::string &s) {
    bit_.clear();
    negative_ = false;

    auto pr = trim(s);
    if (s[0] == '-') {
        negative_ = true;
        ++pr.first;
    }
    str2big(bit_, s, pr);
    return *this;
}

std::ostream &operator << (std::ostream &os, const BigInt &bigInt) {
    if (bigInt.negative_) {
        //输出负号
        os << "-";
    }
    auto &bit = bigInt.bit_;
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
    if (!negative_ && rhs.negative_) {
        BigInt tmp = rhs;
        return *this = (tmp -= this->setOppo());
    } else if (negative_ && !rhs.negative_) {
        return *this -= -rhs;
    }
    auto &rbit = rhs.bit_;
    int max_len = std::max(bit_.size(), rbit.size());
    int min_len = std::min(bit_.size(), rbit.size());
    int g = 0;      //进位
    for (int i = 0; i < min_len; ++i) {
        bit_[i] += rbit[i] + g;
        g = bit_[i] / BASE;
        bit_[i] %= BASE;
    }
    if (bit_.size() < rbit.size()) {
        for (int i = min_len; i < max_len; ++i) {
            bit_.push_back(g + rbit[i]);
            g = bit_.back() / BASE;
            bit_.back() %= BASE;
        }
    } else {
        for (int i = min_len; i < max_len; ++i) {
            bit_[i] += g;
            g = bit_[i] / BASE;
            bit_[i] %= BASE;
            if (!g) break;
        }
    }
    if (g) {
        //加法的进位最多为1
        bit_.push_back(g);
    }
    return *this;
}


BigInt operator + (const BigInt &lhs, const BigInt &rhs) {
    BigInt ret = lhs;
    ret += rhs;
    return ret;
}

BigInt & BigInt::operator-=(const BigInt &rhs) {
    if (!negative_ && !rhs.negative_) {
        if (*this >= rhs) {

        } else {
            BigInt tmp = rhs;
            tmp -= *this;
            return *this = tmp.setOppo();
        }
    } else if (!negative_ && rhs.negative_) {
        this->setOppo();
        *this += rhs;
        this->setOppo();
        return *this;
    } else if (negative_ && !rhs.negative_) {
        return *this += -rhs;
    } else {
        BigInt tmp = -rhs;
        this->setOppo();
        return *this = (tmp -= *this);
    }
    auto &rbit = rhs.bit_;
    for (int i = 0; i < rbit.size(); ++i) {
        if (bit_[i] < rbit[i]) {
            bit_[i] += BASE;
            bit_[i + 1] -= 1;
        }
        bit_[i] -= rbit[i];
    }
    for (int i = rbit.size(); i < bit_.size(); ++i) {
        if (bit_[i] >= 0) {
            break;
        }
        bit_[i] += BASE;
        bit_[i + 1] -= 1;
    }
    //删除前导0
    for (int i = bit_.size() - 1; i > 0; --i) {
        if (!bit_[i]) bit_.pop_back();
    }
    return *this;
}


BigInt operator - (const BigInt &lhs, const BigInt &rhs) {
    BigInt res = lhs;
    res -= rhs;
    return res;
}

BigInt & BigInt::operator*=(const BigInt &rhs) {
    //负负得正
    if (negative_ && rhs.negative_ || !negative_ && !rhs.negative_) {
        negative_ = false;
    } else {
        negative_ = true;
    }
    auto &rbit = rhs.bit_;
    constexpr ll LBASE = BASE;
    std::vector<ll> c(bit_.size() + rbit.size(), 0);
    for (int i = 0; i < bit_.size(); ++i) {
        for (int j = 0; j < rbit.size(); ++j) {
            c[i + j] += static_cast<ll>(bit_[i]) * static_cast<ll>(rbit[j]);
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
    bit_.resize(c.size());
    for (int i = 0; i < c.size(); ++i) {
        bit_[i] = static_cast<int>(c[i]);
    }
    return *this;
}

BigInt operator * (const BigInt &lhs, const BigInt &rhs) {
    BigInt res = lhs;
    res *= rhs;
    return res;
}

std::string BigInt::toString() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

bool BigInt::less(const BigInt &lhs, const BigInt &rhs) {
    if (lhs.bit_.size() != rhs.bit_.size()) return lhs.bit_.size() < rhs.bit_.size();
    for (int i = lhs.bit_.size() - 1; i >= 0; --i) {
        if (lhs.bit_[i] != rhs.bit_[i]) return lhs.bit_[i] < rhs.bit_[i];
    }
    //相等
    return false;
}

bool operator < (const BigInt &lhs, const BigInt &rhs) {
    if (!lhs.negative_ && !rhs.negative_) {
        return BigInt::less(lhs, rhs);
    } else if (lhs.negative_ && !rhs.negative_) {
        return true;
    } else if (!lhs.negative_ && rhs.negative_) {
        return false;
    } else if (lhs.negative_ && rhs.negative_) {
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
